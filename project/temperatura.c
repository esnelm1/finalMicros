/***************************************************************************//**
 @file     temperatura.c
 @brief    temperatura for .c file
 @author   Conradeus
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "temperatura.h"
#include "onewire.h"
#include "common.h"
#include "timer.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CONVERT_T               0x44  // Comando para que el sensor convierta la temperatura a un valor digital
#define READ_SCRATCHPAD         0xBE  // Leer la temperatura digital
#define SENSOR_RESOLUTION       625/10000 //0.0625 
#define CONVERTION_TIME         750 //ms 
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t convert(uint16_t);
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint8_t temp_lsb;
static uint8_t temp_msb;
static uint16_t temp_read; 
/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
uint16_t readTemperatura(void){
    //reset y conversion de temperatura a valor digital
    oneWire_singleReset();
    oneWire_singleWrite(SKIP_ROM);
    oneWire_singleWrite(CONVERT_T);

    //espero un tiempo para darle tiempo al sensor que convierta
    timerDelay(CONVERTION_TIME);

    //reset y leo el valor digital
    oneWire_singleReset();
    oneWire_singleWrite(SKIP_ROM);
    oneWire_singleWrite(READ_SCRATCHPAD);

    temp_lsb = oneWire_singleRead();
    temp_msb = oneWire_singleRead();

    temp_read = (temp_msb << 8) | temp_lsb; //dato del sensor en escala de 0.0625°C(ver datasheet)

    return convert(temp_read);//convierto a 4 cifras. 0445 es 44.5°C

}

/*******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

//OJO EL TEMA DEL PUNTO FLOTANTE Y DEMAS
static uint16_t convert(uint16_t temp_raw) {
    // scale raw measure
    uint16_t temp_scaled = (temp_raw * 10) / 16;
    return temp_scaled; //
}



/******************************************************************************/

