
#include <LayadCircuits_SalengGSM.h>
#include <SoftwareSerial.h>
#include <Servo.h>
Servo servo;
// do not use the following: OK, ERROR, CMT, CMTI, Ready, ready
#define KEYWORD1 "Close"
#define KEYWORD2 "Open"

SoftwareSerial gsmSerial(6, 7);

LayadCircuits_SalengGSM salengGSM = LayadCircuits_SalengGSM(&gsmSerial);
int servoPin = 9;
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    salengGSM.begin(9600); // this is the default baud rate
    Serial.begin(9600);
    Serial.print(F("Preparing Saleng GSM Shield.Pls wait for 10 seconds..."));
    delay(10000); // allow 10 seconds for modem to boot up and register
    salengGSM.initSalengGSM();
    Serial.println(F("Done"));
    Serial.println(F("Send the keywords \"Open\" or \"Close\" via SMS to turn the builtin LED on or off"));
    servo.attach(servoPin);
}

void loop()
{
    salengGSM.smsMachine();         // we need to pass here as fast as we can. this allows for non-blocking SMS transmission
    if (salengGSM.isSMSavailable()) // we also need to pass here as frequent as possible to check for incoming messages
    {
        salengGSM.readSMS(); // updates the read flag
        Serial.print("Sender=");
        Serial.println(salengGSM.smsSender);
        Serial.print("Whole Message=");
        Serial.println(salengGSM.smsRxMsg); // if we receive an SMS, print the contents of the receive buffer

        if (strstr(salengGSM.smsRxMsg, KEYWORD1)) // check if keyword 1 was received
        {
            // do something here
            Serial.println("on recived");
            digitalWrite(LED_BUILTIN, HIGH);
            for (int i = 0; i < 180; i++)
            {
                servo.write(i);
                delay(10);
            }
        }
        else if (strstr(salengGSM.smsRxMsg, KEYWORD2)) // check if keyword 2 was received
        {
            Serial.println("off recived");
            // do something here
            digitalWrite(LED_BUILTIN, LOW);
            for (int i = 180; i > 0; i--)
            {
                servo.write(i);
                delay(10);
            }
        }
    }
}
