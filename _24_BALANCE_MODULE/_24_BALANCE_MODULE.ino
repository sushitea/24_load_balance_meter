#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <EEPROM.h>

#define NUM_LOAD_CELL 24

#define ON 0
#define OFF 1

#define START 0
#define CALIBRATION 1
#define TARE 2
#define TRIGGER_SETTING 3

#define FACTOR 0
#define STEP 1
#define OK_MENU 2

#define ARROW_SYMBOL 0
#define ARROW_UP 1
#define ARROW_DOWN 2
#define ARROW_KG 3

#define SCK_1 13
#define DT_1 12

#define SCK_2 11
#define DT_2 10

#define SCK_3 9
#define DT_3 8

#define SCK_4 7
#define DT_4 6

#define SCK_5 5
#define DT_5 4

#define SCK_6 3
#define DT_6 2

#define SCK_7 19
#define DT_7 18

#define SCK_8 14
#define DT_8 15

#define SCK_9 16
#define DT_9 17

#define SCK_10 23
#define DT_10 25

#define SCK_11 27
#define DT_11 29

#define SCK_12 31
#define DT_12 33

#define SCK_13 39
#define DT_13 41

#define SCK_14 43
#define DT_14 45

#define SCK_15 47
#define DT_15 46

#define SCK_16 49
#define DT_16 48

#define SCK_17 A13
#define DT_17 A12

#define SCK_18 50
#define DT_18 52

#define SCK_19 A11
#define DT_19 A10

#define SCK_20 A9
#define DT_20 A8

#define SCK_21 A7
#define DT_21 A6

#define SCK_22 A5
#define DT_22 A4

#define SCK_23 A3
#define DT_23 A2

#define SCK_24 A1
#define DT_24 A0

#define RELAY_PIN 22

#define AVG_VALUE 1

#define BUT_UP 35
#define BUT_DOWN 37
#define BUT_OK 51
#define NO_PRESS -1

#define BALANCE_1 0
#define BALANCE_2 1
#define BALANCE_3 2
#define BALANCE_4 3
#define BALANCE_5 4
#define BALANCE_6 5
#define BALANCE_7 6
#define BALANCE_8 7
#define BALANCE_9 8
#define BALANCE_10 9
#define BALANCE_11 10
#define BALANCE_12 11
#define BALANCE_13 12
#define BALANCE_14 13
#define BALANCE_15 14
#define BALANCE_16 15
#define BALANCE_17 16
#define BALANCE_18 17
#define BALANCE_19 18
#define BALANCE_20 19
#define BALANCE_21 20
#define BALANCE_22 21
#define BALANCE_23 22
#define BALANCE_24 23
#define BACK 24

#define TIMER 300

#define BALANCE_1_ADDR 0
#define BALANCE_2_ADDR 8
#define BALANCE_3_ADDR 16
#define BALANCE_4_ADDR 24
#define BALANCE_5_ADDR 32
#define BALANCE_6_ADDR 40
#define BALANCE_7_ADDR 48
#define BALANCE_8_ADDR 56
#define BALANCE_9_ADDR 64
#define BALANCE_10_ADDR 72
#define BALANCE_11_ADDR 80
#define BALANCE_12_ADDR 88
#define BALANCE_13_ADDR 96
#define BALANCE_14_ADDR 104
#define BALANCE_15_ADDR 112
#define BALANCE_16_ADDR 120
#define BALANCE_17_ADDR 128
#define BALANCE_18_ADDR 136
#define BALANCE_19_ADDR 144
#define BALANCE_20_ADDR 152
#define BALANCE_21_ADDR 160
#define BALANCE_22_ADDR 168
#define BALANCE_23_ADDR 176
#define BALANCE_24_ADDR 184

#define ZERO_1_ADDR 192
#define ZERO_2_ADDR 200
#define ZERO_3_ADDR 208
#define ZERO_4_ADDR 216
#define ZERO_5_ADDR 224
#define ZERO_6_ADDR 232
#define ZERO_7_ADDR 240
#define ZERO_8_ADDR 248
#define ZERO_9_ADDR 256
#define ZERO_10_ADDR 264
#define ZERO_11_ADDR 272
#define ZERO_12_ADDR 280
#define ZERO_13_ADDR 288
#define ZERO_14_ADDR 296
#define ZERO_15_ADDR 304
#define ZERO_16_ADDR 312
#define ZERO_17_ADDR 320
#define ZERO_18_ADDR 328
#define ZERO_19_ADDR 336
#define ZERO_20_ADDR 344
#define ZERO_21_ADDR 352
#define ZERO_22_ADDR 360
#define ZERO_23_ADDR 368
#define ZERO_24_ADDR 376

#define TRIGGER_ADDR 377
#define TRIGGER_START 378

#define LCD_BLACK_LIGHT_TIMER 60000

LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the default I2C bus address of the backpack-see article
LiquidCrystal_I2C lcd(0x26, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the default I2C bus address of the backpack-see article
LiquidCrystal_I2C lcd2(0x25, 2, 1, 0, 4, 5, 6, 7); // 0x27 is the default I2C bus address of the backpack-see article

byte Arrow_symbol[8] =
{
  B00000,
  B00100,
  B11110,
  B11111,
  B11111,
  B11110,
  B00100,
  B00000,
};

byte Arrow_Up[8] =
{
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B01110,
  B01110,
  B00000,
};

byte Arrow_Down[8] =
{
  B00000,
  B01110,
  B01110,
  B01110,
  B11111,
  B01110,
  B00100,
  B00000,
};

byte Arrow_Kg[8] =
{
  B10100,
  B11000,
  B10100,
  B00000,
  B00111,
  B00111,
  B00001,
  B00111,
};

HX711 Balance_1(DT_1, SCK_1);
HX711 Balance_2(DT_2, SCK_2);
HX711 Balance_3(DT_3, SCK_3);
HX711 Balance_4(DT_4, SCK_4);
HX711 Balance_5(DT_5, SCK_5);
HX711 Balance_6(DT_6, SCK_6);
HX711 Balance_7(DT_7, SCK_7);
HX711 Balance_8(DT_8, SCK_8);
HX711 Balance_9(DT_9, SCK_9);
HX711 Balance_10(DT_10, SCK_10);
HX711 Balance_11(DT_11, SCK_11);
HX711 Balance_12(DT_12, SCK_12);
HX711 Balance_13(DT_13, SCK_13);
HX711 Balance_14(DT_14, SCK_14);
HX711 Balance_15(DT_15, SCK_15);
HX711 Balance_16(DT_16, SCK_16);
HX711 Balance_17(DT_17, SCK_17);
HX711 Balance_18(DT_18, SCK_18);
HX711 Balance_19(DT_19, SCK_19);
HX711 Balance_20(DT_20, SCK_20);
HX711 Balance_21(DT_21, SCK_21);
HX711 Balance_22(DT_22, SCK_22);
HX711 Balance_23(DT_23, SCK_23);
HX711 Balance_24(DT_24, SCK_24);

unsigned short gusIsNeedDisp = 1;
short gsW_Menu = START;
unsigned short gusIsButPress = 0;
float gfWeight[NUM_LOAD_CELL] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float gfCalibration_Factor[NUM_LOAD_CELL] = { -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100};
unsigned long gulStart_Time = 0;
short gsTrigger_Kg = 100;
unsigned long gulTimer_Count = 0;
unsigned short gusIsNeedON_BL = 1;
long glZero_factor[NUM_LOAD_CELL] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
short gsIsNeedStart = 0;

void setup()
{
  char szTemp[10];
  short sFactor_Addr[NUM_LOAD_CELL] = {BALANCE_1_ADDR, BALANCE_2_ADDR, BALANCE_3_ADDR, BALANCE_4_ADDR, BALANCE_5_ADDR, BALANCE_6_ADDR, BALANCE_7_ADDR, BALANCE_8_ADDR, BALANCE_9_ADDR, BALANCE_10_ADDR, BALANCE_11_ADDR, BALANCE_12_ADDR, BALANCE_13_ADDR, BALANCE_14_ADDR, BALANCE_15_ADDR, BALANCE_16_ADDR, BALANCE_17_ADDR, BALANCE_18_ADDR, BALANCE_19_ADDR, BALANCE_20_ADDR, BALANCE_21_ADDR, BALANCE_22_ADDR, BALANCE_23_ADDR, BALANCE_24_ADDR};
  short sZero_Addr[NUM_LOAD_CELL] = {ZERO_1_ADDR, ZERO_2_ADDR, ZERO_3_ADDR, ZERO_4_ADDR, ZERO_5_ADDR, ZERO_6_ADDR, ZERO_7_ADDR, ZERO_8_ADDR, ZERO_9_ADDR, ZERO_10_ADDR, ZERO_11_ADDR, ZERO_12_ADDR, ZERO_13_ADDR, ZERO_14_ADDR, ZERO_15_ADDR, ZERO_16_ADDR, ZERO_17_ADDR, ZERO_18_ADDR, ZERO_19_ADDR, ZERO_20_ADDR, ZERO_21_ADDR, ZERO_22_ADDR, ZERO_23_ADDR, ZERO_24_ADDR};


  Serial.begin(9600);

  Serial.print(sizeof(float));

  //LCD initiliasation
  lcd.begin(20, 4);
  lcd1.begin(20, 4);
  lcd2.begin(20, 4);
  lcd.setBacklightPin(3, POSITIVE); // BL, BL_POL
  lcd1.setBacklightPin(3, POSITIVE);
  lcd2.setBacklightPin(3, POSITIVE);
  vLDC_BlackLight_Control(ON);
  lcd.createChar(ARROW_SYMBOL, Arrow_symbol);
  lcd.createChar(ARROW_UP, Arrow_Up);
  lcd.createChar(ARROW_DOWN, Arrow_Down);
  lcd.createChar(ARROW_KG, Arrow_Kg);
  lcd1.createChar(ARROW_SYMBOL, Arrow_symbol);
  lcd1.createChar(ARROW_UP, Arrow_Up);
  lcd1.createChar(ARROW_DOWN, Arrow_Down);
  lcd1.createChar(ARROW_KG, Arrow_Kg);
  lcd2.createChar(ARROW_SYMBOL, Arrow_symbol);
  lcd2.createChar(ARROW_UP, Arrow_Up);
  lcd2.createChar(ARROW_DOWN, Arrow_Down);
  lcd2.createChar(ARROW_KG, Arrow_Kg);
  vLCD_Standby();
  vLCD_Disp_Menu(gsW_Menu);

  pinMode(BUT_OK, INPUT_PULLUP);
  //digitalWrite(BUT_OK, HIGH);
  pinMode(BUT_UP, INPUT_PULLUP);
  pinMode(BUT_DOWN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  vRelay_Control(RELAY_PIN, OFF);

  for (short i = 0; i < NUM_LOAD_CELL; i++)
  {
    memset(szTemp, '\0', sizeof(szTemp));
    gfCalibration_Factor[i] = fEEPROM_Read_factor(sFactor_Addr[i]);
    dtostrf(gfCalibration_Factor[i], 1, 2, szTemp);

    if ((strcmp(szTemp, "NAN") == 0) || (strcmp(szTemp, "OVF") == 0) || (strcmp(szTemp, "INF") == 0))
    {
      gfCalibration_Factor[i] = -1000.00;
    }
  }

  for (short i = 0; i < NUM_LOAD_CELL; i++)
  {
    memset(szTemp, '\0', sizeof(szTemp));
    glZero_factor[i] = lEEPROM_Read_Zerofactor(sZero_Addr[i]);

    if ((glZero_factor[i] == -1) || (glZero_factor[i] == -255))
    {
      if (i == BALANCE_1)
      {
        Balance_1.set_scale();
        Balance_1.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_1.read_average(); //Get a baseline reading
      }
      else if (i == BALANCE_2)
      {
        Balance_2.set_scale();
        Balance_2.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_2.read_average();
      }
      else if (i == BALANCE_3)
      {
        Balance_3.set_scale();
        Balance_3.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_3.read_average();
      }
      else if (i == BALANCE_4)
      {
        Balance_4.set_scale();
        Balance_4.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_4.read_average();
      }
      else if (i == BALANCE_5)
      {
        Balance_5.set_scale();
        Balance_5.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_5.read_average();
      }
      else if (i == BALANCE_6)
      {
        Balance_6.set_scale();
        Balance_6.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_6.read_average();
      }
      else if (i == BALANCE_7)
      {
        Balance_7.set_scale();
        Balance_7.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_7.read_average();
      }
      else if (i == BALANCE_8)
      {
        Balance_8.set_scale();
        Balance_8.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_8.read_average();
      }
      else if (i == BALANCE_9)
      {
        Balance_9.set_scale();
        Balance_9.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_9.read_average();
      }
      else if (i == BALANCE_10)
      {
        Balance_10.set_scale();
        Balance_10.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_10.read_average();
      }
      else if (i == BALANCE_11)
      {
        Balance_11.set_scale();
        Balance_11.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_11.read_average();
      }
      else if (i == BALANCE_12)
      {
        Balance_12.set_scale();
        Balance_12.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_12.read_average();
      }
      else if (i == BALANCE_13)
      {
        Balance_13.set_scale();
        Balance_13.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_13.read_average();
      }
      else if (i == BALANCE_14)
      {
        Balance_14.set_scale();
        Balance_14.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_14.read_average();
      }
      else if (i == BALANCE_15)
      {
        Balance_15.set_scale();
        Balance_15.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_15.read_average();
      }
      else if (i == BALANCE_16)
      {
        Balance_16.set_scale();
        Balance_16.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_16.read_average();
      }
      else if (i == BALANCE_17)
      {
        Balance_17.set_scale();
        Balance_17.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_17.read_average();
      }
      else if (i == BALANCE_18)
      {
        Balance_18.set_scale();
        Balance_18.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_18.read_average();
      }
      else if (i == BALANCE_19)
      {
        Balance_19.set_scale();
        Balance_19.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_19.read_average();
      }
      else if (i == BALANCE_20)
      {
        Balance_20.set_scale();
        Balance_20.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_20.read_average();
      }
      else if (i == BALANCE_21)
      {
        Balance_21.set_scale();
        Balance_21.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_21.read_average();
      }
      else if (i == BALANCE_22)
      {
        Balance_22.set_scale();
        Balance_22.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_22.read_average();
      }
      else if (i == BALANCE_23)
      {
        Balance_23.set_scale();
        Balance_23.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_23.read_average();
      }
      else if (i == BALANCE_24)
      {
        Balance_24.set_scale();
        Balance_24.tare();  //Reset the scale to 0
        glZero_factor[i] = Balance_24.read_average();
      }
      vEEPROM_write_Zerofactor(sZero_Addr[i], glZero_factor[i]);
    }
    else
    {
      if (i == BALANCE_1)
      {
        Balance_1.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_2)
      {
        Balance_2.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_3)
      {
        Balance_3.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_4)
      {
        Balance_4.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_5)
      {
        Balance_5.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_6)
      {
        Balance_6.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_7)
      {
        Balance_7.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_8)
      {
        Balance_8.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_9)
      {
        Balance_9.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_10)
      {
        Balance_10.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_11)
      {
        Balance_11.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_12)
      {
        Balance_12.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_13)
      {
        Balance_13.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_14)
      {
        Balance_14.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_15)
      {
        Balance_15.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_16)
      {
        Balance_16.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_17)
      {
        Balance_17.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_18)
      {
        Balance_18.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_19)
      {
        Balance_19.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_20)
      {
        Balance_20.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_21)
      {
        Balance_21.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_22)
      {
        Balance_22.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_23)
      {
        Balance_23.set_offset(glZero_factor[i]);
      }
      else if (i == BALANCE_24)
      {
        Balance_24.set_offset(glZero_factor[i]);
      }
    }
  }
  gsTrigger_Kg = sEEPROM_Read(TRIGGER_ADDR);

  if ((gsTrigger_Kg > 250) || (gsTrigger_Kg < 0))
  {
    gsTrigger_Kg = 100;
  }

  gsIsNeedStart = sEEPROM_Read(TRIGGER_START);

  if ((gsIsNeedStart < 0) || (gsIsNeedStart > 1))
  {
    gsIsNeedStart = 0;
  }

  vEn_Timer2Interrupt();
  Serial.println("Initialised");
}

void loop()
{
  short sBut = NO_PRESS;
  sBut = sRead_Button();
  Serial.println(sBut);
  Serial.print("gsW_Menu:");
  Serial.println(gsW_Menu);

  if (sBut == NO_PRESS)
  {
    gusIsButPress = 0;
  }
  else
  {
    if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
    {
      gusIsButPress = 1;
    }

    gusIsNeedON_BL = 1;
    vLDC_BlackLight_Control(ON);
    gulTimer_Count = 0;
    vEn_Timer2Interrupt();
  }

  if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
  {
    gusIsNeedON_BL = 0;
    vLDC_BlackLight_Control(OFF);
    gulTimer_Count = 0;
  }

  if (sBut == BUT_UP)
  {
    if (gusIsButPress == 0)
    {
      gsW_Menu--;
      vLCD_Disp_Menu(gsW_Menu);
      if (gsW_Menu < START)
      {
        gsW_Menu = TRIGGER_SETTING;
        vLCD_Disp_Menu(gsW_Menu);
      }
      gusIsNeedDisp = 1;
      gusIsButPress = 1;
    }
  }
  else if (sBut == BUT_DOWN)
  {
    if (gusIsButPress == 0)
    {
      gsW_Menu++;
      vLCD_Disp_Menu(gsW_Menu);
      if (gsW_Menu > TRIGGER_SETTING)
      {
        gsW_Menu = START;
        vLCD_Disp_Menu(gsW_Menu);
      }
      gusIsNeedDisp = 1;
      gusIsButPress = 1;
    }
  }
  else if ((sBut == BUT_OK) || (gsIsNeedStart == 1))
  {
    if (gusIsButPress == 0)
    {
      gusIsNeedDisp = 1;
      gusIsButPress = 1;

      if ((gsW_Menu == START) || (gsIsNeedStart == 1))
      {
        gsIsNeedStart = 1;
        vEEPROM_write(TRIGGER_START, gsIsNeedStart);
        vStart_Main();
        gsIsNeedStart = 0;
        vEEPROM_write(TRIGGER_START, gsIsNeedStart);
      }
      else if (gsW_Menu == CALIBRATION)
      {
        vCalibration();
      }
      else if (gsW_Menu == TARE)
      {
        vTare();
      }
      else
      {
        vTrigger_Setting();
      }
    }

    if (gusIsNeedDisp == 1)
    {
      vLCD_Disp_Menu(gsW_Menu);
      gusIsNeedDisp = 0;
    }
  }
}

void vRelay_Control(short sRelay, short sOn_Off)
{
  if (sOn_Off == ON)
  {
    digitalWrite(sRelay, LOW);
  }
  else
  {
    digitalWrite(sRelay, HIGH);
  }
}

void vTrigger_Setting(void)
{
  unsigned short usExit = 0;
  short sBut = NO_PRESS;
  short usDelay = 200;

  gusIsButPress = 1;

  do
  {
    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      usDelay = 200;
      gusIsButPress = 0;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (sBut == BUT_UP)
    {
      gsTrigger_Kg++;

      if (gsTrigger_Kg >= 250)
      {
        gsTrigger_Kg = 250;
      }

      delay(usDelay);
      usDelay -= 10;
      if (usDelay <= 10)
      {
        usDelay = 10;
      }
      gusIsNeedDisp = 1;
    }
    else if (sBut == BUT_DOWN)
    {
      gsTrigger_Kg--;

      if (gsTrigger_Kg < 0)
      {
        gsTrigger_Kg = 0;
      }
      delay(usDelay);
      usDelay -= 10;
      if (usDelay <= 10)
      {
        usDelay = 10;
      }
      gusIsNeedDisp = 1;
    }
    else if (sBut == BUT_OK)
    {
      if (gusIsButPress == 0)
      {
        usExit = 1;
        gusIsButPress = 1;
      }
    }

    if (gusIsNeedDisp == 1)
    {
      vLCD_Disp_Trigger_Setting(gsTrigger_Kg);
      gusIsNeedDisp = 0;
    }

  } while (usExit == 0);

  vEEPROM_write(TRIGGER_ADDR, gsTrigger_Kg);
  gusIsNeedDisp = 1;
}

void vLCD_Disp_Trigger_Setting(short sTrigger_Kg)
{
  lcd.clear();
  lcd.print("--------------------");
  lcd.setCursor(0, 1);
  lcd.print("Trigger KG :");
  lcd.setCursor(0, 2);
  lcd.print(sTrigger_Kg);
  lcd.setCursor(4, 2);
  lcd.print("KG");
  lcd.setCursor(17, 2);
  lcd.print("+ -");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");
}

void vLCD_Standby(void)
{
  lcd1.clear();
  lcd1.print("--------------------");
  lcd1.setCursor(0, 1);
  lcd1.print("|24 Channel Balance|");
  lcd1.setCursor(0, 2);
  lcd1.print("|      Module      |");
  lcd1.setCursor(0, 3);
  lcd1.print("--------------------");
  lcd2.clear();
  lcd2.print("--------------------");
  lcd2.setCursor(0, 1);
  lcd2.print("|24 Channel Balance|");
  lcd2.setCursor(0, 2);
  lcd2.print("|      Module      |");
  lcd2.setCursor(0, 3);
  lcd2.print("--------------------");
}

void vStart_Main(void)
{
  unsigned short usExit = 0;
  short sBut = NO_PRESS;
  short sWhichFull[NUM_LOAD_CELL] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  short sIsNeed_ON_Relay[NUM_LOAD_CELL] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  short sIsRelay_ON = 0;
  short sIsNeedOffRelay = 0;

  vStart_Timer();

  do
  {
    sIsNeedOffRelay = 1;

    for (short i = 0; i < NUM_LOAD_CELL; i++)
    {
      gfWeight[i] = fGet_Weight(i);

      if (gfWeight[i] > gsTrigger_Kg)
      {
        sWhichFull[i] = 1;
        sIsNeedOffRelay = 0;
      }
      else
      {
        sWhichFull[i] = 0;
        sIsNeed_ON_Relay[i] = 1;
      }

      if ((sWhichFull[i] == 1) && (sIsNeed_ON_Relay[i]) == 1)
      {
        vRelay_Control(RELAY_PIN, ON);
        sIsRelay_ON = 1;
      }
    }

    if (sIsNeedOffRelay == 1)
    {
      vRelay_Control(RELAY_PIN, OFF);
      sIsRelay_ON = 0;
    }

    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      gusIsButPress = 0;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();

      if (sIsRelay_ON == 1)
      {
        if (gusIsButPress == 0)
        {
          vRelay_Control(RELAY_PIN, OFF);
          sIsRelay_ON = 0;
          for (short i = 0; i < NUM_LOAD_CELL; i++)
          {
            if (sWhichFull[i] == 1)
            {
              sIsNeed_ON_Relay[i] = 0;
            }
          }
          gusIsButPress = 1;
        }
      }
      else
      {
        if (sBut == BUT_OK)
        {
          if (gusIsButPress == 0)
          {
            usExit = 1;
            gusIsButPress = 1;
          }
        }
      }
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (ulCheck_Timer() >= TIMER)
    {
      vStart_Timer();
      gusIsNeedDisp = 1;
    }

    if (gusIsNeedDisp == 1)
    {
      vLCD_Disp_Weight(sWhichFull);
      gusIsNeedDisp = 0;
    }

  } while (usExit == 0);

  gusIsNeedDisp = 1;
}

void vLCD_Disp_Weight(short sWhichFull[])
{
  lcd1.clear();

  lcd1.setCursor(0, 0);
  lcd1.print("1: ");
  lcd1.print(gfWeight[0]);

  lcd1.setCursor(0, 1);
  lcd1.print("2: ");
  lcd1.print(gfWeight[1]);

  lcd1.setCursor(0, 2);
  lcd1.print("3: ");
  lcd1.print(gfWeight[2]);

  lcd1.setCursor(0, 3);
  lcd1.print("4: ");
  lcd1.print(gfWeight[3]);


  lcd1.setCursor(10, 0);
  lcd1.print("5: ");
  lcd1.print(gfWeight[4]);

  lcd1.setCursor(10, 1);
  lcd1.print("6: ");
  lcd1.print(gfWeight[5]);

  lcd1.setCursor(10, 2);
  lcd1.print("7: ");
  lcd1.print(gfWeight[6]);

  lcd1.setCursor(10, 3);
  lcd1.print("8: ");
  lcd1.print(gfWeight[7]);
  lcd1.setCursor(19, 3);
  lcd1.write(byte(ARROW_KG));

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("9:  ");
  lcd.print(gfWeight[8]);

  lcd.setCursor(0, 1);
  lcd.print("10: ");
  lcd.print(gfWeight[9]);

  lcd.setCursor(0, 2);
  lcd.print("11: ");
  lcd.print(gfWeight[10]);

  lcd.setCursor(0, 3);
  lcd.print("12: ");
  lcd.print(gfWeight[11]);


  lcd.setCursor(10, 0);
  lcd.print("13: ");
  lcd.print(gfWeight[12]);

  lcd.setCursor(10, 1);
  lcd.print("14: ");
  lcd.print(gfWeight[13]);

  lcd.setCursor(10, 2);
  lcd.print("15: ");
  lcd.print(gfWeight[14]);

  lcd.setCursor(10, 3);
  lcd.print("16: ");
  lcd.print(gfWeight[15]);
  lcd.setCursor(19, 3);
  lcd.write(byte(ARROW_KG));

  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("17: ");
  lcd2.print(gfWeight[16]);

  lcd2.setCursor(0, 1);
  lcd2.print("18: ");
  lcd2.print(gfWeight[17]);

  lcd2.setCursor(0, 2);
  lcd2.print("19: ");
  lcd2.print(gfWeight[18]);

  lcd2.setCursor(0, 3);
  lcd2.print("20: ");
  lcd2.print(gfWeight[19]);


  lcd2.setCursor(10, 0);
  lcd2.print("21: ");
  lcd2.print(gfWeight[20]);

  lcd2.setCursor(10, 1);
  lcd2.print("22: ");
  lcd2.print(gfWeight[21]);

  lcd2.setCursor(10, 2);
  lcd2.print("23: ");
  lcd2.print(gfWeight[22]);

  lcd2.setCursor(10, 3);
  lcd2.print("24: ");
  lcd2.print(gfWeight[23]);
  lcd2.setCursor(19, 3);
  lcd2.write(byte(ARROW_KG));

  for (short i = 0; i < 24; i++)
  {
    if (sWhichFull[i] == 1)
    {
      if (i < 4)
      {
        lcd1.setCursor(2, i);
        lcd1.print("*");
      }

      else if (i < 8)
      {
        lcd1.setCursor(12, (i - 4));
        lcd1.print("*");
      }

      else if (i < 12)
      {
        lcd.setCursor(3, (i - 8));
        lcd.print("*");
      }

      else if (i < 16)
      {
        lcd.setCursor(13, (i - 12));
        lcd.print("*");
      }

      else if (i < 20)
      {
        lcd2.setCursor(3, (i - 16));
        lcd2.print("*");
      }

      else if (i < 24)
      {
        lcd2.setCursor(13, (i - 20));
        lcd2.print("*");
      }
      
      
      
    }
  }
}

void vCalibration(void)
{
  unsigned short usExit = 0;
  short sWhich_Balance = 0;
  short sBut = NO_PRESS;

  do
  {
    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      gusIsButPress = 0;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (sBut == BUT_UP)
    {
      if (gusIsButPress == 0)
      {
        sWhich_Balance--;
        if (sWhich_Balance < 0)
        {
          sWhich_Balance = BACK;
        }
        gusIsNeedDisp = 1;
        gusIsButPress = 1;
      }
    }
    else if (sBut == BUT_DOWN)
    {
      if (gusIsButPress == 0)
      {
        sWhich_Balance++;
        if (sWhich_Balance > BACK)
        {
          sWhich_Balance = BALANCE_1;
        }
        gusIsNeedDisp = 1;
        gusIsButPress = 1;
      }
    }
    else if (sBut == BUT_OK)
    {
      if (gusIsButPress == 0)
      {
        gusIsButPress = 1;
        if (sWhich_Balance == BACK)
        {
          usExit = 1;
        }
        else
        {
          vLCD_Standby();
          vCalibrate_Step1(sWhich_Balance);
          vCalibrate_Step2(sWhich_Balance);
          vCalibrate_Step3(sWhich_Balance);
        }
      }
    }

    if (gusIsNeedDisp == 1)
    {
      vLCD_Disp_Which_Balance(sWhich_Balance);
      gusIsNeedDisp = 0;
    }
  } while (usExit == 0);

  gusIsNeedDisp = 1;
}

float fGet_Weight(short sWhich_Balance)
{
  float fWeight = 0.0;

  if (sWhich_Balance == BALANCE_1)
  {
    Balance_1.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_1.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_2)
  {
    Balance_2.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_2.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_3)
  {
    Balance_3.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_3.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_4)
  {
    Balance_4.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_4.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_5)
  {
    Balance_5.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_5.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_6)
  {
    Balance_6.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_6.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_7)
  {
    Balance_7.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_7.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_8)
  {
    Balance_8.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_8.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_9)
  {
    Balance_9.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_9.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_10)
  {
    Balance_10.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_10.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_11)
  {
    Balance_11.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_11.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_12)
  {
    Balance_12.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_12.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_13)
  {
    Balance_13.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_13.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_14)
  {
    Balance_14.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_14.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_15)
  {
    Balance_15.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_15.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_16)
  {
    Balance_16.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_16.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_17)
  {
    Balance_17.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_17.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_18)
  {
    Balance_18.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_18.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_19)
  {
    Balance_19.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_19.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_20)
  {
    Balance_20.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_20.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_21)
  {
    Balance_21.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_21.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_22)
  {
    Balance_22.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_22.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_23)
  {
    Balance_23.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_23.get_units(AVG_VALUE), 1;
  }
  else if (sWhich_Balance == BALANCE_24)
  {
    Balance_24.set_scale(gfCalibration_Factor[sWhich_Balance]);
    fWeight += Balance_24.get_units(AVG_VALUE), 1;
  }

  //fWeight = fWeight * 0.035274;

  //  if(sWhich_Balance == BALANCE_1)
  //  {
  //    Serial.println(fWeight);
  //  }

  if (fWeight < 0)
  {
    fWeight = 0;
  }

  return fWeight;
}

float fEEPROM_Read_factor(short sAddr)
{
  float ffactor = 0.0;

  EEPROM.get(sAddr, ffactor);

  return ffactor;
}

void vEEPROM_write_factor(short sAddr, float ffactor)
{
  EEPROM.put(sAddr, ffactor);
}

long lEEPROM_Read_Zerofactor(short sAddr)
{
  long lZerofactor = 0;

  EEPROM.get(sAddr, lZerofactor);

  return lZerofactor;
}

void vEEPROM_write_Zerofactor(short sAddr, long lZerofactor)
{
  EEPROM.put(sAddr, lZerofactor);
}

short sEEPROM_Read(short sAddr)
{
  short sData = 0;

  sData = (short)EEPROM.read(sAddr);

  return sData;
}

void vEEPROM_write(short sAddr, short sData)
{
  EEPROM.write(sAddr, sData);
}

void vCalibrate_Step3(short sWhich_Balance)
{
  unsigned short usExit = 0;
  short sBut = NO_PRESS;
  float fUnits = 0.0;
  short sDelay = 200;
  float fOunces = 0.0;
  short sFactor_Addr[24] = {BALANCE_1_ADDR, BALANCE_2_ADDR, BALANCE_3_ADDR, BALANCE_4_ADDR, BALANCE_5_ADDR, BALANCE_6_ADDR, BALANCE_7_ADDR, BALANCE_8_ADDR, BALANCE_9_ADDR, BALANCE_10_ADDR, BALANCE_11_ADDR, BALANCE_12_ADDR, BALANCE_13_ADDR, BALANCE_14_ADDR, BALANCE_15_ADDR, BALANCE_16_ADDR, BALANCE_17_ADDR, BALANCE_18_ADDR, BALANCE_19_ADDR, BALANCE_20_ADDR, BALANCE_21_ADDR, BALANCE_22_ADDR, BALANCE_23_ADDR, BALANCE_24_ADDR};
  short sStep_Value = 1;
  short sMenu_Index = 0;
  short sWhichSel = -1;

  vStart_Timer();

  do
  {
    fOunces = fGet_Weight(sWhich_Balance);

    gfWeight[sWhich_Balance] = fOunces;//100;

    if (ulCheck_Timer() >= TIMER)
    {
      vStart_Timer();
      gusIsNeedDisp = 1;
    }

    if (gusIsNeedDisp == 1)
    {
      vDisp_Calibration_Factor(sWhich_Balance, sStep_Value, sMenu_Index, sWhichSel);
      gusIsNeedDisp = 0;
    }

    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      gusIsButPress = 0;
      sDelay = 200;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (sBut == BUT_OK)
    {
      if (gusIsButPress == 0)
      {
        if (sMenu_Index == OK_MENU)
        {
          usExit = 1;
        }
        else
        {
          if (sWhichSel == -1)
          {
            sWhichSel = sMenu_Index;
          }
          else
          {
            sWhichSel = -1;
          }
        }
        gusIsButPress = 1;
      }
    }
    else if (sBut == BUT_UP)
    {
      if (sWhichSel == FACTOR)
      {
        gfCalibration_Factor[sWhich_Balance] += sStep_Value;
        delay(sDelay);
        sDelay -= 20;
        if (sDelay < 0)
        {
          sDelay = 0;
        }
      }
      else if (sWhichSel == STEP)
      {
        if (gusIsButPress == 0)
        {
          if (sStep_Value == 1)
          {
            sStep_Value = 10;
          }
          else if (sStep_Value == 10)
          {
            sStep_Value = 100;
          }
          else if (sStep_Value == 100)
          {
            sStep_Value = 1000;
          }
          else
          {
            sStep_Value = 1;
          }
          gusIsButPress = 1;
        }
      }
      else
      {
        if (gusIsButPress == 0)
        {
          sMenu_Index--;
          if (sMenu_Index < FACTOR)
          {
            sMenu_Index = OK_MENU;
          }
          gusIsButPress = 1;
        }
      }
    }
    else if (sBut == BUT_DOWN)
    {
      if (sWhichSel == FACTOR)
      {
        gfCalibration_Factor[sWhich_Balance] -= sStep_Value;
        delay(sDelay);
        sDelay -= 20;
        if (sDelay < 0)
        {
          sDelay = 0;
        }
      }
      else if (sWhichSel == STEP)
      {
        if (gusIsButPress == 0)
        {
          if (sStep_Value == 1000)
          {
            sStep_Value = 100;
          }
          else if (sStep_Value == 100)
          {
            sStep_Value = 10;
          }
          else if (sStep_Value == 10)
          {
            sStep_Value = 1;
          }
          else
          {
            sStep_Value = 1000;
          }
          gusIsButPress = 1;
        }
      }
      else
      {
        if (gusIsButPress == 0)
        {
          sMenu_Index++;
          if (sMenu_Index > OK_MENU)
          {
            sMenu_Index = FACTOR;
          }
          gusIsButPress = 1;
        }
      }
    }

  } while (usExit == 0);

  for (short i = 0; i < 24; i++)
  {
    vEEPROM_write_factor(sFactor_Addr[i], gfCalibration_Factor[i]);
  }

  gusIsNeedDisp = 1;
}

void vCalibrate_Step2(short sWhich_Balance)
{
  unsigned short usExit = 0;
  short sBut = NO_PRESS;

  vDisp_Calibrate(1 , sWhich_Balance);

  do
  {
    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      gusIsButPress = 0;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (sBut == BUT_OK)
    {
      if (gusIsButPress == 0)
      {
        usExit = 1;
        gusIsButPress = 1;
      }
    }

  } while (usExit == 0);
}

void vCalibrate_Step1(short sWhich_Balance)
{
  unsigned short usExit = 0;
  short sBut = NO_PRESS;
  short sZero_Addr[24] = {ZERO_1_ADDR, ZERO_2_ADDR, ZERO_3_ADDR, ZERO_4_ADDR, ZERO_5_ADDR, ZERO_6_ADDR, ZERO_7_ADDR, ZERO_8_ADDR, ZERO_9_ADDR, ZERO_10_ADDR, ZERO_11_ADDR, ZERO_12_ADDR, ZERO_13_ADDR, ZERO_14_ADDR, ZERO_15_ADDR, ZERO_16_ADDR, ZERO_17_ADDR, ZERO_18_ADDR, ZERO_19_ADDR, ZERO_20_ADDR, ZERO_21_ADDR, ZERO_22_ADDR, ZERO_23_ADDR, ZERO_24_ADDR};

  vDisp_Calibrate(0 , sWhich_Balance);

  do
  {
    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      gusIsButPress = 0;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (sBut == BUT_OK)
    {
      if (gusIsButPress == 0)
      {
        usExit = 1;
        gusIsButPress = 1;
      }
    }

  } while (usExit == 0);

  if (sWhich_Balance == BALANCE_1)
  {
    Balance_1.set_scale();
    Balance_1.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_1.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_2)
  {
    Balance_2.set_scale();
    Balance_2.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_2.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_3)
  {
    Balance_3.set_scale();
    Balance_3.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_3.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_4)
  {
    Balance_4.set_scale();
    Balance_4.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_4.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_5)
  {
    Balance_5.set_scale();
    Balance_5.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_5.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_6)
  {
    Balance_6.set_scale();
    Balance_6.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_6.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_7)
  {
    Balance_7.set_scale();
    Balance_7.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_7.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_8)
  {
    Balance_8.set_scale();
    Balance_8.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_8.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_9)
  {
    Balance_9.set_scale();
    Balance_9.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_9.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_10)
  {
    Balance_10.set_scale();
    Balance_10.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_10.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_11)
  {
    Balance_11.set_scale();
    Balance_11.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_11.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_12)
  {
    Balance_12.set_scale();
    Balance_12.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_12.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_13)
  {
    Balance_13.set_scale();
    Balance_13.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_13.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_14)
  {
    Balance_14.set_scale();
    Balance_14.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_14.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_15)
  {
    Balance_15.set_scale();
    Balance_15.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_15.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_16)
  {
    Balance_16.set_scale();
    Balance_16.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_16.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_17)
  {
    Balance_17.set_scale();
    Balance_17.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_17.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_18)
  {
    Balance_18.set_scale();
    Balance_18.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_18.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_19)
  {
    Balance_19.set_scale();
    Balance_19.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_19.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_20)
  {
    Balance_20.set_scale();
    Balance_20.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_20.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_21)
  {
    Balance_21.set_scale();
    Balance_21.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_21.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_22)
  {
    Balance_22.set_scale();
    Balance_22.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_22.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_23)
  {
    Balance_23.set_scale();
    Balance_23.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_23.read_average(); //Get a baseline reading
  }
  else if (sWhich_Balance == BALANCE_24)
  {
    Balance_24.set_scale();
    Balance_24.tare();  //Reset the scale to 0
    glZero_factor[sWhich_Balance] = Balance_24.read_average(); //Get a baseline reading
  }

  vEEPROM_write_Zerofactor(sZero_Addr[sWhich_Balance], glZero_factor[sWhich_Balance]);
}

void vTare(void)
{
  unsigned short usExit = 0;
  short sWhich_Balance = 0;
  short sBut = NO_PRESS;
  short sZero_Addr[24] = {ZERO_1_ADDR, ZERO_2_ADDR, ZERO_3_ADDR, ZERO_4_ADDR, ZERO_5_ADDR, ZERO_6_ADDR, ZERO_7_ADDR, ZERO_8_ADDR, ZERO_9_ADDR, ZERO_10_ADDR, ZERO_11_ADDR, ZERO_12_ADDR, ZERO_13_ADDR, ZERO_14_ADDR, ZERO_15_ADDR, ZERO_16_ADDR, ZERO_17_ADDR, ZERO_18_ADDR, ZERO_19_ADDR, ZERO_20_ADDR, ZERO_21_ADDR, ZERO_22_ADDR, ZERO_23_ADDR, ZERO_24_ADDR};

  vStart_Timer();

  do
  {
    for (short i = 0; i < 23; i++)
    {
      gfWeight[i] = fGet_Weight(i);
    }

    sBut = sRead_Button();

    if (sBut == NO_PRESS)
    {
      gusIsButPress = 0;
    }
    else
    {
      if ((gusIsButPress == 0) && (gusIsNeedON_BL == 0))
      {
        gusIsButPress = 1;
      }

      gusIsNeedON_BL = 1;
      vLDC_BlackLight_Control(ON);
      gulTimer_Count = 0;
      vEn_Timer2Interrupt();
    }

    if (gulTimer_Count >= LCD_BLACK_LIGHT_TIMER)
    {
      gusIsNeedON_BL = 0;
      vLDC_BlackLight_Control(OFF);
      gulTimer_Count = 0;
    }

    if (sBut == BUT_OK)
    {
      if (gusIsButPress == 0)
      {
        if (sWhich_Balance < BALANCE_5)
        {
          lcd1.setCursor(0, sWhich_Balance);
          lcd1.print("*");
        }
        else if (sWhich_Balance <= BALANCE_8)
        {
          lcd1.setCursor(0, (sWhich_Balance - 4));
          lcd1.print("*");
        }
        else if (sWhich_Balance < BALANCE_13)
        {
          lcd.setCursor(0, (sWhich_Balance - 8));
          lcd.print("*");
        }
        else if (sWhich_Balance <= BALANCE_16)
        {
          lcd.setCursor(0, (sWhich_Balance - 12));
          lcd.print("*");
        }
        else if (sWhich_Balance <= BALANCE_20)
        {
          lcd2.setCursor(0, (sWhich_Balance - 16));
          lcd2.print("*");
        }
        else if (sWhich_Balance <= BALANCE_24)
        {
          lcd2.setCursor(0, (sWhich_Balance - 20));
          lcd2.print("*");
        }

        if (sWhich_Balance == BALANCE_1)
        {
          Balance_1.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_1.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_1.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_2)
        {
          Balance_2.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_2.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_2.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_3)
        {
          Balance_3.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_3.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_3.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_4)
        {
          Balance_4.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_4.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_4.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_5)
        {
          Balance_5.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_5.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_5.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_6)
        {
          Balance_6.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_6.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_6.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_7)
        {
          Balance_7.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_7.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_7.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_8)
        {
          Balance_8.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_8.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_8.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_9)
        {
          Balance_9.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_9.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_9.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_10)
        {
          Balance_10.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_10.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_10.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_11)
        {
          Balance_11.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_11.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_11.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_12)
        {
          Balance_12.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_12.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_12.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_13)
        {
          Balance_13.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_13.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_13.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_14)
        {
          Balance_14.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_14.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_14.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_15)
        {
          Balance_15.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_15.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_15.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_16)
        {
          Balance_16.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_16.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_16.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_17)
        {
          Balance_17.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_17.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_17.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_18)
        {
          Balance_18.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_18.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_18.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_19)
        {
          Balance_19.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_19.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_19.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_20)
        {
          Balance_20.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_20.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_20.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_21)
        {
          Balance_21.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_21.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_21.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_22)
        {
          Balance_22.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_22.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_22.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_23)
        {
          Balance_23.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_23.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_23.read_average(); //Get a baseline reading
          delay(200);
        }
        else if (sWhich_Balance == BALANCE_24)
        {
          Balance_24.set_scale(gfCalibration_Factor[sWhich_Balance]);
          Balance_24.tare();  //Reset the scale to 0
          glZero_factor[sWhich_Balance] = Balance_24.read_average(); //Get a baseline reading
          delay(200);
        }
        else
        {
          usExit = 1;
        }

        vEEPROM_write_Zerofactor(sZero_Addr[sWhich_Balance], glZero_factor[sWhich_Balance]);

        gusIsNeedDisp = 1;
        gusIsButPress = 1;
      }
    }
    else if (sBut == BUT_UP)
    {
      if (gusIsButPress == 0)
      {
        sWhich_Balance--;
        if (sWhich_Balance < BALANCE_1)
        {
          sWhich_Balance = BACK;
        }
        gusIsNeedDisp = 1;
        gusIsButPress = 1;
      }
    }
    else if (sBut == BUT_DOWN)
    {
      if (gusIsButPress == 0)
      {
        sWhich_Balance++;
        if (sWhich_Balance > BACK)
        {
          sWhich_Balance = BALANCE_1;
        }
        gusIsNeedDisp = 1;
        gusIsButPress = 1;
      }
    }

    if (ulCheck_Timer() >= TIMER)
    {
      vStart_Timer();
      gusIsNeedDisp = 1;
    }

    if (gusIsNeedDisp == 1)
    {
      vLCD_Disp_Balance_Tare(sWhich_Balance);
      gusIsNeedDisp = 0;
    }

  } while (usExit == 0);

  gusIsNeedDisp = 1;
}

void vDisp_Calibrate(short sIndex, short sWhich_Balance)
{
  char szTemp[20];

  memset(szTemp, '\0', sizeof(szTemp));

  lcd.clear();
  /**/


  if (sIndex == 0)
  {
    lcd.print("Remove all weight");
    lcd.setCursor(0, 1);
    sprintf(szTemp, "from Balance %d...", sWhich_Balance + 1);
    lcd.print(szTemp);
    lcd.setCursor(0, 2);
    lcd.print("Press [OK] to");
    lcd.setCursor(0, 3);
    lcd.print("continue.");
  }
  else if (sIndex == 1)
  {
    lcd.print("Place known weight");
    lcd.setCursor(0, 1);
    sprintf(szTemp, "on Balance %d...", sWhich_Balance + 1);
    lcd.print(szTemp);
    lcd.setCursor(0, 2);
    lcd.print("Press [OK] to");
    lcd.setCursor(0, 3);
    lcd.print("continue.");
  }
}

void vDisp_Calibration_Factor(short sWhich_Balance, short sStep_Value , short sMenu_Index, short sWhichSel)
{
  lcd.clear();
  /**/
  lcd.print(gfWeight[sWhich_Balance]);
  lcd.setCursor(18, 0);
  lcd.print("KG");
  lcd.setCursor(1, 1);
  lcd.write(byte(ARROW_UP));
  lcd.write(byte(ARROW_DOWN));
  lcd.print("Fac: ");
  lcd.print(gfCalibration_Factor[sWhich_Balance]);
  lcd.setCursor(1, 2);
  lcd.print("Step : ");
  lcd.print(sStep_Value);
  lcd.setCursor(1, 3);
  lcd.print("OK ");

  if (sMenu_Index == FACTOR)
  {
    lcd.setCursor(0, 1);

    if (sWhichSel == FACTOR)
    {
      lcd.print("*");
    }
    else
    {
      lcd.write(byte(ARROW_SYMBOL));
    }
  }
  if (sMenu_Index == STEP)
  {
    lcd.setCursor(0, 2);

    if (sWhichSel == STEP)
    {
      lcd.print("*");
    }
    else
    {
      lcd.write(byte(ARROW_SYMBOL));
    }
  }
  else if (sMenu_Index == OK_MENU)
  {
    lcd.setCursor(0, 3);
    lcd.write(byte(ARROW_SYMBOL));
  }
}

void vLCD_Disp_Balance_Tare(short sWhich_Balance)
{

  lcd.clear();
  lcd1.clear();
  lcd2.clear();

  if (sWhich_Balance <= BALANCE_4)
  {
    lcd1.setCursor(1, 0);
    lcd1.print("BAL-1: ");
    lcd1.setCursor(1, 1);
    lcd1.print("BAL-2: ");
    lcd1.setCursor(1, 2);
    lcd1.print("BAL-3: ");
    lcd1.setCursor(1, 3);
    lcd1.print("BAL-4: ");

    lcd1.setCursor(8, 0);
    lcd1.print(gfWeight[0]);
    lcd1.setCursor(18, 0);
    lcd1.print("KG");

    lcd1.setCursor(8, 1);
    lcd1.print(gfWeight[1]);
    lcd1.setCursor(18, 1);
    lcd1.print("KG");

    lcd1.setCursor(8, 2);
    lcd1.print(gfWeight[2]);
    lcd1.setCursor(18, 2);
    lcd1.print("KG");

    lcd1.setCursor(8, 3);
    lcd1.print(gfWeight[3]);
    lcd1.setCursor(18, 3);
    lcd1.print("KG");

    lcd1.setCursor(0, sWhich_Balance);
    lcd1.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance <= BALANCE_8)
  {
    lcd1.setCursor(1, 0);
    lcd1.print("BAL-5: ");
    lcd1.setCursor(1, 1);
    lcd1.print("BAL-6: ");
    lcd1.setCursor(1, 2);
    lcd1.print("BAL-7: ");
    lcd1.setCursor(1, 3);
    lcd1.print("BAL-8: ");

    lcd1.setCursor(8, 0);
    lcd1.print(gfWeight[4]);
    lcd1.setCursor(18, 0);
    lcd1.print("KG");

    lcd1.setCursor(8, 1);
    lcd1.print(gfWeight[5]);
    lcd1.setCursor(18, 1);
    lcd1.print("KG");

    lcd1.setCursor(8, 2);
    lcd1.print(gfWeight[6]);
    lcd1.setCursor(18, 2);
    lcd1.print("KG");

    lcd1.setCursor(8, 3);
    lcd1.print(gfWeight[7]);
    lcd1.setCursor(18, 3);
    lcd1.print("KG");

    lcd1.setCursor(0, (sWhich_Balance - 4));
    lcd1.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance <= BALANCE_12)
  {
    lcd.setCursor(1, 0);
    lcd.print("BAL-9: ");
    lcd.setCursor(1, 1);
    lcd.print("BAL-10: ");
    lcd.setCursor(1, 2);
    lcd.print("BAL-11: ");
    lcd.setCursor(1, 3);
    lcd.print("BAL-12: ");

    lcd.setCursor(8, 0);
    lcd.print(gfWeight[8]);
    lcd.setCursor(18, 0);
    lcd.print("KG");

    lcd.setCursor(8, 1);
    lcd.print(gfWeight[9]);
    lcd.setCursor(18, 1);
    lcd.print("KG");

    lcd.setCursor(8, 2);
    lcd.print(gfWeight[10]);
    lcd.setCursor(18, 2);
    lcd.print("KG");

    lcd.setCursor(8, 3);
    lcd.print(gfWeight[11]);
    lcd.setCursor(18, 3);
    lcd.print("KG");

    lcd.setCursor(0, (sWhich_Balance - 8));
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance <= BALANCE_16)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("BAL-13: ");
    lcd.setCursor(1, 1);
    lcd.print("BAL-14: ");
    lcd.setCursor(1, 2);
    lcd.print("BAL-15: ");
    lcd.setCursor(1, 3);
    lcd.print("BAL-16: ");

    lcd.setCursor(8, 0);
    lcd.print(gfWeight[12]);
    lcd.setCursor(18, 0);
    lcd.print("KG");

    lcd.setCursor(8, 1);
    lcd.print(gfWeight[13]);
    lcd.setCursor(18, 1);
    lcd.print("KG");

    lcd.setCursor(8, 2);
    lcd.print(gfWeight[14]);
    lcd.setCursor(18, 2);
    lcd.print("KG");

    lcd.setCursor(8, 3);
    lcd.print(gfWeight[15]);
    lcd.setCursor(18, 3);
    lcd.print("KG");

    lcd.setCursor(0, (sWhich_Balance - 12));
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance <= BALANCE_20)
  {
    lcd2.clear();
    lcd2.setCursor(1, 0);
    lcd2.print("BAL-17: ");
    lcd2.setCursor(1, 1);
    lcd2.print("BAL-18: ");
    lcd2.setCursor(1, 2);
    lcd2.print("BAL-19: ");
    lcd2.setCursor(1, 3);
    lcd2.print("BAL-20: ");

    lcd2.setCursor(8, 0);
    lcd2.print(gfWeight[16]);
    lcd2.setCursor(18, 0);
    lcd2.print("KG");

    lcd2.setCursor(8, 1);
    lcd2.print(gfWeight[17]);
    lcd2.setCursor(18, 1);
    lcd2.print("KG");

    lcd2.setCursor(8, 2);
    lcd2.print(gfWeight[18]);
    lcd2.setCursor(18, 2);
    lcd2.print("KG");

    lcd2.setCursor(8, 3);
    lcd2.print(gfWeight[19]);
    lcd2.setCursor(18, 3);
    lcd2.print("KG");

    lcd2.setCursor(0, (sWhich_Balance - 16));
    lcd2.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance <= BALANCE_24)
  {
    lcd2.clear();
    lcd2.setCursor(1, 0);
    lcd2.print("BAL-21: ");
    lcd2.setCursor(1, 1);
    lcd2.print("BAL-22: ");
    lcd2.setCursor(1, 2);
    lcd2.print("BAL-23: ");
    lcd2.setCursor(1, 3);
    lcd2.print("BAL-24: ");

    lcd2.setCursor(8, 0);
    lcd2.print(gfWeight[20]);
    lcd2.setCursor(18, 0);
    lcd2.print("KG");

    lcd2.setCursor(8, 1);
    lcd2.print(gfWeight[21]);
    lcd2.setCursor(18, 1);
    lcd2.print("KG");

    lcd2.setCursor(8, 2);
    lcd2.print(gfWeight[22]);
    lcd2.setCursor(18, 2);
    lcd2.print("KG");

    lcd2.setCursor(8, 3);
    lcd2.print(gfWeight[23]);
    lcd2.setCursor(18, 3);
    lcd2.print("KG");

    lcd2.setCursor(0, (sWhich_Balance - 20));
    lcd2.write(byte(ARROW_SYMBOL));
  }

  else
  {
    lcd.setCursor(1, 0);
    lcd.print("Back");
    lcd.setCursor(0, 0);
    lcd.write(byte(ARROW_SYMBOL));
  }
}

void vLCD_Disp_Which_Balance(short sWhich_Balance)
{
  lcd1.clear();
  lcd.clear();
  lcd2.clear();

  lcd1.setCursor(1, 0);
  lcd1.print("BAL-1");
  lcd1.setCursor(1, 1);
  lcd1.print("BAL-2");
  lcd1.setCursor(1, 2);
  lcd1.print("BAL-3");
  lcd1.setCursor(1, 3);
  lcd1.print("BAL-4");

  lcd1.setCursor(8, 0);
  lcd1.print("BAL-5");
  lcd1.setCursor(8, 1);
  lcd1.print("BAL-6");
  lcd1.setCursor(8, 2);
  lcd1.print("BAL-7");
  lcd1.setCursor(8, 3);
  lcd1.print("BAL-8");

  lcd.setCursor(1, 0);
  lcd.print("BAL-9");
  lcd.setCursor(1, 1);
  lcd.print("BAL-10");
  lcd.setCursor(1, 2);
  lcd.print("BAL-11");
  lcd.setCursor(1, 3);
  lcd.print("BAL-12");

  lcd.setCursor(8, 0);
  lcd.print("BAL-13");
  lcd.setCursor(8, 1);
  lcd.print("BAL-14");
  lcd.setCursor(8, 2);
  lcd.print("BAL-15");
  lcd.setCursor(8, 3);
  lcd.print("BAL-16");

  lcd2.setCursor(1, 0);
  lcd2.print("BAL-17");
  lcd2.setCursor(1, 1);
  lcd2.print("BAL-18");
  lcd2.setCursor(1, 2);
  lcd2.print("BAL-19");
  lcd2.setCursor(1, 3);
  lcd2.print("BAL-20");

  lcd2.setCursor(8, 0);
  lcd2.print("BAL-21");
  lcd2.setCursor(8, 1);
  lcd2.print("BAL-22");
  lcd2.setCursor(8, 2);
  lcd2.print("BAL-23");
  lcd2.setCursor(8, 3);
  lcd2.print("BAL-24");

  lcd2.setCursor(15, 0);
  lcd2.print("Back");

  if (sWhich_Balance < 4)
  {
    lcd1.setCursor(0, sWhich_Balance);
    lcd1.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance < 8)
  {
    lcd1.setCursor(7, (sWhich_Balance - 4));
    lcd1.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance < 12)
  {
    lcd.setCursor(0, (sWhich_Balance - 8));
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance < 16)
  {
    lcd.setCursor(7, (sWhich_Balance - 12));
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance < 20)
  {
    lcd2.setCursor(0, (sWhich_Balance - 16));
    lcd2.write(byte(ARROW_SYMBOL));
  }
  else if (sWhich_Balance < 24)
  {
    lcd2.setCursor(7, (sWhich_Balance - 20));
    lcd2.write(byte(ARROW_SYMBOL));
  }
  else
  {
    lcd2.setCursor(14, 0);
    lcd2.write(byte(ARROW_SYMBOL));
  }
}

void vLCD_Disp_Menu(short sW_Menu)
{
  /**/
  vLCD_Standby();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Start");
  lcd.setCursor(1, 1);
  lcd.print("Calibration");
  lcd.setCursor(1, 2);
  lcd.print("Tare");
  lcd.setCursor(1, 3);
  lcd.print("Trigger Setting");

  if (sW_Menu == START)
  {
    lcd.setCursor(0, 0);
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sW_Menu == CALIBRATION)
  {
    lcd.setCursor(0, 1);
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sW_Menu == TARE)
  {
    lcd.setCursor(0, 2);
    lcd.write(byte(ARROW_SYMBOL));
  }
  else if (sW_Menu == TRIGGER_SETTING)
  {
    lcd.setCursor(0, 3);
    lcd.write(byte(ARROW_SYMBOL));
  }
}

short sRead_Button(void)
{
  short sWhich_But[3] = {BUT_UP, BUT_DOWN, BUT_OK};

  for (short i = 0; i < 3; i++)
  {
    if (usRead_But(sWhich_But[i]) == 1)
    {
      //delay(10);
      for (short i = 0; i < 3; i++)
      {
        if (usRead_But(sWhich_But[i]) == 1)
        {
          return sWhich_But[i];
        }
      }
    }
  }
  return NO_PRESS;
}

unsigned short usRead_But(short sWhich_But)
{

  if (digitalRead(sWhich_But) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  
}

void vStart_Timer(void)
{
  gulStart_Time = millis();
}

unsigned long ulCheck_Timer(void)
{
  unsigned long ulTime = 0;
  ulTime = millis() - gulStart_Time;
  return ulTime;
}

void vLDC_BlackLight_Control(short sOn_Off)
{
  if (sOn_Off == ON)
  {
    lcd.setBacklight(HIGH);
    lcd1.setBacklight(HIGH);
    lcd2.setBacklight(HIGH);
  }
  else
  {
    lcd.setBacklight(LOW);
    lcd1.setBacklight(LOW);
    lcd2.setBacklight(LOW);
  }
}

//Timer2 Overflow Interrupt Vector, called every 1ms
ISR(TIMER2_OVF_vect)
{
  vDis_Timer2Interrupt();

  gulTimer_Count++;
  /*if(gulTimer_Count >= 2000)
    {
    gusIsNeedON_BL = 0;
    vLDC_BlackLight_Control(OFF);
    gulTimer_Count = 0;
    }*/

  vEn_Timer2Interrupt();
}

//enable interrupt timer 2
void vEn_Timer2Interrupt(void)
{
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Normal port operation, Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
}

//disable interrupt timer 2
void vDis_Timer2Interrupt(void)
{
  TCCR2B = 0x00;        //Disable Timer2 while we set it up
}
