/*------------------------------------------------------------------------------
  10/04/2020
  Author: Narendok/Shivam @ROBOSLOG TEAM
  Platforms: ESP8266
  Language: C++/Arduino
  File: ESP_Agri_thingspeak.ino
------------------------------------------
  Please consider buying products from ROBOSLOF to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use.

  https://roboslog.in/agri
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.   
 ----------------------------------------------------------------------------- */
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

IPAddress ip(192, 168, 43, 228); //set static ip
IPAddress gateway(192, 168, 43, 1); //set getteway
IPAddress subnet(255, 255, 255, 0);//set subnet
// initialize the library with the numbers of the interface pins
int period =200;
unsigned long time_now = 0;
String elm="";
byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
String WorkingString="";
long DisplayValue;
long A, B,C,D;
bool temp=0;
String 
Gps_Lat="",
Gps_Lon="",
STATUS2="",
FREEZE_DTC="",
FUEL_STATUS="",
ENGINE_LOAD="",
COOLANT_TEMP="",
SHORT_FUEL_TRIM_1="",
LONG_FUEL_TRIM_1="",
SHORT_FUEL_TRIM_2="",
LONG_FUEL_TRIM_2="",
FUEL_PRESSURE="",
INTAKE_PRESSURE="",
RPM="",
SPEED="",
TIMING_ADVANCE="",
INTAKE_TEMP="",
MAF="",
THROTTLE_POS="",
AIR_STATUS="",
O2_SENSORS="",
O2_B1S1="",
O2_B1S2="",
O2_B1S3="",
O2_B1S4="",
O2_B2S1="",
O2_B2S2="",
O2_B2S3="",
O2_B2S4="",
OBD_COMPLIANCE="",
O2_SENSORS_ALT="",
AUX_INPUT_STATUS="",
RUN_TIME="",
DISTANCE_W_MIL="",
FUEL_RAIL_PRESSURE_VAC="",
FUEL_RAIL_PRESSURE_DIRECT="",
O2_S1_WR_VOLTAGE="",
O2_S2_WR_VOLTAGE="",
O2_S3_WR_VOLTAGE="",
O2_S4_WR_VOLTAGE="",
O2_S5_WR_VOLTAGE="",
O2_S6_WR_VOLTAGE="",
O2_S7_WR_VOLTAGE="",
O2_S8_WR_VOLTAGE="",
COMMANDED_EGR="",
EGR_ERROR="",
EVAPORATIVE_PURGE="",
FUEL_LEVEL="",
WARMUPS_SINCE_DTC_CLEAR="",
DISTANCE_SINCE_DTC_CLEAR="",
EVAP_VAPOR_PRESSURE="",
BAROMETRIC_PRESSURE="",
O2_S1_WR_CURRENT="",
O2_S2_WR_CURRENT="",
O2_S3_WR_CURRENT="",
O2_S4_WR_CURRENT="",
O2_S5_WR_CURRENT="",
O2_S6_WR_CURRENT="",
O2_S7_WR_CURRENT="",
O2_S8_WR_CURRENT="",
CATALYST_TEMP_B1S1="",
CATALYST_TEMP_B2S1="",
CATALYST_TEMP_B1S2="",
CATALYST_TEMP_B2S2="",
STATUS_DRIVE_CYCLE="",
CONTROL_MODULE_VOLTAGE="",
ABSOLUTE_LOAD="",
COMMANDED_EQUIV_RATIO="",
RELATIVE_THROTTLE_POS="",
AMBIANT_AIR_TEMP="",
THROTTLE_POS_B="",
THROTTLE_POS_C="",
ACCELERATOR_POS_D="",
ACCELERATOR_POS_E="",
ACCELERATOR_POS_F="",
THROTTLE_ACTUATOR="",
RUN_TIME_MIL="",
TIME_SINCE_DTC_CLEARED="",
unsupported="",
MAX_MAF="",
FUEL_TYPE="",
ETHANOL_PERCENT="",
EVAP_VAPOR_PRESSURE_ABS="",
EVAP_VAPOR_PRESSURE_ALT="",
SHORT_O2_TRIM_B1="",
LONG_O2_TRIM_B1="",
SHORT_O2_TRIM_B2="",
LONG_O2_TRIM_B2="",
FUEL_RAIL_PRESSURE_ABS="",
RELATIVE_ACCEL_POS="",
HYBRID_BATTERY_REMAINING="",
OIL_TEMP="",
FUEL_INJECT_TIMING="",
FUEL_RATE="",
PID01to20,PID21to40,PID41to60,PID61to80,PID81toA0;
TinyGPSPlus gps;  // The TinyGPS++ object

SoftwareSerial ss(4,5); // The serial connection to the GPS device
//SoftwareSerial s(D5, D6); // 

const char* ssid = "Robo";
const char* password = "lopklopk";
const char* host = "gpsobd.000webhostapp.com";

float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
bool pm;
char buff[32];
String Dev_ID="RSSS101";
 WiFiServer server(80); 
