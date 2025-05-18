// FILE: DUAL22.h
// VERSION: v20230112
// PURPOSE: DUAL22 library for ESP32
// Co., : yesio.net/歐利科技有限公司
// URL: https://yesio.net/
// Author: Nick

#ifndef e32DUAL_h

#define e32DUAL_h
#include "Arduino.h"

	/*---電池電壓感測 / Battery Voltage---*/
	const int Battery_Pin = 36;


	/*---紅外線循線感測 / IR Road Following---*/
	const int IR_1 = 34;
	const int IR_2 = 35;
	const int IR_3 = 39;

	const int TCRT_R = IR_3;
	const int TCRT_M = IR_2;
	const int TCRT_L = IR_1;
	
	const int DEBUG = 1;


	/*---超音波 / Ultrasonic---*/
	const byte TrigPin_R = 4;      // 超音波模組R的觸發腳
	const int EchoPin_R = 5;       // 超音波模組R的接收腳
	const byte TrigPin_L = 18;     // 超音波模組L的觸發腳
	const int EchoPin_L = 19;      // 超音波模組L的接收腳
	const byte TrigPin_F = 27;     // 超音波模組F的觸發腳
	const int EchoPin_F = 13;      // 超音波模組F的接收腳


	/*---蜂鳴器 / Buzzer---*///蜂鳴器暫停使用
	const byte Buzzer_Pin = 15;    // 定義蜂鳴器控制腳位


	/*---馬達相關設定---*/
	const int mCW = 11;
	const int mCCW = 22;
	const int mSTOP = 33;

	const int M1 = 0;
	const int M2 = 1;
	const int M3 = 2;
	const int M4 = 3;

	const int M1A = 14; // 馬達1 A PIN
	const int M1B = 32; // 馬達1 B PIN
	const int M2A = 12; // 馬達2 A PIN
	const int M2B = 33; // 馬達2 B PIN
	
	const int M3A = 25; // 馬達3 A PIN
	const int M3B = 26; // 馬達3 B PIN
	const int M4A = 23; // 馬達4 A PIN
	const int M4B = 22; // 馬達4 B PIN
	
	//Note by Nick, PWM Channel 0-15 in total. 2023/1/12
	const int BUZZER_CHANNEL = 7;  // BUZZER PWM 設定, 
	const int M1A_CH = 8;    //PWM 設定, 馬達1 A PIN PWM 通道
	const int M1B_CH = 9;    //PWM 設定, 馬達1 B PIN PWM 通道
	const int M2A_CH = 10;   //PWM 設定, 馬達2 A PIN PWM 通道
	const int M2B_CH = 11;   //PWM 設定, 馬達2 B PIN PWM 通道
	
	const int M3A_CH = 12;   //PWM 設定, 馬達3 A PIN PWM 通道
	const int M3B_CH = 13;   //PWM 設定, 馬達3 B PIN PWM 通道
	const int M4A_CH = 14;   //PWM 設定, 馬達4 A PIN PWM 通道
	const int M4B_CH = 15;   //PWM 設定, 馬達4 B PIN PWM 通道
	
	const int MFreq = 5000; // for PWM 設定
	const int MRes = 10;    // for PWM 設定

	void MotorSet(int Mx);
	void MotorPWMSet(int Mx);

	/*------------------------------------------------------------
	>  FUNCTION : void DUAL22Inital(), DUAL22初始化
	>  Return : 無回傳值
	>  Parameter Description: 無參數
	--------------------------------------------------------------*/
	void DUAL22Inital();

	/*------------------------------------------------------------
	>  FUNCTION : void mPWM(int Mx, int Dir, int PWMSpeed)
	>  Return : 無回傳值
	>  Parameter Description:
	>    Mx : 馬達接口
	>    Dir: (1) mCW : 順時針 (2) mCCW : 逆時針 (3) mSTOP : 停止
	>    PWMSpeed : 數值 0 ~ 1023, 數值越大、轉速越快。
	--------------------------------------------------------------*/
	void mPWM(int Mx, int Dir, int PWMSpeed);

	/*------------------------------------------------------------
	>  FUNCTION : GoSUMO Motor PWM控制巨集指令
	>  Return : 無回傳值
	>  Parameter Description: [int PWM], 0-1023數值，控制馬達轉速
	--------------------------------------------------------------*/
	void GS_FW(int PWM);    //前進 / Go Forward
	void GS_BW(int PWM);    //後退 / Go Backward
	void GS_LEFT(int PWM);  //左轉 / Go Left
	void GS_RIGHT(int PWM); //右轉 / Go Right
	void GS_STOP();         //停止 / STOP
	
	//麥克納姆---------------------------------------------------
	void GS_RM(int PWM); //右橫移
	void GS_LM(int PWM); //左橫移
	void GS_RF(int PWM); //右前橫移
	void GS_LB(int PWM); //左後橫移
	void GS_LF(int PWM); //左前橫移
	void GS_RB(int PWM); //右後橫移
	/*------------------------------------------------------------
	>  FUNCTION : bool ObjSeeking(String P, int Thresh), 物件是否進入搜尋範圍(Thresh)
	>  Return : bool, 回傳是否有障礙物
	>  Parameter Description:
	>   (1) String P : 偵測方向, "F"->前 / "L"->左 / "R"->右
	>   (2) Thresh：設定偵測範圍，公分(CM)。
	--------------------------------------------------------------*/
	bool ObjSeeking(String P, int Thresh);

	/*------------------------------------------------------------
	>  FUNCTION : float ObjDistance(String P), 物件距離量測
	>  Return : float, 回傳與物件的距離(單位為公分)
	>  Parameter Description:
	>   (1) String P : 偵測方向, "F"->前 / "L"->左 / "R"->右
	--------------------------------------------------------------*/
	float ObjDistance(String P);

	/*------------------------------------------------------------
	>  FUNCTION : unsigned long probing(String P), 物件距離量測 (脈衝微秒時間長度/5.8 = mm)
	>  Reture : unsigned long, 回傳脈衝微秒時間長度
	>  Parameter Description:
	>   (1) String P : 偵測方向, "F"->前 / "L"->左 / "R"->右
	--------------------------------------------------------------*/
	unsigned long probing(String P);

	/*------------------------------------------------------------
	>  FUNCTION : Buzzer_Alarm(unsigned int duration, unsigned int frequency, unsigned int vol), 蜂鳴器警報聲響
	>  Return : 無回傳值
	>  Parameter Description:
	>   (1) duration : 聲音長度，毫秒(ms)
	>   (2) frequency : 聲音頻率
	>   (3) vol : 聲音大小 (0 - 255)
	--------------------------------------------------------------*/
	void Buzzer_Alarm(unsigned int duration, unsigned int frequency, unsigned int vol);

	/*------------------------------------------------------------
	>  FUNCTION : unsigned int Battery_Value(), 電池檢測
	>  Return : [_fVal]回傳電池檢測值
	--------------------------------------------------------------*/
	float Battery_Value();

	/*------------------------------------------------------------
   >  FUNCTION : 三通道IR感測器
   >  Return : [_bTrace]回傳3通道感測結果。MSB：TCRT_L / LSB：TCRT_R
	>  Parameter Description: [TH]感測閥值 (0 - 4095), [DEBUG]0：分析模式關閉 / 1：分析模式開啟
	--------------------------------------------------------------*/
	byte ReadTCRT(int TH, bool DEBUG);
	
	/*---------------------------------------------------------------
	> 三通道IR感測器
	> TH : 輸入感測閥值 (0 - 4095)
	> _bTrace : 回傳3通道感測結果。MSB：TCRT_L / LSB：TCRT_R
	-----------------------------------------------------------------*/
	int signalTCRT(char TCRT);

#endif
//
// END OF FILE
//
