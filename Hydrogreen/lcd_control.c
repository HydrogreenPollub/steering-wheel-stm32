/**
* @file lcd_control.c
* @brief Biblioteka do kontroli wyswietlacza Nextion Enhanced NX3224K028
* @author Piotr Durakiewicz
* @date 28.12.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#include "lcd_control.h"
#include "Nextion_Enhanced_NX3224K028.h"
#include "Nextion_Enhanced_Expansion_Board.h"
#include "buttons.h"
#include "rs485.h"
#include "timers.h"
#include "watchdog.h"
#include "hydrogreen.h"

#define USE_EXPANSION_BOARD 			0

// ******************************************************************************************************************************************************** //

static uint16_t cntTickInitPage;		///< Zmienna odemierzajaca czas przez ktory ma zostac wyswietlana strona startowa w trakcie inicjalizacji
static uint16_t cntTickMode1Page;		///< Zmienna odmierzajaca czas co ktory maja zostac zaktualizowane wartosci na LCD w trybie MODE1_PAGE
static uint16_t cntTickMode2Page;		///< Zmienna odmierzajaca czas co ktory maja zostac zaktualizowane wartosci na LCD w trybie MODE2_PAGE
static uint16_t cntTickEmPage;			///< Zmienna odmierzajaca czas w trybie "EM_PAGE"
#if USE_EXPANSION_BOARD == 1
static uint16_t cntTickLeakPage;		///< Zmienna odmierzajaca czas w trybie "LEAK_PAGE"
#endif
static uint16_t cntTickDevReset;		///< Zmienna odmierzajaca czas do resetu urzadzenia

static uint8_t mainStepFsm;			///< FSM funkcji lcd_control_step()
static uint8_t initFsm;				///< FSM funkcji initPage()
static uint8_t initCplt;			///< Flaga informujaca o zakonczeniu inicjaliacji (jezeli 1 = inicjalizacja zakonczona)
static uint8_t mode1FsmLowVal;			///< FSM funkcji mode1Page(), dla wartosci wymagajacych czestszego odswiezania
static uint8_t mode1FsmHighVal;			///< FSM funkcji mode1Page(), dla wartosci ktore moga byc aktualizowane rzadziej
static uint8_t mode2Fsm;			///< FSM funkcji mode2Page()

// ******************************************************************************************************************************************************** //

/**
* @enum MAIN_MENU_FSM
* @brief Typ wyliczniowy zawierajacy ekrany LCD
*/
typedef enum
{
  INIT_PAGE,
  MODE1_PAGE,
  MODE2_PAGE,
  LEAK_PAGE,
  EM_PAGE
} MAIN_MENU_FSM;

// ******************************************************************************************************************************************************** //

void lcd_control_step(void);
static void initPage(void);
static void mode1Page(void);
static void mode2Page(void);
static void resetAllCntAndFsmState(void);
static uint8_t emPage(void);
#if USE_EXPANSION_BOARD == 1
static void leakPage(void);
#endif
static uint8_t choosePage(void);

// ******************************************************************************************************************************************************** //

/**
* @fn lcd_control_step(void)
* @brief Glowna funkcja obslugujaca wyswietlacz, powinna zostac wywolana wewnatrz hydrogreen_step1kHz()
*/
void lcd_control_step(void)
{
  if (initCplt) choosePage();	//Zmiana strony jest mozliwa dopiero po zakonczeniu inicjalizacji LCD (initCplt musi wynosic 1)

  switch (mainStepFsm)
  {
    case INIT_PAGE:
      initPage();
      break;

    case MODE1_PAGE:
      mode1Page();
      break;

    case MODE2_PAGE:
      mode2Page();
      break;

    case LEAK_PAGE:
#if USE_EXPANSION_BOARD == 1
      leakPage();
#endif
      break;

    case EM_PAGE:
      emPage();
      break;

    default:
      break;
  }
}

