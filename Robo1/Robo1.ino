/*
   created 3 Nov 2008
   by David A. Mellis
   modified 30 Aug 2011
   by Tom Igoe
   
  seuraa vasenta seinää
  jos seinä eteen ja vasemmalla seinä käänny oikealle
  jos edessä seinä ja vasemmalla ei käänny vasemmalle
   

 */

// this constant won't change.  It's the pin number
// of the sensor's output:
const int pingPin = 6;
const int sidepingPin = 7;
int Pin;
//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 10; //standbyint STBY = 10; //standby

//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction

// Accelerator sensor pins
int xPin = 0;
int yPin = 1;
int zPin = 2;
int nave = 10;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop()
{
  /*long cm = 0;
  long sidecm = 0;
  cm = dist(pingPin);
  sidecm = dist(sidepingPin);
  */
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration2, cm2;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(sidepingPin, OUTPUT);
  digitalWrite(sidepingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sidepingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sidepingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(sidepingPin, INPUT);
  duration2 = pulseIn(sidepingPin, HIGH);

  // convert the time into a distance
  cm2 = microsecondsToCentimeters(duration2);
  
  //Accelerator code
  int xval = 0;
  int yval = 0;
  int zval = 0;
  
  xval = analogRead(xPin);
  yval = analogRead(yPin);
  zval = analogRead(zPin);
  
  /*for (int i = 0; i < nave; i++) {
    // Lue eri akselien arvot (0-1023). Lisää summaan.
    xval += analogRead(xPin); // xval = xval + analogRead(xPin) 
    yval += analogRead(yPin);
    zval += analogRead(zPin);
    delay(5);
  }// Jaa summa termien määrällä,
  xval = xval/nave;
  yval = yval/nave;
  zval = zval/nave;
  */
  /*
  Serial.print("Arvot: ");
  Serial.print(xval);
  Serial.print(",");
  Serial.print(yval);
  Serial.print(",");
  Serial.println(zval);
  */
  // Raakadata kulmiksi lineaarimuunnoksen avulla
  // Esim: xmin > -90, xmax > 90, (xmax+xmin)/2 > 0
  // Kulmia -90 ja 90 astetta vastaavat arvot
  int xmin = 0; // Muuta kalibrointivaiheessa
  int xmax = 1; // Muuta kalibrointivaiheessa

  float xang = map(xval, xmin, xmax, -90, 90);
  float yang = map(yval, xmin, xmax, -90, 90);
  float zang = map(zval, xmin, xmax, -90, 90);
  if(cm < 5)
    dodge();
    
  //else if(cm2 > 20);
      //forward();
  
  else if(cm2 > 7)
      turnLeft();
      
  else if(cm2 < 5)
      turnRight();
  
  else
    forward();
  
  Serial.println(cm2);
  //Serial.print("cm");
  
  //  else if(cm2 > 6)stop();
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
//Own functions:
void dodge()
{
Serial.println("dodge");
move(1, 255, 1);
move(0, 255, 1);
delay(500);
}
void forward()
{
  Serial.println("forward");
  move(1, 255, 1);
  move(0, 255, 0);
  delay(100);
}
void turnRight()
{
  Serial.println("Right");
  move(1, 255, 1);
  move(0, 100, 0);
  delay(100);
}
void turnLeft()
{
Serial.println("Left");
move(1, 100, 1);
move(0, 255, 0);
delay(100);
}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}
void stop()
{
  digitalWrite(STBY, LOW);
}
