#include "EmonLib.h"
#include "Arduino.h"
#include <EEPROM.h>
#include <avr/pgmspace.h>

EnergyMonitor emon1, emon2, emon3;

double Irmsold1 = 0;
double Irms1 = 0;
double Vrms1 = 0;
double Pwr1 = 0;
float Wh1 = 0;
uint32_t kWh1 = 0;

double Irmsold2 = 0;
double Irms2 = 0;
double Vrms2 = 0;
double Pwr2 = 0;
float Wh2 = 0;
uint32_t kWh2 = 0;

double Irmsold3 = 0;
double Irms3 = 0;
double Vrms3 = 0;
double Pwr3 = 0;
float Wh3 = 0;
uint32_t kWh3 = 0;

int adresse1=0;   // startadress for kWh1 adress 0 to 1
int adresse2=2;   // startadress for kWh2 adress 2 to 3
int adresse3=4;   // startadress for kWh3 adress 4 to 5

const long writeinterval = 3600000; // 60 minute timer for write kWh to eeprom
const long measureinterval = 60000; // 1 minute timer for meassure volt & current

unsigned long previousMillis = 0;   // counter for write
unsigned long previousTime = 0;     // counter for meassure


int erasePin = 4;    //the erase button pin

void setup()


{
  Serial.begin(9600);
  Serial.println("Starting 3 Phase Energy Monitor V2 :  ");
  pinMode(erasePin, INPUT_PULLUP);

  readWh();
  Serial.print("Energy : ");
  Serial.print(kWh1);              // kWh
  Serial.print(" kWh1 : ");

  Serial.print(kWh2);              // kWh
  Serial.print(" kWh2 : ");

  Serial.print(kWh3);              // kWh
  Serial.println(" kWh3 ");

  
  emon1.current(A2, 64.5);             // Current: input pin, calibration.
  emon2.current(A1, 64.3);             // Current: input pin, calibration.
  emon3.current(A0, 64.4);             // Current: input pin, calibration.


  emon1.voltage(A3, 210, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon2.voltage(A4, 210, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon3.voltage(A5, 210, 1.7);  // Voltage: input pin, calibration, phase_shift
}

void loop()
{

if(!digitalRead(erasePin)) //check if erase button is pressed
  {
    eraseFlash();
    delay(500);
  }


unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= writeinterval) 
   {
    
    previousMillis = currentMillis;
    writeWh();
   }
   
if (currentMillis - previousTime >= measureinterval) 
   {
    
    previousTime = currentMillis;
    measure();
   }
}

void writeWh()
{
   Serial.println("writing values to eeprom :  ");

  eepromWriteInt(adresse1,kWh1);
  eepromWriteInt(adresse2,kWh2);
  eepromWriteInt(adresse3,kWh3);


}

void readWh()
{

  Serial.println("reading values from eeprom :  ");
  kWh1 = (eepromReadInt(adresse1));
  Wh1 = kWh1*1000;
  delay(10);
  kWh2 = (eepromReadInt(adresse2));
  Wh2 = kWh2*1000;
  delay(10);
  kWh3 = (eepromReadInt(adresse3));
  Wh3 = kWh3*1000;


}

void eraseFlash()
{
for (int i = 0; i <= 5; i++) {
    EEPROM.write(i, 0); 
    Serial.println(i);
    delay(100);
    
  }


  Serial.println("EEPROM erased");
  
}

void measure()
{
  emon1.calcVI(1480,2000);
Vrms1 = emon1.Vrms; 
Irms1 = emon1.Irms-0.11;  // Calculate Irms only
if (Irms1<0.01){
  Irms1 = 0.00;

}
Pwr1 = Vrms1*Irms1;
Wh1 = Wh1+(Pwr1/60);
kWh1 = Wh1/1000;

emon2.calcVI(1480,2000);
Vrms2 = emon2.Vrms; 
Irms2 = emon2.Irms-0.11;  // Calculate Irms only
if (Irms2<0.01){
  Irms2 = 0.00;

}
Pwr2 = Vrms2*Irms2;
Wh2 = Wh2+(Pwr2/60);
kWh2 = Wh2/1000;

emon3.calcVI(1480,2000);
Vrms3 = emon3.Vrms; 
Irms3 = emon3.Irms-0.11;  // Calculate Irms only
if (Irms3<0.01){
  Irms3 = 0.00;

}
Pwr3 = Vrms3*Irms3;
Wh3 = Wh3+(Pwr3/60);
kWh3 = Wh3/1000;




Serial.print("Phase_1 : ");
  Serial.print(Pwr1);              // Pwr
  Serial.print(" W : ");

  Serial.print(Irms1);             // Irms
  Serial.print(" A : ");

  Serial.print(Vrms1);             // Vrms
  Serial.println(" V ");

Serial.print("Phase_2 : ");
  Serial.print(Pwr2);              // Pwr
  Serial.print(" W : ");

  Serial.print(Irms2);             // Irms
  Serial.print(" A : ");

  Serial.print(Vrms2);             // Vrms
  Serial.println(" V ");

Serial.print("Phase_3 : ");
  Serial.print(Pwr3);              // Pwr
  Serial.print(" W : ");

  Serial.print(Irms3);             // Irms
  Serial.print(" A : ");

  Serial.print(Vrms3);             // Vrms
  Serial.println(" V ");

Serial.print("Energy : ");
  Serial.print(kWh1);              // kWh
  Serial.print(" kWh1 : ");

  Serial.print(kWh2);              // kWh
  Serial.print(" kWh2 : ");

  Serial.print(kWh3);              // kWh
  Serial.println(" kWh3 ");

}


void eepromWriteInt(int adr, uint16_t wert) {


byte low, high;

  low=wert&0xFF;
  high=(wert>>8)&0xFF;
  EEPROM.write(adr, low); // dauert 3,3ms 
  EEPROM.write(adr+1, high);
  return;
} 


int eepromReadInt(int adr) {


byte low, high;

  low=EEPROM.read(adr);
  high=EEPROM.read(adr+1);
  return low + ((high << 8)&0xFF00);
} 
