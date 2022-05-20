/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************

  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "main_page.h"
#include "main_page_desactive.h"
#include "call_page.h"
#include "preview_page.h"
#include "112call_page.h"
//#include "callODS_page.h"
#include "concierge_page.h"
#include "dispetcher_page.h"
#include "open_door_page.h"
#include "spi.h"
#include "gpio.h"
#include "flash.h"
#include "MY_CS43L22.h"
#include <math.h>
#include "stm32f4xx_hal_i2s.h"
#include "at45db161d.h" // spi2 память


//сеть 

#include "net.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
I2C_HandleTypeDef hi2c1;
//DMA_HandleTypeDef hdma_spi3_tx;
extern char str1[60];

extern char total_number[4];
extern int count;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//для считывателя карт
uint8_t RxBuffer[32], TxBuffer[32];
    char IDcard[10];
    char __IDcard[10] = "4ba18a25";
    char IDcard__[10]= "3957ee5c";
    static int IDCard_Err_Num = 0;
    char status = 1;
		//для считывателя
		
//для чипа памяти
uint8_t buff_flash[10];
uint8_t byte;															
uint16_t page=0; uint16_t offset=0; uint32_t razmer=0; uint32_t temp=0;


//для АЦП анализа снятой трубки
uint16_t adc = 0;

//для клавиатуры
extern char comparisonNone;


//для клавиатуры и контроля состояния домофона
/* состояние домофона  */
typedef struct 
{
	bool DeviceIsSleepingNow;
}DeviceStatus;

DeviceStatus* NewStatus;



		

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RNG_HandleTypeDef hrng;
SPI_HandleTypeDef hspi1;//дисплей и сеть
SPI_HandleTypeDef hspi3;//камера
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart1;//обратная связь

//добавили таймеры прерываний
TIM_HandleTypeDef htim6; //++++++++
uint8_t tim6_counter; //++++++

void SystemClock_Config(void);
static void MX_GPIO_Init(void); //gpio раньше всего

static void MX_TIM6_Init(void);//++++++++
static void MX_ADC1_Init(void); //для анализа снятой трубки
static void MX_USART1_UART_Init(void);//обратная связь

//таймер сброса домофона в исходное окно ожидания + watchdog
IWDG_HandleTypeDef hiwdg;
TIM_HandleTypeDef htim2;


/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/


static void MX_IWDG_Init(void);
static void MX_TIM2_Init(void);//таймер сброса домофона в исходное окно ожидания + watchdog
static int timerCount=0;



static void MX_RNG_Init(void);
static void MX_SPI1_Init(uint32_t Desired_SPI_BAUDRATEPRESCALER);
static void MX_SPI3_Init();
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);



void DoorIsOpenFunc();
void GoToDomDefaulPos();
void TubeStateControlCheck();
/* USER CODE BEGIN PFP */


#define USE_EXTI
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/


/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/*
  * @brief  The application entry point.
  */
void Draw_speak_page ()
{
	CS_ACTIVE();
	TFT9341_FillScreen(TFT9341_BLACK);
	CS_ACTIVE();
	//TFT9341_SetTextColor(TFT9341_WHITE); //возвращаем нормальный шрифт и тд для ввода ном вартиры 
	//TFT9341_SetBackColor(TFT9341_BLACK);
	//TFT9341_SetFont(&Font48);
	
	//TFT9341_String(100,100, "ГОВОРИТЕ");
	//lcdprop.pFont->Width=8;
	
	TFT9341_StringRus(80,120,TFT9341_WHITE, TFT9341_BLACK,"ГОВОРИТЕ");
	CS_ACTIVE();
}


void Draw_c112call_page ()
{
	CS_ACTIVE();
	ILI9341_Draw_Image((const char*)c112call_page);
	CS_ACTIVE();
	
}

