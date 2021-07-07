// Obstacle avoidance algorithm
#include<ESP8266WiFi.h >
const char* ssid="Anusha's One plus!";
const char* password="Thengo!!!!";
#define led D0
#define ML D1
#define MR D3

#define ENA D7
#define ENB D8


#define trigpin D5
#define echopin D6
#define device_pin D4

WiFiServer server(80);
int flag;


void setup() {
  // put your setup code here, to run once:
pinMode(led,OUTPUT);
pinMode(ML,OUTPUT);
pinMode(MR,OUTPUT);


pinMode(ENA,OUTPUT);
pinMode(ENB,OUTPUT);

analogWrite(ENA,1023);
analogWrite(ENB,1023);

pinMode(trigpin,OUTPUT);
pinMode(echopin,INPUT);
Serial.begin(115200);

digitalWrite(led,LOW);
digitalWrite(ML,OUTPUT);
digitalWrite(MR,OUTPUT);
pinMode(device_pin,OUTPUT);


Serial.println();
Serial.println();
Serial.print("connecting to");
Serial.println(ssid);

WiFi.begin(ssid,password);

/*IPAddress ip(192,168,1,254);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WiFi.config(ip,gateway,subnet);
delay(5000);*/
while(WiFi.status() !=WL_CONNECTED){
  delay(100);
  Serial.print(".");
}
Serial.println();
Serial.println("WiFi Connected");

server.begin();
Serial.println("Server started");

Serial.print("Use this URL to connect");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
}


int getdistance_cm() {
  long duration;
  int distance;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(15);
   digitalWrite(trigpin, LOW);
   duration = pulseIn(echopin, HIGH);
  // d' = v*t  where  sound velocity v = 343 mtr/sec  , t = duration 
  // d' = (343*100 * (10^-6) cm)* duration  =  0.0343 * duration cm
  //  distance = d' /2 ;
   
  distance = (0.0343 * duration)/2; 

  
  delay(100);
  return distance;
}


void forward() {
  digitalWrite(ML,LOW);
  digitalWrite(MR,LOW);
 
}


void left_turn() {
 digitalWrite(ML,HIGH);
 digitalWrite(MR,LOW);
  
}


void right_turn() {
  digitalWrite(ML,LOW);
 digitalWrite(MR,HIGH);
  
}



void stop_now() {
  digitalWrite(ML,HIGH);
  digitalWrite(MR,HIGH);
   
}




void loop() {
  // put your main code here, to run repeatedly:
int distance_cm = getdistance_cm();

WiFiClient client = server.available();

if(!client){
  
  return;
}


Serial.println("new client");
while(!client.available()){
  delay(1);
}

Serial.println("HTTP/1.1 200 OK");
Serial.println("Content-Type: text/html");
Serial.println("");
Serial.println("<!DOCTYPE HTML>");
Serial.println("<html>");
Serial.println(distance_cm);
Serial.println("</html>");
delay(1);


String request=client.readStringUntil('\r');
Serial.println(request);
client.flush();

if(request.indexOf("/Stop_F") != -1) {
  stop_now();
  Serial.println("Robot Stop");
}
if(request.indexOf("/Forward") != -1) {
  Serial.print("Distance= ");
  Serial.println(distance_cm);
  forward();
}

if(request.indexOf("/Stop_F") != -1) {
  stop_now();
  Serial.println("Robot Stop");
}


if(request.indexOf("/Left") != -1) {
  left_turn();
  Serial.println("Robot turning left");
}

if(request.indexOf("/Stop_L") != -1) {
  stop_now();
  Serial.println("Robot Stop");
}

if(request.indexOf("/Right") != -1) {
  right_turn();
  Serial.println("Robot turning right");
}

if(request.indexOf("/Stop_R") != -1) {
  stop_now();
  Serial.println("Robot Stop");
}

if(request.indexOf("/device_on") != -1) {
  digitalWrite(device_pin,LOW );
  Serial.println("Device ON");
  
}

if(request.indexOf("/device_off") != -1) {
  
  digitalWrite(device_pin,HIGH);
  
  stop_now();
  Serial.println("Device OFF");
  
}


/*if(distance_cm > 30) {
  //forward();
  forward();
    Serial.print(" Moving Forward and   ");Serial.print("distance = ");Serial.println(distance_cm);

}
else {
  Serial.print(" Obstacle Detected and   ");Serial.print("distance = ");Serial.println(distance_cm);
  
  stop_now();
  delay(1000);
  left_turn();
  delay(2000);
  stop_now();
  
}*/
}
