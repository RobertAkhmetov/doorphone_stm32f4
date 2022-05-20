
/* ------------------------------------
	
	GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
	     0           0           0           0					 0			домофон занят BUSY
	     0           0           0           0					 1			домофон в спящем режиме  SLEEPING
		   0           0           0           1					 0			не установлено  NONE
		   0           0           0           1					 1				нажата кнопка 1
		   0           0           1           0					 0				нажата кнопка 2
		   0           0           1           0					 1				нажата кнопка 3
		   0           0           1           1					 0				нажата кнопка 4
		   0           0           1           1					 1				нажата кнопка 5
		   0           1           0           0					 0				нажата кнопка 6
		   0           1           0           0					 1				нажата кнопка 7
		   0           1           0           1					 0				нажата кнопка 8
		   0           1           0           1					 1				нажата кнопка 9
		   0           1           1           0					 0				нажата кнопка 0
		   0           1           1           0					 1				нажали С
		   0           1           1           1					 0				нажали вызов
		   0           1           1           1					 1				нажали 112
		   1           0           0           0					 0				нажали диспетчер
			 1           0           0           0					 1				нажали консъерж
			 1           0           0           1					 0				нажали ОДС
	
			------------------------------------		*/
#include "statusbypins.h"

void ActivationPins()
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4, GPIO_PIN_SET);	
}

void DesactivationPins()
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4, GPIO_PIN_RESET);	
}


void SetStatusNone ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
	
}


void SetStatusSleeping ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
	
}

void SetStatus112pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
}


void SetStatusDispetcherPressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}


void SetStatus3pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
}

void SetStatus6pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}

void SetStatus9pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
}	

void SetStatusCallButtonPressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}	

	
void SetStatus2pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}

void SetStatus5pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
}

void SetStatus8pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}

void SetStatus0pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}

void SetStatus1pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
}

void SetStatus4pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_RESET);	
}

void SetStatus7pressed ()
{
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15, GPIO_PIN_SET);	
}