void Draw_dispetcher_page ()
{
	CS_ACTIVE();
//ILI9341_Draw_Image((const char*)concierge_page);
	TFT9341_FillScreen(TFT9341_BLACK);
	CS_ACTIVE();
	TFT9341_StringRus(80,100,TFT9341_WHITE, TFT9341_BLACK,"ИДЕТ ВЫЗОВ ДИСПЕТЧЕРУ");
	TFT9341_StringRus(100,120,TFT9341_WHITE, TFT9341_BLACK,"ОЖИДАЙТЕ ОТВЕТА");
	CS_ACTIVE();
}

void Draw_concierge_page ()
{
	CS_ACTIVE();
//ILI9341_Draw_Image((const char*)concierge_page);
	TFT9341_FillScreen(TFT9341_BLACK);
	CS_ACTIVE();
	TFT9341_StringRus(80,100,TFT9341_WHITE, TFT9341_BLACK,"ИДЕТ ВЫЗОВ КОНСЪЕРЖА");
	TFT9341_StringRus(100,120,TFT9341_WHITE, TFT9341_BLACK,"ОЖИДАЙТЕ ОТВЕТА");
	CS_ACTIVE();
}

void Draw_callODS_page ()
{
	CS_ACTIVE();
//ILI9341_Draw_Image((const char*)concierge_page);
	TFT9341_FillScreen(TFT9341_BLACK);
	CS_ACTIVE();
	TFT9341_StringRus(80,100,TFT9341_WHITE, TFT9341_BLACK,"ИДЕТ ВЫЗОВ В ОДС");
	TFT9341_StringRus(85,120,TFT9341_WHITE, TFT9341_BLACK,"ОЖИДАЙТЕ ОТВЕТА");
	CS_ACTIVE();
}

void Draw_main_page ()
{
	CS_ACTIVE();
	ILI9341_Draw_Image((const char*)main_page);
	CS_ACTIVE();
}

void Draw_main_page_desactive ()
{
	CS_ACTIVE();
	ILI9341_Draw_Image((const char*)main_page_desactive);
	CS_ACTIVE();
}

void Draw_call_page ()
{
	CS_ACTIVE();
 ILI9341_Draw_Image((const char*)call_page);
CS_ACTIVE();
}

void Draw_open_door_page()
{
	CS_ACTIVE();
	ILI9341_Draw_Image((const char*)open_door_page);
	CS_ACTIVE();
}

void FlatCall()
{
	int i=0;
	for(;i<4;i++)
	{
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
	
	int j=0;	
	while(j<50)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		DelayMicro(744);
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		DelayMicro(756);
		
	 j++;
	}
	
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
	
	
	j=0;
	
	while(j<50)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
		DelayMicro(604);
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
		DelayMicro(592);
		
	 j++;
	}
	
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}
	
	
	j=0;
	
	}
	TubeStateControlCheck();//проверяем,что в этот момент не сняли трубку
	
}

/*-------------------------------------------
								Sys Sound Zone
-------------------------------------------*/
void ClickSound()
{
	for(int i=0;i<40;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(505);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(505);
			}
}

void ClickCallButtonSound()
{
	for(int i=0;i<40;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(700);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(700);
			}
}

void ResetSound()
{
	for(int i=0;i<40;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(290);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(290);
			}
	
}

void OpenDoorSound()
{
	for(int i=0;i<14000;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(220);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(220);
			}
}

void CallDispCons112Sound()
{
	for(int j=0;j<2;j++)
	{
		for(int i=0;i<300;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(1400);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(1400);
			}
	}
}

void CallingSound()
{
	for(int j=0;j<2;j++)
	{
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}
		TubeStateControlCheck();//проверяем,что в этот момент не сняли трубку
		for(int i=0;i<350;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(800);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(800);
			}
		
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}
		TubeStateControlCheck();//проверяем,что в этот момент не сняли трубку
			
		for(int i=0;i<450;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(1100);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(1100);
			}
		DelayMicro(450000);
		TubeStateControlCheck();//проверяем,что в этот момент не сняли трубку
		DelayMicro(450000);
	}
}

