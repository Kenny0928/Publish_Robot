//======================================
//2025-4-27更新
//Modified:(1)buzzer behavior onConnect; (2)marked disconnect reset behavior; (3)Omni R&L TVL inverse issue
//======================================
#include <Ps3Controller.h>
#include "esp_bt.h"      // ESP32 藍牙控制函式庫
#include "esp_system.h"  // ESP32 重新啟動函式
#include <dual2s.h>
#include <Servo.h>

//DC電機GPIO
#define PIN_MOTO_1A  14
#define PIN_MOTO_1B  32
#define PIN_MOTO_2A  12
#define PIN_MOTO_2B  33
#define PIN_MOTO_3A  25
#define PIN_MOTO_3B  26
#define PIN_MOTO_4A  23
#define PIN_MOTO_4B  22

//蜂鳴器GPIO
#define PIN_BUZZER   15

//超音波GPIO
#define PIN_USC_FRONT_ECHO 13
#define PIN_USC_FRONT_TRIG 27
#define PIN_USC_RIGHT_ECHO 5
#define PIN_USC_RIGHT_TRIG 4
#define PIN_USC_LEFT_ECHO 19
#define PIN_USC_LEFT_TRIG 18

//短距紅外線GPIO
#define PIN_IR_LEFT    34
#define PIN_IR_MIDDLE  35
#define PIN_IR_RIGHT   39

//供電偵測GPIO
#define PIN_BATTERY    36

//預設使用的PWM通道
#define PWM_CH_BUZZER 7
#define PWM_CH_M1A    8
#define PWM_CH_M1B    9
#define PWM_CH_M2A    10
#define PWM_CH_M2B    11
#define PWM_CH_M3A    12
#define PWM_CH_M3B    13
#define PWM_CH_M4A    14
#define PWM_CH_M4B    15

GoSUMO gs;
Buzzer bz(PIN_BUZZER, PWM_CH_BUZZER);
IR3CH  ir(PIN_IR_LEFT, PIN_IR_MIDDLE, PIN_IR_RIGHT);

/*-- 宣告區(全域變數) --*/
int SeekCnt = 0;
unsigned long lastRecTime_P3x = 0;  // 記錄上次收到 PS3 訊號的時間
const unsigned long disconnectTimeout = 5000;  // 5 秒未收到資料則判定為斷線
const long interval = 5000;     // 指定間隔時間5秒(5000毫秒)。

int PWM_Speed = 800;

//Servo Motor 變數區 & 機械臂微調區 -------
//重要，詳見影片介紹：https://youtu.be/PnZQBKgZXok
Servo servoJoga;
int homeJoga = 90; //機械臂原點角度。

int posBase = homeJoga;
int posBaseMAX = 170; //機械臂最大抬升角度。
int posBaseMIN = 90;  //機械臂最小降低角度。


