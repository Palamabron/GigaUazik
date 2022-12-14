const unsigned int MAX_MESSAGE_LENGTH = 12;
int LMSP = 5;
int LMF = A0;
int LMB = A1;


int RMSP = 6;
int RMF = A3;
int RMB = A2;


void setup() 
{
  Serial.begin(9600);

  pinMode(LMSP,OUTPUT);
  pinMode(LMF,OUTPUT);
  pinMode(LMB,OUTPUT);
  
  pinMode(RMSP,OUTPUT);
  pinMode(RMF,OUTPUT);
  pinMode(RMB,OUTPUT);
}

/*
Pulse width   Rotation angle (servo)
0.5ms         0 degree
1.0ms         45 degree
1.5ms         90 degree
2.0ms         135 degree
2.5ms         180 degree
*/

void loop() {
//Check to see if anything is available in the serial receive buffer
while (Serial.available() > 0)
{
  //Create a place to hold the incoming message
  static char message[MAX_MESSAGE_LENGTH];
  static unsigned int message_pos = 0;

  //Read the next available byte in the serial receive buffer
  char inByte = Serial.read();

  //Message coming in (check not terminating character) and guard for over message size
  if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
  {
     //Add the incoming byte to our message
    message[message_pos] = inByte;
    message_pos++;
  }
   //Full message received...
  else
  {
    //Add null character to string
    message[message_pos] = '\0';


    //Print the message (or do other things)
    Serial.println(message);
    //move forward with given speed
    if (message[1] == 'm')
    {
      int distance =  getValue(message, ' ', 1).toInt();
      forward(distance);
    }
    //rotate by a given angle (arg > 0 means right, else left)
    else if (message[1] == 'r')
    {
      int angle = getValue(message, ' ', 1).toInt();
      rotate(angle);
    }
    //set robot speed (if arg<0 means going back)
    else if (message[1] == 'v')
    {
      int velocity = getValue(message, ' ', 1).toInt();
      setVelocity(velocity);
    }
    //stop immediately
    else if (message[1] == 's')
    {
      brake();
    }
    //sonar value in cm
    else if (message[1] == 'b')
    {
      Serial.println("sensor");
    }
    //infrared sensor in cm
    else if (message[1] == 'i')
    {
      Serial.println("sensor");
    }
    //Reset for the next message
    message_pos = 0;
  }
}
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

   for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
// t in ms
// alpha(t) = 90t - 45
// S(t) = alpha(t) * 2 * pi * R
// (S(t) / (2 * pi * R) + 45)/90 = t
void forward(int distance)
{
  const int radius = 7; //in cms
  const float pi = 3.14;

  digitalWrite(LMB,LOW);
  digitalWrite(LMF,HIGH);
  
  digitalWrite(RMB,LOW);
  digitalWrite(RMF,HIGH);
  
  analogWrite(LMSP, distance);
  analogWrite(RMSP, distance);
  float time = (distance / (2 * pi * radius) + 45) / 90; 
  delay(time); // in ms
}


void rotate(int angle)
{
  //right
  if (angle > 0)
  {
    digitalWrite(LMB,LOW);
    digitalWrite(LMF,HIGH);
  
    digitalWrite(RMB,HIGH);
    digitalWrite(RMF,LOW);
  }
  else //wrong
  {
    digitalWrite(LMB,HIGH);
    digitalWrite(LMF,LOW);
  
    digitalWrite(RMB,LOW);
    digitalWrite(RMF,HIGH);
  }

  analogWrite(LMSP, 100);
  analogWrite(RMSP, 100);
  delay(angle * 2);
  brake();
}


void sensor()
{
}


void setVelocity(int speed)
{
  if (speed > 0)
  {
    digitalWrite(LMB,LOW);
    digitalWrite(LMF,HIGH);
  
    digitalWrite(RMB,LOW);
    digitalWrite(RMF,HIGH);
  }
  else
  {
    digitalWrite(LMB,HIGH);
    digitalWrite(LMF,LOW);
  
    digitalWrite(RMB,HIGH);
    digitalWrite(RMF,LOW);
  }
  analogWrite(LMSP, speed);
  analogWrite(RMSP, speed);
}


void brake()
{
  analogWrite(LMSP, 0);
  analogWrite(RMSP, 0);
}


void sonar()
{

}