void FailedOperationSound()
{
	for(int i=0;i<300;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(675);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(675);
			}
			
	for(int i=0;i<300;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(900);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(900);
			}
			
	for(int i=0;i<250;i++)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				DelayMicro(1100);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				DelayMicro(1100);
			}
	
}


/* Tube Zone */

void TubeStateControlCheck()
{
	uint16_t adc = 0;
	HAL_ADC_Start(&hadc1); // запускаем преобразование сигнала АЦП
  HAL_ADC_PollForConversion(&hadc1, 500); // ожидаем окончания преобразования
	DelayMicro(100000);
  adc = HAL_ADC_GetValue(&hadc1); // читаем полученное значение в переменную adc
  // HAL_ADC_Stop(&hadc1); // останавливаем АЦП (не обязательно)
					
					
	if (adc<100) //НЕТ НА ЛИНИИ
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET); //зажигаем ОРАНЖЕВЫЙ светодиод
	}
					
	if (adc>100 && adc<1000) //НА ЛИНИИ, ТРУБКА НЕ СНЯТА
	{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET); //зажигаем ЗЕЛЕНЫЙ светодиод
	}
	else if(adc>1000) //&& adc<2495) //ТРУБКА СНЯТА
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET); //зажигаем КРАСНЫЙ светодиод	
		Draw_speak_page ();
		SetStatusNone ();
		while(1)
		{
		//TubeStateControlCheck();
		uint16_t adc = 0;
		HAL_ADC_Start(&hadc1); // запускаем преобразование сигнала АЦП
		HAL_ADC_PollForConversion(&hadc1, 500); // ожидаем окончания преобразования
		DelayMicro(100000);
		adc = HAL_ADC_GetValue(&hadc1); // читаем полученное значение в переменную adc
		if(adc<1000)
		{
		FailedOperationSound();
		break;
		}
		else if(adc>2495)//КНОПКА НАЖАТА   ///Дело не в этом участке
		{
		DoorIsOpenFunc();
		break;
		}
				
		}
		GoToDomDefaulPos();
	}
	
					
  
}


/*Get card number*/    //кардридер мешает работе АЦП с трубки
void CheckCardReader_Request()
{
		status = PCD_Request(PICC_REQALL, RxBuffer);//The return value is 0, which means the card search is successful; and the card type is stored in tempbuf
	
        if(status == PCD_OK)  
        {
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
						//DoorIsOpenFunc();
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
								
        }
        if(status != PCD_OK)
        {
					  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
            PCD_Reset();
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
        }
}


void CheckNetworkRequest()
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
		CS_IDLE();//дезактивировали поток в дисплей
		SS_SELECT();//активизировали поток в сетевой контроллер
		net_poll();//узнали, есть ли запрос по сети к домофону
		SS_DESELECT();//деактивизировали поток в сетевой контроллер
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);
}

void DoorIsOpenFunc()
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);//зажигаем ЗЕЛЕНЫЙ светодиод
	Draw_open_door_page();
	OpenDoorSound();
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
	SetStatusNone ();
	GoToDomDefaulPos();
}

void GoToDomDefaulPos()
{
	count = 0;//обнуляем ранее набранный номер
	total_number[0] = ' ';
	total_number[1] = ' ';
	total_number[2] = ' ';
	total_number[3] = ' ';
	
	//Draw_main_page_desactive();
	Draw_main_page ();
	CS_IDLE();
	
	
	//CheckFirstButtonPressed (); 
	//CS_ACTIVE();
	//
	//keyboard_default_waiting();
	
}



		/*
		==========================      MAIN        =================================
																																									*/

