/*
 * bmp085.c
 *
 *  Created on: 08/03/2015
 *      Author: Victor
 */

#include <math.h>
#include "bmp085.h"
#include "../twi_master.h"

extern void delay(uint32_t milliseconds);

// Variaveis para calibracao
uint16_t ac1, ac2, ac3, ac4, ac5, ac6, b1, b2, mb, mc, md;

// Variavel compartilhada
long b5;

// Variaveis para calculo de temperatura
uint16_t ut;

// Variaveis para calculo de pressao
uint32_t up;

//--------------------------------------
// "Oversampling Setting"
//--------------------------------------
uint8_t oss;
// 0 - Ultra low power        -> 4.5ms
// 1 - Standard               -> 7.5ms
// 2 - High Resolution        -> 13.5ms
// 3 - Ultra High Resolition  -> 25.5ms

// Pressão no nivel do mar (em Pa)
const uint32_t p0 = 101325;

uint8_t bmp085_detect(void) {
	uint8_t bmp085_RxBuffer[3] = {0, 0, 0};
	twi_master_readRegister(BMP085_ADDR, BMP085_REG_ID, bmp085_RxBuffer, sizeof(bmp085_RxBuffer));

	return (bmp085_RxBuffer[0] == BMP085_CHIP_ID);
}

void bmp085_config(uint8_t oss_value) {
	oss = oss_value;

	ac1 = twi_master_read16(BMP085_ADDR, 0xAA);
	ac2 = twi_master_read16(BMP085_ADDR, 0xAC);
	ac3 = twi_master_read16(BMP085_ADDR, 0xAE);
	ac4 = twi_master_read16(BMP085_ADDR, 0xB0);
	ac5 = twi_master_read16(BMP085_ADDR, 0xB2);
	ac6 = twi_master_read16(BMP085_ADDR, 0xB4);
	b1 = twi_master_read16(BMP085_ADDR, 0xB6);
	b2 = twi_master_read16(BMP085_ADDR, 0xB8);
	mb = twi_master_read16(BMP085_ADDR, 0xBA);
	mc = twi_master_read16(BMP085_ADDR, 0xBC);
	md = twi_master_read16(BMP085_ADDR, 0xBE);
}

/*
 * Ler temperatura descompensada
 */
void bmp085_ut(void) {
	uint8_t reg = BMP085_TEMPERATURE_REG;
	twi_master_writeRegister(BMP085_ADDR, BMP085_CTRL_REG, &reg, sizeof(reg));
	delay(5);
	ut = twi_master_read16(BMP085_ADDR, BMP085_MSB_REG);
}

/*
 * Ler pressão descompensada
 */
void bmp085_up(void) {
	uint8_t msb, lsb, xlsb;
	uint8_t bmp085_RxBuffer[3] = {0, 0, 0};
	uint8_t data = BMP085_PRESSURE_REG + (oss<<6);
	twi_master_writeRegister(BMP085_ADDR, BMP085_CTRL_REG, &data, sizeof(data));
	delay(2 + (3<<oss)); // Delay depois de TX de ~ 2.0ms + (3<<OSS)
	twi_master_readRegister(BMP085_ADDR, BMP085_MSB_REG, bmp085_RxBuffer, sizeof(bmp085_RxBuffer));

	msb = bmp085_RxBuffer[0];
	lsb = bmp085_RxBuffer[1];
	xlsb = bmp085_RxBuffer[2];

	up = ( ( (uint32_t) msb << 16) | ( (uint16_t) lsb << 8) | xlsb ) >> (8-oss);
}

/*
 * Ler temperatura real (por 0.1ºC)
 */
void bmp085_t(long* temp) {
	long x1, x2;
	x1 = (ut - ac6) * (long)ac5 >> 15;
	x2 = ((long)mc << 11) / (x1 + md);
	b5 = x1 + x2;
	*temp = ((b5 + 8) >> 4);
}

/*
 * Calcular a pressão real (por Pa)
 */
void bmp085_p(long* pressure) {
	long x1, x2, x3, b3, b6, p;
	unsigned long b4, b7;

	b6 = b5 - 4000;

	// Calculate B3
	x1 = (b2 * (b6 * b6)>>12)>>11;
	x2 = (ac2 * b6)>>11;
	x3 = x1 + x2;
	b3 = (((((long)ac1)*4 + x3)<<oss) + 2)>>2;

	// Calculate B4
	x1 = (ac3 * b6)>>13;
	x2 = (b1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

	b7 = ((unsigned long)(up - b3) * (50000>>oss));
	if (b7 < 0x80000000)
		p = (b7<<1)/b4;
	else
		p = (b7/b4)<<1;

	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	p += (x1 + x2 + 3791)>>4;

	*pressure = p;
}

/*
 * Ler a temperatura e a pressão
 */
void bmp085_read(long* temp, long* pressure) {
	bmp085_ut();
	bmp085_up();
	bmp085_t(temp);
	bmp085_p(pressure);
}

/*
 * Ler a altitude com a pressão lida do BMP085
 */
float bmp085_readAltitude(long pressure) {
	return (float)44330 * (1 - powf(((float) pressure/p0), 0.190295));
}