/**
* @fn initPage(void)
* @brief Inicjalizacja wyswietlacza
*/
static void initPage(void)
{
  cntTickInitPage++;

  switch (initFsm)
  {
    //Zresetuj wyswietlacz
    case 0:
      if (Nextion_Enhanced_NX3224K028_deviceReset()) initFsm++;
      break;

    //Odczekaj 150ms (jest to czas inicjalizacji wyswietlacza)
    case 1:
      if (cntTickInitPage > 150 * PERIOD_1MS)
	{
	  cntTickInitPage = 0;
	  initFsm++;
	}
      break;

      //Narysuj prostokat wyznaczajacy krawedzie LCD
    case 2:
      if (Nextion_Enhanced_NX3224K028_drawRectangle(0, 0, 320, 240, (const uint8_t *)"GRAY"))
	{
#if USE_EXPANSION_BOARD == 1
	  initFsm++;
#else
	  initFsm = 11;
#endif
	}
      break;

#if USE_EXPANSION_BOARD == 1
    case 3:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(0, 2, 0)) initFsm++;
      break;

    case 4:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(1, 2, 0)) initFsm++;
      break;

    case 5:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(2, 2, 0)) initFsm++;
      break;

    case 6:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(3, 2, 0)) initFsm++;
      break;

    case 7:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(4, 2, 0)) initFsm++;
      break;

    case 8:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(5, 2, 0)) initFsm++;
      break;

    case 9:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(6, 2, 0)) initFsm++;
      break;

    case 10:
      if (Nextion_Enhanced_Expansion_Board_configureGPIO(7, 2, 0)) initFsm++;
      break;
#endif

      //Wyswietlaj przez 3 sekundy strone startowa
    case 11:
      if (cntTickInitPage >= 3 * PERIOD_1S)
	{
	  cntTickInitPage = 0;
	  initFsm++;
	}
      break;

    case 12:
      //Sprawdz czy wykryto wyciek wodoru
      if (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin == 1)
	{
	  //Wyciek wodoru wykryty, przejdz do strony LEAK_PAGE
	  if (Nextion_Enhanced_NX3224K028_loadNewPage(3))
	    {
	      mainStepFsm = LEAK_PAGE;
	      initCplt = 1;
	      cntTickInitPage = 0;
	    }
	  break;
	}

      //Sprawdz czy przycisk bezpieczenstwa nie jest wduszony
      if (RS485_RX_VERIFIED_DATA.emergencyButton == 1)
	{
	  //Przycisk bezpieczenstwa jest wcisniety, przejdz do strony EM_PAGE
	  if (Nextion_Enhanced_NX3224K028_loadNewPage(4))
	    {
	      mainStepFsm = EM_PAGE;
	      initCplt = 1;
	      cntTickInitPage = 0;
	    }
	  break;
	}

      //Sprawdz czy przycisk trybu mode2 nie jest wduszony
      if ( (BUTTONS.mode1 == 0) && (BUTTONS.mode2 == 1) )
	{
	  //Przycisk MODE 2 jest wcisniety, przejdz do strony MODE2_PAGE
	  if (Nextion_Enhanced_NX3224K028_loadNewPage(2))
	    {
	      mainStepFsm = MODE2_PAGE;
	      initCplt = 1;
	      cntTickInitPage = 0;
	    }
	  break;
	}

      // Nie spelniono zadnej z powyzszych opcji, przejdz do domyslnego MODE1_PAGE
      if (Nextion_Enhanced_NX3224K028_loadNewPage(1))
	{
	  mainStepFsm = MODE1_PAGE;
	  initCplt = 1;
	  cntTickInitPage = 0;
	}
      break;

    default:
      break;
  }
}

