/*
 * IRRemoteControl - Lightweight library for send/receive infra-red signal.
 * This library can read codes from flash memory if necessary.
 * Created by Cristiano Borges - STI - SENAI Fatesg
 * August, 2017
 */

#ifndef IRRemoteControl_h
  #define IRRemoteControl_h
  
  class IRRecv {
    public:
      IRRecv() {}
      void start(int pin, unsigned int buf[], int bufLength, int& currentBufLength);
      void stop(int pin);
    private:
      static unsigned int* irBuffer;
      static int* irBufferLength;
      static int maxIrBufferLength;
      static unsigned long currentMicros;
      static unsigned long previousMicros;
      static void irInterruptHandler();
  };
  
  class IRSend {
    public:
      IRSend() {}
      void sendRaw(unsigned int buf[], int bufLength, int freqKhz, bool useFlashMemory);
    private:
      void enableIROut(int freqKhz);
      void mark(int uSec);
      void space(int uSec);
  };
#endif
