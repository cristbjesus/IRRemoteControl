/*
 * IRRemoteControl - Lightweight library for send/receive infra-red signal.
 * This library can read codes from flash memory if necessary.
 * Created by Cristiano Borges - STI - SENAI Fatesg
 * August, 2017
 */

#include "IRRemoteControl.h"
#include "IRRemoteControlInt.h"

static unsigned int* IRRecv::irBuffer;
static int* IRRecv::irBufferLength;
static int IRRecv::maxIrBufferLength;

static unsigned long IRRecv::currentMicros = 0;
static unsigned long IRRecv::previousMicros = 0;

void IRRecv::start(int pin, unsigned int buf[], int bufLength, int& currentBufLength) {
  currentBufLength = 0;
  
  irBuffer = buf;
  irBufferLength = &currentBufLength;
  maxIrBufferLength = bufLength;
  
  currentMicros = 0;
  previousMicros = 0;
  
  attachInterrupt(digitalPinToInterrupt(pin), irInterruptHandler, CHANGE);
}

void IRRecv::stop(int pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}

void IRRecv::irInterruptHandler() {
  if (*irBufferLength < maxIrBufferLength) {
    previousMicros = currentMicros;
    currentMicros = micros();
    
    if (previousMicros > 0) {
      irBuffer[(*irBufferLength)++] = currentMicros - previousMicros;
    }
  }
}

void IRSend::sendRaw(unsigned int buf[], int bufLength, int freqKhz, bool useFlashMemory) {
  enableIROut(freqKhz);
  for (int i = 0; i < bufLength; i++) {
    int uSec = useFlashMemory ? pgm_read_word_near(buf + i) : buf[i];
    if (i & 1) {
      space(uSec);
    } else {
      mark(uSec);
    }
  }
  space(0); // Just to be sure
}

void IRSend::enableIROut(int freqKhz) {
  // Enables IR output.  The freqKhz value controls the modulation frequency in kilohertz.
  // The IR output will be on pin 3 (OC2B).
  // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
  // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
  // TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
  // controlling the duty cycle.
  // There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
  // To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
  // A few hours staring at the ATmega documentation and this will all make sense.
  // See the Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.
  
  
  // Disable the Timer2 Interrupt (which is used for receiving IR)
  TIMER_DISABLE_INTR; //Timer2 Overflow Interrupt
  
  pinMode(TIMER_PWM_PIN, OUTPUT);
  digitalWrite(TIMER_PWM_PIN, LOW); // When not sending PWM, we want it low
  
  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2 = 000: no prescaling
  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  TIMER_CONFIG_KHZ(freqKhz);
}

void IRSend::mark(int uSec) {
  // Sends an IR mark for the specified number of microseconds.
  // The mark output is modulated at the PWM frequency.
  TIMER_ENABLE_PWM; // Enable pin 3 PWM output
  delayMicroseconds(uSec);
}

/* Leave pin off for time (given in microseconds) */
void IRSend::space(int uSec) {
  // Sends an IR space for the specified number of microseconds.
  // A space is no output, so the PWM output is disabled.
  TIMER_DISABLE_PWM; // Disable pin 3 PWM output
  delayMicroseconds(uSec);
}
