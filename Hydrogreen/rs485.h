/**
* @file rs485.h
* @brief Biblioteka do obslugi komunikacji UART <-> RS485 <-> UART
* @author Piotr Durakiewicz
* @date 08.12.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/
#pragma once

#include <stdint-gcc.h>

// ******************************************************************************************************************************************************** //

#define RS485_FLT_NONE 0x00					///< Brak bledu
#define RS485_NEW_DATA_TIMEOUT 0x11				///< Nie otrzymano nowych dane (polaczenie zostalo zerwane)

extern uint8_t rs485_flt; 					///< Zmienna przechowujaca aktualny kod bledu magistrali

// ******************************************************************************************************************************************************** //

extern void rs485_init(void);					///< Inicjalizacja magistrali RS-485, umiescic wewnatrz hydrogreen_init(void)
extern void rs485_step(void);					///< Funkcja obslugujaca magistrale, umiescic wewnatrz hydrogreen_step(void)

// ******************************************************************************************************************************************************** //

/**
* @struct RS485_RECEIVED_VERIFIED_DATA
* @brief Struktura zawierajaca sprawdzone otrzymane dane
*/
typedef struct
{
  ///< ELEMENTY W STRUKTURZE MUSZA BYC POSORTOWANE W PORZADKU MALEJACYM
  ///< https://www.geeksforgeeks.org/is-sizeof-for-a-struct-equal-to-the-sum-of-sizeof-of-each-member/
  union
  {
    float value;
    char array[4];
  } FC_V;

  union
  {
    float value;
    char array[4];
  } FC_TEMP;

  union
  {
    float value;
    char array[4];
  } CURRENT_SENSOR_FC_TO_SC;

  union
  {
    float value;
    char array[4];
  } CURRENT_SENSOR_SC_TO_MOTOR;

  union
  {
    float value;
    char array[4];
  } SC_V;

  union
  {
    float value;
    char array[4];
  } H2_SENSOR_V;

  union
  {
    uint16_t value;
    char array[2];
  } laptime_minutes;

  union
  {
    uint16_t value;
    char array[2];
  } laptime_miliseconds;

  union
  {
    uint16_t value;
    char array[2];
  } fcFanRPM;

  uint8_t interimSpeed;
  uint8_t averageSpeed;
  uint8_t laptime_seconds;

  uint8_t electrovalve;
  uint8_t purgeValve;

  uint8_t fcToScMosfetPWM;
  uint8_t motorPWM;

  uint8_t h2SensorDigitalPin;
  uint8_t emergencyButton;
} RS485_RECEIVED_VERIFIED_DATA;
extern RS485_RECEIVED_VERIFIED_DATA RS485_RX_VERIFIED_DATA;
