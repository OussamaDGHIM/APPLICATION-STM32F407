//*********************************** Série STM32F407VG avec Keil IDE version 1 **********************************
/* 

 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Nom : DGHIM
 Prénom : Oussama
 Niveau d'étude : 3ème année ingénieur en électronique et systèmes embarqués à l'ISIMM
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 **Application : L'objectif de mon application est de créer une interruption externe 
 pour allumer et éteindre une LED en appuyant sur un bouton,en accédant directement aux adresses mémoire 
 des registres du STM32F407VG, sans utiliser les bibliothèques ni les outils fournis par ST, tels que CubeMX.
 
 ** Informations extraites à partir des documents : 
 LED PD13 OUTPUT port D pin 13 color orange
 User button PA0 INPUT PORT A pin 0
 RCC adresse : 0x4002 3800 - 0x4002 3BFF
 GPIOD adresse : 0x4002 0C00 - 0x4002 0FFF
 GPIOA adresse : 0x4002 0000 - 0x4002 03FF 
 
 */
#include "stm32f4xx.h" // Device header
void GPIO_PD13_AND_BUTTON_Configuration();
void Interruption_Conf(void);
void delay(uint32_t time)
{
	while(time--)
	{}
}
int main(void)
{
	GPIO_PD13_AND_BUTTON_Configuration();
	Interruption_Conf();
	while (1)
	{
		*(int *)0x40020C14 |=(1<<13);// bit 13 = 1
	}
	return 0;
}
void GPIO_PD13_AND_BUTTON_Configuration()
{
  //***************** Activation Clock bus AHB1 *********************
	//offset 0x30
  *(int *)0x40023830 |= ((1<<3) | (1<<0)); // bit 3 = 1 => bus clock D (GPIOD) et bit 0  = 1 => bus clock A (GPIOA)
	
	//*****************OUPUT LED Configuration ********************
	// pin mode output offset 0x00 GPIOD_MODER
	*(int *)0x40020C00 |=(1<<26); // bit 24 = 1
	*(int *)0x40020C00 &=~(1<<27);// bit 25 = 0
	// OUTPUT PUSH PULL offset 0x04 GPIOD_OTYPER
  *(int *)0x40020C04 &=~(1<<13); // bit 12 = 0	
	// Medium speed offset 0x08 GPIOD_OSPEEDR
  *(int *)0x40020C08 |=(1<<26);// bit 24 = 1
	*(int *)0x40020C08 &=~(1<<27);// bit 25 = 0
	
	//*****************INPUT BUTTON Configuration ******************
		// pin mode input offset 0x00 GPIOA_MODER
	*(int *)0x40020000 &=~(1<<0); // bit 0 = 0
	*(int *)0x40020000 &=~(1<<1);// bit 1 = 0
	// No pull-up, pull-down offset 0x0C GPIOA__PUPDR
  *(int *)0x4002000C &=~(1<<0);// bit 0 = 0
	*(int *)0x4002000C &=~(1<<1);// bit 1 = 0

}
void Interruption_Conf(void){
		 //0x44
	*(int *)0x40023844 |= (1<<14);
	//0000
	// Address offset: 0x08 
	//  0x4001 3800
	*(int *)0x40013808 &=~ ((1<<0) |(1<<1) |(1<<2) |(1<<3));
	// 0x4001 3C00 - 0x4001 3FFF EXTI
	*(int *)0x40013C00 |=(1<<0);// bit 0 = 1
	//Address offset: 0x08 men
	*(int *)0x40013C08 |=(1<<0);// bit 0 = 1
	//Address offset: 0x0C des
	*(int *)0x40013C0C &=~(1<<0);// bit 0 = 0
	NVIC_SetPriority(EXTI0_IRQn,1);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(){
	//Address offset: 0x14
	if(*(int *)0x40013C14 & (1<<0))
	{
		*(int *)0x40013C14 |= (1<<0); 
		*(int *)0x40020C14 &=~(1<<13);// bit 13 = 1
		delay(1000000);
	}
}