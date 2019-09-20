#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN A0
#define DHTTYPE DHT22
#define _baudrate   115200
#define _rxpin      4
#define _txpin      5
SoftwareSerial Wifi( _rxpin, _txpin ); // RX, TX

//*-- IoT Information
#define SSID "xxxxx"
#define PASS "xxxxx"
#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149
// 使用 GET 傳送資料的格式
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&filed2=[data 2]...;
String GET = "GET /update?key=xxxxx";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin( _baudrate );
    Wifi.begin( _baudrate );
    dht.begin();
    sendDebug("AT");
    delay(1000);
    if(Wifi.find("OK"))
    {
        Serial.println("RECEIVED: OK\nData ready to sent!");
        connectWiFi();
    }
    else{
    Serial.println("NO RESEPONCE!");
  }
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  SentOnCloud( String(t), String(h));
  delay(5000);
 }
    
void SentOnCloud( String T, String H )
{
    // 設定 ESP8266 作為 Client 端
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += IP;
    cmd += "\",80";
    sendDebug(cmd);
    if( Wifi.find( "Error" ) )
    {
        Serial.print( "RECEIVED: Error\nExit1" );
        return;
    }
    cmd = GET + "&field1=" + T + "&field2=" + H +"\r\n";
    Wifi.print( "AT+CIPSEND=" );
    Wifi.println( cmd.length() );
    if(Wifi.find( ">" ) )
    {
        Serial.print(">");
        Serial.print(cmd);
        Wifi.print(cmd);
    }
    else
    {
        Wifi.print( "AT+CIPCLOSE" );
    }
    if( Wifi.find("OK") )
    {
        Serial.println( "RECEIVED: OK" );
    }
    else
    {
        Serial.println( "RECEIVED: Error\nExit2" );
    }
}
void sendDebug(String cmd)
{
    Serial.print("SEND: ");
    Serial.println(cmd);
    Wifi.println(cmd);
} 
 
boolean connectWiFi()
{
    Wifi.println("AT+CWMODE=1");
    //delay(2000);
    String cmd="AT+CWJAP=\"";
    cmd+=SSID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";
    sendDebug(cmd);
    if(Wifi.find("OK"))
    {
        Serial.println("RECEIVED: OK");
        return true;
    }
    else
    {
        Serial.println("RECEIVED: Error");
        return false;
    }
    cmd = "AT+CIPMUX=0";
    sendDebug( cmd );
    if( Wifi.find( "Error") )
    {
        Serial.print( "RECEIVED: Error" );
        return false;
    }
}
