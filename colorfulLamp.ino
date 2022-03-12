#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT      //小爱
#define BLINKER_ALIGENIE_LIGHT  //天猫精灵
#define BLINKER_WITHOUT_SSL
#include <FastLED.h>
#include <Blinker.h>


char auth[] = "d69d03c69cb9";
char ssid[] = "HUAWEI-G10E94_Wi-Fi5";
char pswd[] = "hcq12345679";

//char ssid[] = "Transland_2.4G";
//char pswd[] = "Transland@UBT";

//4个开关按键 2个滑条 1个色轮
BlinkerButton Button1("k1");
BlinkerButton Button2("k2");
BlinkerButton Button3("k3");
BlinkerButton Button4("k4");
BlinkerSlider Slider1("hk1");//亮度
BlinkerSlider Slider2("hk2");//频率
BlinkerRGB RGBWS2812("RGB1");

//===========================================天猫精灵
#define RGB_1 "RGBKey"
String TMJLwsMode = BLINKER_CMD_COMMON;
//===========================================天猫精灵

int openState = 0;//灯效模式开关，默认0关灯
bool wsState;
uint8_t wsMode = BLINKER_CMD_MIOT_DAY;

//===========================================FastLED
#define PIN         14       //ws2812 DAT 接的引脚编号，注意开发板不同，=====请更改=====//触控按键引脚A0
#define NUM_LEDS    30       //ws2812 灯数
#define LED_TYPE    WS2812B  //芯片型号FastLED
#define COLOR_ORDER GRB      //红绿蓝顺序FastLED
int UPDATES_PER_SECOND = 50; //每秒刷新频率FastLED
int BRIGHTNESS = 20;         //亮度值FastLED


uint8_t colorR, colorG, colorB, colorW;//rgb以及亮度

CRGB leds[NUM_LEDS];//引入灯数FastLED

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
//===========================================FastLED

void heartbeat()//心跳回调开关状态数据
{
  if (openState == 0){
      Slider1.print(BRIGHTNESS);
      Slider2.print(UPDATES_PER_SECOND);
      RGBWS2812.print(255, 255, 255, 127);
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
      }
   else if (openState == 1){
      Slider1.print(BRIGHTNESS);
      Slider2.print(UPDATES_PER_SECOND);
      Button1.print("on");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
      }
   else if (openState == 2){
      Slider1.print(BRIGHTNESS);
      Slider2.print(UPDATES_PER_SECOND);
      Button1.print("off");
      Button2.print("on");
      Button3.print("off");
      Button4.print("off");
      }
   else if (openState == 3){
      Slider1.print(BRIGHTNESS);
      Slider2.print(UPDATES_PER_SECOND);
      Button1.print("off");
      Button2.print("off");
      Button3.print("on");
      Button4.print("off");
      }
   else if (openState == 4){
      Slider1.print(BRIGHTNESS);
      Slider2.print(UPDATES_PER_SECOND);
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
      Button4.print("on");
      }
   else if (openState == 5){
      Slider1.print(BRIGHTNESS);
      Slider2.print(UPDATES_PER_SECOND);
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");}
}


///////////////////////////////////////////////////////////////////亮度滑块

void slider1_callback(int32_t value)
{
  BLINKER_LOG("get slider value: ", value);
  BRIGHTNESS = value;
  Serial.println(BRIGHTNESS);
}
///////////////////////////////////////////////////////////////////频率滑块

void slider2_callback(int32_t value)
{
  BLINKER_LOG("get slider value: ", value);
  UPDATES_PER_SECOND = value;
  Serial.println(UPDATES_PER_SECOND);
}

///////////////////////////////////////////////////////////////////调色盘
void ws2812_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
  BLINKER_LOG("R value: ", r_value);
  BLINKER_LOG("G value: ", g_value);
  BLINKER_LOG("B value: ", b_value);
  BLINKER_LOG("Rrightness value: ", bright_value);//颜色和亮度赋值
  colorR = r_value;
  colorG = g_value;
  colorB = b_value;
  BRIGHTNESS = bright_value;
  //Text1.print("灯光模式：" , "单颜色");
     if (openState == 1){
      Button1.print("off");
      }
   else if (openState == 2){
      Button2.print("off");
      }
   else if (openState == 3){
      Button3.print("off");
      }
   else if (openState == 4){
      Button4.print("off");
      }
   openState = 5;
}

