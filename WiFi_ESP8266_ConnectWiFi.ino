#include <SoftwareSerial.h>
#define _baudrate   115200
#define _rxpin      4
#define _txpin      5
SoftwareSerial Wifi( _rxpin, _txpin ); // RX, TX

//*-- IoT Information
#define SSID "xxxxx"
#define PASS "xxxxx"

void setup() {
    Serial.begin( _baudrate );
    Wifi.begin( _baudrate );
    //Sensor.begin(_baudrate );
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
    //delay(5000);
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
