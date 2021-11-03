#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
unsigned int seconds=0;
unsigned int minutes=0;
unsigned int hours=0;
void Timer1_Compare_Mode(unsigned int ticks)
{
	TCCR1A|=(1<<FOC1A);
	TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS12); // enable compare mode with prescaler Fcpu/1024
	TCNT1=0;  //
	OCR1A=ticks;
	TIMSK|=(1<<OCIE1A); // enable interrupt flag for timer 1 compare mode A
}
void INT0_init()
{
	MCUCR|=(1<<ISC01); //make falling edge of INT0 generates an interrupt request
	GICR|=(1<<INT0); //enable module interrupt
	DDRD&=~(1<<2); //pin 2 of port D is input
	PORTD|=(1<<2); // enable internal pull
}
void INT1_init()
{
	MCUCR|=(1<<ISC10)|(1<<ISC10); //make rising edge of INT1 generates an interrupt request
	GICR|=(1<<INT1); //enable module interrupt
	DDRD&=~(1<<3); //pin 2 of port D is input
}
void INT2_init()
{
	MCUCSR&=~(1<<ISC2); //make falling edge of INT2 generates an interrupt request
	GICR|=(1<<INT2); //enable module interrupt
	DDRB&=~(1<<2); //pin 2 of port B is input
	PORTB|=(1<<2); // enable internal pull
}
ISR(TIMER1_COMPA_vect)
{
	seconds++;
	if(seconds==60)
	{
		minutes++;
		seconds=0;
	}
	if(minutes==60)
	{
		hours++;
		minutes=0;
	}

}
ISR(INT0_vect)
{
 seconds=0;
 minutes=0;
 hours=0;
}
ISR(INT1_vect)
{
	TCCR1B=0; //disable clock
}
ISR(INT2_vect)
{
	TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS12); //enable clock
}

int main(void)
{
	DDRC|=0x0f; // first 4 pins in port c are outputs
	PORTC&=0xf0; //initialize first 4 pins in port c with zero
	DDRA&=0x60; // first six pins in port a are inputs
	SREG|=(1<<7);
	Timer1_Compare_Mode(977);
	INT0_init();
	INT1_init();
	INT2_init();
	while(1)
	{

		for(int i=0;i<6;i++)
		{
			PORTA=(1<<i);
			if(i==0)
			{
				PORTC=seconds%10;
			}
			else if(i==1)
			{
				PORTC=seconds/10;
			}
			else if(i==2)
			{
				PORTC=minutes%10;
			}
			else if(i==3)
			{
				PORTC=minutes/10;
			}
			else if(i==4)
			{
				PORTC=hours%10;
			}
			else if(i==5)
			{
				PORTC=hours/10;
			}
			_delay_ms(5);
		}



	}
}
