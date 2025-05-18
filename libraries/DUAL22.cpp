// FILE: DUAL22.cpp
// VERSION: v20230112
// PURPOSE: DUAL22 library for ESP32
// Co., : yesio.net/歐利科技有限公司
// URL: https://yesio.net/
// Author: Nick

#include "DUAL22.h"
#include "Arduino.h"

void DUAL22Inital(){
  Serial.begin(115200);        // 設定序列傳輸速率為115200
  pinMode(TrigPin_R, OUTPUT);  // 觸發腳R設定為「輸出」
  pinMode(EchoPin_R, INPUT);   // 接收腳R設定為「輸入」
  pinMode(TrigPin_L, OUTPUT);  // 觸發腳L設定為「輸出」
  pinMode(EchoPin_L, INPUT);   // 接收腳L設定為「輸入」
  pinMode(TrigPin_F, OUTPUT);  // 觸發腳F設定為「輸出」
  pinMode(EchoPin_F, INPUT);   // 接收腳F設定為「輸入」
  //pinMode(Buzzer_Pin, OUTPUT); // 設定蜂鳴器控制腳為輸出模式
  pinMode(Battery_Pin, INPUT); // 設定電池電壓檢測腳為輸入模式
  
  analogSetAttenuation(ADC_11db); //衰減值3.6v：ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  analogSetWidth(12); //解析度10bits：9, 10, 11, 12
  
  MotorSet(M1); //Motor1腳位模式設定
  MotorSet(M2); //Motor2腳位模式設定
  MotorSet(M3); //Motor1腳位模式設定
  MotorSet(M4); //Motor2腳位模式設定
  
  MotorPWMSet(M1); //Motor1 PWM設定
  MotorPWMSet(M2); //Motor1 PWM設定
  MotorPWMSet(M3); //Motor1 PWM設定
  MotorPWMSet(M4); //Motor1 PWM設定
}

void MotorSet(int Mx){
  if(Mx==M1) {pinMode(M1A, OUTPUT); pinMode(M1B, OUTPUT);}
  else if(Mx==M2) {pinMode(M2A, OUTPUT); pinMode(M2B, OUTPUT);}
  else if(Mx==M3) {pinMode(M3A, OUTPUT); pinMode(M3B, OUTPUT);}
  else if(Mx==M4) {pinMode(M4A, OUTPUT); pinMode(M4B, OUTPUT);}
}

void MotorPWMSet(int Mx){ 	
  if (ledcRead(Mx)) {
    log_e("PWM channel %d is already in use", ledcRead(Mx));
    return;
  }
  
  if(Mx==M1) {
    ledcSetup(M1A_CH, MFreq, MRes);
	ledcSetup(M1B_CH, MFreq, MRes);
    ledcAttachPin(M1A, M1A_CH);    
    ledcAttachPin(M1B, M1B_CH);
  }
  else if(Mx==M2) {
    ledcSetup(M2A_CH, MFreq, MRes);
	ledcSetup(M2B_CH, MFreq, MRes);
    ledcAttachPin(M2A, M2A_CH);
    ledcAttachPin(M2B, M2B_CH);
  } 
  else if(Mx==M3) {
    ledcSetup(M3A_CH, MFreq, MRes);
	  ledcSetup(M3B_CH, MFreq, MRes);
    ledcAttachPin(M3A, M3A_CH);
    ledcAttachPin(M3B, M3B_CH);
  }  
  else if(Mx==M4) {
    ledcSetup(M4A_CH, MFreq, MRes);
	ledcSetup(M4B_CH, MFreq, MRes);
    ledcAttachPin(M4A, M4A_CH);
    ledcAttachPin(M4B, M4B_CH);
  }  
}


void mPWM(int Mx, int Dir, int PWMSpeed){

  if(Mx==M1) {
    if(Dir == mCW)       {ledcWrite(M1A_CH, PWMSpeed); ledcWrite(M1B_CH, 0);}
    else if(Dir == mCCW) {ledcWrite(M1A_CH, 0); ledcWrite(M1B_CH, PWMSpeed);}
    else if(Dir == mSTOP) {ledcWrite(M1A_CH, 0); ledcWrite(M1B_CH, 0);}
  }
  else if(Mx==M2) {
    if(Dir == mCW) {ledcWrite(M2A_CH, PWMSpeed); ledcWrite(M2B_CH, 0);}
    else if(Dir == mCCW) {ledcWrite(M2A_CH, 0); ledcWrite(M2B_CH, PWMSpeed);}
    else if(Dir == mSTOP) {ledcWrite(M2A_CH, 0); ledcWrite(M2B_CH, 0);}
  }
  else if(Mx==M3) {
    if(Dir == mCW) {ledcWrite(M3A_CH, PWMSpeed); ledcWrite(M3B_CH, 0);}
    else if(Dir == mCCW) {ledcWrite(M3A_CH, 0); ledcWrite(M3B_CH, PWMSpeed);}
    else if(Dir == mSTOP) {ledcWrite(M3A_CH, 0); ledcWrite(M3B_CH, 0);}
  } 
  else if(Mx==M4) {
    if(Dir == mCW) {ledcWrite(M4A_CH, PWMSpeed); ledcWrite(M4B_CH, 0);}
    else if(Dir == mCCW) {ledcWrite(M4A_CH, 0); ledcWrite(M4B_CH, PWMSpeed);}
    else if(Dir == mSTOP) {ledcWrite(M4A_CH, 0); ledcWrite(M4B_CH, 0);}
  }  
}