void RGBWShow()  
{
  FastLED.setBrightness( BRIGHTNESS );
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(colorR,colorG,colorB);
  }
  FastLED.show();
}

//===========================================天猫精灵


String TMJLgetColor()
{
    uint32_t color = colorR << 16 | colorG << 8 | colorB;

    switch (color)
    {
        case 0xFF0000 :
            return "Red";
        case 0xFFFF00 :
            return "Yellow";
        case 0x0000FF :
            return "Blue";
        case 0x00FF00 :
            return "Green";
        case 0xFFFFFF :
            return "White";
        case 0x000000 :
            return "Black";
        case 0x00FFFF :
            return "Cyan";
        case 0x800080 :
            return "Purple";
        case 0xFFA500 :
            return "Orange";
        default :
            return "White";
    }
}

void aligeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();
        wsState = true;
        colorR = 255;
        colorG = 255;
        colorB = 255;
        BRIGHTNESS = 127;
        BlinkerAliGenie.brightness(BRIGHTNESS * 255 / 100);
        openState = 5;
        Button1.print("off");
        Button2.print("off");
        Button3.print("off");
        Button4.print("off");
    }
    else if (state == BLINKER_CMD_OFF) {
        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();
        wsState = false;
        openState = 0;
        Button1.print("off");
        Button2.print("off");
        Button3.print("off");
        Button4.print("off");
    }
}

void aligenieColor(const String & color)
{
    BLINKER_LOG("need set color: ", color);

    if (color == "Red") {
        openState = 5;
        colorR = 255;
        colorG = 0;
        colorB = 0;
    }
    else if (color == "Yellow") {
        openState = 5;
        colorR = 255;
        colorG = 255;
        colorB = 0;
    }
    else if (color == "Blue") {
        openState = 5;
        colorR = 0;
        colorG = 0;
        colorB = 255;
    }
    else if (color == "Green") {
        openState = 5;
        colorR = 0;
        colorG = 255;
        colorB = 0;
    }
    else if (color == "White") {
        openState = 5;
        colorR = 255;
        colorG = 255;
        colorB = 255;
    }
    else if (color == "Black") {
        openState = 5;
        colorR = 0;
        colorG = 0;
        colorB = 0;
    }
    else if (color == "Cyan") {
        openState = 5;
        colorR = 0;
        colorG = 255;
        colorB = 255;
    }
    else if (color == "Purple") {
        openState = 5;
        colorR = 128;
        colorG = 0;
        colorB = 128;
    }
    else if (color == "Orange") {
        openState = 5;
        colorR = 255;
        colorG = 165;
        colorB = 0;
    }

    if (wsState == false) {
        wsState = true;
        openState = 5;
        colorW = 255;
    }

    if (colorW == 0) {
        openState = 5;
        colorW = 255;
    }

    RGBWShow();

    BlinkerAliGenie.color(color);
    BlinkerAliGenie.print();
}
void aligenieMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_ALIGENIE_READING) {//阅读
        // Your mode function
      BlinkerAliGenie.powerState("on");
      openState = 1;
      Button1.print("on");
      Button1.color("#00a4ff");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
    }
    else if (mode == BLINKER_CMD_ALIGENIE_MOVIE) {//影院
    // Your mode function
    BlinkerAliGenie.powerState("on");
    openState = 2;
    Button2.print("on");
    Button2.color("#00a4ff");
    Button1.print("off");
    Button3.print("off");
    Button4.print("off");
    }
    else if (mode == BLINKER_CMD_ALIGENIE_SLEEP) {//睡眠
    // Your mode function
    BlinkerAliGenie.powerState("on");
    openState = 4;
    Button4.print("on");
    Button4.color("#00a4ff");
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    }
    else if (mode == BLINKER_CMD_ALIGENIE_HOLIDAY) {//假日
    // Your mode function

    }
    else if (mode == BLINKER_CMD_ALIGENIE_MUSIC) {//音乐
        // Your mode function

    }
    else if (mode == BLINKER_CMD_ALIGENIE_COMMON) {//正常
        // Your mode function

    BlinkerAliGenie.powerState("on");
    openState = 3;
    Button3.print("on");
    Button3.color("#00a4ff");
    Button1.print("off");
    Button2.print("off");
    Button4.print("off");
    }

    TMJLwsMode = mode;

    BlinkerAliGenie.mode(mode);
    BlinkerAliGenie.print();
}

