#ifndef GPIO_PIN_CONFIG_H
#define GPIO_PIN_CONFIG_H

//Define GPIO pin No. config as MACRO.
#define GPIO_PIN_05                     (5)
#define GPIO_PIN_06                     (6)
#define GPIO_PIN_07                     (7)
#define GPIO_PIN_08                     (8)
#define GPIO_PIN_13                     (13)
#define GPIO_PIN_14                     (14)
#define GPIO_PIN_15                     (15)
#define GPIO_PIN_18                     (18)
#define GPIO_PIN_19                     (19)
#define GPIO_PIN_26                     (26)

#define SPI_MAIN_CE0                    (0)
#define SPI_MAIN_CE1                    (1)

#define GPIO_PIN_FRONT_BRAKE            GPIO_PIN_19
#define GPIO_PIN_REAR_BRAKE             GPIO_PIN_26
#define GPIO_PIN_OPTION_FRONT_BRAKE     GPIO_PIN_14
#define GPIO_PIN_OPTION_REAR_BRAKE      GPIO_PIN_15
#define GPIO_PIN_FRONT_WHEEL            SPI_MAIN_CE0
#define GPIO_PIN_REAR_WHEEL             SPI_MAIN_CE1
//#define GPIO_PIN_WHEEL_ROTATION         GPIO_PIN_13
//#define GPIO_PIN_WHEEL_VELOCITY         GPIO_PIN_06
#define GPIO_PIN_LIGHT                  GPIO_PIN_05

#define PIN_WHEEL                       ((int)CGpio::CSpiMode::SPI_CE::SPI_CE_0)
#define PIN_FRONT_WHEEL                 ((int)CGpio::CSpiMode::SPI_CE::SPI_CE_0)
#define PIN_REAR_WHEEL                  ((int)CGpio::CSpiMode::SPI_CE::SPI_CE_1)
#define PIN_FRONT_BRAKE                 (GPIO_PIN_FRONT_BRAKE)
#define PIN_REAR_BRAKE                  (GPIO_PIN_REAR_BRAKE)
#define PIN_LIGHT                       (GPIO_PIN_LIGHT)

#endif // GPIO_PIN_CONFIG_H
