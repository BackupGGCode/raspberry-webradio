//
// Button and Rotary Encoder Bridge for Raspberry PI
//
// 18.08.2012
// 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define PHASE_A     (PIND & (1<<PD5))
#define PHASE_B     (PIND & (1<<PD6))

#define BUTTON_LONG_PRESS     80
#define BUTTONS               8
#define Buttons_ReadOne(idx)  (PINB & idx)

volatile int8_t enc_delta;
volatile uint8_t buttons = 0;
volatile uint8_t buttons_long = 0;
volatile uint8_t button_must_release = 0;

volatile uint8_t button_time[BUTTONS];


// Dekodertabelle für wackeligen Rastpunkt
// halbe Auflösung
int8_t table[16] PROGMEM = {0, 0, -1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, -1, 0, 0};    

#define Timer_Off()  TIMSK &=~(1 << TOIE0)
#define Timer_On()   TIMSK |= (1 << TOIE0) 
 
void Rotary_Read() {
  static int8_t last = 0;
 
  last = (last << 2) & 0x0F;
  if(PHASE_A) last |= 2;
  if(PHASE_B) last |= 1;
  enc_delta += pgm_read_byte(&table[last]);
}

 
int8_t Rotary_Get(void) {
  int8_t val;
 
  Timer_Off();
  val = enc_delta;
  enc_delta = 0;
  Timer_On();
  return val;
}

void Buttons_Read() {
  uint8_t i, idx = 1;
  for(i = 0; i < BUTTONS; i++) {
    
    if(Buttons_ReadOne(idx)) {
      button_must_release |= idx;
      if(button_time[i] < BUTTON_LONG_PRESS) {
		button_time[i]++;
      } else if(button_time[i] == BUTTON_LONG_PRESS) {
		buttons_long |= idx; 
		button_time[i]++;
      }
    } else {
      if(button_must_release & idx) {
		if(button_time[i] == 0) {
			buttons |= idx;
		} else {
			button_time[i] = 0;
		}		
		button_must_release &=~idx;
      }
    }
    idx <<=1;
  }
}


uint8_t Buttons_Get(uint8_t id) {
  Timer_Off();
  uint8_t ret = (buttons & (1 << id)) ? 1 : 0; 
  buttons &=~(1 << id);
  Timer_On();
  return ret;
}

uint8_t Buttons_GetLong(uint8_t id) {
  Timer_Off();
  uint8_t ret = (buttons_long & (1 << id)) ? 1 : 0;
  buttons_long &=~(1 << id);
  Timer_On();
  return ret;
}

void TIMER0_OVF_vect(void)__attribute__((interrupt));
void TIMER0_OVF_vect(void) {
  Rotary_Read();
  Buttons_Read();
}

volatile uint8_t bit_count = 0;
ISR(INT1_vect) {
	// button pressed value
	if(bit_count < 8) {
		if(Buttons_Get(bit_count)) PORTD |= (1 << PD4); else PORTD &=~(1 << PD4);
	} 
	// button long pressed value
	else if(bit_count < 16) {
		if(Buttons_GetLong(bit_count - 8)) PORTD |= (1 << PD4); else PORTD &=~(1 << PD4);
	}
	// rotary value
	else if(bit_count < 24){
		if((uint8_t)Rotary_Get() & (1 << (bit_count - 16))) PORTD |= (1 << PD4); else PORTD &=~(1 << PD4);
	}
	// reset bit count
	else {
		bit_count = 0;
	}
}


int main() {
 // init transfer interrupt
 MCUCR |= (1 << ISC11) | (1 << ISC10); // rising edge of int1
 GIMSK |= (1 << INT1);

 // init ports
 DDRD = (1 << PD4); // pd4 out, rest in
 PORTD = 0xff & ~((1 << PD3) | (1 << PD4)); // internal pull ups (except for clk and data)
 DDRB = 0;
 PORTB = 0xff; // internal pull ups
 
 // init timer
 TCCR0B |= (1 << CS02);
 TIMSK |= (1 << TOIE0);
 
 // enable bridge functionality
 sei();
  
 while(1) {
 }
  
}
