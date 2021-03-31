#ifndef HARDWAREPROFILE_H_INCLUDED
#define HARDWAREPROFILE_H_INCLUDED

#include <xc.h>
#include "system.h"
#define LED_RUN_TRIS TRISAbits.TRISA10
#define LED_RUN LATAbits.LATA10
#define RELE1_TRIS TRISGbits.TRISG7
#define RELE1 LATGbits.LATG7
#define RELE2_TRIS TRISGbits.TRISG6
#define RELE2 LATGbits.LATG6
#define RELE3_TRIS TRISDbits.TRISD5
#define RELE3 LATDbits.LATD5
#define IN1_TRIS TRISBbits.TRISB9
#define IN1 PORTBbits.RB9
#define IN2_TRIS TRISBbits.TRISB8
#define IN2 PORTBbits.RB8
#define IN3_TRIS TRISCbits.TRISC13
#define IN3 PORTCbits.RC13
#define INT0_TRIS TRISBbits.TRISB7
#define INT0 PORTBbits.RB7
#define INT1_TRIS TRISBbits.TRISB12
#define INT1 PORTBbits.RB12
#define INT2_TRIS TRISBbits.TRISB13
#define INT2 PORTBbits.RB13
#define WRITE_PROTECT_TRIS TRISEbits.TRISE15
#define WRITE_PROTECT LATEbits.LATE15
#define I2C_CLK_TRIS TRISBbits.TRISB4
#define I2C_CLK LATBbits.LATB4
#define I2C_DATA_TRIS TRISAbits.TRISA8
#define I2C_DATA_OUT LATAbits.LATA8
#define I2C_DATA_IN PORTAbits.RA8
#define LED2_TRIS TRISCbits.TRISC10
#define LED2 LATCbits.LATC10
#define LED1_TRIS TRISBbits.TRISB6
#define LED1 LATBbits.LATB6
#define LED5_TRIS TRISBbits.TRISB5
#define LED5 LATBbits.LATB5
#define LED4_TRIS TRISDbits.TRISD8
#define LED4 LATDbits.LATD8
#define LED3_TRIS TRISCbits.TRISC5
#define LED3 LATCbits.LATC5
#define BUTTON1_TRIS TRISAbits.TRISA4
#define BUTTON1_PORT PORTAbits.RA4
#define BUTTON2_TRIS TRISAbits.TRISA9
#define BUTTON2_PORT PORTAbits.RA9
#define BUTTON3_TRIS TRISCbits.TRISC3
#define BUTTON3_PORT PORTCbits.RC3
#define BUTTON4_TRIS TRISCbits.TRISC4
#define BUTTON4_PORT PORTCbits.RC4
#define PWM1_TRIS TRISAbits.TRISA7
#define PWM1_LAT LATAbits.LATA7
#define PWM2_TRIS TRISBbits.TRISB15
#define PWM2_LAT LATBbits.LATB15
#define PWM3_TRIS TRISBbits.TRISB14
#define PWM3_LAT LATBbits.LATB14
#define DIR_TRIS TRISCbits.TRISC7
#define DIR_LAT LATCbits.LATC7

#define  UART_RX_TRIS TRISCbits.TRISC8
#define  UART_TX_TRIS TRISCbits.TRISC6
#define  ADC_CHANNEL 14
#define ADC_TRIS TRISEbits.TRISE14




#endif

