#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <wiringPi.h>
#include <sched.h>

#define PIN 4

/* XXX dump values */
#define TIME_LOCK        2750    // Time use for lock
#define TIME_HIGH        275     // Time use for 1
#define TIME_LOW         1225    // Time use for 0
#define TIME_UP          275     // time use for up signal

#define countof(_x)  (sizeof(_x) / sizeof(_x[0]))

/* init / shutdown {{{ */

static int emission_init(void)
{
    struct sched_param p;

    wiringPiSetup();
    pinMode(PIN, OUTPUT);

    p.__sched_priority = sched_get_priority_max(SCHED_RR);
    if (sched_setscheduler(0, SCHED_RR, &p) == -1) {
        fprintf(stderr, "setscheduler() failed: %m\n");
    }

    return 0;
}

static void emission_shutdown(void)
{
}

/* }}} */
/* handle TCP connection {{{ */

static int incoming_connection(const char *hostname, int port)
{
    int                 sock;
    struct protoent     *protocol;
    struct sockaddr_in  sin;
    struct hostent     *ent;

    protocol = getprotobyname("TCP");
    sock = socket(AF_INET, SOCK_STREAM, protocol->p_proto);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (int []){1}, sizeof(int));

    if ((ent = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "%s: %s\n", hostname, hstrerror(h_errno));
        return -1;
    }

    sin.sin_family      = AF_INET;
    sin.sin_port        = htons(port);

    memcpy((void *)&sin.sin_addr.s_addr, ent->h_addr_list[0], ent->h_length);

    sin.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "cannot bind: %m\n");
        return -1;
    }

    if (listen(sock, 100) < 0) {
        fprintf(stderr, "cannot listen: %m\n");
        return -1;
    }

    int fd;
    if ((fd = accept(sock, NULL, NULL)) == -1) {
	fprintf(stderr, "Accept fail: %m\n");
	return -1;
    }
    return fd;
}

/* }}} */

static int send_signal(int pin, unsigned int time_sig)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(TIME_UP);
    digitalWrite(pin, LOW);
    delayMicroseconds(time_sig);

    return 0;
}

static int send_frame(const unsigned char array[], int len)
{
    send_signal(PIN, TIME_LOCK);

    for (int i = 0; i < len; ++i) {
        switch (array[i]) {
            /* XXX beware endianess */

            /* loop through every bit */
            for (int j = 0; j < 8; ++j) {
                unsigned char bit = array[i] & (0x80 >> j);

                switch (bit) {
                    case 0:
                        send_signal(PIN, TIME_LOW);
                        send_signal(PIN, TIME_HIGH);
                        break;
                    case 1:
                        send_signal(PIN, TIME_HIGH);
                        send_signal(PIN, TIME_LOW);
                        break;

                    default:
                        assert (false);
                }
            }
        }
    }

    digitalWrite(PIN, HIGH);
    delayMicroseconds(TIME_UP);
    digitalWrite(PIN, LOW);

    return 0;
}

static int main_loop(int csock)
{
    unsigned char array[4];
    char octet;

    memcpy(array, "\x00\x42", 2); /* XXX hardcoded drone id */
    array[2] = 9; /* XXX hardcoded module */

    while (recv(csock, &octet, 1, 0) > 0) {
        switch (octet) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                array[3] = octet;

		printf("Sending frame with data: %02X\n", octet);
                send_frame(array, countof(array));
		break;

            default:
                fprintf(stderr, "invalid byte: %x\n", octet);
                break;
        }
    }

    return 0;
}

int main(void)
{
    int csock;

    if (emission_init() < 0) {
        return -1;
    }

    csock = incoming_connection("localhost", 5001);

    if (csock < 0) {
        return -1;
    }

    main_loop(csock);

    emission_shutdown();

    return 0;
}
