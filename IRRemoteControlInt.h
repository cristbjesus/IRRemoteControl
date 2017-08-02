/*
 * IRRemoteControl - Lightweight library for send/receive infra-red signal.
 * This library can read codes from flash memory if necessary.
 * Created by Cristiano Borges - STI - SENAI Fatesg
 * August, 2017
 */

#ifndef IRRemoteControlInt_h
  #define IRRemoteControlInt_h
  
  #if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
  
  // define which timer to use
  //
  // Uncomment the timer you wish to use on your board.  If you
  // are using another library which uses timer2, you have options
  // to switch IRremote to use a different timer.
  
  // Arduino Mega
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    //#define IR_USE_TIMER1   // tx = pin 11
    #define IR_USE_TIMER2     // tx = pin 9
    //#define IR_USE_TIMER3   // tx = pin 5
    //#define IR_USE_TIMER4   // tx = pin 6
    //#define IR_USE_TIMER5   // tx = pin 46
    
  // Teensy 1.0
  #elif defined(__AVR_AT90USB162__)
    #define IR_USE_TIMER1     // tx = pin 17
    
  // Teensy 2.0
  #elif defined(__AVR_ATmega32U4__)
    //#define IR_USE_TIMER1   // tx = pin 14
    //#define IR_USE_TIMER3   // tx = pin 9
    #define IR_USE_TIMER4_HS  // tx = pin 10
    
  // Teensy++ 1.0 & 2.0
  #elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
    //#define IR_USE_TIMER1   // tx = pin 25
    #define IR_USE_TIMER2     // tx = pin 1
    //#define IR_USE_TIMER3   // tx = pin 16
    
  // Sanguino
  #elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
    //#define IR_USE_TIMER1   // tx = pin 13
    #define IR_USE_TIMER2     // tx = pin 14
    
  // Atmega8
  #elif defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
    #define IR_USE_TIMER1   // tx = pin 9
    
  // Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, etc
  #else
    //#define IR_USE_TIMER1   // tx = pin 9
    #define IR_USE_TIMER2     // tx = pin 3
  #endif
  
  
  #ifdef F_CPU
    #define SYSCLOCK F_CPU     // main Arduino clock
  #else
    #define SYSCLOCK 16000000  // main Arduino clock
  #endif
  
  
  // defines for timer2 (8 bits)
  #if defined(IR_USE_TIMER2)
    #define TIMER_ENABLE_PWM (TCCR2A |= _BV(COM2B1))
    #define TIMER_DISABLE_PWM (TCCR2A &= ~(_BV(COM2B1)))
    #define TIMER_DISABLE_INTR (TIMSK2 = 0)
    #define TIMER_CONFIG_KHZ(val) ({ const uint8_t pwmval = SYSCLOCK / 2000 / (val); TCCR2A = _BV(WGM20); TCCR2B = _BV(WGM22) | _BV(CS20); OCR2A = pwmval; OCR2B = pwmval / 3; })
    #if defined(CORE_OC2B_PIN)
      #define TIMER_PWM_PIN CORE_OC2B_PIN  /* Teensy */
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define TIMER_PWM_PIN 9  /* Arduino Mega */
    #elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
      #define TIMER_PWM_PIN 14 /* Sanguino */
    #else
      #define TIMER_PWM_PIN 3  /* Arduino Duemilanove, Diecimila, LilyPad, etc */
    #endif
    
  // defines for timer1 (16 bits)
  #elif defined(IR_USE_TIMER1)
    #define TIMER_ENABLE_PWM (TCCR1A |= _BV(COM1A1))
    #define TIMER_DISABLE_PWM (TCCR1A &= ~(_BV(COM1A1)))
    #if defined(__AVR_ATmega8P__) || defined(__AVR_ATmega8__)
      #define TIMER_DISABLE_INTR (TIMSK = 0)
    #else
      #define TIMER_DISABLE_INTR (TIMSK1 = 0)
    #endif
    #define TIMER_CONFIG_KHZ(val) ({ const uint16_t pwmval = SYSCLOCK / 2000 / (val); TCCR1A = _BV(WGM11); TCCR1B = _BV(WGM13) | _BV(CS10); ICR1 = pwmval; OCR1A = pwmval / 3; })
    #if defined(CORE_OC1A_PIN)
      #define TIMER_PWM_PIN CORE_OC1A_PIN  /* Teensy */
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define TIMER_PWM_PIN 11  /* Arduino Mega */
    #elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
      #define TIMER_PWM_PIN 13 /* Sanguino */
    #else
      #define TIMER_PWM_PIN 9  /* Arduino Duemilanove, Diecimila, LilyPad, etc */
    #endif
    
  // defines for timer3 (16 bits)
  #elif defined(IR_USE_TIMER3)
    #define TIMER_ENABLE_PWM (TCCR3A |= _BV(COM3A1))
    #define TIMER_DISABLE_PWM (TCCR3A &= ~(_BV(COM3A1)))
    #define TIMER_DISABLE_INTR (TIMSK3 = 0)
    #define TIMER_CONFIG_KHZ(val) ({ const uint16_t pwmval = SYSCLOCK / 2000 / (val); TCCR3A = _BV(WGM31); TCCR3B = _BV(WGM33) | _BV(CS30); ICR3 = pwmval; OCR3A = pwmval / 3; })
    #if defined(CORE_OC3A_PIN)
      #define TIMER_PWM_PIN CORE_OC3A_PIN  /* Teensy */
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define TIMER_PWM_PIN 5  /* Arduino Mega */
    #else
      #error "Please add OC3A pin number here\n"
    #endif
    
  // defines for timer4 (10 bits, high speed option)
  #elif defined(IR_USE_TIMER4_HS)
    #define TIMER_ENABLE_PWM (TCCR4A |= _BV(COM4A1))
    #define TIMER_DISABLE_PWM (TCCR4A &= ~(_BV(COM4A1)))
    #define TIMER_DISABLE_INTR (TIMSK4 = 0)
    #define TIMER_CONFIG_KHZ(val) ({ const uint16_t pwmval = SYSCLOCK / 2000 / (val); TCCR4A = (1<<PWM4A); TCCR4B = _BV(CS40); TCCR4C = 0; TCCR4D = (1<<WGM40); TCCR4E = 0; TC4H = pwmval >> 8; OCR4C = pwmval; TC4H = (pwmval / 3) >> 8; OCR4A = (pwmval / 3) & 255; })
    #if defined(CORE_OC4A_PIN)
      #define TIMER_PWM_PIN CORE_OC4A_PIN  /* Teensy */
    #elif defined(__AVR_ATmega32U4__)
      #define TIMER_PWM_PIN 13  /* Leonardo */
    #else
      #error "Please add OC4A pin number here\n"
    #endif
    
  // defines for timer4 (16 bits)
  #elif defined(IR_USE_TIMER4)
    #define TIMER_ENABLE_PWM (TCCR4A |= _BV(COM4A1))
    #define TIMER_DISABLE_PWM (TCCR4A &= ~(_BV(COM4A1)))
    #define TIMER_DISABLE_INTR (TIMSK4 = 0)
    #define TIMER_CONFIG_KHZ(val) ({ const uint16_t pwmval = SYSCLOCK / 2000 / (val); TCCR4A = _BV(WGM41); TCCR4B = _BV(WGM43) | _BV(CS40); ICR4 = pwmval; OCR4A = pwmval / 3; })
    #if defined(CORE_OC4A_PIN)
      #define TIMER_PWM_PIN CORE_OC4A_PIN
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define TIMER_PWM_PIN 6  /* Arduino Mega */
    #else
      #error "Please add OC4A pin number here\n"
    #endif
    
  // defines for timer5 (16 bits)
  #elif defined(IR_USE_TIMER5)
    #define TIMER_ENABLE_PWM (TCCR5A |= _BV(COM5A1))
    #define TIMER_DISABLE_PWM (TCCR5A &= ~(_BV(COM5A1)))
    #define TIMER_DISABLE_INTR (TIMSK5 = 0)
    #define TIMER_CONFIG_KHZ(val) ({ const uint16_t pwmval = SYSCLOCK / 2000 / (val); TCCR5A = _BV(WGM51); TCCR5B = _BV(WGM53) | _BV(CS50); ICR5 = pwmval; OCR5A = pwmval / 3; })
    #if defined(CORE_OC5A_PIN)
      #define TIMER_PWM_PIN CORE_OC5A_PIN
    #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      #define TIMER_PWM_PIN 46  /* Arduino Mega */
    #else
      #error "Please add OC5A pin number here\n"
    #endif
    
  #else // unknown timer
    #error "Internal code configuration error, no known IR_USE_TIMER# defined\n"
  #endif
#endif
