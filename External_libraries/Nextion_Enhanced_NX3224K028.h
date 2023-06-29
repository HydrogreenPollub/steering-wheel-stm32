/**
* @file Nextion_Enhanced_NX3224K028.h
* @brief Biblioteka do obslugi wyswietlacza Nextion Enhanced 2,8" NX3224K028.
* @details https://elty.pl/pl/p/Wyswietlacz-Nextion-Enhanced-2%2C8-320x240-NX3224K028-rezystancyjny-panel-dotykowy/1815
* @author Piotr Durakiewicz
* @date 28.12.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#pragma once

#include "stdint.h"

#define UART_PORT_Nextion 		huart1
#define USART_Nextion 			USART1

extern uint8_t dataToWrite[100];

extern uint8_t Nextion_Enhanced_NX3224K028_writeTxtToControl(const uint8_t *controlName, const uint8_t *valueToWrite);
extern uint8_t Nextion_Enhanced_NX3224K028_writeNumberToControl(const uint8_t *controlName, uint16_t valueToWrite);
extern uint8_t Nextion_Enhanced_NX3224K028_writeFloatToControl(const uint8_t *controlName, float valueToWrite);
extern uint8_t Nextion_Enhanced_NX3224K028_writeFltToControl(const uint8_t *controlName, uint8_t valueToWrite);
extern uint8_t Nextion_Enhanced_NX3224K028_writeValueToProgressBar(const uint8_t *controlName, uint8_t value, uint8_t maxAllowableValue);
extern uint8_t Nextion_Enhanced_NX3224K028_drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t *color);
extern uint8_t Nextion_Enhanced_NX3224K028_changeControlColor(const uint8_t *controlName, uint16_t color_value_565);
extern uint8_t Nextion_Enhanced_NX3224K028_loadNewPage(uint8_t pageId);
extern uint8_t Nextion_Enhanced_NX3224K028_setBacklight(uint8_t dimPercentValue);
extern uint8_t Nextion_Enhanced_NX3224K028_dispResoursePicture(uint16_t xPos, uint16_t yPos, uint8_t picId);
extern uint8_t Nextion_Enhanced_NX3224K028_setPassFailReturnData(uint8_t bkcmdValue);
extern uint8_t Nextion_Enhanced_NX3224K028_setBaudRate(uint32_t baudrateValue);
extern uint8_t Nextion_Enhanced_NX3224K028_deviceReset(void);
extern uint8_t Nextion_Enhanced_NX3224K028_removeBytesFromSerialBuffer(uint16_t numberOfBytesToRemove);
