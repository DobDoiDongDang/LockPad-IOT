#include <WiFiS3.h>  // Include the WiFi library for Arduino Uno R4 WiFi
#include <WiFiServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Servo.h>

// Replace with your WiFi network credentials
const char ssid[] = "Tenda";
const char password[] = "023153000";
String formValue = "00000";
const char *login_log[100] = {};
int log_count = 0;  // used slots
String nowtime;

int buzzer = 10;
Servo myservo;
// Set up a server on port 80 (HTTP)
WiFiServer server(80);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte numRows = 4; // Number of rows on the keypad
const byte numCols = 4; // Number of columns on the keypad

String pass = "00000";
String inpass = "";
bool pass_type = true;

String buffer = "";
const char* lcdtext;
// Keymap defines the key pressed according to the row and columns
char keymap[numRows][numCols] = 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Keypad connections to the Arduino pins
byte rowPins[numRows] = {9, 8, 7, 6}; // Rows 0 to 3
byte colPins[numCols] = {5, 4, 3, 2}; // Columns 0 to 3

// Initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void log_add(const char *s) {
  if (log_count < 100) {
    login_log[log_count++] = s;
  }
}

void refreshLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  if(pass_type == 0){
  lcd.print(inpass);
  }else{
  for(int i = 1;i<=inpass.length();i++){
    lcd.print("*");
    }
  }
}

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP, "th.pool.ntp.org", 25200);

void success_login(){
  tone(buzzer, 1000);
  myservo.write(90);
  delay(1500);
  myservo.write(0);
  noTone(buzzer);
}

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  myservo.attach(11);
  myservo.write(0);
  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  // Print IP Address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");

  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("A=delete c=reset");
  lcd.setCursor(0, 1);
  lcd.print("B=view type D=OK");

  timeClient.begin();
}

void loop() {

  //-------------------server-----------------------
  
  //update clock
  timeClient.update();
  // Check for incoming client connection
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client connected");
    
    // Read the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Parse the request
    if (request.indexOf("GET /?input=") >= 0) {
      int inputIndex = request.indexOf("input=") + 6;
      int spaceIndex = request.indexOf(' ', inputIndex);
      formValue = request.substring(inputIndex, spaceIndex);
      formValue.replace("+", " "); // Replace '+' with spaces for readability
    }

    // Send the response back to the client (a simple form)
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<body background='https://www.it.kmitl.ac.th/~pattarachai/PIC/BG/stone.gif'>");
    client.println("<div style=\"margin: auto; width: 50%;\">");
    client.println("<h1>LOCKPAT-IOT</h1>");
    
    // If there's a value from the form, display it
    if (formValue != "") {
      client.println("<p>Your password: " + formValue + "</p>");
      Serial.println(formValue);
      pass = formValue;
    }
    
    // Display a form for the user to submit data
    client.println("<form action='/' method='GET'>");
    client.println("Enter new password: <input type='text' name='input'><br>");
    client.println("<input type='submit' value='Submit'>");
    client.println("</form>");

    client.println("<ul>");

    //If there's a value in log
    if (sizeof(login_log) / sizeof(login_log[0]) > 1){
      for (int i = 1;i < log_count;){
          client.print("<li>");
          Serial.println(String(i));
          client.print(login_log[i++]);
          client.print(" : ");
          Serial.println(" : ");
          Serial.println(login_log[i]);
          client.print(login_log[i++]);
          client.println("</li>");
      }
    }

    client.println("</ul>");
    client.println("</div>");
    client.println("</body>");
    client.println("</html>");
    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }

  //-------------------Hardware-----------------------

  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY) {
    Serial.println(keypressed);

    // Control LEDs based on key presses
    switch (keypressed) {
      case '1':
        inpass = inpass + "1";
        refreshLCD();
        break;
      case '2':
        inpass = inpass + "2";
        refreshLCD();
        break;
      case '3':
        inpass = inpass + "3";
        refreshLCD();
        break;
      case '4':
        inpass = inpass + "4";
        refreshLCD();
        break;
      case '5':
        inpass = inpass + "5";
        refreshLCD();
        break;
      case '6':
        inpass = inpass + "6";
        refreshLCD();
        break;
      case '7':
        inpass = inpass + "7";
        refreshLCD();
        break;
      case '8':
        inpass = inpass + "8";
        refreshLCD();
        break;
      case '9':
        inpass = inpass + "9";
        refreshLCD();
        break;
      case '0':
        inpass = inpass + "0";
        refreshLCD();
        break;
      case '*':
        inpass = inpass + "*";
        refreshLCD();
        break;
      case '#':
        inpass = inpass + "#";
        refreshLCD();
        break;
      case 'A':
        if (inpass.length() >= 1) {
          inpass.remove(inpass.length() - 1);
        }
        refreshLCD();
        break;
      case 'B':
        pass_type = !pass_type;
        refreshLCD();
        break;
      case 'C':
        inpass = "";
        refreshLCD();
        break;
      case 'D':
        nowtime = timeClient.getFormattedTime();
        if (inpass.equals("")) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Welcome!!!");
          lcd.setCursor(0, 1);
          lcd.print("now ");
          lcd.print(nowtime);
          delay(1500);
          refreshLCD();
        } else {
          lcd.clear();
          //log_add(nowtime.c_str());
          if (pass.compareTo(inpass) == 0) {
            log_add("Success");
            lcd.print("Success");
            success_login();
          } else {
            log_add("fail!");
            lcd.print("fail!");
            delay(1500);
          }
          for (int i = 0;i<log_count;i++){Serial.println(login_log[i]);}
          inpass = "";
          refreshLCD();
        }
        break;
      default:
        break;
    }
  }
}
