#include <SPI.h>
#include <Ethernet.h>

void(* resetFunc) (void) = 0;//reset function is built-in

const int boton = 9;
const int boton2 = 8;
const int boton3 = 7;

int valor = 0;
int valor2 = 0;
int valor3 = 0;



// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xDB, 0xEF, 0xFE, 0xFD }; //if it doesnt have a sticker just make it up as long no device on your network has this same mac you should be fine


IPAddress server(192,168,1,141);  // Numeric address
//char server[] = "http://mxmtsvrandon1"; //you can use DNS too.    


// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 8, 54);
IPAddress myDns(192, 168, 0, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;


// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement


void setup() {
  // put your setup code here, to run once:
    pinMode(boton, OUTPUT);
    pinMode(boton2, OUTPUT);
    pinMode(boton3, OUTPUT);
    pinMode(LED_BUILTIN, INPUT);

    // You can use Ethernet.init(pin) to configure the CS pin
    Ethernet.init(10);  // Most Arduino shields
   
    
    Serial.begin(9600);
    while (!Serial) 
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // start the Ethernet connection:
    Serial.println("Initialize Ethernet with DHCP:");

    
    if (Ethernet.begin(mac) == 0) 
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) 
        {
            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            while (true) {
                delay(1); // do nothing, no point running without Ethernet hardware
            }
        }
        if (Ethernet.linkStatus() == LinkOFF) 
        {
            Serial.println("Ethernet cable is not connected.");
        }
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, ip, myDns);
    } 
    else 
    {
        Serial.print("  DHCP assigned IP ");
        Serial.println(Ethernet.localIP());
    }
    
    // give the Ethernet shield a second to initialize:
    delay(1000);
    Serial.print("connecting to ");
    Serial.print(server);
    Serial.println("...");
}


void loop() {
 valor = digitalRead(boton);
 valor2 = digitalRead(boton2);
 valor3 = digitalRead(boton3);

 
    if(valor == HIGH || valor2 == HIGH || valor3 == HIGH) 
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
  
        // if you get a connection, report back via serial:
        if (client.connect(server, 80)== 1) 
        {
            Serial.print("connected to ");
            Serial.println(client.remoteIP());
            // Make a HTTP request:
            
            if(valor == HIGH)
            {
              Serial.print("Pin 9");
              client.println("GET /production-tracker-medcomp/Api/v1/insert.php?site=1037&value=1 HTTP/1.1");
            }

            if(valor2 == HIGH)
            {
              Serial.print("Pin 8");
              client.println("GET /production-tracker-medcomp/Api/v1/insert_b.php?site=1038&value=1 HTTP/1.1");
            }

            if(valor3 == HIGH)
            {
              Serial.print("Pin 7");
              client.println("GET /production-tracker-medcomp/Api/v1/insert_b.php?site=1038&value=1 HTTP/1.1");
            }
            
            client.println("Host: 192.168.1.141");
            client.println("Connection: close");
            client.println();

       


        } 
        else 
        {
            // if you didn't get a connection to the server:
            Serial.println("connection failed");
            delay(1000);
            Serial.println("Trying to reconnect...");
            delay(1000);

            resetFunc();
        }
        beginMicros = micros();
        // if there are incoming bytes available
        // from the server, read them and print them:
        int len = client.available();
        if (len > 0) 
        {
            byte buffer[80];
            if (len > 80) len = 80;
            client.read(buffer, len);
            if (printWebData) {
                Serial.write(buffer, len); // show in the serial monitor (slows some boards)
            }
            byteCount = byteCount + len;
        }

        // if the server's disconnected, stop the client:
        if (!client.connected()) 
        {
            endMicros = micros();
            Serial.println();
            Serial.println("disconnecting.");
            client.stop();
            Serial.print("Received ");
            Serial.print(byteCount);
            Serial.print(" bytes in ");
            float seconds = (float)(endMicros - beginMicros) / 1000000.0;
            Serial.print(seconds, 4);
            float rate = (float)byteCount / seconds / 1000.0;
            Serial.print(", rate = ");
            Serial.print(rate);
            Serial.print(" kbytes/second");
            Serial.println();

            setup();

            // do nothing forevermore:
            while (true) 
            {
                delay(1000);
            }
        }




    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
