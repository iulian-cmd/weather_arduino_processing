// définition des broches utilisées
const int LDR = A0;
const int TMP36 = A1;
const int ANEMOMETER = 2;
int seuil = 600;

//Anénomètre - Avec interruption
unsigned long tempsFin;
const float pi = 3.1415;
const float RayonDesBras = 0.07;
int nombreTourSec = 0;
int nombreTourMin = 0;
float vitesseVent(0);
float FEtalonage(1);
volatile unsigned int comptageILS = 0; // une variable utilisée dans une interruption doit être déclarée "volatile"

void interruptILS() //comptage de l'ILS
{
  comptageILS++;
}

/*
  Web Server
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x64, 0xD0
};
IPAddress ip(192, 168, 1, 13);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
 
void setup()
{
  Serial.begin(9600);  
  pinMode(2, INPUT);//pour le pin 2
  attachInterrupt(digitalPinToInterrupt(ANEMOMETER), interruptILS, RISING);
  tempsFin = millis();


    // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop()                   
{
// Lecture de l'entrée analogique
 int reading = analogRead(TMP36);  

// Conversion de la lecture d'entrée en tension
 float volt = reading * 5.0;
 volt = volt / 1024.0; 

 // Afficher la tension d'entrée
// Serial.print("The voltage is: ");
// Serial.print(volt); Serial.println(" volts");
 
 // Afficher la température
float temperature = (volt - 0.5) * 100 ;  // Convertir la tension en temperature                          
// Serial.print("Temperature is: ");
// Serial.print( temperature); 
// Serial.println(" C");
 
// Lecture de l'entrée analogique
int reading_light = analogRead(LDR);  

 // Afficher la lumiere
// Serial.print("Luminosity is: ");
// Serial.println(reading_light); 

// Serial.println("________________________________");


// Anénomètre - Avec interruption
  if ((millis() - tempsFin) > 1000) { // durée d'1 seconde
    nombreTourSec = comptageILS; // comptage du nombre de tours par seconde
    vitesseVent = 2 * pi * RayonDesBras * nombreTourSec * FEtalonage * 3.6 / 30; // calcul de la vitesse du vent
    comptageILS = 0; // réinitialisation du comptage

    // Serial.print("Vitesse du vent = ");
    // Serial.print(vitesseVent);
    // Serial.print(" km/h\n\r");
    tempsFin = millis();
}

    Serial.print(temperature);
    Serial.print(",");
    Serial.print(reading_light);
    Serial.print(",");
    Serial.println(vitesseVent);
    delay(500);
    
// listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html style='background-color:#ced7ee;'>");
          client.println("<body style='display:flex; justify-content:center; align-items:center;'>");
          // output the value of each analog input pin
          // for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
          //   int sensorReading = analogRead(analogChannel);
          //   client.print("analog input ");
          //   client.print(analogChannel);
          //   client.print(" is ");
          //   client.print(sensorReading);
          //   client.println("<br />");
          // } 

          client.print("<div style='text-align: center; border-radius: 76px; background: linear-gradient(145deg, #dce6ff, #b9c2d6);\
          box-shadow:  41px 41px 82px #b3bbcf, -41px -41px 82px #e9f3ff; width:300px; height: auto; display:flex; justify-content:center;\
          align-items:center; flex-direction: column;'>");
          client.print("<div style='text-align: center;  padding: 2px;'>");
          client.print("<h4>Temperature is: </h4>");
          client.println("<input type='text' style='width:80px; text-align:center; margin-bottom:5px; font-weight: bold;' placeholder='");
          client.print(temperature); 
          client.println(" C");
          client.println("'>");
          client.println("</div>");

          client.print("<div style='align: center;'>");
          client.print("<h4>Luminosity is: </h4>");
          client.println("<input type='text' style='width:80px; text-align:center; margin-bottom:5px; font-weight: bold;' placeholder='");
          client.println(reading_light);
          client.println("'>");
          client.println("<br />");
          if (reading_light >= seuil){
              client.print("<p style='margin-top: 4px';>It's daylight</p>");
          } else if (reading_light>= 300 && reading_light < seuil){
              client.print("It's evening");
          } else {
              client.print("It's night");
        }
          
          client.println("</div>");

          client.print("<div style='text-align: center; padding: 2px 16px;'>");
          client.print("<h4>Wind power is: </h4>");
          client.println("<input type='text' style='width:80px; text-align:center; margin-bottom:5px; font-weight: bold;' placeholder='");
          client.print(vitesseVent);
          client.print(" km/h\n\r");
          client.println("'>");
          client.println("</div>");
          client.println("</div>");
          client.println("</div>");

          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}