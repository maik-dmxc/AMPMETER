#include "EmonLib.h"
#include "Arduino.h"

// Include Emon Library
EnergyMonitor emon1, emon2, emon3;
// Create an instance
double Irmsold1 = 0;
double Irms1 = 0;
double Vrms1 = 0;
double Pwr1 = 0;

double Irmsold2 = 0;
double Irms2 = 0;
double Vrms2 = 0;
double Pwr2 = 0;

double Irmsold3 = 0;
double Irms3 = 0;
double Vrms3 = 0;
double Pwr3 = 0;

void setup()


{
  Serial.begin(9600);



  emon1.current(A2, 64.5);             // Current: input pin, calibration.
  emon2.current(A1, 64.3);             // Current: input pin, calibration.
  emon3.current(A0, 64.4);             // Current: input pin, calibration.

/*  emon1.voltage(A3, 163.4, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon2.voltage(A4, 164.4, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon3.voltage(A5, 163.4, 1.7);  // Voltage: input pin, calibration, phase_shift
*/

  emon1.voltage(A3, 204.8, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon2.voltage(A4, 205, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon3.voltage(A5, 203.8, 1.7);  // Voltage: input pin, calibration, phase_shift
}

void loop()
{

emon1.calcVI(1480,2000);
Vrms1 = emon1.Vrms; 
Irms1 = emon1.Irms-0.11;  // Calculate Irms only
if (Irms1<0.01){
  Irms1 = 0.00;

}
Pwr1 = Vrms1*Irms1;

emon2.calcVI(1480,2000);
Vrms2 = emon2.Vrms; 
Irms2 = emon2.Irms-0.11;  // Calculate Irms only
if (Irms2<0.01){
  Irms2 = 0.00;

}
Pwr2 = Vrms2*Irms2;

emon3.calcVI(1480,2000);
Vrms3 = emon3.Vrms; 
Irms3 = emon3.Irms-0.11;  // Calculate Irms only
if (Irms3<0.01){
  Irms3 = 0.00;

}
Pwr3 = Vrms3*Irms3;


Serial.print("Phase_1 : ");
  Serial.print(Pwr1);           // Apparent power
  Serial.print(" W : ");

  Serial.print(Irms1);             // Irms
  Serial.print(" A : ");

  Serial.print(Vrms1);             // Irms
  Serial.println(" V ");

Serial.print("Phase_2 : ");
  Serial.print(Pwr2);           // Apparent power
  Serial.print(" W : ");

  Serial.print(Irms2);             // Irms
  Serial.print(" A : ");

  Serial.print(Vrms2);             // Irms
  Serial.println(" V ");

Serial.print("Phase_3 : ");
  Serial.print(Pwr3);           // Apparent power
  Serial.print(" W : ");

  Serial.print(Irms3);             // Irms
  Serial.print(" A : ");

  Serial.print(Vrms3);             // Irms
  Serial.println(" V ");


delay(54000);

}