//void aligenieCancelMode(const String & cmode)
//{
//    BLINKER_LOG("need cancel mode: ", cmode);
//
//    if (cmode == BLINKER_CMD_ALIGENIE_READING) {
//        // Your mode function
//    }
//    else if (cmode == BLINKER_ALIGENIE_CMD_MOVIE) {
//        // Your mode function
//    }
//    else if (cmode == BLINKER_ALIGENIE_CMD_SLEEP) {
//        // Your mode function
//    }
//    else if (cmode == BLINKER_ALIGENIE_CMD_HOLIDAY) {
//        // Your mode function
//    }
//    else if (cmode == BLINKER_ALIGENIE_CMD_MUSIC) {
//        // Your mode function
//    }
//    else if (cmode == BLINKER_ALIGENIE_CMD_COMMON) {
//        // Your mode function
//    }
//
//    TMJLwsMode = BLINKER_CMD_COMMON; // new mode
//
//    BlinkerAliGenie.mode(TMJLwsMode); // must response
//    BlinkerAliGenie.print();
//}

void aligenieBright(const String & bright)
{
    BLINKER_LOG("need set brightness: ", bright);

    if (bright == BLINKER_CMD_MAX) {
        BRIGHTNESS = 255;
    }
    else if (bright == BLINKER_CMD_MIN) {
        colorW = 0;
    }
    else {
        BRIGHTNESS = bright.toInt() * 255 / 100;
    }

    BLINKER_LOG("now set brightness: ", bright);
    BlinkerAliGenie.brightness(BRIGHTNESS * 255 / 100);
    Slider1.print(BRIGHTNESS);
    BlinkerAliGenie.print();
}

void aligenieRelativeBright(int32_t bright)
{
    BLINKER_LOG("need set relative brightness: ", bright);

    if (BRIGHTNESS + bright < 255 && BRIGHTNESS + bright >= 0) {
        BRIGHTNESS += bright;
    }

    BLINKER_LOG("now set brightness: ", BRIGHTNESS);
    BlinkerAliGenie.brightness(BRIGHTNESS * 255 / 100);
    Slider1.print(BRIGHTNESS);
    BlinkerAliGenie.print();
}

void aligenieColoTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set colorTemperature: ", colorTemp);

    BlinkerAliGenie.colorTemp(colorTemp);
    BlinkerAliGenie.print();
}

void aligenieRelativeColoTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set relative colorTemperature: ", colorTemp);

    BlinkerAliGenie.colorTemp(colorTemp);
    BlinkerAliGenie.print();
}

void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState("on");
            BlinkerAliGenie.color(TMJLgetColor());
            BlinkerAliGenie.mode(TMJLwsMode);
            BlinkerAliGenie.colorTemp(50);
            BlinkerAliGenie.brightness(BRIGHTNESS * 100 / 255);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState("on");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLOR_NUMBER :
            BLINKER_LOG("AliGenie Query Color");
            BlinkerAliGenie.color(TMJLgetColor());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("AliGenie Query Mode");
            BlinkerAliGenie.mode(TMJLwsMode);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
            BLINKER_LOG("AliGenie Query ColorTemperature");
            BlinkerAliGenie.colorTemp(50);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("AliGenie Query Brightness");
            BlinkerAliGenie.brightness(BRIGHTNESS * 100 / 255);
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState("on");
            BlinkerAliGenie.color(TMJLgetColor());
            BlinkerAliGenie.mode(TMJLwsMode);
            BlinkerAliGenie.colorTemp(50);
            BlinkerAliGenie.brightness(BRIGHTNESS * 100 / 255);
            BlinkerAliGenie.print();
            break;
    }
}


//===========================================天猫精灵

///////////////////////////////////////////////////////////////////K1
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("日光模式开启"); 
    openState = 0;
    openState = 1;
      Button1.print("on");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("日光模式开关闭!");
    Button1.print("off");
    openState = 0;
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }
   
}

