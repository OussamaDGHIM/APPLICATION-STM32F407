//*********************************** S�rie STM32F407VG **********************************
// Nom : DGHIM
// Pr�nom : Oussama
// Niveau d'�tude : 3�me ann�e ing�nieur en �lectronique et syst�mes embarqu�s � l'ISIMM
// ******** Application : L'objectif de mon application est de cr�er une application simple
// qui permet d'allumer une LED impl�ment�e sur STM32F407 sans utiliser la biblioth�que ni l'outil de configuration automatique
// fourni par la soci�t� ST, CubeMX.
// ******** Informations extraites � partir des documents : 
// LED PD12 OUTPUT port D pin 12 
// RCC adresse : 0x4002 3800 - 0x4002 3BFF
// GPIOD adresse : 0x4002 0C00 - 0x4002 0FFF
void GPIOD_PIN12_Configuration();
int main(void)
{
	GPIOD_PIN12_Configuration();
	while (1)
	{
	// write pin 1 or 0 offset 0x014 (GPIOx_ODR)
		*(int *)0x40020C14 |=(1<<12);// bit 24 = 1
	}
	return 0;
}
void GPIOD_PIN12_Configuration()
{
  //Clock bus AHB1 est activ�e offset 0x0030
  *(int *)0x40023830 |=(1<<3); // bit 3 = 1
	// pin mode output offset 0x00 GPIOx_MODER
	*(int *)0x40020C00 |=(1<<24); // bit 24 = 1
	*(int *)0x40020C00 &=~(1<<25);// bit 25 = 0
	// OUTPUT PUSH PULL offset 0x04 GPIOx_OTYPER
  *(int *)0x40020C04 &=~(1<<12); // bit 12 = 0	
	// Medium speed offset 0x08 GPIOx_OSPEEDR
  *(int *)0x40020C08 |=(1<<24);// bit 24 = 1
	*(int *)0x40020C08 &=~(1<<25);// bit 25 = 0
}