/**
* @fn mode1Page(void)
* @brief Wyswietlanie informacji na LCD w trybie MODE_1
*/
static void mode1Page(void)
{
  cntTickMode1Page++;

  //Sprawdz czy czas miedzy aktualizacjami minal
  if (cntTickMode1Page >= 5 * PERIOD_1MS)
    {
      cntTickMode1Page = 0;

      //Wyswietlaj w pierwszej kolejnosci wartosci krytyczne (m.in paski postepu wymagajace czestego odswiezania)
      switch (mode1FsmHighVal)
      {
	//Pasek postepu napiecie ogniwa
	case 0:
	  if (RS485_RX_VERIFIED_DATA.FC_V.value <= 20.00)
	    {
	      if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "FCBr", RS485_RX_VERIFIED_DATA.FC_V.value, 60))
		{
		  mode1FsmHighVal++;
		  break;
		}
	    }
	  else
	    {
	      mode1FsmHighVal++;
	    }

	case 1:
	  if ((RS485_RX_VERIFIED_DATA.FC_V.value > 20.00) && (RS485_RX_VERIFIED_DATA.FC_V.value < 40.00))
	    {
	      if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "FCBy", RS485_RX_VERIFIED_DATA.FC_V.value, 60))
		{
		  mode1FsmHighVal++;
		  break;
		}
	    }
	  else
	    {
	      mode1FsmHighVal++;
	    }

	case 2:
	  if (RS485_RX_VERIFIED_DATA.FC_V.value >= 40.00)
	    {
	      if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "FCBg", RS485_RX_VERIFIED_DATA.FC_V.value, 60))
		{
		  mode1FsmHighVal++;
		  break;
		}
	    }
	  else
	    {
	      mode1FsmHighVal++;
	    }

	  //Pasek postepu napiecie superkondensatorow
	case 3:
	  if (RS485_RX_VERIFIED_DATA.SC_V.value <= 20.00)
	    {
	      if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "SCBr", RS485_RX_VERIFIED_DATA.SC_V.value, 60))
		{
		  mode1FsmHighVal++;
		  break;
		}
	    }
	  else
	    {
	      mode1FsmHighVal++;
	    }

	case 4:
	  if ((RS485_RX_VERIFIED_DATA.SC_V.value > 20.00) && (RS485_RX_VERIFIED_DATA.SC_V.value < 40.00))
	    {
	      if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "SCBy", RS485_RX_VERIFIED_DATA.SC_V.value, 60))
		{
		  mode1FsmHighVal++;
		  break;
		}
	    }
	  else
	    {
	      mode1FsmHighVal++;
	    }

	case 5:
	  if (RS485_RX_VERIFIED_DATA.SC_V.value >= 40.00)
	    {
	      if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "SCBg", RS485_RX_VERIFIED_DATA.SC_V.value, 60))
		{
		  mode1FsmHighVal++;
		  break;
		}
	    }
	  else
	    {
	      mode1FsmHighVal++;
	    }

	  //Pasek postepu predkosc chwilowa
	case 6:
	  if (Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t*) "SB", RS485_RX_VERIFIED_DATA.interimSpeed, 50)) mode1FsmHighVal++;
	  break;

	  //Czas okrazenia (milisekundy)
	case 7:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "ms", RS485_RX_VERIFIED_DATA.laptime_miliseconds.value))
	    {
#if USE_EXPANSION_BOARD == 1
	      mode1FsmHighVal++;
#else
	      mode1FsmHighVal = 9;
#endif
	    }
	  break;

#if USE_EXPANSION_BOARD == 1
	    case 8:
	      if (BUTTONS.speedReset == 1)
		{
		  if (Nextion_Enhanced_Expansion_Board_pinState(7, 1)) mode1FsmHighVal++;
		}
	      else
		{
		  if (Nextion_Enhanced_Expansion_Board_pinState(7, 0)) mode1FsmHighVal++;
		}
	      break;
