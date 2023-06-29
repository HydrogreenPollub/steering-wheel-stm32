/**
* @file Nextion_Enhanced_NX3224K028.c
* @brief Biblioteka do obslugi wyswietlacza Nextion Enhanced 2,8" NX3224K028.
* @details https://elty.pl/pl/p/Wyswietlacz-Nextion-Enhanced-2%2C8-320x240-NX3224K028-rezystancyjny-panel-dotykowy/1815
* @author Piotr Durakiewicz
* @date 28.12.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#include <stdint-gcc.h>
#include <stdio_ext.h>
#include <strings.h>
#include "Nextion_Enhanced_NX3224K028.h"
#include "usart.h"

uint8_t dataToWrite[100];

/*
 * Modify txt value in control:
 * ex. Nextion_Enhanced_NX3224K028_writeTxtToControl((const uint8_t *)"t0", (const uint8_t *)"20:12");
 */
uint8_t Nextion_Enhanced_NX3224K028_writeTxtToControl(const uint8_t *controlName, const uint8_t *valueToWrite)
{
  uint16_t size = sprintf((char*)dataToWrite, "%s.txt=\"%s\"", controlName, valueToWrite);

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

/*
 * Modify number value in control:
 * ex. Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t *)"n0", 25);
 */
uint8_t Nextion_Enhanced_NX3224K028_writeNumberToControl(const uint8_t *controlName, uint16_t valueToWrite)
{
  uint8_t size = sprintf((char*)dataToWrite, "%s.val=%d", controlName, valueToWrite);

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

/*
 * Modify number value in control:
 * ex. Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t *)"x0", 25.55);
 */
uint8_t Nextion_Enhanced_NX3224K028_writeFloatToControl(const uint8_t *controlName, float valueToWrite)
{
  uint16_t size = sprintf((char*)dataToWrite, "%s.txt=\"%.2f\"", controlName, valueToWrite);

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

/*
 * Modify number value in control:
 * ex. Nextion_Enhanced_NX3224K028_writeFltToControl((const uint8_t *)"x0", 25.55);
 */
uint8_t Nextion_Enhanced_NX3224K028_writeFltToControl(const uint8_t *controlName, uint8_t valueToWrite)
{
  uint16_t size = sprintf((char*)dataToWrite, "%s.txt=\"0x%.2X\"", controlName, valueToWrite);

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

/*
 * Modify progress bar value in control:
 * ex. Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t *)"j0", 76);
 */
uint8_t Nextion_Enhanced_NX3224K028_writeValueToProgressBar(const uint8_t *controlName, uint8_t value, uint8_t maxAllowableValue)
{
  // Wykonaj mapowanie

  uint16_t valueToWrite = (100 * value) / maxAllowableValue;

  uint8_t size = sprintf((char*)dataToWrite, "%s.val=%d", controlName, valueToWrite);

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

/*
 * Modify txt value in control:
 * ex. Nextion_Enhanced_NX3224K028_changeControlColor((const uint8_t *)"electrovalve", 31);
 */
uint8_t Nextion_Enhanced_NX3224K028_changeControlColor(const uint8_t *controlName, uint16_t color_value_565)
{
  uint16_t size = sprintf((char*)dataToWrite, "%s.pco=%d", controlName, color_value_565);

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

uint8_t Nextion_Enhanced_NX3224K028_drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t *color)
{
  uint16_t size = sprintf((char*)dataToWrite, "draw %d,%d,%d,%d,%s", x1, y1, x2, y2, color);

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

/*
 * Change picture on LCD
 * ex. Nextion_Enhanced_NX3224K028_dispResoursePicture(8, 192, 3);
 */
uint8_t Nextion_Enhanced_NX3224K028_dispResoursePicture(uint16_t xPos, uint16_t yPos, uint8_t picId)
{
  uint8_t size = sprintf((char*)dataToWrite, "pic %d,%d,%d", xPos, yPos, picId);

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

/*
 * Change using page:
 * ex. Nextion_Enhanced_NX3224K028_loadNewPage(1);
 */

uint8_t Nextion_Enhanced_NX3224K028_loadNewPage(uint8_t pageId)
{
  uint8_t size = sprintf((char*)dataToWrite, "page %d", pageId);

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

/*
 * Change brightness:
 * ex. Nextion_Enhanced_NX3224K028_setBacklight(100); means 100%
 */
uint8_t Nextion_Enhanced_NX3224K028_setBacklight(uint8_t dimPercentValue)
{
  uint8_t size = sprintf((char*)dataToWrite, "dims=%d", dimPercentValue);

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

/*
 * Change Pass/Fail return data:
 * ex. Nextion_Enhanced_NX3224K028_setPassFailReturnData(2); level of Return Data on commands processed over Serial.
 */
uint8_t Nextion_Enhanced_NX3224K028_setPassFailReturnData(uint8_t bkcmdValue)
{
  uint8_t size = sprintf((char*)dataToWrite, "bkcmd=%d", bkcmdValue);

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

/*
 * Change baudrate:
 * ex. Nextion_Enhanced_NX3224K028_setBaudRate(921600); means 921600 bits/s
 */
uint8_t Nextion_Enhanced_NX3224K028_setBaudRate(uint32_t baudrateValue)
{
  UART_PORT_Nextion.Instance = USART_Nextion;
  UART_PORT_Nextion.Init.BaudRate = 9600;
  UART_PORT_Nextion.Init.WordLength = UART_WORDLENGTH_8B;
  UART_PORT_Nextion.Init.StopBits = UART_STOPBITS_1;
  UART_PORT_Nextion.Init.Parity = UART_PARITY_NONE;
  UART_PORT_Nextion.Init.Mode = UART_MODE_TX;
  UART_PORT_Nextion.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART_PORT_Nextion.Init.OverSampling = UART_OVERSAMPLING_8;
  UART_PORT_Nextion.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
  UART_PORT_Nextion.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  UART_PORT_Nextion.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  UART_PORT_Nextion.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&UART_PORT_Nextion) != HAL_OK)
  {
    Error_Handler();
    return 0;
  }

  uint8_t size = sprintf((char*)dataToWrite, "bauds=%ld", baudrateValue);

  dataToWrite[size] = 0xFF;
  dataToWrite[size + 1] = 0xFF;
  dataToWrite[size + 2] = 0xFF;

  HAL_UART_Transmit(&UART_PORT_Nextion, dataToWrite, size + 3, HAL_MAX_DELAY);


  HAL_UART_DeInit(&UART_PORT_Nextion);

  UART_PORT_Nextion.Instance = USART_Nextion;
  UART_PORT_Nextion.Init.BaudRate = baudrateValue;
  UART_PORT_Nextion.Init.WordLength = UART_WORDLENGTH_8B;
  UART_PORT_Nextion.Init.StopBits = UART_STOPBITS_1;
  UART_PORT_Nextion.Init.Parity = UART_PARITY_NONE;
  UART_PORT_Nextion.Init.Mode = UART_MODE_TX;
  UART_PORT_Nextion.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART_PORT_Nextion.Init.OverSampling = UART_OVERSAMPLING_8;
  UART_PORT_Nextion.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
  UART_PORT_Nextion.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  UART_PORT_Nextion.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  UART_PORT_Nextion.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&UART_PORT_Nextion) != HAL_OK)
  {
    Error_Handler();
    return 0;
  }

  return 1;
}

/*
 * Removes bytes from Serial Buffer:
 * ex. Nextion_Enhanced_NX3224K028_removeBytesFromSerialBuffer(24); delete first 24 bytes on Buffer
 */
uint8_t Nextion_Enhanced_NX3224K028_removeBytesFromSerialBuffer(uint16_t numberOfBytesToRemove)
{
  uint8_t size = sprintf((char*)dataToWrite, "udelete %d", numberOfBytesToRemove);

  dataToWrite[size] = 0xFF;
  dataToWrite[size + 1] = 0xFF;
  dataToWrite[size + 2] = 0xFF;

  HAL_UART_AbortTransmitCpltCallback(&UART_PORT_Nextion);

  HAL_UART_Transmit(&UART_PORT_Nextion, dataToWrite, size + 3, HAL_MAX_DELAY);

  /*
  if (HAL_UART_GetState(&UART_PORT_Nextion) == HAL_UART_STATE_READY)
    {
      HAL_UART_Transmit_DMA(&UART_PORT_Nextion, dataToWrite, size + 3);
      return 1;
    }
    */

  return 0;
}


/*
 * This function reset device:
 * ex. Nextion_Enhanced_NX3224K028_deviceReset();
 */
uint8_t Nextion_Enhanced_NX3224K028_deviceReset(void)
{
  uint8_t size = sprintf((char*)dataToWrite, "rest");

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
