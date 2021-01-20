

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
                          
#define FIREBASE_HOST "habd-d39f6.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "gB8oKjcpXpE1JT9rAGsqA0c63ucqVGkyn2eLd8Ur" //Your Firebase Database Secret goes here

#define WIFI_SSID "HUAWEI Y7 Prime 2018"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "menna123"                                      //Password of your wifi network 
 
  

// Declare the Firebase Data object in the global scope
int FSR = A0;
float Read;
#define RT0 10000   // Ω
#define B 3977      // K
//--------------------------------------


#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ
float RT, VR, ln, TX, T0, VRT;
//float A = 0.001129148; 
//float B = 0.000234125;
////float C = 8.76741*10^-8;
//const float VCC = 3.3;             // NodeMCU on board 3.3v vcc
//const float R2 = 10000;            // 10k ohm series resistor
//const float adc_resolution = 1023; // 10-bit adc 
FirebaseData firebaseData;

// Declare global variable to store value
int val=0;



void setup() {

  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor

  Serial.println("Serial communication started\n\n");  

  T0 = 25 + 273.15;         
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

    
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}

void loop() { 

// Firebase Error Handling And Writing Data At Specifed Path************************************************
VRT = analogRead(FSR);
VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15;
if (Firebase.setFloat(firebaseData, "/SmartConnector/cd", TX)) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("Val = ");
               Serial.println(TX);
               Serial.println("\n");
               
               
               delay(1000);

     }

else {        
  
    Serial.println(firebaseData.errorReason());
  }

}


/* NOTE:
 *  To upload value, command is ===> Firebase.setInt(firebaseData, "path", variable);
 *  Example                     ===>  Firebase.setInt(firebaseData, "/data", val);
 */ 
