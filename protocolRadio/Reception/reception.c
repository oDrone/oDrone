#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include <wiringPi.h>
#include <sched.h>

/* reception pin */
int pin;

/* switch to realtime */
void scheduler_realtime() {
    struct sched_param p;
    p.__sched_priority = sched_get_priority_max(SCHED_RR);

    if ( sched_setscheduler( 0, SCHED_RR, &p ) == -1 ) {
        perror("Failed to switch to realtime scheduler.");
    }
}

/* switch back to normal */
void scheduler_standard() {
    struct sched_param p;
    p.__sched_priority = 0;

    if ( sched_setscheduler( 0, SCHED_OTHER, &p ) == -1 ) {
        perror("Failed to switch to normal scheduler.");
    }
}

/* time needed to change state */
int pulseIn(int pin, int level, int timeout)
{
    struct timeval tn, t0, t1;
    long micros;
    gettimeofday(&t0, NULL);
    micros = 0;

    /* loop getting the state */
    while (digitalRead(pin) != level) {
        gettimeofday(&tn, NULL);
        if (tn.tv_sec > t0.tv_sec) {
            micros = 1000000L;
        } else {
            micros = 0;
        }

        micros += (tn.tv_usec - t0.tv_usec);

        if (micros > timeout)
            return 0;
    }
    gettimeofday(&t1, NULL);

    while (digitalRead(pin) == level) {
        gettimeofday(&tn, NULL);
        if (tn.tv_sec > t0.tv_sec) {
            micros = 1000000L;
        } else {
            micros = 0;
        }

        micros = micros + (tn.tv_usec - t0.tv_usec);

        if (micros > timeout)
            return 0;
    }
    if (tn.tv_sec > t1.tv_sec) {
        micros = 1000000L;
    } else {
        micros = 0;
    }

    micros = micros + (tn.tv_usec - t1.tv_usec);
    return micros;
}

// TODO 1: do not forge the packet until the 64 bits are received
// TODO 2: optimize
int main (int argc, char** argv)
{
    /* go to realtime mode */
    scheduler_realtime();

    pin = (argc == 2) ? atoi(argv[1]) : 1;

    /* load the wiriingPi library */
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Unable to load the wiringPi library\n");
        return -1;
    }

    /* set the pin to input mode */
    pinMode(pin, INPUT);

    /* loop through every packets */
    for (;;) {
        int i;

        // time elapsed during the last reception */
        unsigned long t = 0;

        // prev bit
        int prevBit = 0;

        // last bit
        int bit = 0;

        /* packet description */
        unsigned long id = 0;
        unsigned long value = 0;
        unsigned long module = 0;

        /* waiting for a lock */
        do {
            t = pulseIn(pin, LOW, 100000);
        } while ((t < 2700 || t > 2800));


        /* loop through every received value */
        for(i = 0; i < 64; ++i) {
            t = pulseIn(pin, LOW, 100000);

            /* get a bit value */
            if (t > 200 && t < 350) { // XXX theorical values (increased from basic protocol)
                bit = 1;
            } else if (t > 1100 && t < 1350) { // XXX theorical values (increased as well)
                bit = 0;
            } else {
                // Debug if fail
                /*
                   std::cout << "fail, i = " << i << std::endl;
                   std::cout << "bad value " << t << " timeout?" << std::endl;
                   */
                i = 0;
                break;
            }

            // 0 = 01, 1 = 10, prevBit will give us our real value */
            if (i % 2 == 1) {
                if ((prevBit ^ bit) == 0) {
                    // Debug if fail
                    //std::cout << "fail manchester, i = " << i << std::endl;
                    i = 0;
                    break;
                }

                if (i < (16 * 2) + 1) {
                    /* handle the ID */
                    id <<= 1;
                    id |= prevBit;
                } else if (i < (16 * 3) + 1) {
                    /* handle the module */
                    module <<= 1;
                    module |= prevBit;
                } else {
                    /* handle the data (value) */
                    value <<= 1;
                    value|= prevBit;
                }
            }

            prevBit = bit;
        }
        /* display the received packet */
        if (i == 64)
            printf("Packet recv -> [id = %ld, module = %ld, value = %ld]\n", id, module, value);
    }
    scheduler_standard();
}

