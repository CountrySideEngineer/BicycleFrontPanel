/**
 * @file	slave_device_wheel.ino
 * @author	Country side engineer
 * @date	2019/05/04
 * @brief	Sketch for slave device to detect wheel rotate and velocity
 * 			by interruption by magnet and hall-sensor.
 */
#include "Arduino.h"
#include <SPI.h>

#define	INTERRUPT_PIN			(2)
#define	SPI_BUFFER_SIZE			(13)
#define	INTERRUPT_BUFFER_SIZE	(10)
#define	SECONDS_PER_MINUTE		((uint32_t)60)
#define	SEC_TO_MILLI_SEC(sec)	((uint32_t)(sec * 1000))
#define	MILLISEC_PER_MINUTE		(SEC_TO_MILLI_SEC(SECONDS_PER_MINUTE))
#define	LOOP_INTERVAL			(200)
#define	CONVERT_ROTATE_TO_RPM_MILLISEC(rotate, interval_millisec)	\
	((rotate * MILLISEC_PER_MINUTE) / interval_millisec)

int spiDataBufferIndex = 0;
byte spiDataBuffer[SPI_BUFFER_SIZE];
int interruptCountBufferIndex = 0;
word interruptCountBuffer[INTERRUPT_BUFFER_SIZE];

int spiCount = 0;
int interruptCount = 0;
uint32_t rotate;

/**
 * @brief	The setup function is called once at startup of the sketch.
 */
void setup()
{
	//Interrupt configuration.
	interruptCount = 0;
	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN),
			interruptHandle,
			RISING);

	//SPI configuration. - PINs
	pinMode(MISO, OUTPUT);
	pinMode(MOSI, INPUT);
	pinMode(SCK, INPUT);
	pinMode(SS, INPUT);

	//SPI configuration. - mode
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV64);	//125kHz
	SPI.begin();

	//SPI configuration. - register
	SPCR &= ~_BV(MSTR);
	SPCR |= _BV(SPE);
	SPI.attachInterrupt();

	//Serial configuration : just for debug.
	Serial.begin(9600);

	//Initialize variables.
	interruptCount = 0;
	for (int buffIndex = 0; buffIndex < SPI_BUFFER_SIZE; buffIndex++) {
		spiDataBuffer[buffIndex] = 0;
	}

	interruptCountBufferIndex = 0;
	for (int buffIndex = 0; buffIndex < INTERRUPT_BUFFER_SIZE; buffIndex++) {
		interruptCountBuffer[buffIndex] = 0;
	}

	rotate = 0;
}

#define _PI					(314)       //LSB = 0.01
#define METOR_PER_INCH			(254)		//LSB = 0.1mm
#define INCH_PER_M				(254)       //LSB = 0.1 mm
#define WHEEL_R_INCH			(26)        //LSB = 1.0
#define WHEEL_R_MM				((WHEEL_R_INCH) * (INCH_PER_MM))	//LSB = 0.01 * 0.1 = 1.0 * 10^(-3) mm
/*
 * The value 2073656 is result of calculation "3.14 * 26 * 25.4",
 * its LSB is 0.001[mm].
 * BUT, the value is over detailed.
 * So the value is rounded into 2073 by converting LSB into 1.0[mm]
 */
//#define WHEEL_CIRCUMFERENCE	(2073656)     //LSB = 0.001 [mm]
#define WHEEL_CIRCUMFERENCE	(2074)     //LSB = 1.000 [mm]

typedef struct _VELOCITY_DATA {
	uint32_t	rotate;
	uint32_t	velocityInteger;
	uint32_t	velocityDecade;
} VELOCITY_DATA;

/**
 * @brief	Calculate sum in 16bit data type buffer.
 */
uint32_t GetSum_16bit(uint16_t* buffer, int bufferSize)
{
	uint32_t sum = 0;
	for (int index = 0; index < bufferSize; index++) {
		sum += *buffer;
		buffer++;
	}

	return sum;
}

