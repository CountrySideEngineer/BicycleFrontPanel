#include <iostream>
#include "pigpio/pigpio.h"
using namespace std;

#define STUB_BUFFER_SIZE    (256)

unsigned int gpioInitialise_called_counter;
int gpioInitialise_return[STUB_BUFFER_SIZE];
/**
 * @brief gpioInitialise    Stub function of gpioInitialise.
 * @return  Dummy value in gpioInitialise_return, int type array.
 */
int gpioInitialise()
{
    int return_value = gpioInitialise_return[gpioInitialise_called_counter];

    gpioInitialise_called_counter++;

    return return_value;
}

unsigned int gpioTerminate_called_counter;
/**
 * @brief gpioTerminate Stub function of gpioTerminate.
 */
void gpioTerminate()
{
    gpioTerminate_called_counter++;
}

unsigned int gpioSetMode_called_counter;
unsigned int gpioSetMode_gpio[STUB_BUFFER_SIZE];
unsigned int gpioSetMode_mode[STUB_BUFFER_SIZE];
int gpioSetMode_return[STUB_BUFFER_SIZE];
/**
 * @brief gpioSetMode   Stub function of gpioSetMode.
 * @param gpio  Dummy of 1st argument.
 * @param mode  Dummy of 2nd argument.
 * @return  Dummy value in gpioSetMode_return, int type array.
 */
int gpioSetMode(unsigned gpio, unsigned mode)
{
    gpioSetMode_gpio[gpioSetMode_called_counter] = gpio;
    gpioSetMode_mode[gpioSetMode_called_counter] = mode;

    int return_value = gpioSetMode_return[gpioSetMode_called_counter];
    gpioSetMode_called_counter++;

    return return_value;
}

unsigned int gpioRead_called_counter;
unsigned int gpioRead_gpio[STUB_BUFFER_SIZE];
int gpioRead_return[STUB_BUFFER_SIZE];
/**
 * @brief gpioRead  Stub function of gpioRead.
 * @param gpio  Dummy of 1st argument.
 * @return  Dummy value in gpioRead_return, int type array.
 */
int gpioRead(unsigned gpio)
{
    gpioRead_gpio[gpioRead_called_counter] = gpio;

    int return_value = gpioRead_return[gpioRead_called_counter];
    gpioRead_called_counter++;

    return return_value;
}


unsigned int gpioSetTimerFunc_called_counter;
unsigned int gpioSetTimerFunc_timer[STUB_BUFFER_SIZE];
unsigned int gpioSetTimerFunc_millis[STUB_BUFFER_SIZE];
gpioTimerFunc_t gpioSetTimerFunc_gpioTimerFunc[STUB_BUFFER_SIZE];
int gpioSetTimerFunc_return[STUB_BUFFER_SIZE];
/**
 * @brief gpioSetTimerFunc  Stub of gpioSetTimerFunc function.
 * @param timer     Dummy 1st argument.
 * @param millis    Dummy 2nd argument.
 * @param f         Dummy 3rd argument.
 * @return  Dummy value in gpioSetTimerFunc_return, int type array.
 */
int gpioSetTimerFunc(unsigned timer, unsigned millis, gpioTimerFunc_t f)
{
    gpioSetTimerFunc_timer[gpioSetTimerFunc_called_counter] = timer;
    gpioSetTimerFunc_millis[gpioSetTimerFunc_called_counter] = millis;
    gpioSetTimerFunc_gpioTimerFunc[gpioSetTimerFunc_called_counter] = f;

    int return_value = gpioSetTimerFunc_return[gpioSetTimerFunc_called_counter];
    gpioSetTimerFunc_called_counter++;

    return return_value;
}

unsigned int gpioSetISRFunc_called_counter;
unsigned int gpioSetISRFunc_gpio[STUB_BUFFER_SIZE];
unsigned int gpioSetISRFunc_edge[STUB_BUFFER_SIZE];
int gpioSetISRFunc_timeout[STUB_BUFFER_SIZE];
gpioISRFunc_t gpioSetISRFunc_f[STUB_BUFFER_SIZE];
int gpioSetISRFunc_return[STUB_BUFFER_SIZE];
int gpioSetISRFunc(unsigned gpio, unsigned edge, int timeout, gpioISRFunc_t f)
{
    gpioSetISRFunc_gpio[gpioSetISRFunc_called_counter] = gpio;
    gpioSetISRFunc_edge[gpioSetISRFunc_called_counter] = edge;
    gpioSetISRFunc_timeout[gpioSetISRFunc_called_counter] = timeout;
    gpioSetISRFunc_f[gpioSetISRFunc_called_counter] = f;

    int return_value = gpioSetISRFunc_return[gpioSetISRFunc_called_counter];
    gpioSetISRFunc_called_counter++;
    return return_value;
}

void pigpio_stub_init()
{
    int index = 0;

    //Initalize variable for gpioInitialise stub.
    gpioInitialise_called_counter = 0;
    for (index = 0; index < STUB_BUFFER_SIZE; index++) {
        gpioInitialise_return[index] = 0;
    }

    //Initialize variable for gpioTerminate stub.
    gpioTerminate_called_counter = 0;

    //gpioSetMode.
    gpioSetMode_called_counter = 0;
    for (index = 0; index < STUB_BUFFER_SIZE; index++) {
        gpioSetMode_gpio[index] = 0;
        gpioSetMode_mode[index] = 0;
        gpioSetMode_return[index] = 0;
    }

    //gpioRead
    gpioRead_called_counter = 0;
    for (index = 0; index < STUB_BUFFER_SIZE; index++) {
        gpioRead_gpio[index] = 0;
        gpioRead_return[index] = 0;
    }

    //gpioSetTimerFunc
    gpioSetTimerFunc_called_counter = 0;
    for (index = 0; index < STUB_BUFFER_SIZE; index++) {
        gpioSetTimerFunc_timer[index] = 0;
        gpioSetTimerFunc_millis[index] = 0;
        gpioSetTimerFunc_gpioTimerFunc[index] = NULL;
        gpioSetTimerFunc_return[index] = 0;
    }

    //gpioSetISRFunc
    gpioSetISRFunc_called_counter = 0;
    for (index = 0; index < STUB_BUFFER_SIZE; index++) {
        gpioSetISRFunc_gpio[index] = 0;
        gpioSetISRFunc_edge[index] = 0;
        gpioSetISRFunc_timeout[index] = 0;
        gpioSetISRFunc_f[index] = NULL;
        gpioSetISRFunc_return[index] = 0;
    }
}
