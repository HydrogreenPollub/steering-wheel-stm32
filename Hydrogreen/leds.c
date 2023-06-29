/**
* @file leds.c
* @brief Biblioteka do obslugi diod statusu
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

// ******************************************************************************************************************************************************** //

#include "leds.h"
#include "watchdog.h"
#include "hydrogreen.h"
#include "gpio.h"

// ******************************************************************************************************************************************************** //

/**
* @enum LEDS_EFFECT
* @brief Typ wyliczeniowy zawierajacy aktualny stan fsm funkcji leds_step()
*/
typedef enum
{
  FREQ_1HZ,
  FREQ_5HZ,
} LEDS_EFFECTS;

// ******************************************************************************************************************************************************** //

/**
* @fn leds_step(void)
* @brief Obsluga diody LED_STS, wywolac wewnatrz hydrogreen_step1kHz()
*/
void leds_step(void)
{
  static uint8_t ledsFsm;
  static uint16_t period;

  if (watchdog_flt == WATCHDOG_FLT_NONE)
    {
      ledsFsm = FREQ_5HZ;
    }
  else
    {
      ledsFsm = FREQ_1HZ;
    }

  period++;

  switch (ledsFsm)
  {
    case FREQ_1HZ:
      if (period >= 500 * PERIOD_1MS)
	{
	  HAL_GPIO_TogglePin(GPIOA, LED_STS_Pin);
	  period = 0;
	}
      break;

    case FREQ_5HZ:
      if (period >= 100 * PERIOD_1MS)
	{
	  HAL_GPIO_TogglePin(GPIOA, LED_STS_Pin);
	  period = 0;
	}
      break;

    default:
      break;
  }
}