#endif

	  //Wyswietlaj wartosci nie wymagajace tak czestego odswiezania
	case 9:
	  mode1FsmHighVal = 0;

	  switch (mode1FsmLowVal)
	    {

	    //Temperatura ogniwa
	    case 0:
	      if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "temp", RS485_RX_VERIFIED_DATA.FC_TEMP.value)) mode1FsmLowVal++;
	      break;

	      //Obroty wentylatora ogniwa
	    case 1:
	      if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "RPM", RS485_RX_VERIFIED_DATA.fcFanRPM.value)) mode1FsmLowVal++;
	      break;

	      //Napiecie ogniwa
	    case 2:
	      if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "FCV", RS485_RX_VERIFIED_DATA.FC_V.value)) mode1FsmLowVal++;
	      break;

	      //Napiecie superkondensatorow
	    case 3:
	      if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "SCV", RS485_RX_VERIFIED_DATA.SC_V.value)) mode1FsmLowVal++;
	      break;

	      //Predkosc chwilowa
	    case 4:
	      if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "V", RS485_RX_VERIFIED_DATA.interimSpeed)) mode1FsmLowVal++;
	      break;

	      //Predkosc srednia
	    case 5:
	      if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "Avg", RS485_RX_VERIFIED_DATA.averageSpeed)) mode1FsmLowVal++;
	      break;

	      //Czas okrazenia (minuty)
	    case 6:
	      if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "mi", RS485_RX_VERIFIED_DATA.laptime_minutes.value))mode1FsmLowVal++;
	      break;

	      //Czas okrazenia (sekundy)
	    case 7:
	      if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "sec", RS485_RX_VERIFIED_DATA.laptime_seconds)) mode1FsmLowVal++;
	      break;

	      //Sygnalizuj stan przycisku SUPPLY_BUTTON w postaci kolorowej obwodki na wokol ekranu (jezeli czerwona - zasilanie jest wylaczone)
	    case 8:
	      if (BUTTONS.powerSupply == 1)
		{
		  if (Nextion_Enhanced_NX3224K028_drawRectangle(0, 0, 320, 240, (const uint8_t*) "GRAY")) mode1FsmLowVal++;
		}
	      else
		{
		  if (Nextion_Enhanced_NX3224K028_drawRectangle(0, 0, 320, 240, (const uint8_t*) "RED")) mode1FsmLowVal++;
		}
	      break;

	      //Tryby ogniwa
	    case 9:
	      if ((BUTTONS.fuelcellOff == 1) && (BUTTONS.fuelcellPrepareToRace == 0) && (BUTTONS.fuelcellRace == 0))
		{
		  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "FCM", 1)) mode1FsmLowVal++;	// Fuelcell Off
		}
	      else if ((BUTTONS.fuelcellOff == 0) && (BUTTONS.fuelcellPrepareToRace == 0) && (BUTTONS.fuelcellRace == 0))
		{
		  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "FCM", 2)) mode1FsmLowVal++;	// Fuelcell prepare to race
		}
	      else if ((BUTTONS.fuelcellOff == 0) && (BUTTONS.fuelcellPrepareToRace == 0) && (BUTTONS.fuelcellRace == 1))
		{
		  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "FCM", 3)) mode1FsmLowVal++;	// Fuelcell prepare to race
		}
	      break;

	      //Stan superkondensatorow (wlaczone lub wylaczona)
	    case 10:
	      if (BUTTONS.scClose == 0)
		{
		  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "SCM", 1)) mode1FsmLowVal = 0;  // supercapacitors off
		}
	      else if (BUTTONS.scClose == 1)
		{
		  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "SCM", 2)) mode1FsmLowVal = 0;	// supercapacitors on
		}
	      break;

	    default:
	      break;

	  break;
	    }

	default:
	  break;
	}
    }
}

