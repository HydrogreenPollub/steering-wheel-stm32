/**
* @file buttons.c
* @brief Biblioteka do obslugi przyciskow
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#include "buttons.h"
#include "gpio.h"

// ******************************************************************************************************************************************************** //

BUTTONS_ON_STEERINGWHEEL BUTTONS; 	///< Struktura globalna zawierajaca stany przyciskow

// ******************************************************************************************************************************************************** //

/**
* @fn buttons_step(void)
* @brief Funkcja sprawdzajaca stany przyciskow przy kazdym obiegu petli, powinna zostac wywolana wewnatrz hydrogreen_step()
*/
void buttons_step(void)
{
  if (HAL_GPIO_ReadPin(GPIOA, HALF_GAS_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.halfGas = 1;
    }
  else
    {
      BUTTONS.halfGas = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOA, FULL_GAS_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.fullGas = 1;
    }
  else
    {
      BUTTONS.fullGas = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOA, MODE_1_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.mode1 = 1;
    }
  else
    {
      BUTTONS.mode1 = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOB, MODE_2_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.mode2 = 1;
    }
  else
    {
      BUTTONS.mode2 = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOB, HORN_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.horn = 1;
    }
  else
    {
      BUTTONS.horn = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOB, SPEED_RESET_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.speedReset = 1;
    }
  else
    {
      BUTTONS.speedReset = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOB, SUPPLY_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.powerSupply = 1;
    }
  else
    {
      BUTTONS.powerSupply = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOA, SC_CLOSE_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.scClose = 1;
    }
  else
    {
      BUTTONS.scClose = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOB, FUELCELL_OFF_MODE_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.fuelcellOff = 1;
    }
  else
    {
      BUTTONS.fuelcellOff = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOB, FUELCELL_PREPARETORACE_MODE_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.fuelcellPrepareToRace = 1;
    }
  else
    {
      BUTTONS.fuelcellPrepareToRace = 0;
    }

  if (HAL_GPIO_ReadPin(GPIOA, FUELCELL_RACE_MODE_BUTTON_Pin) == GPIO_PIN_RESET)
    {
      BUTTONS.fuelcellRace = 1;
    }
  else
    {
      BUTTONS.fuelcellRace = 0;
    }
}
