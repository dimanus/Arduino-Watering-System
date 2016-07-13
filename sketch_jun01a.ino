#include <avr/power.h>
#include <avr/sleep.h>
#include "tones.h"
#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x25,8,2);  //0x25 on work version LCD address to 0x27 for a 20 chars and 4 line display

unsigned int _ABVAR_2_PolivStart;
unsigned int current_millis_value,previous_millis_value;
unsigned long lastPoliv,seconds,timeToUpdate;
int _ABVAR_3_Poliv;
int _ABVAR_1_EarthLiq;
int wakePin = 2;  
boolean toggle1 = 0;

void ChkTimer();
void PolivOff();
void PolivOn();

#define 	WDTO_15MS   0
#define 	WDTO_30MS   1
#define 	WDTO_60MS   2
#define 	WDTO_120MS   3
#define 	WDTO_250MS   4
#define 	WDTO_500MS   5
#define 	WDTO_1S   6
#define 	WDTO_2S   7
#define 	WDTO_4S   8
#define 	WDTO_8S   9

void setup()
{
  init_timer();  
_ABVAR_3_Poliv = 0;
_ABVAR_2_PolivStart = 0;
_ABVAR_1_EarthLiq = 0;
lastPoliv=0;
pinMode( 6 , OUTPUT);
pinMode( 13 , OUTPUT);
pinMode( 10 , INPUT);
pinMode( A0 , INPUT);
pinMode( 2 , INPUT_PULLUP);
PolivOff();
attachInterrupt(0, wakeUpNow, LOW);

lcd.init();                      // initialize the lcd 
   // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Poliv");
  lcd.setCursor(0,1);
  lcd.print("ver 1.0");
  digitalWrite(13,HIGH);
  delay(3000);
  lcd.clear();
  printTime();
lastPoliv = millis();
digitalWrite(13,LOW);
}


void printTime(){
  unsigned int minutes,sec,hours;
 
  //current_millis_value = millis(); m += current_millis_value - lastPoliv- previous_millis_value; // should work even when millis rolls over 
  //seconds += m / 1000; 
  //m = m % 1000; 
  minutes = seconds / 60; sec = seconds % 60; hours = minutes / 60; minutes = minutes % 60; 
  //previous_millis_value = current_millis_value;   
  
  lcd.setCursor(0,0);
  printDigits(hours);
  lcd.print(":");
  printDigits(minutes);
   lcd.print(":");
  printDigits(sec);
  lcd.setCursor(4,1);
  lcd.print(analogRead(A0));
  timeToUpdate=millis();

}
void loop()
{
if ((digitalRead(10)==HIGH || digitalRead(2)==LOW ) && ( ( _ABVAR_2_PolivStart ) == ( 0 ) ))
{
PolivOn();
}
ChkTimer();
//printTime();
if ((timeToUpdate + 1000) < millis()){
  printTime();  
   timeToUpdate=millis();
}
sleepNow();
}

void printDigits(byte digits){
 // utility function for digital clock display: prints colon and leading 0
 
 if(digits < 10)
   lcd.print('0');
 lcd.print(digits);  
}

void ChkTimer()
{
if (( ( _ABVAR_2_PolivStart ) > ( 1 ) ))
{
if (( ( millis() ) > ( ( _ABVAR_2_PolivStart + 5010 ) ) ))
{
PolivOff();
_ABVAR_2_PolivStart = 0 ;
}
}
}

void PolivOff()
{
_ABVAR_3_Poliv = 0 ;
digitalWrite( 6 , LOW );
digitalWrite(13,LOW);
}

void PolivOn()
{
  if ((lastPoliv + 60000) > millis() && lastPoliv>1){
    playSong();
   // sleepNow();
    
  }else {
_ABVAR_3_Poliv = 1 ;
digitalWrite( 6 , HIGH );
digitalWrite(13,HIGH);
delay(4000);
_ABVAR_3_Poliv = 0 ;
digitalWrite( 6 , LOW );
_ABVAR_2_PolivStart = millis() ;
lastPoliv=millis();
seconds=0;
printTime();
  }
   //LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); //maybe best type for sleep
   
delay(20000); //for sensor adjust
}