void setup()
{
  Serial1.begin(115200);
  ss.begin(9600);
  Serial.begin(38400);
  //Serial.println();
//  Serial.print("Connecting to ");
  //Serial.println(ssid);
    WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
 Serial1.print(".");
  }
//  Serial.println("");
 Serial1.println("WiFi connected");

delay(1000);
//  lcd.clear();

//********************************************************************
  
  //Set up s communication at 9600 baud
  Serial.begin(38400);   //initialize  
  
server.begin();

 Serial.println("Server started");

  // Print the IP address
 // Serial.println(WiFi.localIP());
  retry:
Serial.println("atz");
 
  time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
  // Serial.println("reading...      ");
  ReadData();
   Serial1.println(BuildINString);                                           // If used substring(1,4)=="ATZ" needed a space before ATZ in Serial Monitor and it did not work
if (BuildINString.indexOf("EL")>=0)    
{
   Serial1.println("Elm Connected");
    elm="Connected";
    temp=1;
 // digitalWrite(D4,0);
    }
    
    else
    {
  Serial1.println("No connected");
  // digitalWrite(D4,1);
   elm="Not_Conn";
   temp=0;
   goto retry;
    }
    
  Serial.println("ATSP0");
Serial1.println("Auto searching protocol");
  time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
  ReadData();
  Serial1.println(BuildINString);

Serial.println("01 00");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<50)
  WorkingString = BuildINString.substring(13, 23);
  else
   WorkingString = BuildINString.substring(41, 52);
Serial1.println(WorkingString);
  WorkingString.replace(" ", "");
  Serial1.println(WorkingString);
  B = strtoul(WorkingString.c_str(), NULL, 16);
   Serial1.println(B);
 PID01to20= itoa(B,buff,2);
  Serial1.println(PID01to20);
   // Creating array of string length  
   
}

void loop()

{
  
 Serial.println("01 01");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
  ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   STATUS2 = String(DisplayValue);
   

 Serial.println("01 02");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   FREEZE_DTC = String(DisplayValue);


 Serial.println("01 03");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   FUEL_STATUS = String(DisplayValue);


 Serial.println("01 04");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);
Serial1.println(WorkingString);
  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2.55;
   ENGINE_LOAD = String(DisplayValue);


   Serial.println("01 05");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);
   Serial1.println(WorkingString);
    A = strtol(WorkingString.c_str(),NULL,16);
   DisplayValue = A-40;
   COOLANT_TEMP = String(DisplayValue);

   Serial.println("01 06");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = (A-128)*100/128;
   SHORT_FUEL_TRIM_1 = String(DisplayValue);


      Serial.println("01 07");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = (A-128)*100/128;
   LONG_FUEL_TRIM_1 = String(DisplayValue);


      Serial.println("01 08");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = (A-128)*100/128;
   SHORT_FUEL_TRIM_2 = String(DisplayValue);



      Serial.println("01 09");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = (A-128)*100/128;
   LONG_FUEL_TRIM_2 = String(DisplayValue);
   


   Serial.println("01 0A");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A*3;
   FUEL_PRESSURE = String(DisplayValue);

   
   Serial.println("01 0B");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   INTAKE_PRESSURE = String(DisplayValue);
   
////////////////////////////////////////// Need RPM & speed to be copy paste

   Serial.println("01 0C");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
  Serial1.println(WorkingString);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
    Serial1.println(WorkingString);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
     Serial1.println(WorkingString);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
  Serial1.println(WorkingString);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }

   DisplayValue = ((A * 256)+B)/4;
   RPM = String(DisplayValue);

