#include <Ticker.h>
#include <DHT.h>
#include <ArduinoJson.h>

const byte ledT =  12;
const byte ledH = 11;

void enviarDatos();

DHT dht(2,DHT11);
Ticker rutina1(enviarDatos, 3000);

float temp1;
float hum;

int Tpunto_max = 30;
int Tpunto_min = 28;

int Hpunto_max = 90;
int Hpunto_min = 70;

String estado_ledH = "OFF";
String estado_ledT = "OFF";

void setup() {
  Serial.begin(9600);
  pinMode(ledT, OUTPUT);
  pinMode(ledH, OUTPUT);
  dht.begin();
  rutina1.start();
}

void loop() {

  rutina1.update();

  if(Serial.available())
  {
    String cad = Serial.readString();
    if(cad == "ledT:1" && temp1 > Tpunto_min && temp1 < Tpunto_max)        led_ON(ledT);
    else if (cad == "ledT:0" && temp1 > Tpunto_min && temp1 < Tpunto_max)  led_OFF(ledT);
    else if (cad == "ledH:1" && hum > Hpunto_min && hum < Hpunto_max)      led_ON(ledH);
    else if (cad == "ledH:0" && hum > Hpunto_min && hum < Hpunto_max)      led_OFF(ledH);
  }

}


void enviarDatos()
{
  temp1 = dht.readTemperature(); 
  hum = dht.readHumidity();

  if (temp1 >= Tpunto_max)
  {
    led_ON(ledT);
  }
  else if (temp1 <= Tpunto_min)
  {
    led_OFF(ledT);
  }

  if (hum >= Hpunto_max)
  {
    led_ON(ledH);
  }
  else if (hum <= Hpunto_min)
  {
    led_OFF(ledH);
  }
  
   String mensajeJson;
   DynamicJsonDocument doc(1024);
  
   doc["TempActual"] = String(temp1);
   doc["HumActual"] = String(hum);
   doc["TsetpointMin"] = String(Tpunto_min);
   doc["TsetpointMax"] = String(Tpunto_max);
   doc["HsetpointMin"] = String(Hpunto_min);
   doc["HsetpointMax"] = String(Hpunto_max);
   doc["LedH"] = estado_ledH;
   doc["LedT"] = estado_ledT;
   serializeJson(doc, mensajeJson);
   
   Serial.println(mensajeJson);
}


void led_ON(const byte led)
{
    digitalWrite(led,HIGH);
    if(led == ledT) estado_ledT = "ON";
    else if(led == ledH) estado_ledH = "ON";
}

void led_OFF(const byte led)
{
    digitalWrite(led,LOW);
    if(led == ledT) estado_ledT = "OFF";
    else if(led == ledH) estado_ledH = "OFF";
}
  
  

  