void sleepNow()
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and 
     * wake up sources are available in which sleep modus.
     *
     * In the avr/sleep.h file, the call names of these sleep modus are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings 
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     *  the power reduction management <avr/power.h>  is described in 
     *  http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
     */  
     
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin 
  
  power_adc_disable();
  power_spi_disable();
  //power_timer0_disable();
  //power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  
  
  sleep_mode();            // here the device is actually put to sleep!!
 
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // first thing after waking from sleep:
                            // disable sleep...

  power_all_enable();
   
}

void wakeUpNow() {

}

void playSong() {
  //tone(pin, note, duration)
    tone(8,LA3,Q); 
    delay(1+Q); //delay duration should always be 1 ms more than the note in order to separate them.
    tone(8,LA3,Q);
    delay(1+Q);
    tone(8,LA3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    
    tone(8,LA3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    tone(8,LA3,H);
    delay(1+H);
    
    tone(8,E4,Q); 
    delay(1+Q); 
    tone(8,E4,Q);
    delay(1+Q);
    tone(8,E4,Q);
    delay(1+Q);
    tone(8,F4,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    
    tone(8,Ab3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    tone(8,LA3,H);
    delay(1+H);
    
    tone(8,LA4,Q);
    delay(1+Q);
    tone(8,LA3,E+S);
    delay(1+E+S);
    tone(8,LA3,S);
    delay(1+S);
    tone(8,LA4,Q);
    delay(1+Q);
    tone(8,Ab4,E+S);
    delay(1+E+S);
    tone(8,G4,S);
    delay(1+S);
    
    tone(8,Gb4,S);
    delay(1+S);
    tone(8,E4,S);
    delay(1+S);
    tone(8,F4,E);
    delay(1+E);
    delay(1+E);//PAUSE
    tone(8,Bb3,E);
    delay(1+E);
    tone(8,Eb4,Q);
    delay(1+Q);
    tone(8,D4,E+S);
    delay(1+E+S);
    tone(8,Db4,S);
    delay(1+S);
    
    tone(8,C4,S);
    delay(1+S);
    tone(8,B3,S);
    delay(1+S);
    tone(8,C4,E);
    delay(1+E);
    delay(1+E);//PAUSE QUASI FINE RIGA
    tone(8,F3,E);
    delay(1+E);
    tone(8,Ab3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,LA3,S);
    delay(1+S);
    
    tone(8,C4,Q);
    delay(1+Q);
     tone(8,LA3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    tone(8,E4,H);
    delay(1+H);
    
     tone(8,LA4,Q);
    delay(1+Q);
    tone(8,LA3,E+S);
    delay(1+E+S);
    tone(8,LA3,S);
    delay(1+S);
    tone(8,LA4,Q);
    delay(1+Q);
    tone(8,Ab4,E+S);
    delay(1+E+S);
    tone(8,G4,S);
    delay(1+S);
    
    tone(8,Gb4,S);
    delay(1+S);
    tone(8,E4,S);
    delay(1+S);
    tone(8,F4,E);
    delay(1+E);
    delay(1+E);//PAUSE
    tone(8,Bb3,E);
    delay(1+E);
    tone(8,Eb4,Q);
    delay(1+Q);
    tone(8,D4,E+S);
    delay(1+E+S);
    tone(8,Db4,S);
    delay(1+S);
    
    tone(8,C4,S);
    delay(1+S);
    tone(8,B3,S);
    delay(1+S);
    tone(8,C4,E);
    delay(1+E);
    delay(1+E);//PAUSE QUASI FINE RIGA
    tone(8,F3,E);
    delay(1+E);
    tone(8,Ab3,Q);
    delay(1+Q);
    tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    
    tone(8,LA3,Q);
    delay(1+Q);
     tone(8,F3,E+S);
    delay(1+E+S);
    tone(8,C4,S);
    delay(1+S);
    tone(8,LA3,H);
    delay(1+H);
    
    delay(2*H);
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1){
    digitalWrite(13,HIGH);
    toggle1 = 0;
  }
  else{
    digitalWrite(13,LOW);
    toggle1 = 1;
  }
  seconds+=1;

}
void init_timer(){
  
  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
  
  //no SPI
  power_spi_disable();
//  power_timer0_disable(); //used for TWI
//  power_timer1_disable(); //no timers
//  power_timer2_disable();
}