Serial.println("01 0D");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   SPEED = String(DisplayValue);
   
   Serial.println("01 0E");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2-64;
   TIMING_ADVANCE = String(DisplayValue);
 


   Serial.println("01 0F");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A-40;
   INTAKE_TEMP = String(DisplayValue);
  




   Serial.println("01 10");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A * 256)+B)/4;
   MAF = String(DisplayValue);
  




   Serial.println("01 11");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2.55;
   THROTTLE_POS = String(DisplayValue);
  


   Serial.println("01 12");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2-64;
   AIR_STATUS = String(DisplayValue);
  




   Serial.println("01 13");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2-64;
   O2_SENSORS = String(DisplayValue);
  



   Serial.println("01 14");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
 String volta=String(A/200);
 String stfb=String((B*100/128)-100);
   O2_B1S1 = volta+"volt"+stfb+"%";
  



   Serial.println("01 15");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B1S2 = volta+"volt"+stfb+"%";


   Serial.println("01 16");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B1S3 = volta+"volt"+stfb+"%";


   Serial.println("01 17");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B1S4 = volta+"volt"+stfb+"%";


   Serial.println("01 18");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B2S1 = volta+"volt"+stfb+"%";


   Serial.println("01 19");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B2S2 = volta+"volt"+stfb+"%";
 


   Serial.println("01 1A");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B2S3 = volta+"volt"+stfb+"%";
  


   Serial.println("01 1B");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
  volta=String(A/200);
  stfb=String((B*100/128)-100);
   O2_B2S4 = volta+"volt"+stfb+"%";



   Serial.println("01 1C");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2-64;
   OBD_COMPLIANCE = String(DisplayValue);
  



   Serial.println("01 1D");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2-64;
   O2_SENSORS_ALT = String(DisplayValue);
  



   Serial.println("01 1E");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A/2-64;
   AUX_INPUT_STATUS = String(DisplayValue);
  



   Serial.println("01 1F");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
 if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = A*256+B;
   RUN_TIME = String(DisplayValue);
  
 ///////////////////////////////////////////////////////////////////////////////////////////////

   Serial.println("01 21");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =  A*256+B;
   DISTANCE_W_MIL = String(DisplayValue);
  



   Serial.println("01 22");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = (A*256+B)*0.079;
   FUEL_RAIL_PRESSURE_VAC = String(DisplayValue);
  



   Serial.println("01 23");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
       if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = 10*(256*A+B);
   FUEL_RAIL_PRESSURE_DIRECT = String(DisplayValue);


   Serial.println("01 24");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
 String ratioAB=String(((256*A)+B)*2/65536);
 String voltCD=String(((256*C)+D)*8/65536);
   O2_S1_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;
    

   Serial.println("01 25");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S2_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;


   Serial.println("01 26");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S3_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;



   Serial.println("01 27");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S4_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;


   Serial.println("01 28");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S5_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;

   Serial.println("01 29");
   
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S6_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;
   

   Serial.println("01 2A");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S7_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;



   Serial.println("01 2B");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  voltCD=String(((256*C)+D)*8/65536);
   O2_S8_WR_VOLTAGE = "A:B"+ ratioAB +"volt="+voltCD;




   Serial.println("01 2C");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = 100*A/255;
   COMMANDED_EGR = String(DisplayValue);
  



   Serial.println("01 2D");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = 100*A/128-100;
   EGR_ERROR = String(DisplayValue);
  


   Serial.println("01 2E");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =100*A/255;
   EVAPORATIVE_PURGE = String(DisplayValue);
  



   Serial.println("01 2F");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =100*A/255;
   FUEL_LEVEL = String(DisplayValue);
  



   Serial.println("01 30");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   WARMUPS_SINCE_DTC_CLEAR = String(DisplayValue);
  



   Serial.println("01 31");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
 if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = 256*A+B;
   DISTANCE_SINCE_DTC_CLEAR = String(DisplayValue);
  



   Serial.println("01 32");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = 256*A+B;
   EVAP_VAPOR_PRESSURE = String(DisplayValue);
  



   Serial.println("01 33");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   BAROMETRIC_PRESSURE = String(DisplayValue);
  