int main(void)
{
  HAL_Init();
  SystemClock_Config();
	
	//MX_GPIO_Init1();
	MX_GPIO_Init();
	//MX_DMA_Init();
  MX_I2C1_Init();
	MX_ADC1_Init();//АЦП для распознвания нажатия
	
  //MX_RNG_Init();
  MX_SPI1_Init(SPI_BAUDRATEPRESCALER_4);
	MX_SPI2_Init();
	MX_SPI3_Init();
	
	//HAL_UART_MspInit(&huart1);
  MX_USART1_UART_Init();
	//USART1_UART_Init(115200);
	
	
	//инициализируем таймеры
	//MX_TIM6_Init();//++++++
	
	
	//----------------------------CARD READER ZONE
	
	MFRC_Init();
	PCD_Reset();//device reset
  PCD_Request(PICC_REQALL, RxBuffer);
	PCD_Anticoll(RxBuffer);
   
	//-------------------------------------READER
	
	/*
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		DelayMicro(500000);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);// внимание,сейчас ЗАПУСТИТСЯ ДОМОФОН
	*/	
		
	TFT9341_ini(320,240); // инициализируем дисплей 240х320
	TFT9341_SetRotation(3); // устанавливаем горизонтальную ориентацию для домофона
	
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)"Hello World\n", 12);
	
	
	CS_ACTIVE();
	ILI9341_Draw_Image((const char*)preview_page);
	uint8_t startmessg[] = "start DF working";
	HAL_UART_Transmit(&huart1,startmessg,16,0xFFFF);
	//DelayMicro(2000000);
	
	
	// Отправим что-нибудь по UART
	
	/*
	uint8_t str[]="USART Transmit\r\n";
	while(1)
	{
		
		HAL_UART_Transmit(&huart1,str,16,0xFFFF);
		DelayMicro(100000);
		//printf("ACK CMD SPI interface Error!\n");
	}
	*/
	
	//printf("ACK CMD SPI interface Error!\n");
	
	/*   */
	CS_ACTIVE(); //команды типа TFT9341_  FillScreen,StringRus,String и тп требуют CS 0, чтобы вывестись 

	
	MX_IWDG_Init();
  MX_TIM2_Init();
	HAL_TIM_Base_Start_IT(&htim2);
	

	
	
	//ВОТ ТУТ ПРОВЕРЯЕМ РАСПОЗНАВАНИЕ ЛИЦ ******
	 //******
	//проверили распознавание лиц ******
	
	
		
	HAL_TIM_Base_Start(&htim6);
	HAL_TIM_Base_Start_IT(&htim6); //запуск таймера
	tim6_counter=0;
	
	CS_IDLE();//дезактивировали поток в дисплей
	MX_SPI1_Init(SPI_BAUDRATEPRESCALER_4);//заново инициализировали SPI1
	//SS_SELECT();//активизировали поток в сетевой контроллер
	net_ini();//сеть заинициализировали
	

	//может удалить?HAL_TIM_Base_Stop(&htim6);
	//HAL_TIM_Base_Stop_IT(&htim6);
	
	
	//CheckFirstButtonPressed (); //ждем,пока будет нажата любая кнопка на клавиатуре
	
	
	//MX_SPI1_Init(SPI_BAUDRATEPRESCALER_4);//заново инициализировали SPI1
	//SS_SELECT();//активизировали поток в сетевой контроллер
	//net_ini();//сеть заинициализировали
	
	CS_ACTIVE();
	
	Draw_main_page_desactive();
	SetStatusSleeping();
	
	
	while(1) {
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0) break;}
	
	ActivationPins();
	//TFT9341_ini(320,240); // инициализируем дисплей 240х320
	//TFT9341_SetRotation(3); // устанавливаем горизонтальную ориентацию для домофона
	CS_ACTIVE();
	Draw_main_page ();
	
	//keyboard_default_waiting();
	while(1)
	{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1, GPIO_PIN_RESET);	
		
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&													//112 нажата         
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1)
	{
		
	CS_ACTIVE();
	TFT9341_SetTextColor(TFT9341_RED); //меняем шрифт и тд для страницы вызова
	TFT9341_SetBackColor(TFT9341_WHITE);
	TFT9341_SetFont(&Font24);
		
		
	//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14, GPIO_PIN_SET);//красный
	CS_ACTIVE();
	Draw_c112call_page ();
	
	//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14, GPIO_PIN_RESET);//красный	
	
	CS_ACTIVE();
	int local_count = 0;
	while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) != 0) //пока статус None не будет установлен
	{
	TFT9341_String(220,137,".");		
	DelayMicro(600000);
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0) break;
	TFT9341_String(230,137,".");
	DelayMicro(600000);
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0) break;
	TFT9341_String(240,137,".");
	DelayMicro(600000);
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0) break;
	TFT9341_String(220,137," ");
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0) break;
	TFT9341_String(230,137," ");
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0) break;
	TFT9341_String(240,137," ");
	DelayMicro(600000);
	if(local_count%3 == 0 || local_count == 0)
		CallDispCons112Sound();
	local_count++;
	}}	
	
