#ifdef __XC32
#include <plib.h>
#endif
#ifdef __XC16
#include <xc.h>
#endif

#include <stdio.h>
#include "set_uart.h"
#include "uart.h"
#include "PPS.h"


void uart_init(void)
{
#if defined(__XC32) || defined(__XC16)
   
    //port settings
    AD1PCFG = 0xFFFF;
    TRISB = 0x70A0;

    //set UART1 port tx:RP5 2pin,  rx:RP6 3pi
    //RPINR18bits.U1RXR = 6; //rx
    //enRPOR2bits.RP5R = 3;  //tx
    iPPSOutput( OUT_PIN_PPS_RP5, OUT_FN_PPS_U1TX ); // RP5? UART1?TXD?????
    iPPSInput(  IN_FN_PPS_U1RX,  IN_PIN_PPS_RP6  ); // RP6? UART1?RXD?????



//----------------------
    // UART1?????
    //----------------------
    CloseUART1();

    unsigned int config1 =  UART_EN &               // UART??
                            UART_IDLE_CON &         // ???????????
                            UART_DIS_WAKE &         // ??????Wakeup????
                            UART_IrDA_DISABLE &
                            UART_DIS_LOOPBACK &     // ??????????
                            UART_NO_PAR_8BIT &      // ??????8bit
                            UART_1STOPBIT &         // ???????1
                            UART_MODE_SIMPLEX  &    // ???????????
                            UART_UEN_00 &
                            UART_UXRX_IDLE_ONE &    // U1RX??????1???
                            UART_BRGH_SIXTEEN &     // ???????
                            UART_DIS_ABAUD;         // ?????????????

    unsigned int config2 =  UART_INT_TX_BUF_EMPTY &  // TXD????????????????
                            UART_IrDA_POL_INV_ZERO & // U1TX?????0???
                            UART_TX_ENABLE &         // TXD???(???RXD??????
                            UART_INT_RX_CHAR &       // RXD???????????
                            UART_ADR_DETECT_DIS &
                            UART_RX_OVERRUN_CLEAR;

    OpenUART1(config1, config2, UART_CLK8MHZ_9600BPS );

    ConfigIntUART1( UART_RX_INT_DIS & UART_TX_INT_DIS );
#endif
}

