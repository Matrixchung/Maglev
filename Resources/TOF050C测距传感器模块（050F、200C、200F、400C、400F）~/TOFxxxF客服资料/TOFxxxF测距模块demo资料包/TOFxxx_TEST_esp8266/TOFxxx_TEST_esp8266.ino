/**************************
*************************************************
Connection: esp8266/Nodemcu    TOFxxxH
                5V/3.3V   ---  VIN  
                  GND     ---  GND 
                 12/D6    ---  RXD 
                 14/D5    ---  TXD 

            OLED-> esp8266/Nodemcu
                 VIN -> 5V/3.3V 
                 GND -> GND    
                 SDA -> D2
                 SCL -> D1
            button1  -> 13/D7
            button2  -> 0/D3
下列无特殊说明 则编号均为GPIO
默认测试单个模块，模块的从机地址为0x01
************************************************  
*/
#define Button_Pin 13 //pin as GPIO LOW effective nodemcu-D7
#define Button_Pin1 0 //nodemcu-D3
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial DT(12, 14); //RX--nodemcu-D6  TX--nodemcu-D5
Adafruit_SSD1306 display(128, 64, &Wire, -1);


////////////////////////////////////指令集///////////////////////////////////////////////
const byte SYSCMD[4][8] = {
  {0x01, 0x06, 0x00, 0x20, 0x00, 0x8C, 0xA5, 0x89},//01 06 00 20 00 8C A5 89设置offcet校准距离140mm
  {0x01, 0x06, 0x00, 0x21, 0x00, 0x64, 0xD8, 0x2B},//01 06 00 21 00 64 D8 2B设置xtalk 校准距离100mm
  {0x01, 0x06, 0x00, 0x06, 0x00, 0x01, 0x0B, 0xA8},//01 06 00 06 00 01 0B A8加载校准
  {0x01, 0x06, 0x00, 0x01, 0x10, 0x00, 0xCA, 0xD5},//01 06 00 01 10 00 CA D5 测距模块重启
};
const byte distanceCMD[2][8] = {
  {0x01, 0x06, 0x00, 0x04, 0x00, 0x00, 0x0B, 0xC8},//01 06 00 04 00 00 0B C8 测距量程设置为1.3m
  {0x01, 0x06, 0x00, 0x04, 0x00, 0x01, 0xCB, 0x09},//01 06 00 04 00 01 CB 09 测距量程设置为4.0m
};
const byte timeCMD[4][8] = {
  {0x01, 0x06, 0x00, 0x05, 0x00, 0x64, 0x20, 0x98},//01 06 00 05 00 64 20 98连续输出及输出时间间隔设置,此处设置为100MS
  {0x01, 0x06, 0x00, 0x05, 0x01, 0xF4, 0xDC, 0x99},//01 06 00 05 01 F4 DC 99连续输出及输出时间间隔设置,此处设置为500MS
  {0x01, 0x06, 0x00, 0x05, 0x07, 0xD0, 0x67, 0x9A},//01 06 00 05 07 D0 67 9A连续输出及输出时间间隔设置,此处设置为2000MS
  {0x01, 0x06, 0x00, 0x05, 0x27, 0x10, 0xF7, 0x83},//01 06 00 05 27 10 F7 83连续输出及输出时间间隔设置,此处设置为10000MS
};
///////////////////////////////////////////////////////////////////////////////////////


void setup()
{
  DT.begin(115200);
  Serial.begin(115200);
  pinMode(Button_Pin, INPUT_PULLUP);
  pinMode(Button_Pin1, INPUT_PULLUP);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      delay(1);
  }
  display.clearDisplay();
  display.display();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("------MH-ET LIVE-----");
  display.println("-Distance  measuring-");
  display.println("---initializing...---");
  display.display();


  DIS4000mm();//测距量程设置为4.0m
  //DIS1300mm();//测距量程设置为1.3m
  delay(2000);
  //outputTIME10S();//设置输出间隔时间10S
  outputTIME2S();//设置输出间隔时间2S
  //outputTIME500MS();//设置输出间隔时间500MS
  //outputTIME100MS();//设置输出间隔时间100MS
  delay(2000);
  JZJZ();//设置为加载校准模式
  delay(2000);

  modRST();
  delay(2000);
}

void loop()
{
  if (!digitalRead(Button_Pin))
  {
    offset();
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Offset:");
    display.println("14cm");
    display.display();
    delay(3000);
  }
  if (!digitalRead(Button_Pin1))
  {
    xtalk();
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Xtalk:10cm");
    display.display();
    delay(3000);
  }


  
  if (DT.available() > 6)
  {
    char a = DT.read();
    if(a != 0x01)
      return;
    byte Buf[6];
    DT.readBytes(Buf, 6);
    for (int i = 0; i < 6; i++)
    {
      if (Buf[i] < 0x10)
        Serial.print("0x0");
      else
        Serial.print("0x");
      Serial.print(Buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (Buf[2] == 0xFF)
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Distance:");
      display.println("invalid");
      display.display();
      return;
    }
    
    long distance = Buf[2] * 256 + Buf[3];
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Distance:");
    display.print(distance);
    display.print("mm");
    display.display();
  }
}
//*********************SYS系统设置*************************//
//*******offset偏移校准函数
void offset()
{
  for (int i = 0; i < 8; i++)
    DT.write(SYSCMD[0][i]);
}
//*******xtalk串扰校准函数
void xtalk()
{
  for (int i = 0; i < 8; i++)
    DT.write(SYSCMD[1][i]);
}
//*********是/否 加载校准
void JZJZ()//加载校准
{
  for (int i = 0; i < 8; i++)
    DT.write(SYSCMD[2][i]);
}
//*********测距模块重启函数
void modRST()//测距模块重启
{
  for (int i = 0; i < 8; i++)
    DT.write(SYSCMD[8][i]);
}
//*********************设置连续输出时间间隔函数集*************************//
void outputTIME100MS()//输出时间间隔100ms
{
  for (int i = 0; i < 8; i++)
    DT.write(timeCMD[0][i]);
}
void outputTIME500MS()//输出时间间隔500ms
{
  for (int i = 0; i < 8; i++)
    DT.write(timeCMD[1][i]);
}
void outputTIME2S()//输出时间间隔2s
{
  for (int i = 0; i < 8; i++)
    DT.write(timeCMD[2][i]);
}
void outputTIME10S()//输出时间间隔10s
{
  for (int i = 0; i < 8; i++)
    DT.write(timeCMD[3][i]);
}
//**********************测距距离（量程）模式选择函数***************************//
void DIS1300mm()//测距量程设置为1.3m
{
  for (int i = 0; i < 8; i++)
    DT.write(distanceCMD[0][i]);
}

void DIS4000mm()//测距量程设置为4m
{
  for (int i = 0; i < 8; i++)
    DT.write(distanceCMD[1][i]);
}
