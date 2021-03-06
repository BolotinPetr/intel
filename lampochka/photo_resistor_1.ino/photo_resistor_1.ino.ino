#include <CurieBLE.h>
const int relayPin = 10;      // the number of the LED pin
const int PhotoPin = A0; 
const int buttonPin = 12;   
const int BUTTON =0;
const int AUTO =1;
int i=1;
int buttonState = 0;    
int a=0;
int photo = 0;
int state=0;
int statepred=2;
int light2 = 1;
int online_prov = 0;
int online_prov_pred = 0;

BLEPeripheral blePeripheral; // create peripheral instance
BLEService lampService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

BLECharCharacteristic stateCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLEWrite | BLERead | BLENotify);
BLECharCharacteristic onlineCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLEWrite |BLERead | BLENotify); 

void setup() {
   Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
   pinMode(buttonPin, INPUT);
   
 // set the local name peripheral advertises
  blePeripheral.setLocalName("lamp");
  // set the UUID for the service this peripheral advertises:
  blePeripheral.setAdvertisedServiceUuid(lampService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(lampService);
  blePeripheral.addAttribute(stateCharacteristic);
  blePeripheral.addAttribute(onlineCharacteristic);

  //stateCharacteristic.setValue(0);
 // onlineCharacteristic.setValue(0);

  // advertise the service
  blePeripheral.begin();

Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll peripheral
  blePeripheral.poll();
  state=stateCharacteristic.value();
  if((state == 1) && (statepred !=state ))
              {
                 
                 i=BUTTON;
              }
   if((state == 0) && (statepred !=state))
              {
                 
                 i=AUTO;
              }
  Serial.println(online_prov);
  switch (i) {
    
  
    case BUTTON:   
          buttonState = digitalRead(buttonPin);

            online_prov = onlineCharacteristic.value();

            if(online_prov_pred != online_prov)
            {
                   a = (a + 1) % 2;  
              
            }
                 
            if((light2 == 1) && (buttonState == 0))
              {
                   a = (a + 1) % 2;    
                  
              }
       
          light2 = buttonState;
          online_prov_pred = online_prov;
          delay(200);
      break;
      
    case AUTO:   
     
      buttonState = digitalRead(buttonPin);
      photo = analogRead(PhotoPin);
       if (a==0)
             {
                 if (photo>750)
                        {a=1;}
              }
        else
              {
                  if (photo<600)
              
                   {a=0;}
                }

             
            if((light2 == 1) && (buttonState == 0))
              {
                 light2 = (light2 + 1) % 2;
                 i=BUTTON;
                  break;
              }
            light2 = buttonState;
            delay(200);
      break;
      
  
  }
   statepred=state;
  digitalWrite(relayPin, a);
 
  
}
