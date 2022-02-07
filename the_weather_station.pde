import processing.serial.*;
Serial port;
float  wind, temp, light;
void setup(){
  size(800, 600);
  port = new Serial(this, "COM5", 9600);
  port.bufferUntil('\n');
  }
  
void draw(){
  background(127);
  fill(255,255,0);
  textSize(32);
  textAlign(CENTER);
  text("Température: " + temp, width/2-100,50);
  text("Luminosité: " + light, width/2-100,100);
  text("Vitesse de vent: " + wind, width/2-100,150);
}

void serialEvent(Serial port){
  String serialStr = port.readStringUntil('\n');
  serialStr= trim(serialStr);
  float values[] = float(split(serialStr,','));
  if (values.length == 3){
    temp = values[0];
    light = values[1];
    wind = values[2];
    }
  }
  
  
