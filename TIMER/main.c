//*********************************** Série STM32F407VG avec Keil IDE version 1 **********************************
// Nom : DGHIM
// Prénom : Oussama
// Niveau d'étude : 3ème année ingénieur en électronique et systèmes embarqués à l'ISIMM
// ******** Application :L'objectif de mon application est de créer une application simple
// qui permet de basculer l'état d'une LED implémentée sur STM32F407 sans utiliser la bibliothèque ni l'outil de configuration automatique
// fourni par la société ST, CubeMX.
// ******** Informations extraites à partir des documents : 
// LED PD12 OUTPUT port D pin 12 color green
// RCC adresse : 0x4002 3800 - 0x4002 3BFF
// GPIOD adresse : 0x4002 0C00 - 0x4002 0FFF
void GPIOD_PIN12_Configuration();
void TimerConfiguration();
int main(void)
{
	GPIOD_PIN12_Configuration();
	TimerConfiguration();
	while (1)
	{
		 // Address offset: 0x10 SR
		if((*(int *) 0x40000010) & (1<<0))
	  {// write 1 in pin 12 port D offset 0x14 (GPIOx_ODR)
		*(int *) 0x40000010 &=~ (1<<0);
		 *(int *)0x40020C14 ^=(1<<12);// bit 24 = 1
	}
}
	return 0;
}
void GPIOD_PIN12_Configuration()
{
  //***************** Activation Clock bus AHB1 *********************
	//offset 0x30
  *(int *)0x40023830 |=(1<<3); // bit 3 = 1
	
	//*****************OUPUT LED Configuration ********************
	// pin mode output offset 0x00 GPIOx_MODER
	*(int *)0x40020C00 |=(1<<24); // bit 24 = 1
	*(int *)0x40020C00 &=~(1<<25);// bit 25 = 0
	// OUTPUT PUSH PULL offset 0x04 GPIOx_OTYPER
  *(int *)0x40020C04 &=~(1<<12); // bit 12 = 0	
	// Medium speed offset 0x08 GPIOx_OSPEEDR
  *(int *)0x40020C08 |=(1<<24);// bit 24 = 1
	*(int *)0x40020C08 &=~(1<<25);// bit 25 = 0
}

void TimerConfiguration()
{
	 //***************** Activation Clock bus ABP1 *********************
	// Address offset: 0x40+RCC address
  *(int *) 0x40023840 |=(1<<0); // bit 3 = 1
	//Address offset: 0x2C ARR
  *(int *) 0x4000002C=1999;
	//Address offset: 0x28 PSC
	*(int *) 0x40000028=8399;
	//Address offset: 0x24 CNT
	*(int *) 0x40000024=0;
	//Address offset: 0x00 CR1
	*(int *) 0x40000000 |=(1<<0);
}