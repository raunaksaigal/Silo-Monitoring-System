#include <dht11.h>
#include <LiquidCrystal_I2C.h>

#define gaspin A1
#define trigpin 7
#define echopin 6
#define temppin 5

int duration = 0 , distance = 0 , gas = 0 , health = 0 , flag = 0 , detected = 0 , resolved = 0 , check_print = 0;
float humidity = 0 , temperature_outside = 35 , temperature_interval = 0 , temperature_inside = 0;
String status;


LiquidCrystal_I2C lcd(0x27, 16, 2);
dht11 DHT11;

void setup() 
{
  lcd.init();
  lcd.backlight();
  pinMode(trigpin , OUTPUT);
  pinMode(echopin , INPUT);
  pinMode(gaspin , INPUT);
  pinMode(temppin , INPUT);

  digitalWrite(trigpin , LOW);
  Serial.begin(9600);
}

void loop() 
{
  digitalWrite(trigpin , HIGH);
  delay(2);
  digitalWrite(trigpin , LOW);

  duration = pulseIn(echopin , HIGH);
  distance = (duration * 0.343)/2;
  

  int chk = DHT11.read(temppin);

  
  humidity = (float)DHT11.humidity;

  
  temperature_inside = (float)DHT11.temperature;

  gas = analogRead(gaspin);
  

  if(check_print == 0)
  {
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Humidity: ");
    Serial.println((float)DHT11.humidity);
    Serial.print("Temperature  (C): ");
    Serial.println((float)DHT11.temperature);
    Serial.print("Gas: ");
    Serial.println(gas);
    Serial.print("\n");
  }

  temperature_interval = temperature_outside - temperature_inside;

  if(distance < 100)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WARNING");
    lcd.setCursor(0,1);
    lcd.print("Storage Full");
    delay(2000);
    humidity = 9;
    gas = 9;
  }
  if(temperature_interval < 10)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print((int)temperature_inside);
    lcd.print((char)223);
    lcd.setCursor(0,1);
    lcd.print("Lower Temp by:");
    lcd.print(abs((int)temperature_interval));
    lcd.print((char)223);
    delay(2000);
  }
  if(humidity > 10 || gas > 10)
  {
    health = 1;
    if(health == 1 && flag == 0)
    {
      detected += health;
      flag = 1; 
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WARNING");
    lcd.setCursor(0,1);
    lcd.print("Food Spoiling");
    delay(2000);
  }
  else
  {
    if(health == 1)
    {
      resolved += 1;
    }
    health = 0;
    flag = 0;
  }

  status = Serial.readString();
  if(status != "status\n" && status != "done\n" && check_print == 1)
  {
    status = "status\n";
  }


  if(status == "status\n")
   {
    Serial.print(status);
    check_print = 1;
    if(detected == 0)
    {
      Serial.println("No Food Spoiling Detected");
      Serial.print("\n");
    }
    else if(resolved==detected)
    {
      Serial.print("Detected: ");
      Serial.println(detected);
      Serial.println("All Resolved");
      Serial.print("\n");
    }
    else
    {
      Serial.print("Detected: ");
      Serial.println(detected);
      Serial.println("Not resolved");
      Serial.print("\n");
    }
  }
  else if(status == "done\n")
  {
    check_print = 0;
  }

}