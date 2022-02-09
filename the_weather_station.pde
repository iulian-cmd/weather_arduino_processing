import processing.serial.*;

Serial port; // création de l'objet de la class Serial
float wind, temp, light; // la data reçu sur le serial port

int rectX, rectY;
int circleX, circleY;
int rectSize = 70;
int circleSize = 73;

color rectColor, circleColor, baseColor;
color rectHighlight, circleHighlight;
color currentColor;

boolean rectOver = false;
boolean ledArduino = false;

PImage soleil;
PImage souffle;
PImage nuageux;
PImage flocon;
PImage neige;
PImage nuit;
PImage soufflefort;
PImage ultrahot;
PImage legerchaud;
PImage tempnormal;
PImage peufroid;
PImage fondecran;

void setup(){
  size(800,600); // taille de fenêtre
  port = new Serial (this, "COM5", 9600); // le COM est celui de Arduino, le baud est celui qui a été mise en place
  port.bufferUntil('\n');
  // bufferUntil : définit un octet spécifique dans la mémoire tampon jusqu'à avant d'appeler  serialEvent() .
  
  rectColor = color(0);
  rectHighlight = color(51);
  circleColor = color(255);
  circleHighlight = color(204);
  baseColor = color(102);
  currentColor = baseColor;
  
  circleX = width/2+circleSize/2+10;
  circleY = height/2;
  rectX = width/2-rectSize-10;
  rectY = height/2-rectSize/2;
  
  soleil = loadImage("soleil.png");
  nuageux = loadImage("nuageux.png");
  nuit = loadImage("nuit.png");
  
  souffle = loadImage("vent.png");
  soufflefort = loadImage("ventfort.png");
  
  ultrahot = loadImage("ultrachaud.png");
  legerchaud = loadImage("legerchaud.png");
  flocon = loadImage("flocon.png");
  neige = loadImage("neige.png");
  tempnormal = loadImage("tempnormal.png");
  peufroid = loadImage("peufroid.png");
  
  fondecran = loadImage("background_image.png");
}

void draw(){
  background(fondecran);
  


  textSize(32);
  textAlign(CENTER);
  

  
  text("Température : " + temp, width / 2-100, 50);
  text("Lumière : " + light, width / 2-100, 150);
  text("Vitesse du vent : " + wind, width / 2-100, 250);
  
  //image selon la température
  if (temp > 25) {
    image(ultrahot,550,20);        
  } else if(temp >= 20 && temp < 25){
    image(legerchaud,550,20);                   
  } else if(temp < 19 && temp >= 15){
    image(tempnormal,550,20);                    
  } else if(temp < 15 && temp >= 10 ){
    image(peufroid,550,20);                   
  } else if(temp < 10 && temp >= 0 ){
    image(neige,550,20);                    
  } else if(temp < 0){
    image(flocon,550,20);                   
  }

  //image selon la luminosité
  if(light >= 500){
    image(soleil,550,90);
  }else if(light >= 300 && light < 500){
    image(nuageux,550,90);
  } else if(light < 300){ 
  image (nuit,550, 90);
  }

   //image selon la vitesse du vent
  if(wind > 0 && wind < 20){
    image(souffle,500,250);
  }else if (wind > 50){
    image(soufflefort,500,250);
  }
  
  update(mouseX,mouseY);

  text("Mettre en maintenance : ", width / 2-20, 400);
  if(rectOver){
    fill(rectHighlight);
  } else {
    fill(rectColor);
  }
  stroke(255);
  rect(rectX,450,rectSize,rectSize);
  

  fill(circleColor);
  stroke(0);
  ellipse(circleX,485, circleSize, circleSize);
}

void update(int x, int y){
  if(overRect(rectX,450,rectSize,rectSize)){
     rectOver = true;
  } else {
     rectOver = false;
  }
}

void mousePressed(){
  if(rectOver){
    
    if(ledArduino == false){
      port.write("111X");
      ledArduino = true;
      circleColor = color(0,0,255);
    } else {
      port.write("000X");
      ledArduino = false;
      circleColor = color(255);
    }
    
  }
}

boolean overRect(int x, int y, int width, int height){
  if(mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+height) {
     return true; 
  } else {
     return false;  
  }
}

//SerialEvent = données série disponibles dans la mémoire tampon
void serialEvent(Serial port){
String serialStr=port.readStringUntil('\n');
serialStr = trim(serialStr);
float values[]= float(split(serialStr, ','));
  if(values.length == 3){
  temp = values[0];
  light = values[1];
  wind = values[2];  
}
}