void notify() {
    lastRecTime_P3x = millis();  // **每當收到 PS3 搖桿訊號，更新時間**

    // 顯示按鍵狀態
    if (Ps3.data.button.cross){servoJoga.write(140);} //Serial.println("按下：X 按鈕");
    if (Ps3.data.button.circle){  gs.act(GoSUMO::GO_RIGHT, PWM_Speed); delay(10);} //Serial.println("按下：O 按鈕");
    if (Ps3.data.button.triangle) {servoJoga.write(30);} //Serial.println("按下：△ 按鈕");
    if (Ps3.data.button.square){  gs.act(GoSUMO::GO_LEFT, PWM_Speed); delay(10);}  //Serial.println("按下：□ 按鈕");

    if (Ps3.data.button.up) Serial.println("按下：上鍵");
    if (Ps3.data.button.down) Serial.println("按下：下鍵");
    if (Ps3.data.button.left) Serial.println("按下：左鍵");
    if (Ps3.data.button.right) Serial.println("按下：右鍵");

    if (Ps3.data.button.l1) Serial.println("按下：L1 按鈕");
    if (Ps3.data.button.l2) Serial.println("按下：L2 按鈕");
    if (Ps3.data.button.r1) {servoJoga.write(90);} //Serial.println("按下：R1 按鈕");
    if (Ps3.data.button.r2) Serial.println("按下：R2 按鈕");

    if (Ps3.data.button.select) Serial.println("按下：SELECT 按鈕");
    if (Ps3.data.button.start) Serial.println("按下：START 按鈕");
    if (Ps3.data.button.ps) Serial.println("按下：PS 按鈕");

    // 顯示搖桿數值
    Serial.printf("左搖桿 X: %d, Y: %d\n", Ps3.data.analog.stick.lx, Ps3.data.analog.stick.ly);
    Serial.printf("右搖桿 X: %d, Y: %d\n", Ps3.data.analog.stick.rx, Ps3.data.analog.stick.ry);
    if((Ps3.data.analog.stick.ry <= -5) && (Ps3.data.analog.stick.rx==0)){ gs.act(GoSUMO::FORWARD, map(Ps3.data.analog.stick.ry,-5,-128,100,1023)); }    //前進
    if((Ps3.data.analog.stick.ry <= -5) && (Ps3.data.analog.stick.rx>5)){  gs.act(GoSUMO::RF_TVL, map(Ps3.data.analog.stick.rx,5,127,100,1023)); }       //右前橫移
    if((Ps3.data.analog.stick.ry <= -5) && (Ps3.data.analog.stick.rx<-5)){ gs.act(GoSUMO::LF_TVL, map(Ps3.data.analog.stick.rx,-5,-128,100,1023));}      //左前橫移

    if((Ps3.data.analog.stick.ry >=  5) && (Ps3.data.analog.stick.rx==0)){ gs.act(GoSUMO::BACKWARD, map(Ps3.data.analog.stick.ry,5,127,100,1023)); }     //後退
    if((Ps3.data.analog.stick.ry >=  5) && (Ps3.data.analog.stick.rx>5)){ gs.act(GoSUMO::RB_TVL, map(Ps3.data.analog.stick.rx,5,127,100,1023)); }        //右後橫移
    if((Ps3.data.analog.stick.ry >=  5) && (Ps3.data.analog.stick.rx<-5)){ gs.act(GoSUMO::LB_TVL, map(Ps3.data.analog.stick.rx,-5,-128,100,1023));}      //左後橫移

    if((Ps3.data.analog.stick.rx >=  5) && (Ps3.data.analog.stick.ry==0)){ gs.act(GoSUMO::R_TVL, map(Ps3.data.analog.stick.rx,5,127,100,1023)); }        //右橫移
    if((Ps3.data.analog.stick.rx <= -5) && (Ps3.data.analog.stick.ry==0)){ gs.act(GoSUMO::L_TVL, map(Ps3.data.analog.stick.rx,-5,-128,100,1023));}       //左橫移  
    
    if((Ps3.data.analog.stick.rx > -5 && Ps3.data.analog.stick.rx < 5) && (Ps3.data.analog.stick.ry > -5 && Ps3.data.analog.stick.ry < 5)) { gs.stop(); }
}

void onConnect() {
  for(int i=0; i<3; i++){ bz.alarm(); delay(200);}

  Serial.println("PS3 控制器已連接！");
  lastRecTime_P3x = millis();  // **成功連線時，記錄時間**
}

void setup() {
  Serial.begin(115200);
  bz.alarm();

  Ps3.attach(notify);              // 設定按鍵變更時的回呼函式
  Ps3.attachOnConnect(onConnect);  // 設定成功連線時的回呼函式
  Ps3.begin("20:00:00:00:43:85");  // 啟動 P3x搖桿的藍牙位址

  //ServoMotor初始化
  servoJoga.attach(18); //舉臂總成、機器鏟
  servoJoga.write(90);  //舉臂總成
}

void loop() {
    // **檢查是否超過 5 秒未收到資料**
    /*if (millis() - lastRecTime_P3x > disconnectTimeout) {
        Serial.println("⚠️ PS3 斷線超過 5 秒，重新啟動 ESP32...");
        delay(1000);
        esp_restart();  // **重新啟動 ESP32**
    }*/

    //delay(1000);  // 每秒檢查一次
}
