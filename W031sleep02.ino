#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Simple_HCSR04.h>
//請修改以下參數--------------------------------------------
char ssid[] = "ALHN03";
char password[] = "48874718";
//請修改為你自己的API Key，並將https改為http
String url = "http://api.thingspeak.com/update?api_key=8W90L44IT668HY1Z";
const int ECHO_PIN = 4; /// the pin at which the sensor echo is connected
const int TRIG_PIN = 5; /// the pin at which the sensor trig is connected

int water;


Simple_HCSR04 *sensor;

void setup()
{
  Serial.begin(115200);
  sensor = new Simple_HCSR04(ECHO_PIN, TRIG_PIN);
  Serial.print("開始連線到無線網路SSID:");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("連線完成");
}

void loop()
{
  unsigned long distance = sensor->measure()->cm();
  int water = (130 - distance);

do
{
  delay(1000);          // 1秒後讀一次
  int distance = sensor->measure()->cm();  // 讀取距離
} while (distance = 0);  //判斷距離是否大於0


  Serial.print("distance: ");
  Serial.print(water);
  Serial.print("cm\n");
  delay(2000);

  //開始傳送到thingspeak
  Serial.println("啟動網頁連線");
  HTTPClient http;
  //將溫度及濕度以http get參數方式補入網址後方
  String url1 = url + "&field1=" + water;
  //http client取得網頁內容
  http.begin(url1);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    //讀取網頁內容到payload
    String payload = http.getString();
    //將內容顯示出來
    Serial.print("網頁內容=");
    Serial.println(payload);
  } else {
    //讀取失敗
    Serial.println("網路傳送失敗");
  }
  http.end();
  ESP.deepSleep(150e6);//休息90秒
}
