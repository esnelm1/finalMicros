///***************************************************************************//**
// @file     commpc.c
// @brief    commpc for .c file
// @author   Conradeus
// ******************************************************************************/
//
///*Este archivo es para armar la comunicacion con la computadora. La comunicacion es
//por uart. Hay que ver de hacer funciones para enviar y transmitir datos, y leerlas
//con una interfaz de matlab. Hay que ver como hacer que sea robusta y no pierda datos*/
//
///*******************************************************************************
// * INCLUDE HEADER FILES
// ******************************************************************************/
//#include "commpc.h"
//#include "cqueue.h"
//#include "UART.h"
//#include "calefactor.h"
//#include "board.h"
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//
///*******************************************************************************
// * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
// ******************************************************************************/
//
///*******************************************************************************
// * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
// ******************************************************************************/
//
///*******************************************************************************
// * VARIABLES WITH GLOBAL SCOPE
// ******************************************************************************/
//
///*******************************************************************************
// * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
// ******************************************************************************/
//
//
///*******************************************************************************
// * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
// ******************************************************************************/
//
///*******************************************************************************
// *******************************************************************************
// GLOBAL FUNCTION DEFINITIONS
// *******************************************************************************
// ******************************************************************************/
//
//// FUNCIONES DE LECTURA DE VOLTAJE DEL SENSOR DE TEMPERATURA - LA TRANSFORMACION DEL VALOR
//// DE TEMPERATURA SE PUEDE HACER ACA O EN MATLAB. SI LO HACEMOS ACA ES MAS ROBUSTO PERO
//// TENEMOS MENOS HERRAMIENTAS MATEMATICAS. YO LO HARIA CACA
//
//// FUNCION DE LECTURA DE ESTADO DE CALEFACTOR. SE DEBE USAR calefactor_status
//
///*******************************************************************************
// GLOBAL FUNCTION DEFINITIONS
// *******************************************************************************
// ******************************************************************************/
//
//
//char check_comunicacion(void)
//{
//    return rx_datosFlag;  // Establece comunicacion con Matlab
//}
//
//void get_setpoint(unsigned char* sp)
//{
//    *sp = uart_get_char();
//}
//
//void get_histeresis(unsigned char* h)
//{
//    *h = uart_get_char();
//}
//
//void get_intMuestra(unsigned int* im)
//{
//    unsigned char lowByte = uart_get_char();   // Lee el byte bajo
//    unsigned char highByte = uart_get_char();  // Lee el byte alto
//
//    // Combina los dos bytes
//    *im = ((unsigned int)highByte << 8) | lowByte;
//}
//
//
//void LED_status(int value){
//    switch (value)
//        {
//        case 0: //funcionamiento error
//            gpioWrite(LED_STATUS,LOW);
//            break;
//
//        case 1: //funcionamiento normal
//            gpioWrite(LED_STATUS,HIGH);
//            break;
//        }
//}
//
//int recibe_parametros(unsigned char *sp, unsigned char *h, unsigned int *im)
//{
//    // Se esperan4 bytes en el buffer
//    if (rx_datosFlag)
//    {
//        get_setpoint(sp);
//        get_histeresis(h);
//        get_intMuestra(im);
//        rx_datosFlag = 0;
//        return 1;
//    }
//    return 0;
//}
//
//
//
//
//
///*******************************************************************************
// LOCAL FUNCTION DEFINITIONS
// *******************************************************************************
// ******************************************************************************/
//
//
//
///******************************************************************************/
//