///////////////////////////////////////////////////////////////////K2
void button2_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("夜光模式开启");
    openState = 0;
    openState = 2;
      Button2.print("on");
      Button1.print("off");
      Button3.print("off");
      Button4.print("off");
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("夜光模式关闭!");
    Button2.print("off");
    openState = 0;
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }
}

///////////////////////////////////////////////////////////////////K3
void button3_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("温馨模式开启");
    openState = 0;
    openState = 3;
      Button3.print("on");
      Button1.print("off");
      Button2.print("off");
      Button4.print("off");
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("温馨模式关闭!");
    Button3.print("off");
    openState = 0;
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }

}

///////////////////////////////////////////////////////////////////K4
void button4_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON) {
    BLINKER_LOG("电脑模式开启");
    openState = 0;
    openState = 4;
      Button4.print("on");
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
  }
  else if (state == BLINKER_CMD_OFF) {
    BLINKER_LOG("电脑模式关闭!");
    Button4.print("off");
    openState = 0;
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }

}



void dataRead(const String & data)//读取时间数据函数
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}


uint32_t getColor()//颜色转换小爱识别
{

  uint32_t color = colorR << 16 | colorG << 8 | colorB;

  return color;
}


void miotPowerState(const String & state)//小爱开关灯
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    wsState = true;
    colorR = 255;
    colorG = 255;
    colorB = 255;
    BRIGHTNESS = 127;
    BlinkerMIOT.brightness(BRIGHTNESS * 255 / 100);
    openState = 5;
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }
  else if (state == BLINKER_CMD_OFF) {
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    wsState = false;
    openState = 0;
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }
}
void miotBright(const String & bright)//小爱亮度设置
{
  BLINKER_LOG("need set brightness: ", bright);
  BRIGHTNESS = bright.toInt() * 255 / 100;
  BlinkerMIOT.brightness(BRIGHTNESS * 255 / 100);
  Slider1.print(BRIGHTNESS);
  BlinkerMIOT.print();
}
void miotColor(int32_t color)       //小爱rgb颜色设置
{
  BLINKER_LOG("need set color: ", color);

  colorR = color >> 16 & 0xFF;
  colorG = color >>  8 & 0xFF;
  colorB = color       & 0xFF;
  BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);
  openState = 5;
  BlinkerMIOT.color(color);
  BlinkerMIOT.print();
}

void miotMode(uint8_t mode)//小爱模式对应
{
  BLINKER_LOG("need set mode: ", mode);

  if (mode == BLINKER_CMD_MIOT_DAY) {//日光//渐变
    // Your mode function
    BlinkerMIOT.powerState("on");
    openState = 1;
    Button1.print("on");
    Button1.color("#00a4ff");
    Button2.print("off");
    Button3.print("off");
    Button4.print("off");
  }
  else if (mode == BLINKER_CMD_MIOT_NIGHT) {//夜光//缤纷
    // Your mode function
    BlinkerMIOT.powerState("on");
    openState = 2;
    Button2.print("on");
    Button2.color("#00a4ff");
    Button1.print("off");
    Button3.print("off");
    Button4.print("off");
  }
  else if (mode == BLINKER_CMD_MIOT_COLOR) {//彩光//火焰
    // Your mode function
    BlinkerMIOT.powerState("on");
    openState = 3;
    Button3.print("on");
    Button3.color("#00a4ff");
    Button1.print("off");
    Button2.print("off");
    Button4.print("off");
  }
  else if (mode == BLINKER_CMD_MIOT_WARMTH) {//温馨//波动
    // Your mode function
    BlinkerMIOT.powerState("on");
    openState = 4;
    Button4.print("on");
    Button4.color("#00a4ff");
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
  }
  else if (mode == BLINKER_CMD_MIOT_TV) {//电视模式
    // Your mode function
  }
  else if (mode == BLINKER_CMD_MIOT_READING) {//阅读模式
    // Your mode function
  }
  else if (mode == BLINKER_CMD_MIOT_COMPUTER) {//电脑模式
    // Your mode function
  }

  wsMode = mode;

  BlinkerMIOT.mode(mode);
  BlinkerMIOT.print();
}


void miotColoTemp(int32_t colorTemp)//小爱设置色温属性(没开启)
{
  BLINKER_LOG("need set colorTemperature: ", colorTemp);

  BlinkerMIOT.colorTemp(colorTemp);
  BlinkerMIOT.print();
}