String ampCD="";
   Serial.println("01 34");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
 if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S1_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;


 
  



   Serial.println("01 35");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S2_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;





   Serial.println("01 36");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
  if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S3_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;




   Serial.println("01 37");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S4_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;




   Serial.println("01 38");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S5_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;


  
   Serial.println("01 39");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S6_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;




  
   Serial.println("01 3A");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S7_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;



   Serial.println("01 3B");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  ratioAB=String(((256*A)+B)*2/65536);
  ampCD=String((((256*C)+D)/256)-128);
   O2_S8_WR_CURRENT = "Fuel:Air"+ ratioAB +"&Amp="+ampCD;



  
   Serial.println("01 3C");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/10)-40;
   CATALYST_TEMP_B1S1 = String(DisplayValue);

  
  
   Serial.println("01 3D");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/10)-40;
   CATALYST_TEMP_B2S1 = String(DisplayValue);
  




   Serial.println("01 3E");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/10)-40;
   CATALYST_TEMP_B1S2 = String(DisplayValue);

  
   Serial.println("01 3F");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/10)-40;
   CATALYST_TEMP_B2S2 = String(DisplayValue);

 ///////////////////////////////////////////////////////////////////////////////////////////////

 

  
   Serial.println("01 41");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   STATUS_DRIVE_CYCLE = String(DisplayValue);



  
   Serial.println("01 42");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/1000);
   CONTROL_MODULE_VOLTAGE = String(DisplayValue);

  
   Serial.println("01 43");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
   if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/255)*100;
   ABSOLUTE_LOAD = String(DisplayValue);



  
   Serial.println("01 44");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B)/65536*2);
   COMMANDED_EQUIV_RATIO = String(DisplayValue);



  
   Serial.println("01 45");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A*100/255;
   RELATIVE_THROTTLE_POS = String(DisplayValue);




  
   Serial.println("01 46");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A-40;
   AMBIANT_AIR_TEMP = String(DisplayValue);



  
   Serial.println("01 47");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A*100/255;
   THROTTLE_POS_B = String(DisplayValue);




  
   Serial.println("01 48");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =  A*100/255;
   THROTTLE_POS_C = String(DisplayValue);



  
   Serial.println("01 49");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =  A*100/255;
   ACCELERATOR_POS_D = String(DisplayValue);
  


   Serial.println("01 4A");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =  A*100/255;
   ACCELERATOR_POS_E = String(DisplayValue);


   
   Serial.println("01 4B");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =  A*100/255;
   ACCELERATOR_POS_F = String(DisplayValue);


   
   Serial.println("01 4C");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue =  A*100/255;
   THROTTLE_ACTUATOR = String(DisplayValue);




   Serial.println("01 4D");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
   if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B));
   RUN_TIME_MIL = String(DisplayValue);



   Serial.println("01 4E");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
     if(BuildINString.length()<25)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  
  }
  else
  { 
     WorkingString = BuildINString.substring(27, 29);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(30, 32);
   B = strtol(WorkingString.c_str(), NULL, 16);
  }
   DisplayValue = ((A*256+B));
   TIME_SINCE_DTC_CLEARED = String(DisplayValue);

   
   Serial.println("01 4F");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
 
   unsupported = "A=";



   Serial.println("01 50");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<34)
  {
  WorkingString = BuildINString.substring(13, 15);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(16, 18);
   B = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(19, 21);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(22, 24);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  else
  { 
     WorkingString = BuildINString.substring(33, 35);
   A = strtol(WorkingString.c_str(), NULL, 16);
  WorkingString = BuildINString.substring(36, 38);
   B = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(39, 41);
   C = strtol(WorkingString.c_str(), NULL, 16);
   WorkingString = BuildINString.substring(42, 44);
   D = strtol(WorkingString.c_str(), NULL, 16);
  }
  A=A*10;
 
   MAX_MAF = "A=";
   

   Serial.println("01 51");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   FUEL_TYPE = String(DisplayValue);



   
   Serial.println("01 52");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = 100*A/255;
   ETHANOL_PERCENT = String(DisplayValue);




   
   Serial.println("01 53");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   EVAP_VAPOR_PRESSURE_ABS = String(DisplayValue);

   
   Serial.println("01 54");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   EVAP_VAPOR_PRESSURE_ALT = String(DisplayValue);



   
   Serial.println("01 55");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   SHORT_O2_TRIM_B1 = String(DisplayValue);



   
   Serial.println("01 56");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   LONG_O2_TRIM_B1 = String(DisplayValue);



   
   Serial.println("01 57");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   SHORT_O2_TRIM_B2 = String(DisplayValue);


   
   Serial.println("01 58");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   LONG_O2_TRIM_B2 = String(DisplayValue);


   
   Serial.println("01 59");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   FUEL_RAIL_PRESSURE_ABS = String(DisplayValue);


   
   Serial.println("01 5A");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   RELATIVE_ACCEL_POS = String(DisplayValue);


   
   Serial.println("01 5B");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   HYBRID_BATTERY_REMAINING = String(DisplayValue);



   
   Serial.println("01 5C");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   OIL_TEMP = String(DisplayValue);

 Serial.println("01 5D");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   FUEL_INJECT_TIMING = String(DisplayValue);

 Serial.println("01 5E");
time_now = millis();
while(millis()< time_now + period)
{
  Gp();
  Up();
}
 ReadData();
  Serial1.println(BuildINString);
    if(BuildINString.length()<22)
  WorkingString = BuildINString.substring(13, 15);
  else
   WorkingString = BuildINString.substring(27, 29);

  A = strtol(WorkingString.c_str(), NULL, 16);
   DisplayValue = A;
   FUEL_RATE = String(DisplayValue);

postData();

}



void ReadData()
{
BuildINString="";  
  while(Serial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = Serial.read();
    inChar=char(inData);
    BuildINString = BuildINString + inChar;
  }

}

void Gp()
{
   while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        Gps_Lat= lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        Gps_Lon= lng_str = String(longitude , 6);
      }

      if (gps.date.isValid())
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          date_str = '0';
        date_str += String(date);

        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);

        date_str += " / ";

        if (year < 10)
          date_str += '0';
        date_str += String(year);
      }

      if (gps.time.isValid())
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();

        minute = (minute + 30);
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;

        if (hour >= 12)
          pm = 1;
        else
          pm = 0;

        hour = hour % 12;

        if (hour < 10)
          time_str = '0';
        time_str += String(hour);

        time_str += " : ";

        if (minute < 10)
          time_str += '0';
        time_str += String(minute);

        time_str += " : ";

        if (second < 10)
          time_str += '0';
        time_str += String(second);

        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";

      }
    }
}

