// Runs an example sketch on a PC: setup() once, then loop() for a few simulated seconds.
#include "Arduino.h"

HostSerial Serial;

namespace { unsigned long theMillis = 0; }

unsigned long millis()       { return theMillis; }
void delay(unsigned long ms) { theMillis += ms; }

void setup();
void loop();

int main()
{
  setup();
  for (int i = 0; i < 150; ++i)
  {
    loop();
    theMillis += 1;   // a loop() that never delays still lets time pass
  }
  return 0;
}
