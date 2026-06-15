#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_WIFI";
const char* password = "123456789";

WebServer server(80);

#define TRG_pin 5
#define ECHO_pin 18

String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Ultrasonic Sensor</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
        body{
            text-align:center;
            font-family:Arial;
            margin-top:50px;
            background-color:#f2f2f2;
        }

        .card{
            width:300px;
            margin:auto;
            padding:20px;
            background:white;
            border-radius:10px;
            box-shadow:0px 0px 10px gray;
        }

        h2{
            color:#333;
        }

        #distance{
            font-size:40px;
            color:blue;
            font-weight:bold;
        }
    </style>
</head>

<body>

<div class="card">
    <h2>Ultrasonic Distance</h2>

    <p id="distance">0 cm</p>
</div>

<script>
setInterval(function() {
    fetch('/distance')
    .then(response => response.text())
    .then(data => {
        document.getElementById('distance').innerHTML = data + " cm";
    })
    .catch(error => {
        document.getElementById('distance').innerHTML = "Error";
        console.log(error);
    });
}, 1000);
</script>

</body>
</html>
)rawliteral";

void handleroot()
{
  server.send(200,"text/html",webpage);
  
}
void handleSensor()
{

 long duration;
 float distance;

 digitalWrite(TRG_pin,LOW);
 delayMicroseconds(10);

 digitalWrite(TRG_pin,HIGH);
 delayMicroseconds(10);

 digitalWrite(TRG_pin,LOW);

 duration = pulseIn(ECHO_pin,HIGH);
 distance = (duration * 0.034) / 2;


 String reading = "distance:"+ String(distance);

 server.send(200,"text/plain",reading);
}
void setup() {
  
 Serial.begin(115200);
 pinMode(TRG_pin,OUTPUT);
 pinMode(ECHO_pin,INPUT);
 digitalWrite(TRG_pin,LOW);

    
    WiFi.softAP(ssid, password);
    Serial.print("IP adress:");
    Serial.println(WiFi.softAPIP());
    server.on("/", handleroot);
    server.on("/distance", handleSensor);

    server.begin();
}

void loop() {
  
server.handleClient();

}