void miotQuery(int32_t queryCode)//小爱查询灯光状态
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)  
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER ://查询所有状态
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState("on");
      BlinkerMIOT.color(getColor());
      BlinkerMIOT.mode(wsMode);
//      BlinkerMIOT.colorTemp();
      BlinkerMIOT.brightness(BRIGHTNESS * 100 / 255);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER ://查询电源状态
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState("on");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_COLOR_NUMBER ://查询颜色状态
      BLINKER_LOG("MIOT Query Color");
      BlinkerMIOT.color(getColor());
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_MODE_NUMBER ://查询模式状态
      BLINKER_LOG("MIOT Query Mode");
      BlinkerMIOT.mode(wsMode);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_COLORTEMP_NUMBER ://查询色温状态
      BLINKER_LOG("MIOT Query ColorTemperature");
//      BlinkerMIOT.colorTemp();
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER ://查询亮度状态
      BLINKER_LOG("MIOT Query Brightness");
      BlinkerMIOT.brightness(BRIGHTNESS * 100 / 255);
      BlinkerMIOT.print();
      break;
    default :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState("on");
      BlinkerMIOT.color(getColor());
      BlinkerMIOT.mode(wsMode);
//      BlinkerMIOT.colorTemp();
      BlinkerMIOT.brightness(BRIGHTNESS * 100 / 255);
      BlinkerMIOT.print();
      break;
  }
}



///////////////////////////////////////////////////////////////////////////K1灯效FastLED(依赖K2灯效FastLED)

typedef void (*SimplePatternList1[])();
SimplePatternList1 gPatterns1 = { rainbow };//rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm 6种特效切换




///////////////////////////////////////////////////////////////////////////K2灯效FastLED
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { juggle };//rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm 6种特效切换

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


///////////////////////////////////////////////////////////////////////////K3灯效
//冷却：空气上升时会冷却多少？
//冷却少=火焰高。 冷却更多=火焰更短。
//默认值50，建议范围20-100
#define COOLING  130

//发出火花：有什么机会（总共255个）点燃新的火花？
//更高的机会=更多的怒吼。 机会降低=火势更加闪烁。
//默认值为120，建议范围为50-200。
#define SPARKING 200
bool gReverseDirection = false;
void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

///////////////////////////////////////////////////////////////////////////K4灯效
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}
///////////////////////////////////////////////////////////////////////////触控按键
long OldTime;
int CNT = 0;
int scjj = 0;
#define TouceValue 50
void touch() {
  int state1 =  analogRead(A0);//触摸按钮IO
 //int state1 = touchRead(2);
// BLINKER_LOG("touchRead value: ", state1);
  if(millis() - OldTime > 100){ //每過100ms運行一次
    if(state1 <= TouceValue){//檢測按鍵按下
    CNT++; //按鍵按下开始计数每秒计10次
    }
  OldTime = millis(); //更新OldTime
    if(CNT>10 && scjj==0){//按下多於1秒
      BRIGHTNESS = BRIGHTNESS-5;
        if(BRIGHTNESS < 1){//限制亮度范围
          BRIGHTNESS = 1;
        }
    }
    if(CNT>10 && scjj==1){//按下多於1秒
      BRIGHTNESS = BRIGHTNESS+5;
        if(BRIGHTNESS > 255){
          BRIGHTNESS = 255;
        }
    }
  }
  if(CNT>10 && state1 > TouceValue && scjj==0){//按鍵有按過1秒以上而且已放開执行
    CNT=0;//計數清零
    scjj=1;//调整为长按加亮度
    Slider1.print(BRIGHTNESS);//松开后亮度值回调到点灯app
  }
  if(CNT>10 && state1 != TouceValue && scjj==1){//按鍵有按過1秒以上而且已放開执行
    CNT=0;//計數清零
    scjj=0;//调整为长按减亮度
    Slider1.print(BRIGHTNESS);//松开后亮度值回调到点灯app
  }
  if(CNT>0 && CNT<10 && state1 != 1024){//按鍵有按過小于1而且已放開秒执行
    switch(openState){
      case 0:
      openState = 1;
      Button1.print("on");
      Button1.color("#00a4ff");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
      Serial.println(openState);
      break;
      case 1:
      openState = 2;
      Button2.print("on");
      Button2.color("#00a4ff");
      Button1.print("off");
      Button3.print("off");
      Button4.print("off");
      Serial.println(openState);
      break;
      case 2:
      openState = 3;
      Button3.print("on");
      Button3.color("#00a4ff");
      Button1.print("off");
      Button2.print("off");
      Button4.print("off");
      Serial.println(openState);
      break;
      case 3:
      openState = 4;
      Button4.print("on");
      Button4.color("#00a4ff");
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
      Serial.println(openState);
      break;
      case 4:
      openState = 0;
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
      Serial.println(openState);
      break;
      case 5:
      openState = 0;
      Button1.print("off");
      Button2.print("off");
      Button3.print("off");
      Button4.print("off");
      Serial.println(openState);
      break;
    default:
      openState = 0;
      break;
    }
   CNT=0;//計數清零
  }
}


