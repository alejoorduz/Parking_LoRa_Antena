/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include <NewPing.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

#define PIN_RED    25 // 
#define PIN_GREEN  26 // 
#define PIN_BLUE   27 //

int counter = 0;

//Sensor de Ultrasonido
#define TRIGGER_PIN2  33   //Sensor de subida, cercano al arduino
#define ECHO_PIN2     32 
#define MAX_DISTANCE 500

bool flag = false;
bool banderaWifi = true;
int contador2 = 0;
String codigon = "";
float distancia2 = 0;
float dist_actual = 0;
int rest;  
int reset_time = 1000;
int sensor_carro = 0;
float sv_vard;  
float dist_set_subida;
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

void succes(){
  Serial.println("succes");
   analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  255);
  delay(1500);
   analogWrite(PIN_RED,   255);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  0);
  delay(1500);
   analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  255);
  //  delay(200);
  //   analogWrite(PIN_RED,   0);
  //    analogWrite(PIN_GREEN, 0);
  //    analogWrite(PIN_BLUE,  0);
}

void setup() {
  //initialize Serial Monitor
    pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT); 
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void send_lora_detect(){
  Serial.print("Sending packet: ");
  // myData.codigo = "Saludos";
  //Serial.println(myData.codigo);
  //Send LoRa packet to receiver
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 255);
    analogWrite(PIN_BLUE,  0);
  LoRa.beginPacket();
  LoRa.print(codigon);
  //LoRa.print('.');
  LoRa.endPacket();
  Serial.println("DATA SENT BY LORA");
}

 void SetDistance(){
  Serial.println("-------------------------PARKING-------------------------------");
  Serial.println("-----------------------------V. 1.0.0-------------------------------");
  float dist_subida = (0.034027/2)*sonar2.ping_median(10);
  Serial.println("DISTANCIA INICIAL: ");
  Serial.println(dist_subida);
  dist_set_subida = dist_subida - 10;
  Serial.println("EL BOTON ACTIVA A ESTA DISTANCIA: ");
  Serial.print("SUBIDA: ");
  Serial.println(dist_set_subida);
//  Serial.print("BAJADA: ");
//  Serial.println(dist_set_bajada);
  Serial.println("--------------------------------------------------------------");
  Serial.println("--------------------------------------------------------------");
}

void loop() {
  if(rest > reset_time){
   // ESP.restart();
    //send_lora_paquet();
    rest = 0;
  }
    rest++;
    Serial.println("StandBy");
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  255);

    distancia2 = (0.034027/2)*sonar2.ping_median(10);
    Serial.print("Distancia del vehiculo: ");
    Serial.println(distancia2);

    if (distancia2 == 0){
      Serial.println("NULL CERO");
    }
    if ( distancia2 < 100 && distancia2 > 0){    

      Serial.println("VEHICULO DETECTADO");
      codigon = "VEHICULO PARQUEADO";
      send_lora_detect();
      banderaWifi = false;
     while(banderaWifi == false){
        analogWrite(PIN_RED,   0);
        analogWrite(PIN_GREEN, 255);
        analogWrite(PIN_BLUE,  0);
        distancia2 = (0.034027/2)*sonar2.ping_median(10);
        Serial.print("Distancia 2: ");
        Serial.println(distancia2);
        delay(100);
        if(distancia2 > 100 && distancia2 > 0){
          analogWrite(PIN_RED,   255);
          analogWrite(PIN_GREEN, 0);
          analogWrite(PIN_BLUE,  0);
          Serial.println("NO HAY VEHICULO");
          codigon = "NO HAY VEHICULO";
          send_lora_detect();
          banderaWifi = true;
        }
      }
    }
//----------------------------------------------------------------------------------------------------------------------
     if ( distancia2 > 0 && distancia2 < dist_set_subida){
    if(flag == false){  
      Serial.println(": ");
       Serial.println(contador2);
          contador2++;
          Serial.println("CARRO PARQEUANDO");
          flag = true;
       }else{
      contador2++;
      delay(250);
      Serial.print("1 loop else");
      Serial.println(contador2);
      }   
    }

   
   if (  distancia2 > 0 && distancia2 < dist_set_subida && contador2 >= 3 && contador2 <= 10){
      if(flag_dos == false){
          if (flag == true){
              Serial.println("Carro parqeuando");
          analogWrite(PIN_RED,   0);
          analogWrite(PIN_GREEN, 0);
          analogWrite(PIN_BLUE,  255);
            } 
             Serial.println("CAMBIANDO A ZONA DE PARQUEO");
             Serial.println("Cuidado, frene el carro: ");
             Serial.println(contador2);
             //resetFunc();  //call reset
             delay(200);
       analogWrite(PIN_RED,   255);
          analogWrite(PIN_GREEN, 0);
          analogWrite(PIN_BLUE,  0);
                codigon = "VEHICULO PARQUEADO";
      send_lora_detect();
          
             flag = false;
             contador2++;
             flag_dos = true;
          }
          else{
            contador2++;
            delay(150);
            Serial.print("2 loop else");
             Serial.println(contador2);
          }
    }
  delay(700);
}