/* ----------------дистанция между функциями кнопок---------------------*/
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 1 &&													//ДИСПЕТЧЕР нажат         
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		CS_ACTIVE();
		Draw_dispetcher_page();
		while (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) != 0 ||  //пока статус Ноне не установлен
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) != 0 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) != 0 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) != 1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) != 0) 
		{
			CallDispCons112Sound();
			DelayMicro(3000000);
		}
		count=4; //чтобы нельзя выло лишнего выводить		
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/	

	//второй столбец справа																		// "3" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1)
	{
		Show_total_number ("3");
		SetStatusNone ();
	}
/* ----------------дистанция между функциями кнопок---------------------*/	

	//второй столбец справа																		// "6" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		Show_total_number ("6");
		SetStatusNone ();
	}
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//второй столбец справа																		// "9" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1)
	{
		Show_total_number ("9");
		SetStatusNone ();
	}
	
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "ВЫЗОВ" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		if (total_number[0]!=comparisonNone)
		{
		ClickSound();
		CS_ACTIVE();
		Draw_call_page();
		CS_ACTIVE();
			
		TFT9341_SetTextColor(TFT9341_WHITE); //меняем шрифт и тд для страницы вызова
		TFT9341_SetBackColor(TFT9341_BLACK);
		TFT9341_SetFont(&Font24);
		
		for (int i=0;i<count;i++)
		{
		//отправляем на экран именно символ, а не строку, чтобы не появился мусор
		TFT9341_DrawChar(215+i*15,168,*&total_number[i]);//выводим номер квартиры
		}
		
		CallingSound();	
		FlatCall();
		
		count = 4; //чвтобы нельзя было лишнего выводить
		
		while (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) != 0 ||  //пока статус Ноне не установлен
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) != 0 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) != 0 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) != 1 ||
			HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) != 0)
			{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET); //зажигаем КРАСНЫЙ светодиод	
				TubeStateControlCheck(); //нужно,чтобы это заняло +-0.6сек
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}		
		
				
				//CS_ACTIVE();
				TFT9341_String(180,2,".");//Три точки возле "Вызов"
				CallingSound();
				TubeStateControlCheck();
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
				
		
				FlatCall();
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}				
				
		
				TFT9341_String(180,2," ");
		
				TFT9341_String(187,2,".");
		
				TubeStateControlCheck();
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
		
				TubeStateControlCheck();				
				TFT9341_String(187,2," ");
				TFT9341_String(194,2,".");
		
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
		
				CallingSound();	
				TubeStateControlCheck();
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
				FlatCall();
				if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
					HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0 )
				{break;}	
				TubeStateControlCheck();
				TFT9341_String(194,2," ");
			}

			
		total_number[0] = ' ';
		total_number[1] = ' ';
		total_number[2] = ' ';
		total_number[3] = ' ';
		count = 0;
		//CS_ACTIVE();
	  //Draw_main_page ();	
		//CS_ACTIVE();//чтобы текст мог написать	
		//continue;
		}
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/	
	
