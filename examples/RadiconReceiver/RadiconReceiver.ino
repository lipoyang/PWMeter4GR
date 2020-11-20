// This is for GR-ROSE

#include "PWMeter4GR.h"

PWMeter pwmeterA1;
PWMeter pwmeterA2;
PWMeter pwmeterA3;
PWMeter pwmeterA4;

void setup()
{
    Serial.begin(115200);
    
    pwmeterA1.begin(A1, PWMETER_POSITIVE, PWMETER_1USEC);
    pwmeterA2.begin(A2, PWMETER_POSITIVE, PWMETER_1USEC);
    pwmeterA3.begin(A3, PWMETER_POSITIVE, PWMETER_1USEC);
    pwmeterA4.begin(A4, PWMETER_POSITIVE, PWMETER_1USEC);
}

void loop()
{
    bool flag1 = pwmeterA1.available();
    bool flag2 = pwmeterA2.available();
    bool flag3 = pwmeterA3.available();
    bool flag4 = pwmeterA4.available();
    
#if 1
    // for 4 channel receiver
    if(flag1 && flag2 && flag3 && flag4){
        uint16_t pw1 = pwmeterA1.getLast();
        uint16_t pw2 = pwmeterA2.getLast();
        uint16_t pw3 = pwmeterA3.getLast();
        uint16_t pw4 = pwmeterA4.getLast();
        
        Serial.print(pw1); Serial.print(", ");
        Serial.print(pw2); Serial.print(", ");
        Serial.print(pw3); Serial.print(", ");
        Serial.print(pw4); Serial.println("");
    }
#else
    // for 2 channel receiver
    if(flag1 && flag2){
        uint16_t pw1 = pwmeterA1.getLast();
        uint16_t pw2 = pwmeterA2.getLast();
        
        Serial.print(pw1); Serial.print(", ");
        Serial.print(pw2); Serial.println("");
    }
#endif
    
    delay(100);
}

