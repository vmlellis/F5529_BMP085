```
//******************************************************************************
//  MSP430F5529 Demo - BMP085
//
//  This demo connects the MSP430 and BMP085 via the I2C bus. 
//  The master transmits to the slave. This is the MASTER CODE.
//  Check if BMP085 is enaled.
//
//
//                                /|\  /|\
//                   BMP085       10k  10k      MSP430F5529
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|              SDA|<-|----+->|P3.0/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|                 |  |       |             XOUT|-
//            |              SCL|<-+------>|P3.1/UCB0SCL     |
//            |                 |          |                 |
//
//   Victor Lellis
//   March 2015
//   Built with CCSv6 with GNU v4.9.1 (Red Hat)
//******************************************************************************
```