/* ----------------дистанция между функциями кнопок---------------------*/	

	//третий столбец справа																		// "2" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		Show_total_number ("2");
		SetStatusNone ();
	}
	
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "5" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1)
	{
		Show_total_number ("5");
		SetStatusNone ();
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "8" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		Show_total_number ("8");
		SetStatusNone ();
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "0" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		Show_total_number ("0");
		SetStatusNone ();
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "1" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1)
	{
		Show_total_number ("1");
		SetStatusNone ();
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "4" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0)
	{
		Show_total_number ("4");
		SetStatusNone ();
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/
	/* ----------------дистанция между функциями кнопок---------------------*/	

	//																									// "7" нажато
	
	if( HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0 &&											      
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 1 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0 &&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1)
	{
		Show_total_number ("7");
		SetStatusNone ();
	}
	
/* ----------------дистанция между функциями кнопок---------------------*/
	
	
	DelayMicro(10000);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1, GPIO_PIN_SET);	
		
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2, GPIO_PIN_RESET);
	DelayMicro(10000);		
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2, GPIO_PIN_SET);
		
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3, GPIO_PIN_RESET);
	DelayMicro(10000);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3, GPIO_PIN_SET);	
		
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4, GPIO_PIN_RESET);
	DelayMicro(10000);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4, GPIO_PIN_SET);
		
	
	}

}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
	
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 50;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
	
	 HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}


static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 2400;//921600; хотелось бы для камеры
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}



/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */

static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }


}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  /*if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
	*/

}



/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}


static void MX_TIM6_Init(void) //++++++++++++++
{
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */

static void MX_SPI1_Init(uint32_t Desired_SPI_BAUDRATEPRESCALER)
{
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT; //вместо "SPI_NSS_SOFT" для того,чтобы на SPI1 попеременно работали дисплей и сеть
  hspi1.Init.BaudRatePrescaler = Desired_SPI_BAUDRATEPRESCALER;//4 подойдет для сетевой микрухи, 2 - для дисплея
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7; //там 10
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_SPI3_Init()
{
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_OUTPUT; //вместо "SPI_NSS_SOFT" для того,чтобы на SPI3 попеременно работали камера
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;//
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7; //там 10
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  } 

}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	/*
	//для сети по идее(не проверял)  МЕШАЕТ КАМЕРЕ Б6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_6, GPIO_PIN_RESET);
	
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //was medium
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	*/
	
	//Конфигурируем I2C1 для камеры
	
	GPIO_InitStruct.Pin = GPIO_PIN_7;//SDA	
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			
	GPIO_InitStruct.Pin = GPIO_PIN_6;//SCL
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	
	
	//Конфигурируем GPIOD для клавиатуры
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
	//далее принимающие пины клавиатуры
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_0, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//прерывания для клавиатуры
	/* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	//пины для высокоприоритетных прерываний клавиатуры и тп
	/*HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	*/
	/* EXTI15 interrupt init*/
  //HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	
	
	//для сети пин CS
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //was medium
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
	

	
	//СВЕТОДИОДЫ НА БОРТУ DISCOVERY
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_12, GPIO_PIN_RESET);
													
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
	//прерывания
	/*
	HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	*/
	
	//активируем прерывания таймера
	//HAL_NVIC_SetPriority(TIM6_DAC_IRQn,0,0);
	//HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
	
	
	/* активируем прерывания таймера*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	
	//конфигурируем gpioC для трубки и системных звуков
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	
	//конфигурируем для считывателя
	
	//D10
	GPIO_InitStruct.Pin = RC522_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(RC522_RST_GPIO_Port, &GPIO_InitStruct);
	
	//E5
	GPIO_InitStruct.Pin = RC522_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(RC522_CS_GPIO_Port, &GPIO_InitStruct);
	
	
	//5 пинов для контроля состояния домофона
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}


static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Reload = 2300;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}




/* USER CODE BEGIN 4 */
/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 499;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99999; //чем меньше,тем быстрее срабатывает таймер 99999 - это 1 сек при частоте 50 МГц
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
}	
	
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // Условно каждую секунду
{
	timerCount++;
	HAL_IWDG_Refresh(&hiwdg);//обновляем таймер WATCHDOGa
	
	
	if(htim == &htim2 && timerCount%2 == 0) //ОПРАШИВАЕТСЯ СЕТЬ И КАРДРИДЕР
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15, GPIO_PIN_SET); //синий светодиод
		CheckCardReader_Request();
		CheckNetworkRequest();
		CS_ACTIVE();//после опроса сети сразу же активируем дисплей
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15, GPIO_PIN_RESET);
  }
	
	
  if(htim == &htim2 && timerCount%257 == 0) // каждую секунду прерывание, через 3 минут - в исходное состояние
  {
		
    GoToDomDefaulPos();
  }
	
}
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	GoToDomDefaulPos();
}