/**
 * @brief	The loop function is called in an endless loop
 * 			This loop is run for each 1000 msec (1 second).
 */
void loop()
{
	interruptCountBuffer[interruptCountBufferIndex] = interruptCount;
	interruptCountBufferIndex++;
	interruptCount = 0;

	if (!(interruptCountBufferIndex < INTERRUPT_BUFFER_SIZE)) {
		interruptCountBufferIndex = 0;
	}

	//A moving average filter.
	rotate = GetSum_16bit((uint16_t *)(&interruptCountBuffer[0]),
			INTERRUPT_BUFFER_SIZE);
	//Convert interrupt count into rotation number in a minute.
	rotate = CONVERT_ROTATE_TO_RPM_MILLISEC(rotate, LOOP_INTERVAL);
	rotate /= INTERRUPT_BUFFER_SIZE;

	/*
	 * The variable "interruptCount" shows the number of rotation in 1 second.
	 * The variable "rotate" shows the number of rotation in 1 minute.
	 * To modify the difference of unit, to multiple "interruptCount" by 60,
	 * second in a minute.
	 */
	uint32_t velocity = WHEEL_CIRCUMFERENCE * rotate;//LSB : mm/min
	velocity *= 60;		//LSB : mm/h
	uint32_t velocityIntegerPart = velocity / 1000;//Integer part of unit m/h.
	uint32_t velocityDecadePart =  velocity % 1000;//Decade part of unit m/h.

	//Setup data to send.
	VELOCITY_DATA* velocityData = (VELOCITY_DATA*)(&spiDataBuffer[0]);
	velocityData->rotate = rotate;
	velocityData->velocityDecade = velocityDecadePart;
	velocityData->velocityInteger = velocityIntegerPart;

	//Set check sum
	uint8_t checkSum = 0;
	for (int index = 0; index < (SPI_BUFFER_SIZE - 1); index++) {
		/*
		 * The upper byte of the data has no meaning as check sum, so the value
		 * is removed by casting data type from uint16_t to uint8_t.
		 */
		checkSum = (uint8_t)(
				(uint16_t)checkSum +
				(uint16_t)spiDataBuffer[index]);
	}
	spiDataBuffer[(SPI_BUFFER_SIZE - 1)] = checkSum;

	//Load buffer data to SPI register.
	SPDR = spiDataBuffer[0];
	spiDataBufferIndex = 1;

#define	DEBUG_PRINT
#ifdef DEBUG_PRINT
	//For debug.
	Serial.print("rotate = ");
	Serial.print(rotate);
	Serial.print(" : velocity = ");
	Serial.print(velocityIntegerPart);
	Serial.print(".");
	Serial.print(velocityDecadePart);
	Serial.print("  (");
	Serial.print(interruptCountBuffer[0]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[1]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[2]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[3]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[4]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[5]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[6]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[7]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[8]);
	Serial.print(", ");
	Serial.print(interruptCountBuffer[9]);
	Serial.print(") - (0x");
	Serial.print(spiDataBuffer[0], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[1], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[2], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[3], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[4], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[5], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[6], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[7], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[8], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[9], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[10], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[11], HEX);
	Serial.print(", 0x");
	Serial.print(spiDataBuffer[12], HEX);
	Serial.print(") - ");
	Serial.println(spiCount);
#endif	/* DEBUG_PRINT */
	delay(LOOP_INTERVAL);
}

/**
 * @brief	SPI interrupt handler, callback when the SPI interrupt detected
 * 			and a data has been stored on a register.
 */
ISR(SPI_STC_vect)
{
	SPDR = spiDataBuffer[spiDataBufferIndex];
	spiDataBufferIndex++;
	if (SPI_BUFFER_SIZE <= spiDataBufferIndex) {
		spiDataBufferIndex = 0;
	}
	spiCount++;
}

/**
 * @brief	Interrupt handler, callback when an interrupt detected.
 */
void interruptHandle()
{
	interruptCount++;
}