/*---------------------------------------------------------------
>  FUNCTION : GoSUMO Motor PWM控制巨集指令
>  Reture : 無參數
>  Parameter Description: [int PWM], 0-1023數值，控制馬達轉速
-----------------------------------------------------------------*/
//-- 前進 --
void GS_FW(int PWM){
  mPWM(M1, mCCW, PWM);
  mPWM(M3, mCCW, PWM);
  mPWM(M2, mCW, PWM);
  mPWM(M4, mCW, PWM);
}

//-- 後退 --
void GS_BW(int PWM){
  mPWM(M1, mCW, PWM);
  mPWM(M3, mCW, PWM);
  mPWM(M2, mCCW, PWM);
  mPWM(M4, mCCW, PWM);
}

//-- 左旋轉 --
void GS_LEFT(int PWM){
  mPWM(M1, mCW, PWM);
  mPWM(M3, mCW, PWM);
  mPWM(M2, mCW, PWM);
  mPWM(M4, mCW, PWM);
}

//-- 右旋轉 --
void GS_RIGHT(int PWM){
  mPWM(M1, mCCW, PWM);
  mPWM(M3, mCCW, PWM);
  mPWM(M2, mCCW, PWM);
  mPWM(M4, mCCW, PWM);
}

//-- 停止 --
void GS_STOP(){
  mPWM(M1, mSTOP, 0);
  mPWM(M2, mSTOP, 0);
  mPWM(M3, mSTOP, 0);
  mPWM(M4, mSTOP, 0);
}

//-- 右橫移 --
void GS_RM(int PWM){
  mPWM(M1, mCW, PWM);
  mPWM(M3, mCCW, PWM);
  mPWM(M2, mCW, PWM);
  mPWM(M4, mCCW, PWM);
}

//-- 左橫移 --
void GS_LM(int PWM){
  mPWM(M1, mCCW, PWM);
  mPWM(M3, mCW, PWM);
  mPWM(M2, mCCW, PWM);
  mPWM(M4, mCW, PWM);
}

//-- 右前橫移 --
void GS_RF(int PWM){
  //mPWM(M1, mCW, PWM);
  mPWM(M3, mCCW, PWM);
  mPWM(M2, mCW, PWM);
  //mPWM(M4, mCCW, PWM);
}

//-- 左後橫移 --
void GS_LB(int PWM){
  //mPWM(M1, mCW, PWM);
  mPWM(M3, mCW, PWM);
  mPWM(M2, mCCW, PWM);
  //mPWM(M4, mCCW, PWM);
}

//-- 左前橫移 --
void GS_LF(int PWM){
  mPWM(M1, mCCW, PWM);
  //mPWM(M3, mCCW, PWM);
  //mPWM(M2, mCW, PWM);
  mPWM(M4, mCW, PWM);
}

//-- 右後橫移 --
void GS_RB(int PWM){
  mPWM(M1, mCW, PWM);
  //mPWM(M3, mCW, PWM);
  //mPWM(M2, mCCW, PWM);
  mPWM(M4, mCCW, PWM);
}

/*---------------------------------------------------------------
>  Ultrasonic
-----------------------------------------------------------------*/
/*---------------------------------------------------------------
>  FUNCTION : bool ObjSeeking(String P, int Thresh), 物件是否進入搜尋範圍(Thresh)
>  Return : bool, 回傳是否有障礙物
>  Paramenter Description:
>   (1) String P : 偵測方向, "F"->前 / "L"->左 / "R"->右
>   (2) int Thresh︰偵測範圍，公分。
-----------------------------------------------------------------*/
bool ObjSeeking(String P, int Thresh){
  int fightThresh = Thresh*58; //單位為公分
  long distance = probing(P); //讀取障礙物的距離
  if(distance < fightThresh) { return 1; } //物體進入範圍
  else { return 0; } //物體不在範圍之內
}

/*---------------------------------------------------------------
>  FUNCTION : float ObjDistance(String P), 物件距離量測 (脈波/58 = cm)
>  Return : float, 回傳與物件的距離(單位為公分)
>  Parameter Description:
>   (1) String P : 偵測方向, "F"->前 / "L"->左 / "R"->右
-----------------------------------------------------------------*/
float ObjDistance(String P){
  float d = 0.0;
  d = probing(P)/58.0; //計算物件的距離

  return d;
}

