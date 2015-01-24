
#define TIMEVERROU      2750    // Time use for lock
#define TIMEHIGH        275     // Time use for 1
#define TIMELOW         1225    // Time use for 0
#define TIMEUP          275     // time use for up signal

// Send Bit
void    sendBit(int pin, unsigned int time)
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(TIMEUP);
    digitalWrite(pin, LOW);
    delayMicroseconds(time);
}

// Send Bit in Manchester coding
void    send(int pin, char *cmd)
{
    int i;

    // verrou
    sendBit(pin, TIMEVERROU);

    for (i = 0; cmd[i]; i++)
    {
        // Send 1 in MANCHESTER -> "1" = "10"
        if (cmd[i] == '1')
        {
            sendBit(pin, TIMEHIGH);
            sendBit(pin, TIMELOW);
        }
        // Send 0 in MANCHESTER -> "0" = "01"
        else
        {
            sendBit(pin, TIMELOW);
            sendBit(pin, TIMEHIGH);
        }
    }
    digitalWrite(2, HIGH);
    delayMicroseconds(TIMEUP);
    digitalWrite(2, LOW);
}

// Set the pin used to send the output signal
void setup()
{
    pinMode(2, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13,LOW);
}

// loop sends signal
void loop()
{
    // We send id module and value (4 Bits)
    // id =     0000000000101010 = 42
    // module = 00001001 = 9
    // value =  00000101 = 5
send(2, "00000000001010100000100100000101");
delay(10);
}
