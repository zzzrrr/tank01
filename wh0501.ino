#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define trigPin 5                  //Trig Pin
#define echoPin 4                  //Echo Pin
int wh;
long duration, cm, inches;

// 設定網路基地台SSID跟密碼
const char* ssid      = "XXXXXXXX";    //輸入自家無線網路名稱
const char* password  = "XXXXXXXX";   //輸入密碼
// 請更換成 Thing Speak WRITE API KEY
const char* apiKey = "XXXXXXXX"; 輸入API碼
const char* resource = "/update?api_key=";

// Thing Speak API server
const char* server = "api.thingspeak.com";


unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);        // 定義輸入及輸出
  pinMode(echoPin, INPUT);
  delay(10);

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);     // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);             // 讀取 echo 的電位
  duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間

  cm = (duration / 2) / 29.1;       // 將時間換算成距離 cm 或 inch
  int wh = XXX - cm;        //XXX要輸入偵測器與水塔出水口的高度落差
  // sleeping so wont get here
  // 量測間等待至少 2 秒
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // 將最後讀取感測值的時間紀錄下來
    previousMillis = currentMillis;

 }

  Serial.println(wh);

  Serial.print("Connecting to ");
  Serial.print(server);
  WiFiClient client;

  // 使用 80 Port 連線
  if (client.connect(server, 80)) {
    Serial.println(F("connected"));
  }
  else  {
    Serial.println(F("connection failed"));
    return;
  }

  Serial.print("Request resource: ");
  Serial.println(resource);
  client.print(String("GET ") + resource + apiKey + "&field1=" + wh +
               " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" +
               "Connection: close\r\n\r\n");

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }

  if (!client.available()) {
    Serial.println("No response, going back to sleep");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();

  // 每2分鐘會上傳一筆資料
  delay(120000);
}
