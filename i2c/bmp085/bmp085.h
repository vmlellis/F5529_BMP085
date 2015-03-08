/*
 * bmp085.h
 *
 *  Created on: 07/03/2015
 *      Author: Victor
 */

#ifndef I2C_BMP085_BMP085_H_
#define I2C_BMP085_BMP085_H_

#include <inttypes.h>

#define BMP085_ADDR                 0x77 // Endereço I2C
#define BMP085_CTRL_REG             0xF4 // Registrador de Controle

// Control Register valoues
#define BMP085_TEMPERATURE_REG      0x2E // Registrador da Temperatura - delay de 4.5ms
#define BMP085_PRESSURE_REG         0x34 // Registrador de pressão (osrs = 0) - delay de 4.5ms
#define BMP085_PRESSURE_REG_OSS1    0x74 // Registrador de pressão (osrs = 1) - delay de 7.5ms
#define BMP085_PRESSURE_REG_OSS2    0xB4 // Registrador de pressão (osrs = 2) - delay de 13.5ms
#define BMP085_PRESSURE_REG_OSS3    0xF4 // Registrador de pressão (osrs = 3) - delay de 25.5ms

#define BMP085_MSB_REG	            0xF6 // Registrador MSB
#define BMP085_LSB_REG	            0xF7 // Registrador LSB
#define BMP085_CONV_REG_XLSB        0xF8 // Registrador XLSB

// Bosch BMP085 chip ID
#define BMP085_CHIP_ID   			0x55

// Registradores não definidos no datasheet, mas achados em exemplos
#define BMP085_REG_ID    0xD0  // chip ID register
#define BMP085_REG_VER   0xD1  // chip version register
#define BMP085_REG_RST   0xE0  // soft reset

void bmp085_config(uint8_t);		// Calibração do BMP085
uint8_t bmp085_detect(void); 		// Verificar a presença do BMP085
void bmp085_read(long*, long*);		// Calcula a temperatura e a pressão

#endif /* I2C_BMP085_BMP085_H_ */
