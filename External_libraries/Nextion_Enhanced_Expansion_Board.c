/**
* @file Nextion_Enhanced_Expansion_Board.c
* @brief Biblioteka do obslugi rozszerzenia Nextion Enhanced Expansion Board
* @details https://elty.pl/pl/p/Rozszerzenie-dla-wyswietlaczy-z-serii-Nextion-Enhanced-/1814
* @author Mateusz Gilewski
* @date 04.01.2021
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#include <stdint-gcc.h>
#include <stdio_ext.h>
#include <strings.h>
#include "Nextion_Enhanced_Expansion_Board.h"
#include "Nextion_Enhanced_NX3224K028.h"
#include "usart.h"

/**
* @fn Nextion_Enhanced_Expansion_Board_configureGPIO(uint8_t io, uint8_t mode, uint8_t comp)
* @brief Funkcja konfigurujaca piny rozszerzenia (mode: 0-pull up input, 1-input binding, 2-push pull output, 3-PWM output, 4-open drain output)
*/
uint8_t Nextion_Enhanced_Expansion_Board_configureGPIO(uint8_t io, uint8_t mode, uint8_t comp)
{
  uint16_t size = sprintf((char*)dataToWrite, "cfgpio %d,%d,%d", io, mode, comp);

  dataToWrite[size] = 0xFF;
  dataToWrite[size + 1] = 0xFF;
  dataToWrite[size + 2] = 0xFF;

  if (HAL_UART_GetState(&UART_PORT_Nextion) == HAL_UART_STATE_READY)
    {
      HAL_UART_Transmit_DMA(&UART_PORT_Nextion, dataToWrite, size + 3);
      return 1;
    }

  return 0;
}

/**
* @fn Nextion_Enhanced_Expansion_Board_pinState(uint8_t io, uint8_t state)
* @brief Funkcja ustawiajaca stan wybranego pinu IO
*/
uint8_t Nextion_Enhanced_Expansion_Board_pinState(uint8_t io, uint8_t state)
{
  uint16_t size = sprintf((char*)dataToWrite, "pio%d=%d", io, state);

  dataToWrite[size] = 0xFF;
  dataToWrite[size + 1] = 0xFF;
  dataToWrite[size + 2] = 0xFF;

  if (HAL_UART_GetState(&UART_PORT_Nextion) == HAL_UART_STATE_READY)
    {
      HAL_UART_Transmit_DMA(&UART_PORT_Nextion, dataToWrite, size + 3);
      return 1;
    }

  return 0;
}