/**
* @fn mode2Page(void)
* @brief Wyswietlanie informacji na LCD w trybie MODE_2
*/
static void mode2Page(void)
{
  cntTickMode2Page++;

  //Sprawdz czy czas juz minal
  if (cntTickMode2Page >= 5 * PERIOD_1MS)
    {
      cntTickMode2Page = 0;

      switch (mode2Fsm)
	{
	//Szara obwodka wokol ekranu
	case 0:
	  if (Nextion_Enhanced_NX3224K028_drawRectangle(0, 0, 320, 240, (const uint8_t *)"GRAY")) mode2Fsm++;
	  break;

	  //Prad ogniwo -> superkondensatory
	case 1:
	  if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "FCSCA", RS485_RX_VERIFIED_DATA.CURRENT_SENSOR_FC_TO_SC.value)) mode2Fsm++;
	  break;

	  //Prad superkondensatory -> silnik
	case 2:
	  if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "SCMA", RS485_RX_VERIFIED_DATA.CURRENT_SENSOR_SC_TO_MOTOR.value)) mode2Fsm++;
	  break;

	  //Sumaryczny pobor pradu
	case 3:
	  if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "TotalPWRA",
	  (RS485_RX_VERIFIED_DATA.CURRENT_SENSOR_FC_TO_SC.value+ RS485_RX_VERIFIED_DATA.CURRENT_SENSOR_SC_TO_MOTOR.value))) mode2Fsm++;
	  break;

	  //PWM mosfeta znajdujacego sie pomiedzy ogniwem a superkondensatorami
	case 4:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "MFCSCPWM", RS485_RX_VERIFIED_DATA.fcToScMosfetPWM)) mode2Fsm++;
	  break;

	  //PWM silnika
	case 5:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "MOTORPWM", RS485_RX_VERIFIED_DATA.motorPWM)) mode2Fsm++;
	  break;

	  //Odczyt napiecia z czujnika wodoru
	case 6:
	  if (Nextion_Enhanced_NX3224K028_writeFloatToControl((const uint8_t*) "H2sensor", RS485_RX_VERIFIED_DATA.H2_SENSOR_V.value)) mode2Fsm++;
	  break;

	  //Kod bledu watchdoga
	case 7:
	  if (Nextion_Enhanced_NX3224K028_writeFltToControl((const uint8_t*) "watchdog", watchdog_flt)) mode2Fsm++;
	  break;

	case 8:
	  if (watchdog_flt == 0x00)
	    {
	      if (Nextion_Enhanced_NX3224K028_changeControlColor((const uint8_t*) "watchdog", 2016)) mode2Fsm++;
	    }
	  else
	    {
	      if (Nextion_Enhanced_NX3224K028_changeControlColor((const uint8_t*) "watchdog", 63488)) mode2Fsm++;
	    }
	  break;

	  //Kod bledu modulu komunikacji
	case 9:
	  if (Nextion_Enhanced_NX3224K028_writeFltToControl((const uint8_t*) "rs485", rs485_flt)) mode2Fsm++;
	  break;

	case 10:
	  if (rs485_flt == 0x00)
	    {
	      if (Nextion_Enhanced_NX3224K028_changeControlColor((const uint8_t*) "rs485", 2016)) mode2Fsm++;
	    }
	  else
	    {
	      if (Nextion_Enhanced_NX3224K028_changeControlColor((const uint8_t*) "rs485", 63488)) mode2Fsm++;
	    }
	  break;

	  //Czas pracy systemu (godziny)
	case 11:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "hours", timers_mainTimeHours)) mode2Fsm++;
	  break;

	  //Czas pracy systemu (minuty)
	case 12:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "minutes", timers_mainTimeMinutes)) mode2Fsm++;
	  break;

	  //Czas pracy systemu (sekundy)
	case 13:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "seconds", timers_mainTimeSeconds)) mode2Fsm++;
	  break;

	  //Sredni czas trwania obiegu petli hydrogreen_step1kHz()
	case 14:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "avg_cycle", timers_avgSysCyclePeriod)) mode2Fsm++;
	  break;

	  //Najmniejszy zanotowany czas trwania obiegu petli hydrogreen_step1kHz()
	case 15:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "min_cycle", timers_minSysCyclePeriod)) mode2Fsm++;
	  break;

	  //Najwiekszy zanotowany czas trwania obiegu petli hydrogreen_step1kHz()
	case 16:
	  if (Nextion_Enhanced_NX3224K028_writeNumberToControl((const uint8_t*) "max_cycle", timers_maxSysCyclePeriod)) mode2Fsm = 0;
	  break;

	default:
	  break;
	}
    }
}

#if USE_EXPANSION_BOARD == 1
/**
* @fn leakPage(void)
* @brief Wyswietlanie informacji na LCD w trybie LEAK_PAGE
*/
static void leakPage(void)
{
  cntTickLeakPage++;

  if ( (cntTickLeakPage >= 100 * PERIOD_1MS) && (cntTickLeakPage < 150 * PERIOD_1MS) )
    {
      Nextion_Enhanced_Expansion_Board_pinState(7, 1);
    }
  else if (cntTickLeakPage >= 150 * PERIOD_1MS)
    {
      Nextion_Enhanced_Expansion_Board_pinState(7, 0);
      cntTickLeakPage = 0;
    }
}
#endif

/**
* @fn emPage(void)
* @brief Wyswietlanie informacji na LCD w trybie EM_PAGE
*/
static uint8_t emPage(void)
{
  cntTickEmPage++;

  if ( (cntTickEmPage >= PERIOD_1S) && (cntTickEmPage < 2 * PERIOD_1S) )
    {
      Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t *)"em_button", 100, 100);

      return 0;
    }
  else if (cntTickEmPage >= 2 * PERIOD_1S)
    {
      Nextion_Enhanced_NX3224K028_writeValueToProgressBar((const uint8_t *)"em_button", 0, 100);
      cntTickEmPage = 0;
      return 1;
    }

  return 0;
}

