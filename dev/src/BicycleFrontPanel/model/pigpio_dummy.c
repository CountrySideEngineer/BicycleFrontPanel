#ifdef __cplusplus
extern "C" {
#endif/* __cplspls */
#include <stdio.h>
#include "pigpio/pigpio.h"

//Dummies of pigpio library api.
int gpioInitialise(void) { return 0; }
void gpioTerminate(void) { /* Nothing to do. */}
int gpioSetMode(unsigned gpio, unsigned mode)
{
    gpio = 0;
    mode = 0;

    return 0;
}
int gpioRead (unsigned gpio)
{
    gpio = 0;

    return 0;
}
int gpioSetTimerFunc(unsigned timer, unsigned millis, gpioTimerFunc_t f)
{
    timer = 0;
    millis = 0;
    f = NULL;

    return 0;
}
int gpioSetISRFunc(unsigned gpio, unsigned edge, int timeout, gpioISRFunc_t f)
{
    gpio = 0;
    edge = 0;
    timeout = 0;
    f = NULL;

    return 0;
}


#ifdef __clusplus
}
#endif/* __cplspls */