/*---------------------------------------------------------------
>  FUNCTION : unsigned long probing(), 物件距離量測 (脈波/5.8 = mm)
>  Return : unsigned long, 回傳脈衝微秒時間長度
>  Parameter Description:
>   (1) String P : 偵測方向, "F"->前 / "L"->左 / "R"->右
-----------------------------------------------------------------*/
unsigned long probing(String P){
  byte tPin = TrigPin_F;
  int ePin = EchoPin_F;
  if(P == "R"){
    tPin = TrigPin_R;
    ePin = EchoPin_R;
  }
  else if(P == "L"){
    tPin = TrigPin_L;
    ePin = EchoPin_L;
  }
  else if(P == "F"){
    tPin = TrigPin_F;
    ePin = EchoPin_F;
  }
  digitalWrite(tPin, HIGH);
  delayMicroseconds(5);      //維持trig腳位在高電位5微秒
  digitalWrite(tPin, LOW);

  return pulseIn(ePin, HIGH);
}


/*---------------------------------------------------------------
> 蜂鳴器警報聲響
> FUNCTION : 內部設定, 不開放使用
-----------------------------------------------------------------*/

void noTone(uint8_t pin, uint8_t channel){
  ledcDetachPin(pin);
  ledcWrite(channel, 0);
}

void setTone(uint8_t pin, uint8_t channel, unsigned int frequency, unsigned int duration, unsigned int vol){
  if (ledcRead(channel)) {
    log_e("Tone channel %d is already in use", ledcRead(channel));
    return;
  }
  ledcSetup(channel, frequency, 8);
  ledcAttachPin(pin, channel);
  ledcWrite(channel, vol);
  if (duration) {
    delay(duration);
    noTone(pin, channel);
  }
}

/*---------------------------------------------------------------
> 蜂鳴器警報聲響
> duration : 輸入聲音長度，毫秒(ms)
> frequency : 輸入聲音頻率
> vol : 輸入聲音大小 (0 - 255)
-----------------------------------------------------------------*/
void Buzzer_Alarm(unsigned int duration, unsigned int frequency, unsigned int vol){
  setTone(Buzzer_Pin, BUZZER_CHANNEL, frequency, duration, vol);
}


/*---------------------------------------------------------------
> 電池檢測
> _fVal : 回傳電池檢測值，單位伏特(V)
-----------------------------------------------------------------*/
float Battery_Value(){
  //ADC解析度為12bits (4096階)
  //ADC衰減值為3.6V
  //分壓比為~0.25 (目標端電阻100 / 量測端電阻33)
  float _fVal = (analogRead(Battery_Pin)*0.0009)/0.25;
  return _fVal;
}


/*---------------------------------------------------------------
> 三通道IR感測器
> TH : 輸入感測閥值 (0 - 4095)，尋黑線為例，電工黑膠布，平均感測值大於2000
> _bTrace : 回傳3通道感測結果。MSB：TCRT_L / LSB：TCRT_R
-----------------------------------------------------------------*/
byte ReadTCRT(int TH, bool DEBUG){
  byte _bTrace = 0;
  
  if(analogRead(TCRT_R) > TH) { _bTrace = _bTrace | B001; }
  else { _bTrace = _bTrace & B110; }
  
  if(analogRead(TCRT_M) > TH) { _bTrace = _bTrace | B010; }
  else { _bTrace = _bTrace & B101; }
  
  if(analogRead(TCRT_L) > TH) { _bTrace = _bTrace | B100; }
  else { _bTrace = _bTrace & B011; }
  
  /* 讀取類比數值，並將3CH結果輸出至串列視窗  */
  if (DEBUG){
  Serial.print(analogRead(TCRT_L));
  Serial.print("(LEFT) / ");
  Serial.print(analogRead(TCRT_M));
  Serial.print("(Middle) / ");
  Serial.print(analogRead(TCRT_R));
  Serial.print("(RIGHT)");
  Serial.println(_bTrace);
  Serial.println("----");
  delay(1000);
  }
  /*-------------------------------------------------------------*/
  
  return _bTrace;
}

/*---------------------------------------------------------------
> 三通道IR感測器
> 回傳值：(0 - 4095)
> char TCRT：TCRT = 'R'，讀取右通道(IR3)；TCRT = 'M'，讀取中通道(IR2)；TCRT = 'L'，讀取左通道(IR1)
-----------------------------------------------------------------*/
int signalTCRT(char TCRT){
  int _iTCRT = 0;

  if(TCRT == 'R') { _iTCRT = analogRead(TCRT_R); }
  else if (TCRT == 'M') { _iTCRT = analogRead(TCRT_M); }
  else if (TCRT == 'L') { _iTCRT = analogRead(TCRT_L); }
  
  return _iTCRT;
}