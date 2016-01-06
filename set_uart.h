/* 
 * File:   set_uart.h
 * Author: Naoy_Miyamoto
 *
 * Created on 2015/05/13, 2:00
 *
 */

#ifdef __XC16
#include <xc.h>
#endif

#include <stdio.h>


#define UART_CLK8MHZ_9600BPS 103 //set baurate to 9600
#define UART_CLK8MHZ_115200BPS 8 //set baurate to 115200
#define UART_CLK8MHZ_2400BPS 415 //set baurate to 115200

#ifndef SET_UART_H
#define	SET_UART_H

#ifdef	__cplusplus
extern "C" {
#endif

void uart_init(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SET_UART_H */