/**
* @fn choosePage(void)
* @brief Wybor aktualnie wyswietlanej strony na LCD
*/
static uint8_t choosePage(void)
{
  //Sprawdz czy wykryto wyciek wodoru
  if ( (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin == 1) && mainStepFsm != LEAK_PAGE )
    {
      resetAllCntAndFsmState();
      Nextion_Enhanced_NX3224K028_loadNewPage(3);
      mainStepFsm = LEAK_PAGE;

      return 1;
    }
  //Sprawdz czy
  else if ( (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin != 1) && (RS485_RX_VERIFIED_DATA.emergencyButton != 1)
      && mainStepFsm == LEAK_PAGE )
    {
      resetAllCntAndFsmState();
      Nextion_Enhanced_NX3224K028_loadNewPage(1);
      mainStepFsm = MODE1_PAGE;

      return 1;
    }
  //Jezeli nie wykryto wycieku wodoru a przycisk bezpieczenstwa jest wcisniety
  else if ( (RS485_RX_VERIFIED_DATA.emergencyButton == 1) &&  (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin == 0) && mainStepFsm != EM_PAGE  )
    {
      resetAllCntAndFsmState();
      Nextion_Enhanced_NX3224K028_loadNewPage(4);
      mainStepFsm = EM_PAGE;

      return 1;
    }
  else if ( (RS485_RX_VERIFIED_DATA.emergencyButton != 1) && (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin != 1)
      && mainStepFsm == EM_PAGE  && mainStepFsm != LEAK_PAGE)
    {
      resetAllCntAndFsmState();
      Nextion_Enhanced_NX3224K028_loadNewPage(1);
      mainStepFsm = MODE1_PAGE;

      return 1;
    }
  //Sprawdz czy przycisk mode1 jest wcisniety
  else if ( (BUTTONS.mode1 == 1) && (BUTTONS.mode2 == 0) && (mainStepFsm != MODE1_PAGE) &&
      (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin != 1) && (RS485_RX_VERIFIED_DATA.emergencyButton != 1) )
    {
      resetAllCntAndFsmState();
      Nextion_Enhanced_NX3224K028_loadNewPage(1);
      mainStepFsm = MODE1_PAGE;

      return 1;
    }
  //Sprawdz czy przycisk mode2 jest wcisniety
  else if ( (BUTTONS.mode1 == 0) && (BUTTONS.mode2 == 1) && (mainStepFsm != MODE2_PAGE)  &&
      (RS485_RX_VERIFIED_DATA.h2SensorDigitalPin != 1) && (RS485_RX_VERIFIED_DATA.emergencyButton != 1))
    {
      resetAllCntAndFsmState();
      Nextion_Enhanced_NX3224K028_loadNewPage(2);
      mainStepFsm = MODE2_PAGE;

      return 1;
    }
  //Sprawdz czy przyciski mode1 oraz mode2 sa wcisniete jednoczesne
  else if ((BUTTONS.mode1 == 1) && (BUTTONS.mode2 == 1))
    {
      cntTickDevReset++;

      //Odczekaj 5 sekund, nastepnie ponownie zainicjalizuj wyswietlacz
      if (cntTickDevReset >= 5 * PERIOD_1S)
	{
	  resetAllCntAndFsmState();
	  initCplt = 0;
	  mainStepFsm = INIT_PAGE;

	  return 1;
	}
    }

  return 0;
}

/**
* @fn resetAllCntAndFsmState(void)
* @brief Funkcja resetujaca wszystkie zmienne odmierzajaca czas w bibliotece
*/
static void resetAllCntAndFsmState(void)
{
  cntTickInitPage = 0;
  cntTickMode1Page = 0;
  cntTickMode2Page = 0;
  cntTickEmPage = 0;
#if USE_EXPANSION_BOARD == 1
  cntTickLeakPage = 0;
#endif
  cntTickDevReset = 0;

  initFsm = 0;
  mode1FsmLowVal = 0;
  mode1FsmHighVal = 0;
  mode2Fsm = 0;
}