void setup()
{

  
  OldTime = millis();
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  Blinker.begin(auth, ssid, pswd);

  Blinker.attachData(dataRead);
  Blinker.attachHeartbeat(heartbeat);
  
  BlinkerAliGenie.attachPowerState(aligeniePowerState);
  BlinkerAliGenie.attachColor(aligenieColor);
  BlinkerAliGenie.attachMode(aligenieMode);
//  BlinkerAliGenie.attachCancelMode(aligenieCancelMode);
  BlinkerAliGenie.attachBrightness(aligenieBright);
  BlinkerAliGenie.attachRelativeBrightness(aligenieRelativeBright);
  BlinkerAliGenie.attachColorTemperature(aligenieColoTemp);
  BlinkerAliGenie.attachRelativeColorTemperature(aligenieRelativeColoTemp);
  BlinkerAliGenie.attachQuery(aligenieQuery);

  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachColor(miotColor);
  BlinkerMIOT.attachMode(miotMode);
  BlinkerMIOT.attachBrightness(miotBright);
  BlinkerMIOT.attachColorTemperature(miotColoTemp);
  BlinkerMIOT.attachQuery(miotQuery);
  
  //pinMode(A0,INPUT);                                //触控按键引脚A0//需要测试触控按键回馈的值//此按键默认回馈8 触发回馈1024
  pinMode(2,INPUT);
  Slider1.attach(slider1_callback);
  Slider2.attach(slider2_callback);
  RGBWS2812.attach(ws2812_callback);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  Button3.attach(button3_callback);
  Button4.attach(button4_callback);
//===========================================FastLED
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, NUM_LEDS)
          .setCorrection( TypicalLEDStrip )
          .setDither(BRIGHTNESS < 255);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
//===========================================FastLED
}

    void mode_1(){
      FastLED.setBrightness( BRIGHTNESS );//实时更新亮度
      gPatterns1[gCurrentPatternNumber]();
      FastLED.show();  
      FastLED.delay(200/UPDATES_PER_SECOND); 
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
      EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
    }
    
    void mode_2(){
      FastLED.setBrightness( BRIGHTNESS );//实时更新亮度
      gPatterns[gCurrentPatternNumber]();
      FastLED.show();  
      FastLED.delay(1000/(UPDATES_PER_SECOND/10)); 
    
      // do some periodic updates
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
      EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
    }
    
    void mode_3(){
      Fire2012();
      FastLED.setBrightness( BRIGHTNESS );//实时更新亮度
      FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
    
    void mode_4(){
      FastLED.setBrightness( BRIGHTNESS );//实时更新亮度
      pride();
      FastLED.show();
      FastLED.delay(1000 / (UPDATES_PER_SECOND/5));
    }
void loop(){
  Blinker.run();

  //touch();
  switch (openState)
  {
    case 0:             //关闭rgb设定全黑
      wsState = false;
//      openState = 0;
      colorR = 0;
      colorG = 0;
      colorB = 0;
      RGBWShow();
      break;
    case 1:
//      openState = 1;
      mode_1();
      break;
    case 2:
//      openState = 2;
      mode_2();
      break;
    case 3:
//      openState = 3;
      mode_3();
      break;
    case 4:
//      openState = 4;
      mode_4();
      break;
    case 5:
      RGBWShow();
      break;
    default:
      break;
  }
}
