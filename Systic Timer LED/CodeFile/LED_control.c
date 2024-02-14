#include <MKL25Z4.H>
#include <stdio.h>


// variable initializations 
volatile uint32_t bekle = 0; // Counter for wait until 10 times it blinks 
volatile uint32_t number = 0; // led number to be represented
volatile uint32_t modulus = 0; // Counter coefficient
volatile uint32_t load = 0x40000; // Green or Red Select
volatile uint32_t glitch_catch = 0; // debouncer for buttton3
volatile uint32_t green = 0; 


void SystemStart(void){
__disable_irq(); // clear all interrupts
	
SIM->SCGC5 |= (0x000000F << 9); /* enable clock to Port A */


// port a configuration
PORTA->PCR[1] = (1 << 8); // PTA1 pin  GPIO set 
PORTA->PCR[2] =(1 << 8); // PTA2 pin  GPIO set 
		
// port b configuration
PORTB->PCR[0] = (1 << 8); // aPTB0 GPIO pin configuration
PORTB->PCR[1] = (1 << 8); 
PORTB->PCR[2] = (1 << 8); // aPTB2 GPIO pin configuration		
PORTB->PCR[3] = (1 << 8); 
PORTB->PCR[17] = (1 << 8); 
PORTB->PCR[18] = (1 << 8); 
PORTB->PCR[19] = (1 << 8); 
PORTB->PCR[20] = (1 << 8); 
	
// port c configuration
PORTC->PCR[1] = (1 << 8); // aPTB0 GPIO pin configuration
PORTC->PCR[2] = (1 << 8); // aPTB0 GPIO pin configuration		
PORTC->PCR[3] = (1 << 8); // aPTB0 GPIO pin configuration
PORTC->PCR[4] = (1 << 8); // aPTB0 GPIO pin configuration

// port d configuration
PORTD->PCR[1] = (1 << 8); // aPTd0 GPIO pin configuration
PORTD->PCR[2] = (1 << 8); 
	
PTB->PDDR |= (1 << 0); // output pin configuration
PTB->PDDR |= (1 << 1); // output pin configuration
PTB->PDDR |= (1 << 2); // output pin configuration
PTB->PDDR |= (1 << 3); // output pin configuration
PTB->PDDR |= (1UL << 17); // output pin configuration	
PTB->PDDR |= (1UL << 18); // output pin configuration
PTB->PDDR |= (1UL << 19); // output pin configuration

PTC->PDDR |= (1 << 1); // output pin configuration
PTC->PDDR |= (1 << 2); // output pin configuration
PTC->PDDR |= (1 << 3); // output pin configuration
PTC->PDDR |= (1 << 4); // output pin configuration

PTD->PDDR |= 0x02; // output pin configuration



//pull upp resistor configuration

PORTA->PCR[1] |= (11 << 0); 
PORTA->PCR[2] |= (11 << 0); 
PORTD->PCR[2] |= (11 << 0); 
	
PTA->PDDR &= ~0x0006; 
PTD->PDDR &= ~0x0004; 
	
PORTD->PCR[2] &= ~0xF0000;//
PORTA->PCR[1]|=0xA0000;//port a1
PORTD->PCR[2]|=0xA0000;//port d
PORTA->PCR[2]|=0xA0000;//port a2




// interrupt enable
NVIC_EnableIRQ(PORTA_IRQn); 
NVIC_EnableIRQ(PORTD_IRQn); 

__enable_irq();//global_interrupt

//interrupt prioraty
NVIC_SetPriority(PORTD_IRQn , 64);
NVIC_SetPriority(PORTA_IRQn , 128);

}	

int main(void)
{
  SystemStart();
	
	//led initializations

	PTC->PCOR |= 0xFFFFFFFF; 
	PTB->PSOR |= 0xFFFFFFFF;
	PTD->PSOR |= 0xFFFFFFFF; 
	
	SysTick->LOAD = 4193999;
	
	SysTick->CTRL = 7;
	
	while(1){
	}
}



void PORTD_IRQHandler(void){ // Counter increment
if (glitch_catch == 0){      
number = number +1;
glitch_catch = 1;
PTC->PDOR = (number << 1 );
}
	
PORTD->ISFR = 0x04;
}
	
void PORTA_IRQHandler(void){
	if (PORTA->ISFR & (1 << 1)){
	green = 0;
	PTB->PDOR = 0x40000; // load green to the output port B (1UL << 18)
	PORTA->ISFR = (1 << 1);
	load = 0x80000;
	
	
	}	
	else	if (PORTA->ISFR & (1 << 2)){
	PORTA->ISFR = (1 << 2); // load red to the output port B (1UL << 19)
	PTB->PSOR = load;
	load = 0;
	}	
	
}


void SysTick_Handler(){
	modulus++;
	
	
	if (load == 0x80000){ // yesil yaniyorsa bu if durumuna gir
	green++ ;
	if (green ==36){
	green = 0;
	PTB->PDOR = 0x80000;
	load = 0x40000;
	}
	

	}
	else if(load == 0){ // kirmizi yaniyorsa bu if durumuna gir
		bekle++;
		if (bekle == 10){
		load  = 0x40000;	
		bekle = bekle - 10;
		}
	
	
	}
		if (modulus == 4){
		modulus = 0;
		PTB->PTOR = load; 


		
}
glitch_catch = 0;
}
