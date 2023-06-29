/**
* @file Nextion_Enhanced_Expansion_Board.h
* @brief Biblioteka do obslugi rozszerzenia dla rozszerzenia Nextion Enhanced Expansion Board
* @details https://elty.pl/pl/p/Rozszerzenie-dla-wyswietlaczy-z-serii-Nextion-Enhanced-/1814
* @author Mateusz Gilewski
* @date 04.01.2021
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#pragma once

#include "stdint.h"

extern uint8_t Nextion_Enhanced_Expansion_Board_configureGPIO(uint8_t io, uint8_t mode, uint8_t comp);
extern uint8_t Nextion_Enhanced_Expansion_Board_pinState(uint8_t io, uint8_t state);
