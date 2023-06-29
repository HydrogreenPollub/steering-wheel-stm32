/**
* @file hydrogreen.c
* @brief Plik glowny programu
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

// ******************************************************************************************************************************************************** //

#include "hydrogreen.h"
#include "timers.h"
#include "watchdog.h"
#include "rs485.h"
#include "buttons.h"
#include "leds.h"
#include "gpio.h"
#include "lcd_control.h"

// ******************************************************************************************************************************************************** //

//#define HYDROGREEN_DEBUG

// ******************************************************************************************************************************************************** //

/**
* @fn hydrogreen_init(void)
* @brief Glowna funkcja inicjalizujaca, powinna zostac wywolana wewnatrz hydrogreen_main()
*/
static void hydrogreen_init(void)
{
  watchdog_init();
  timers_init();
  rs485_init();
}

/**
* @fn hydrogreen_step1kHz(void)
* @brief Glowna funkcja wykonywana co T = 1ms, powinna zostac wywolana wewnatrz hydrogreen_main()
*/
static inline void hydrogreen_step1kHz(void)
{
#ifdef HYDROGREEN_DEBUG
  HAL_GPIO_WritePin(GPIOA, DBG_Pin, GPIO_PIN_RESET);
#endif
  leds_step();
  buttons_step();
  lcd_control_step();
  watchdog_step();
#ifdef HYDROGREEN_DEBUG
  HAL_GPIO_WritePin(GPIOA, DBG_Pin, GPIO_PIN_SET);
#endif
}

/**
* @fn hydrogreen_step(void)
* @brief Glowna funkcja wykonywana co T = 0,1ms, powinna zostac wywolana wewnatrz hydrogreen_main()
*/
static inline void hydrogreen_step10kHz(void)
{
  rs485_step();
}

/**
* @fn hydrogreen_main(void)
* @brief Glowna funkcja programu, powinna zostac wywolana w pliku main.c, pomiedzy  USER CODE BEGIN 2 a USER CODE END 2
*/
void hydrogreen_main(void)
{
  hydrogreen_init();

  while (1)
    {
      //Sprawdz czy wystapil tick timera nastepujacy z f = 1kHz
      if (timers_tick1kHz)
	{
	  timers_beforeStep1kHz();

	  hydrogreen_step1kHz();

	  timers_afterStep1kHz();

	  timers_tick1kHz = 0;
	}

      //Sprawdz czy wystapil tick timera nastepujacy z f = 10kHz
      if (timers_tick10kHz)
	{
	  hydrogreen_step10kHz();
	  timers_tick10kHz = 0;
	}
    }
}

/**
* @fn hydrogreen_hardFault(void)
* @brief Sygnalizacja wystapienia hard fault'a, wywolac w pliku main.c, w funkcji Error_Handler()
*/
void hydrogreen_hardFault(void)
{
  HAL_GPIO_WritePin(GPIOB, LED_NUCLEO_Pin, GPIO_PIN_SET);
}