void HAL_TIM_IC_Callback(TIM_HandleTypeDef *htim)
{
	GoToDomDefaulPos();
}

//обработчик прерываний от клавиатуры
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)

{
	//ClickSound();

																																						
		if((GPIO_Pin== GPIO_PIN_8|  GPIO_Pin==GPIO_PIN_7| 											//сценарий вывода из спящего режима
			GPIO_Pin==GPIO_PIN_6| GPIO_Pin==GPIO_PIN_0)&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 0&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 1
		) 
		{
			DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14, GPIO_PIN_SET);//красный
					ClickSound();
					SetStatusNone ();
					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);
					
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14, GPIO_PIN_RESET);//красный
					
				}
				
		} 																																									
		else if((GPIO_Pin == GPIO_PIN_0)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0)						 //сценарий нажатия на кнопку 112
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus112pressed();	
				}
				
		} 
		else if((GPIO_Pin == GPIO_PIN_6)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4) == 0 &&           //Вызов ДИСПЕТЧЕРА
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11) == 0&& //и статус none установлен
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12) == 0&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13) == 0&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14) == 1&&
		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_15) == 0	)
		{	
			DelayMicro(100000);
			if(HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
			{
				ClickSound();
				SetStatusDispetcherPressed();
			}
	}
	else if((GPIO_Pin == GPIO_PIN_8)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1) == 0)						 //сценарий нажатия на C
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ResetSound();
					SetStatusNone ();
					GoToDomDefaulPos();
				}		
		} 
		else if((GPIO_Pin == GPIO_PIN_0)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) == 0)						 //сценарий нажатия на 3
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus3pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_6)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) == 0)						 //сценарий нажатия на 6
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus6pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_7)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) == 0)						 //сценарий нажатия на 9
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus9pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_8)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) == 0)						 //сценарий нажатия на ВЫЗОВ
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickCallButtonSound();
					SetStatusCallButtonPressed ();
				}		
		}
		
		
		
		else if((GPIO_Pin == GPIO_PIN_0)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2) == 0)						 //сценарий нажатия на 2
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus2pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_6)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2) == 0)						 //сценарий нажатия на 5
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus5pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_7)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2) == 0)						 //сценарий нажатия на 8
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus8pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_8)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2) == 0)						 //сценарий нажатия на 0
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus0pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_0)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1) == 0)						 //сценарий нажатия на 1
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus1pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_6)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1) == 0)						 //сценарий нажатия на 4
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus4pressed ();
				}		
		}
		
		else if((GPIO_Pin == GPIO_PIN_7)&& HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1) == 0)						 //сценарий нажатия на 7
		{
				DelayMicro(100000); 	
				if (HAL_GPIO_ReadPin(GPIOD,GPIO_Pin) == 0)
				{
					ClickSound();
					SetStatus7pressed ();
				}		
		}

		
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

