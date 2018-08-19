/*
 * GccApplication2.cpp
 *
 * Created: 7/25/2018 7:28:33 PM
 * Author : Dhiman Airao
 */ 



#include <avr/io.h >
#include <avr/interrupt.h >
#define F_CPU 8000000UL
#include <util/delay.h>
#include <string.h>

char current[20];
char previous[20];
char str[20];
int i=0;
unsigned char x=0;

void uart_initalize(unsigned int BAUDRATE)
{
	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;
	UCSR0B =0b00011000;
	UCSR0C =0b10000110;
}

void interrupt_enable(void)
{
	UCSR0B =0b10000000;
}

unsigned char usart_recivedata()
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void usart_transmitchar(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0= data;
}

void usart_transmitstring(char s[])
{
	for(i=0;s[i]!='\0';i++)
	{
		usart_transmitchar(s[i]);
	}
}

void usart_transmitnumber(long int num , uint8_t n)
{
	if(num<0)
	{
		usart_transmitchar('-');
		num=(-1)*num;
	}
	
	if(num >= 10){
		usart_transmitnumber(num/10,n);
		num = num%10;
	}
	usart_transmitchar(num+'0');
	
}

void UART1_Rx_Str()
{
	char x;
	x=usart_recivedata();
	if(int(x)!= 13)
	{
		str[i++] = x;
		UART1_Rx_Str();
	}
	str[i] = '\0';
}

void usart_comparestring(void)
{
	if(strcmp(previous,current)!=0)
	{
		usart_transmitstring(current);
		strcpy(previous,current);
	}
}


int main()
{
	interrupt_enable();
	sei();
	uart_initalize(51);
	while(1)
	{
		//x=usart_recivedata();
		//usart_transmitchar(x);
		
	}
	
}

ISR (USART0_RX_vect)
{
	char x ;
	int i=0;
	x=usart_recivedata();
	while(i<12)
	{
		current[i++] = x;
	}
	
	current[i] = '\0';
	usart_comparestring();
	
}




