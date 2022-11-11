
#ifndef DEBUGSERIAL_H_
#define DEBUGSERIAL_H_

#define DEBUG 1
#if DEBUG
#define SPRINT(s)        \
    {                    \
        Serial.print(s); \
    }
#define SPRINTLN(s)        \
    {                      \
        Serial.println(s); \
    }
#else
#define SPRINT(s)
#define SPRINTLN(s)
#endif
#define SERIALSPEED 57600

#endif