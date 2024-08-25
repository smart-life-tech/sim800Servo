#include <LayadCircuits_SalengGSM.h>
#include <SoftwareSerial.h>

#include <EEPROM.h>
int address = 0;
int open = 0;
int close = 1;
int lock_pin = 4;
// do not use the following: OK, ERROR, CMT, CMTI, Ready, ready
#define KEYWORD1 "Open"
#define KEYWORD2 "Close"

SoftwareSerial gsmSerial(3, 2); // rx tx

LayadCircuits_SalengGSM salengGSM = LayadCircuits_SalengGSM(&gsmSerial);
int servoPin = 9;
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    salengGSM.begin(115200); // this is the default baud rate
    Serial.begin(115200);
    gsmSerial.begin(115200);
    Serial.print(F("Preparing Saleng GSM Shield.Pls wait for 10 seconds..."));
    delay(5000); // allow 10 seconds for modem to boot up and register
    salengGSM.initSalengGSM();
    Serial.println(F("Done"));
    Serial.println(F("Send the keywords \"Close\" or \"Open\" via SMS to turn the system Close or Open"));
    pinMode(lock_pin, OUTPUT);
    if (EEPROM.read(0) == open)
    {
        digitalWrite(lock_pin, HIGH);
        delay(10);
    }
    else if (EEPROM.read(0) == 1)
    {
        digitalWrite(lock_pin, LOW);
        delay(10);
    }
}

void loop()
{
    // while (gsmSerial.available()) Serial.print(char(gsmSerial.read()));
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
            Serial.println("open received moving servo");
            EEPROM.update(address, open);
            // do something here
            digitalWrite(LED_BUILTIN, HIGH);

            digitalWrite(lock_pin, HIGH);
            delay(10);
        }
        else if (strstr(salengGSM.smsRxMsg, KEYWORD2)) // check if keyword 2 was received
        {
            Serial.println("close received moving servo");
            // do something here
            EEPROM.update(address, close);
            digitalWrite(LED_BUILTIN, LOW);

            digitalWrite(lock_pin, LOW);
            delay(10);
        }
    }
}