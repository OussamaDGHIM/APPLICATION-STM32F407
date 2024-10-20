//*********************************** S�rie STM32F407VG avec Keil IDE version 1 **********************************
/*
	 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Nom : DGHIM
   Pr�nom : Oussama
   Niveau d'�tude : 3�me ann�e ing�nieur en �lectronique et syst�mes embarqu�s � l'ISIMM
	 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 
   ** Application : L'objectif de mon application est de cr�er une application simple
   qui permet de basculer l'�tat d'une LED impl�ment�e sur STM32F407 sans utiliser la biblioth�que ni l'outil de configuration automatique
   fourni par la soci�t� ST, CubeMX.
	 
   ** Informations extraites � partir des documents : 
   LED PD12 OUTPUT port D pin 12 color green
   RCC adresse : 0x4002 3800 - 0x4002 3BFF
   GPIOD adresse : 0x4002 0C00 - 0x4002 0FFF
*/
void GPIOD_PIN12_Configuration();
void SystickDelay(int ms);
int main(void)
{
	GPIOD_PIN12_Configuration();
	while (1)
	{
			 SystickDelay(1000);// 1s => 1000 ms  
		  *(int *)0x40020C14 ^=(1<<12);// bit 24 = 1
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
	// ******************* Configuration du Systick timer **************************
void SystickDelay(int ms)
{// Configuration du registre de reload => reload = 1 ms / (1 / (16800000))
*(int *)0xE000E014 = 16800 - 1;  // Valeur d'initialisation du compteur (SysTick Reload Value Register)
// Registre de la valeur actuelle du SysTick, la valeur finale est fix�e � 0 (r�initialise la valeur courante)
*(int *)0xE000E018 = 0;
// Configuration du registre de contr�le et de statut du SysTick
// Active le timer (bit 0 = 1) et s�lectionne l'horloge du processeur (bit 2 = 1)
*(int *)0xE000E010 |= ((1 << 0) | (1 << 2));

for (int i = 0; i < ms; i++)  // Boucle pour le nombre de millisecondes � attendre
{
    // Attend que le COUNTFLAG (bit 16) soit positionn�, ce qui signifie que le compteur a atteint 0
    while ((*(int *)0xE000E010 & (1 << 16)) == 0) {
        // Attente active jusqu'� ce que COUNTFLAG soit � 1 (le compteur atteint 0)
    }
}
}