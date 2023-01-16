#ifndef LED_H_
#define LED_H_

#include<string>
#include<fstream>

using std::string;
using std::ofstream;

#define LED_PATH "/sys/class/leds/beaglebone:green:usr"

namespace exploringBB {
    
class LED{
   private:
      string path;
      int number;
      virtual void writeLED(string filename, string value);
   public:
      LED(int number);
      virtual void removeTrigger();
      virtual void turnOn();
      virtual void turnOff();
      virtual void flash(string delayms);
      virtual void outputState();
      virtual ~LED();
};

}

#endif