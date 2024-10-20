//*********************************** Série STM32F407VG avec Keil IDE version 1 **********************************
/*
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Nom : DGHIM
   Prénom : Oussama
   Niveau d'étude : 3ème année ingénieur en électronique et systèmes embarqués à l'ISIMM
	 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 
   ** Application : L'objectif de mon application est de créer une application simple
   qui permet d'allumer une LED à travers un bouton implémenté sur STM32F407 sans utiliser la bibliothèque ni l'outil de configuration automatique
   fourni par la société ST, CubeMX.
	 
   ** Informations extraites à partir des documents : 
   LED PD13 OUTPUT port D pin 13 color orange
   User button PA0 INPUT PORT A pin 0
   RCC adresse : 0x4002 3800 - 0x4002 3BFF
   GPIOD adresse : 0x4002 0C00 - 0x4002 0FFF
   GPIOA adresse : 0x4002 0000 - 0x4002 03FF
*/
void GPIO_PD13_AND_BUTTON_Configuration();
int main(void)
{
	GPIO_PD13_AND_BUTTON_Configuration();
	while (1)
	{
		// Read data  offset 0x10 GPIOA_IDR
  if(*(int *)0x40020010 & (1<<0)) // test read pin PA0 with bit 0   		
	// write 1 in pin 13 offset 0x14 (GPIOx_ODR)
		*(int *)0x40020C14 |=(1<<13);// bit 13 = 1
	else
	// write 0 in pin 13 offset 0x14 (GPIOx_ODR)
		*(int *)0x40020C14 &=~(1<<13);// bit 13 = 0
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