/**
* @file buttons.h
* @brief Biblioteka do obslugi przyciskow
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/
#pragma once

#include <stdint-gcc.h>

// ******************************************************************************************************************************************************** //

/**
* @struct BUTTONS_ON_STEERINGWHEEL
* @brief Struktura globalna zawierajaca stany przyciskow
*/
typedef struct
{
  uint8_t halfGas;
  uint8_t fullGas;
  uint8_t mode1;
  uint8_t mode2;
  uint8_t horn;
  uint8_t speedReset;
  uint8_t powerSupply;
  uint8_t scClose;
  uint8_t fuelcellOff;
  uint8_t fuelcellPrepareToRace;
  uint8_t fuelcellRace;
} BUTTONS_ON_STEERINGWHEEL;
extern BUTTONS_ON_STEERINGWHEEL BUTTONS;

// ******************************************************************************************************************************************************** //

extern void buttons_step(void);