void postData() {

   WiFiClient clientGet;
   const int httpGetPort = 80;
   //the path and file to send the data to:
   String s = "/InsertDB23.php";
   
  // We now create and add parameters:
 s+= "?Dev_ID=" ;
s+=Dev_ID;
s+="&Gps_Lat=";
s+=Gps_Lat;
s+="&Gps_Lon=";
s+=Gps_Lon;
  s+="&STATUS=";s+=STATUS2;s+="&FREEZE_DTC=";s+=FREEZE_DTC;s+="&FUEL_STATUS=";s+=FUEL_STATUS;s+="&ENGINE_LOAD=";s+=ENGINE_LOAD;s+="&COOLANT_TEMP=";s+=COOLANT_TEMP;s+="&SHORT_FUEL_TRIM_1=";s+=SHORT_FUEL_TRIM_1;s+="&LONG_FUEL_TRIM_1=";s+=LONG_FUEL_TRIM_1;s+="&SHORT_FUEL_TRIM_2=";s+=SHORT_FUEL_TRIM_2;s+="&LONG_FUEL_TRIM_2=";s+=LONG_FUEL_TRIM_2;s+="&FUEL_PRESSURE=";s+=FUEL_PRESSURE;s+="&INTAKE_PRESSURE=";s+=INTAKE_PRESSURE;s+="&RPM=";s+=RPM;s+="&SPEED=";s+=SPEED;s+="&TIMING_ADVANCE=";s+=TIMING_ADVANCE;s+="&INTAKE_TEMP=";s+=INTAKE_TEMP;s+="&MAF=";s+=MAF;s+="&THROTTLE_POS=";s+=THROTTLE_POS;s+="&AIR_STATUS=";s+=AIR_STATUS;s+="&O2_SENSORS=";s+=O2_SENSORS;s+="&O2_B1S1=";s+=O2_B1S1;s+="&O2_B1S2=";s+=O2_B1S2;s+="&O2_B1S3=";s+=O2_B1S3;s+="&O2_B1S4=";s+=O2_B1S4;s+="&O2_B2S1=";s+=O2_B2S1;s+="&O2_B2S2=";s+=O2_B2S2;s+="&O2_B2S3=";s+=O2_B2S3;s+="&O2_B2S4=";s+=O2_B2S4;s+="&OBD_COMPLIANCE=";s+=OBD_COMPLIANCE;s+="&O2_SENSORS_ALT=";s+=O2_SENSORS_ALT;s+="&AUX_INPUT_STATUS=";s+=AUX_INPUT_STATUS;s+="&RUN_TIME=";s+=RUN_TIME;s+="&DISTANCE_W_MIL=";s+=DISTANCE_W_MIL;s+="&FUEL_RAIL_PRESSURE_VAC=";s+=FUEL_RAIL_PRESSURE_VAC;s+="&FUEL_RAIL_PRESSURE_DIRECT=";s+=FUEL_RAIL_PRESSURE_DIRECT;s+="&O2_S1_WR_VOLTAGE=";s+=O2_S1_WR_VOLTAGE;s+="&O2_S2_WR_VOLTAGE=";s+=O2_S2_WR_VOLTAGE;s+="&O2_S3_WR_VOLTAGE=";s+=O2_S3_WR_VOLTAGE;s+="&O2_S4_WR_VOLTAGE=";s+=O2_S4_WR_VOLTAGE;s+="&O2_S5_WR_VOLTAGE=";s+=O2_S5_WR_VOLTAGE;s+="&O2_S6_WR_VOLTAGE=";s+=O2_S6_WR_VOLTAGE;s+="&O2_S7_WR_VOLTAGE=";s+=O2_S7_WR_VOLTAGE;s+="&O2_S8_WR_VOLTAGE=";s+=O2_S8_WR_VOLTAGE;s+="&COMMANDED_EGR=";s+=COMMANDED_EGR;s+="&EGR_ERROR=";s+=EGR_ERROR;s+="&EVAPORATIVE_PURGE=";s+=EVAPORATIVE_PURGE;s+="&FUEL_LEVEL=";s+=FUEL_LEVEL;s+="&WARMUPS_SINCE_DTC_CLEAR=";s+=WARMUPS_SINCE_DTC_CLEAR;s+="&DISTANCE_SINCE_DTC_CLEAR=";s+=DISTANCE_SINCE_DTC_CLEAR;s+="&EVAP_VAPOR_PRESSURE=";s+=EVAP_VAPOR_PRESSURE;s+="&BAROMETRIC_PRESSURE=";s+=BAROMETRIC_PRESSURE;s+="&O2_S1_WR_CURRENT=";s+=O2_S1_WR_CURRENT;s+="&O2_S2_WR_CURRENT=";s+=O2_S2_WR_CURRENT;s+="&O2_S3_WR_CURRENT=";s+=O2_S3_WR_CURRENT;s+="&O2_S4_WR_CURRENT=";s+=O2_S4_WR_CURRENT;s+="&O2_S5_WR_CURRENT=";s+=O2_S5_WR_CURRENT;s+="&O2_S6_WR_CURRENT=";s+=O2_S6_WR_CURRENT;s+="&O2_S7_WR_CURRENT=";s+=O2_S7_WR_CURRENT;s+="&O2_S8_WR_CURRENT=";s+=O2_S8_WR_CURRENT;s+="&CATALYST_TEMP_B1S1=";s+=CATALYST_TEMP_B1S1;s+="&CATALYST_TEMP_B2S1=";s+=CATALYST_TEMP_B2S1;s+="&CATALYST_TEMP_B1S2=";s+=CATALYST_TEMP_B1S2;s+="&CATALYST_TEMP_B2S2=";s+=CATALYST_TEMP_B2S2;s+="&STATUS_DRIVE_CYCLE=";s+=STATUS_DRIVE_CYCLE;s+="&CONTROL_MODULE_VOLTAGE=";s+=CONTROL_MODULE_VOLTAGE;s+="&ABSOLUTE_LOAD=";s+=ABSOLUTE_LOAD;s+="&COMMANDED_EQUIV_RATIO=";s+=COMMANDED_EQUIV_RATIO;s+="&RELATIVE_THROTTLE_POS=";s+=RELATIVE_THROTTLE_POS;s+="&AMBIANT_AIR_TEMP=";s+=AMBIANT_AIR_TEMP;s+="&THROTTLE_POS_B=";s+=THROTTLE_POS_B;s+="&THROTTLE_POS_C=";s+=THROTTLE_POS_C;s+="&ACCELERATOR_POS_D=";s+=ACCELERATOR_POS_D;s+="&ACCELERATOR_POS_E=";s+=ACCELERATOR_POS_E;s+="&ACCELERATOR_POS_F=";s+=ACCELERATOR_POS_F;s+="&THROTTLE_ACTUATOR=";s+=THROTTLE_ACTUATOR;s+="&RUN_TIME_MIL=";s+=RUN_TIME_MIL;s+="&TIME_SINCE_DTC_CLEARED=";s+=TIME_SINCE_DTC_CLEARED;s+="&unsupported=";s+=unsupported;s+="&MAX_MAF=";s+=MAX_MAF;s+="&FUEL_TYPE=";s+=FUEL_TYPE;s+="&ETHANOL_PERCENT=";s+=ETHANOL_PERCENT;s+="&EVAP_VAPOR_PRESSURE_ABS=";s+=EVAP_VAPOR_PRESSURE_ABS;s+="&EVAP_VAPOR_PRESSURE_ALT=";s+=EVAP_VAPOR_PRESSURE_ALT;s+="&SHORT_O2_TRIM_B1=";s+=SHORT_O2_TRIM_B1;s+="&LONG_O2_TRIM_B1=";s+=LONG_O2_TRIM_B1;s+="&SHORT_O2_TRIM_B2=";s+=SHORT_O2_TRIM_B2;s+="&LONG_O2_TRIM_B2=";s+=LONG_O2_TRIM_B2;s+="&FUEL_RAIL_PRESSURE_ABS=";s+=FUEL_RAIL_PRESSURE_ABS;s+="&RELATIVE_ACCEL_POS=";s+=RELATIVE_ACCEL_POS;s+="&HYBRID_BATTERY_REMAINING=";s+=HYBRID_BATTERY_REMAINING;s+="&OIL_TEMP=";s+=OIL_TEMP;s+="&FUEL_INJECT_TIMING=";s+=FUEL_INJECT_TIMING;s+="&FUEL_RATE=";s+=FUEL_RATE;                   
                           //Serial.println(s);
      //  Serial.print(">>> Connecting to host: ");
     Serial1.println(host);
      
       if (!clientGet.connect(host, httpGetPort)) {
    //    Serial.print("Connection failed: ");
      //  Serial.print(host);
      } else {
          clientGet.println("GET " + s + " HTTP/1.1");
          clientGet.print("Host: ");
          clientGet.println(host);
          clientGet.println("User-Agent: ESP8266/1.0");
          clientGet.println("Connection: close\r\n\r\n");
          
          unsigned long timeoutP = millis();
          while (clientGet.available() == 0) {
            
            if (millis() - timeoutP > 10000) {
              Serial1.print(">>> Client Timeout: ");
              Serial1.println(host);
              clientGet.stop();
              return;
            }
          }

          //just checks the 1st line of the server response. Could be expanded if needed.
          while(clientGet.available()){
            String retLine = clientGet.readStringUntil('\r');
            Serial1.println(retLine);
            break; 
          }

      } //end client connection if else
                        
      Serial1.print(">>> Closing host: ");
     // Serial.println(host);
          
      clientGet.stop();

}
void Up()
{
       
                
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title> Advanced OBD++GPS Scanner</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> Advanced OBD++GPS Scanner </h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += date_str;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += time_str;
  s += "</td>  </tr> </table> ";
  
 if(temp==1)
 {
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>ELM Details</b></p> <table ALIGN=CENTER style=";
  s+="</td></tr> <tr> <th>FREEZE_DTC</th> <td ALIGN=CENTER >";
s+=FREEZE_DTC;s+="</td></tr> <tr> <th>FUEL_STATUS</th> <td ALIGN=CENTER >";
s+=FUEL_STATUS;s+="</td></tr> <tr> <th>ENGINE_LOAD</th> <td ALIGN=CENTER >";
s+=ENGINE_LOAD;s+="</td></tr> <tr> <th>........COOLANT_TEMP......</th> <td ALIGN=CENTER >";
s+=COOLANT_TEMP;s+="</td></tr> <tr> <th>SHORT_FUEL_TRIM_1</th> <td ALIGN=CENTER >";
s+=SHORT_FUEL_TRIM_1;s+="</td></tr> <tr> <th>LONG_FUEL_TRIM_1</th> <td ALIGN=CENTER >";
s+=LONG_FUEL_TRIM_1;s+="</td></tr> <tr> <th>SHORT_FUEL_TRIM_2</th> <td ALIGN=CENTER >";
s+=SHORT_FUEL_TRIM_2;s+="</td></tr> <tr> <th>LONG_FUEL_TRIM_2</th> <td ALIGN=CENTER >";
s+=LONG_FUEL_TRIM_2;s+="</td></tr> <tr> <th>FUEL_PRESSURE</th> <td ALIGN=CENTER >";
s+=FUEL_PRESSURE;s+="</td></tr> <tr> <th>INTAKE_PRESSURE</th> <td ALIGN=CENTER >";
s+=INTAKE_PRESSURE;s+="</td></tr> <tr> <th>...........RPM...........</th> <td ALIGN=CENTER >";
s+=RPM;s+="</td></tr> <tr> <th>........SPEED.......</th> <td ALIGN=CENTER >";
s+=SPEED;s+="</td></tr> <tr> <th>TIMING_ADVANCE</th> <td ALIGN=CENTER >";
s+=TIMING_ADVANCE;s+="</td></tr> <tr> <th>INTAKE_TEMP</th> <td ALIGN=CENTER >";
s+=INTAKE_TEMP;s+="</td></tr> <tr> <th>MAF</th> <td ALIGN=CENTER >";
s+=MAF;s+="</td></tr> <tr> <th>THROTTLE_POS</th> <td ALIGN=CENTER >";
s+=THROTTLE_POS;s+="</td></tr> <tr> <th>AIR_STATUS</th> <td ALIGN=CENTER >";
s+=AIR_STATUS;s+="</td></tr> <tr> <th>O2_SENSORS</th> <td ALIGN=CENTER >";
s+=O2_SENSORS;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B1S1;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B1S2;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B1S3;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B1S4;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B2S1;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B2S2;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B2S3;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_B2S4;s+="</td></tr> <tr> <th>OBD_COMPLIANCE</th> <td ALIGN=CENTER >";
s+=OBD_COMPLIANCE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_SENSORS_ALT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=AUX_INPUT_STATUS;s+="</td></tr> <tr> <th>RUN_TIME</th> <td ALIGN=CENTER >";
s+=RUN_TIME;s+="</td></tr> <tr> <th>DISTANCE_W_MIL</th> <td ALIGN=CENTER >";
s+=DISTANCE_W_MIL;s+="</td></tr> <tr> <th>FUEL_RAIL_PRESSURE_VAC</th> <td ALIGN=CENTER >";
s+=FUEL_RAIL_PRESSURE_VAC;s+="</td></tr> <tr> <th>FUEL_RAIL_PRESSURE_DIRECT</th> <td ALIGN=CENTER >";
s+=FUEL_RAIL_PRESSURE_DIRECT;s+="</td></tr> <tr> <th>O2_S1_WR_VOLTAGE</th> <td ALIGN=CENTER >";
s+=O2_S1_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S2_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S3_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S4_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S5_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S6_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S7_WR_VOLTAGE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S8_WR_VOLTAGE;s+="</td></tr> <tr> <th>COMMANDED_EGR</th> <td ALIGN=CENTER >";
s+=COMMANDED_EGR;s+="</td></tr> <tr> <th>EGR_ERROR</th> <td ALIGN=CENTER >";
s+=EGR_ERROR;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=EVAPORATIVE_PURGE;s+="</td></tr> <tr> <th>FUEL_LEVEL</th> <td ALIGN=CENTER >";
s+=FUEL_LEVEL;s+="</td></tr> <tr> <th>WARMUPS_SINCE_DTC_CLEAR</th> <td ALIGN=CENTER >";
s+=WARMUPS_SINCE_DTC_CLEAR;s+="</td></tr> <tr> <th>DISTANCE_SINCE_DTC_CLEAR</th> <td ALIGN=CENTER >";
s+=DISTANCE_SINCE_DTC_CLEAR;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=EVAP_VAPOR_PRESSURE;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=BAROMETRIC_PRESSURE;s+="</td></tr> <tr> <th>O2_S1_WR_CURRENT</th> <td ALIGN=CENTER >";
s+=O2_S1_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S2_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S3_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S4_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S5_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S6_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S7_WR_CURRENT;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=O2_S8_WR_CURRENT;s+="</td></tr> <tr> <th>CATALYST_TEMP_B1S1</th> <td ALIGN=CENTER >";
s+=CATALYST_TEMP_B1S1;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=CATALYST_TEMP_B2S1;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=CATALYST_TEMP_B1S2;s+="</td></tr> <tr> <th>ELM</th> <td ALIGN=CENTER >";
s+=CATALYST_TEMP_B2S2;s+="</td></tr> <tr> <th>STATUS_DRIVE_CYCLE</th> <td ALIGN=CENTER >";
s+=STATUS_DRIVE_CYCLE;s+="</td></tr> <tr> <th>CONTROL_MODULE_VOLTAGE</th> <td ALIGN=CENTER >";
s+=CONTROL_MODULE_VOLTAGE;s+="</td></tr> <tr> <th>ABSOLUTE_LOAD</th> <td ALIGN=CENTER >";
s+=ABSOLUTE_LOAD;s+="</td></tr> <tr> <th>COMMANDED_EQUIV_RATIO</th> <td ALIGN=CENTER >";
s+=COMMANDED_EQUIV_RATIO;s+="</td></tr> <tr> <th>RELATIVE_THROTTLE_POS</th> <td ALIGN=CENTER >";
s+=RELATIVE_THROTTLE_POS;s+="</td></tr> <tr> <th>AMBIANT_AIR_TEMP</th> <td ALIGN=CENTER >";
s+=AMBIANT_AIR_TEMP;s+="</td></tr> <tr> <th>THROTTLE_POS_B</th> <td ALIGN=CENTER >";
s+=THROTTLE_POS_B;s+="</td></tr> <tr> <th>THROTTLE_POS_C</th> <td ALIGN=CENTER >";
s+=THROTTLE_POS_C;s+="</td></tr> <tr> <th>ACCELERATOR_POS_D</th> <td ALIGN=CENTER >";
s+=ACCELERATOR_POS_D;s+="</td></tr> <tr> <th>ACCELERATOR_POS_E</th> <td ALIGN=CENTER >";
s+=ACCELERATOR_POS_E;s+="</td></tr> <tr> <th>ACCELERATOR_POS_F</th> <td ALIGN=CENTER >";
s+=ACCELERATOR_POS_F;s+="</td></tr> <tr> <th>THROTTLE_ACTUATOR</th> <td ALIGN=CENTER >";
s+=THROTTLE_ACTUATOR;s+="</td></tr> <tr> <th>RUN_TIME_MIL</th> <td ALIGN=CENTER >";
s+=RUN_TIME_MIL;s+="</td></tr> <tr> <th>TIME_SINCE_DTC_CLEARED</th> <td ALIGN=CENTER >";
s+=TIME_SINCE_DTC_CLEARED;s+="</td></tr> <tr> <th>EunsupportedLM</th> <td ALIGN=CENTER >";
s+=unsupported;s+="</td></tr> <tr> <th>MAX_MAF</th> <td ALIGN=CENTER >";
s+=MAX_MAF;s+="</td></tr> <tr> <th>FUEL_TYPE</th> <td ALIGN=CENTER >";
s+=FUEL_TYPE;s+="</td></tr> <tr> <th>ETHANOL_PERCENT</th> <td ALIGN=CENTER >";
s+=ETHANOL_PERCENT;s+="</td></tr> <tr> <th>EVAP_VAPOR_PRESSURE_ABS</th> <td ALIGN=CENTER >";
s+=EVAP_VAPOR_PRESSURE_ABS;s+="</td></tr> <tr> <th>EVAP_VAPOR_PRESSURE_ALT</th> <td ALIGN=CENTER >";
s+=EVAP_VAPOR_PRESSURE_ALT;s+="</td></tr> <tr> <th>SHORT_O2_TRIM_B1</th> <td ALIGN=CENTER >";
s+=SHORT_O2_TRIM_B1;s+="</td></tr> <tr> <th>LONG_O2_TRIM_B1</th> <td ALIGN=CENTER >";
s+=LONG_O2_TRIM_B1;s+="</td></tr> <tr> <th>SHORT_O2_TRIM_B2</th> <td ALIGN=CENTER >";
s+=SHORT_O2_TRIM_B2;s+="</td></tr> <tr> <th>LONG_O2_TRIM_B2</th> <td ALIGN=CENTER >";
s+=LONG_O2_TRIM_B2;s+="</td></tr> <tr> <th>FUEL_RAIL_PRESSURE_ABS</th> <td ALIGN=CENTER >";
s+=FUEL_RAIL_PRESSURE_ABS;s+="</td></tr> <tr> <th>RELATIVE_ACCEL_POS</th> <td ALIGN=CENTER >";
s+=RELATIVE_ACCEL_POS;s+="</td></tr> <tr> <th>HYBRID_BATTERY_REMAINING</th> <td ALIGN=CENTER >";
s+=HYBRID_BATTERY_REMAINING;
s+="</td></tr> <tr> <th>OIL_TEMP</th> <td ALIGN=CENTER >";
s+=OIL_TEMP;
s+="</td></tr> <tr> <th>FUEL_INJECT_TIMING</th> <td ALIGN=CENTER >";
s+=FUEL_INJECT_TIMING;
s+="</td></tr> <tr> <th>FUEL_RATE</th> <td ALIGN=CENTER >";
s+=FUEL_RATE;
   s += "</td>  </tr> </table> ";
 }
 else
 {
  s+="<p align=center><a style=""color:RED;font-size:125%;";
  s+=">Not_Conn </a> ELM </p>";
 }
 
 if (gps.location.isValid())
  {
     s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += lat_str;
    s += "+";
    s += lng_str;
    s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  }
  s += "</body> </html> \n";
  client.print(s);
      
}
