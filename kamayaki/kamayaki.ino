//---FOR DEBUG
//#include <SoftwareSerial.h>
//---FOR DEBUG

#include <MIDI.h>
#include <A5020.h>
#include "kamayaki.h"

//MUXValue
//M0
int currMux0Value[16] = {0};
int prevMux0Value[16] = {0};
int diffAnalogValueMux0;
//M1
int currMux1Value[16] = {0};
int prevMux1Value[16] = {0};
int diffAnalogValueMux1;
//M2
int currMux2Value[16] = {0};
int prevMux2Value[16] = {0};
int diffAnalogValueMux2;

// LEDの表示状態
// 各配列にどのLEDがアサインされているかはdefineで定義
char ledStatus_decka = 0xFF;
char ledStatus_deckb = 0xFF;

//(clock(green), data(white), ncs(purple))   
A5020 jog_a(JOG_A_CLOCK, JOG_A_DATA, JOG_A_NCS);
A5020 jog_b(JOG_B_CLOCK, JOG_B_DATA, JOG_B_NCS);

int pwm_count = 0;

//---FOR DEBUG
//SoftwareSerial mySerial(11, 12, true);
//---FOR DEBUG

void setup() {
  // シフトレジスタ用ピンの初期化
  pinMode(latchPin, OUTPUT);
  ledRenew();

  // MUXコントロール用ピンをOUTPUTにする
  pinMode(CONTROL0, OUTPUT);
  pinMode(CONTROL1, OUTPUT);
  pinMode(CONTROL2, OUTPUT);
  pinMode(CONTROL3, OUTPUT);

  // ファン制御用ピン
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(PWM_PIN, 1);
  delay(300);


  // 光学センサを初期化
  jog_a.reset();
  delay(100);
  jog_b.reset();
  delay(100);

  // MIDIライブラリ初期化
  MIDI.begin(1);
  MIDI.setInputChannel(2);
  MIDI.turnThruOff();
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleControlChange(HandleControlChange);

  // setup完了を光でお知らせ
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0xFF);   
  shiftOut(dataPin, clockPin, 0xFF);
  digitalWrite(latchPin, 1);
  delay(300);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0x00);   
  shiftOut(dataPin, clockPin, 0x00);
  digitalWrite(latchPin, 1);
}

void loop() {
  for (int i=0; i<16; i++){
    // MUXのコントロール(0000～1111)
    digitalWrite(CONTROL0, (i&15)>>3); 
    digitalWrite(CONTROL1, (i&7)>>2);  
    digitalWrite(CONTROL2, (i&3)>>1);  
    digitalWrite(CONTROL3, (i&1));     

    // MUXの値読み込み
    currMux0Value[i] = analogRead(MUX0);
    currMux1Value[i] = analogRead(MUX1);
    currMux2Value[i] = analogRead(MUX2);

    // 前回読み取り値と比較
    diffAnalogValueMux0 = abs(currMux0Value[i] - prevMux0Value[i]);
    diffAnalogValueMux1 = abs(currMux1Value[i] - prevMux1Value[i]);
    diffAnalogValueMux2 = abs(currMux2Value[i] - prevMux2Value[i]);

    // Mux0
    if(currMux0Value[i] != prevMux0Value[i]){
      //デジタルポート(ポートタイプ:0)の場合
      if(portTypeMux0[i] == 0){
        if(diffAnalogValueMux0 >= DIGITAL_THRESHOLD){
          MIDI.sendNoteOn(ifAssignMux0[i], convAD(currMux0Value[i]), CHANNEL_1);
          prevMux0Value[i] = currMux0Value[i];
        }
      }
      else if(portTypeMux0[i] == 1){
        // ノイズでなければ、読み取り値をCCとして出力
        if(diffAnalogValueMux0 >= VIBRATE_FILTER){
          MIDI.sendControlChange(ifAssignMux0[i], currMux0Value[i]/8, CHANNEL_1);
          prevMux0Value[i] = currMux0Value[i];
        }
      }
      else if(portTypeMux0[i] == 2){
        // 何もしない
      }
    }

    // Mux1
    if(currMux1Value[i] != prevMux1Value[i]){
      //デジタルポート(ポートタイプ:0)の場合
      if(portTypeMux1[i] == 0){
        if(diffAnalogValueMux1 >= DIGITAL_THRESHOLD){
          MIDI.sendNoteOn(ifAssignMux1[i], convAD(currMux1Value[i]), CHANNEL_1);
          prevMux1Value[i] = currMux1Value[i];
        }
      }
      else if(portTypeMux1[i] == 1){
        // ノイズでなければ、読み取り値をCCとして出力
        if(diffAnalogValueMux1 >= VIBRATE_FILTER){
          MIDI.sendControlChange(ifAssignMux1[i], currMux1Value[i]/8, CHANNEL_1);
          prevMux1Value[i] = currMux1Value[i];
        }
      }
      else if(portTypeMux1[i] == 2){
        // 何もしない
      }
    }

    // Mux2
    if(currMux2Value[i] != prevMux2Value[i]){
      //デジタルポート(ポートタイプ:0)の場合
      if(portTypeMux2[i] == 0){
        if(diffAnalogValueMux2 >= DIGITAL_THRESHOLD){
          MIDI.sendNoteOn(ifAssignMux2[i], convAD(currMux2Value[i]), CHANNEL_1);
          prevMux2Value[i] = currMux2Value[i];
        }
      }
      else if(portTypeMux2[i] == 1){
        // ノイズでなければ、読み取り値をCCとして出力
        if(diffAnalogValueMux2 >= VIBRATE_FILTER){
          MIDI.sendControlChange(ifAssignMux2[i], (int)currMux2Value[i]/8, CHANNEL_1);
          prevMux2Value[i] = currMux2Value[i];
        }
      }
      else if(portTypeMux2[i] == 2){
        // 何もしない
      }
    }
  }

  // Aデッキのジョグの状態を読み取る
  jog_a.read();
  // 動きがあれば、MIDIデータとして送信
  if(jog_a.motion){
    MIDI.sendControlChange(DECK_A_JOG, conv_midiscale(jog_a.delta_y), CHANNEL_1);
  }

  // Bデッキのジョグの状態を読み取る
  jog_b.read();
  // 動きがあれば、MIDIデータとして送信
  if(jog_b.motion){
    MIDI.sendControlChange(DECK_B_JOG, conv_midiscale(jog_b.delta_y), CHANNEL_1);
  }

  // 受信したMIDIデータの読み込み
  MIDI.read();

  // ファンの回転数を制御する簡易PWM
  if(digitalRead(PWM_PIN) == 1 && pwm_count == INTERVAL_ON){
    pwm_count = 0;
    digitalWrite(PWM_PIN, 0);
  }
  else if(digitalRead(PWM_PIN) == 0 && pwm_count == INTERVAL_OFF){
    pwm_count = 0;
    digitalWrite(PWM_PIN, 1);
  }
  pwm_count++;
}

char convAD(int analogVal){
  if(analogVal > DIGITAL_THRESHOLD) return 127;
  else return 0;
}

char conv_midiscale(char value){
  unsigned char range = 18;
  if (value >= 1){
    return (value / range) + 1;
  }
  else{
    return 127 + (value / range);
  }
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    if( myDataOut & (1<<i) ){
      pinState= 1;
    }else{
      pinState= 0;
    }
    
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }

  digitalWrite(myClockPin, 0);
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) {
  if(velocity > 5){
    // NoteOnの時にデータによって対応するLEDを点灯させる
    /*
     * LEDの状態が00001000の時に、PLAY(0x01)が押されたとすると、
     * 現在の状態と0x01でOR演算を行なっているので、
     * 00001000 OR 00000001 = 00001001
     * となり、他のLEDの状態に影響を与えず、該当のLEDの状態を
     * ON(1)にできる。
     */
    switch(pitch){
    case DECK_A_PLAY:
      ledStatus_decka &= ~PLAY;
      ledStatus_decka |= STOP;
      break;
    case DECK_B_PLAY:
      ledStatus_deckb &= ~PLAY;
      ledStatus_deckb |= STOP;
      break;
    case DECK_A_PFL:
      ledStatus_decka &= ~HEADON;
      ledStatus_decka |= HEADOFF;
      break;
    case DECK_B_PFL:
      ledStatus_deckb &= ~HEADON;
      ledStatus_deckb |= HEADOFF;
      break;
    case DECK_A_FX:
      ledStatus_decka &= ~FX;
      break;
    case DECK_B_FX:
      ledStatus_deckb &= ~FX;
      break;
    case DECK_A_SCRATCH:
      ledStatus_decka &= ~SCH;
      break;
    case DECK_B_SCRATCH:
      ledStatus_deckb &= ~SCH;
      break;
    case DECK_A_SYNC:
      ledStatus_decka &= ~SYNC;
      break;
    case DECK_B_SYNC:
      ledStatus_deckb &= ~SYNC;
    }
  }
  else{
    // NoteOnの時にデータによって対応するLEDを消灯させる
    /*
     * LEDの状態が00001001の時に、PLAY(0x01)が押されたとすると、
     * 現在の状態とPLAYのNOT演算したものとAND演算を行なっているので、
     * NOT(00000001) = 11111110
     * 00001001 AND 11111110 = 00001000
     * となり、他のLEDの状態に影響を与えず、該当のLEDの状態を
     * OFF(0)にできる。
     */
    switch(pitch){
    case DECK_A_PLAY:
      ledStatus_decka |= PLAY;
      ledStatus_decka &= ~STOP;
      break;
    case DECK_B_PLAY:
      ledStatus_deckb |= PLAY;
      ledStatus_deckb &= ~STOP;
      break;
    case DECK_A_PFL:
      ledStatus_decka |= HEADON;
      ledStatus_decka &= ~HEADOFF;
      break;
    case DECK_B_PFL:
      ledStatus_deckb |= HEADON;
      ledStatus_deckb &= ~HEADOFF;
      break;
    case DECK_A_FX:
      ledStatus_decka |= FX;
      break;
    case DECK_B_FX:
      ledStatus_deckb |= FX;
      break;
    case DECK_A_SCRATCH:
      ledStatus_decka |= SCH;
      break;
    case DECK_B_SCRATCH:
      ledStatus_deckb |= SCH;
      break;
    case DECK_A_SYNC:
      ledStatus_decka |= SYNC;
      break;
    case DECK_B_SYNC:
      ledStatus_deckb |= SYNC;
      break;
    }
  }
  ledRenew();
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  // NoteOnの時にデータによって対応するLEDを消灯させる
  /*
   * LEDの状態が00001001の時に、PLAY(0x01)が押されたとすると、
   * 現在の状態とPLAYのNOT演算したものとAND演算を行なっているので、
   * NOT(00000001) = 11111110
   * 00001001 AND 11111110 = 00001000
   * となり、他のLEDの状態に影響を与えず、該当のLEDの状態を
   * OFF(0)にできる。
   */
  switch(pitch){
  case DECK_A_PLAY:
    ledStatus_decka |= PLAY;
    ledStatus_decka &= ~STOP;
    break;
  case DECK_B_PLAY:
    ledStatus_deckb |= PLAY;
    ledStatus_deckb &= ~STOP;
    break;
  case DECK_A_PFL:
    ledStatus_decka |= HEADON;
    ledStatus_decka &= ~HEADOFF;
    break;
  case DECK_B_PFL:
    ledStatus_deckb |= HEADON;
    ledStatus_deckb &= ~HEADOFF;
    break;
  case DECK_A_FX:
    ledStatus_decka |= FX;
    break;
  case DECK_B_FX:
    ledStatus_deckb |= FX;
    break;
  case DECK_A_SCRATCH:
    ledStatus_decka |= SCH;
    break;
  case DECK_B_SCRATCH:
    ledStatus_deckb |= SCH;
    break;
  case DECK_A_SYNC:
    ledStatus_decka |= SYNC;
    break;
  case DECK_B_SYNC:
    ledStatus_deckb |= SYNC;
    break;
  }
  ledRenew();
}

void HandleControlChange(byte channel, byte number, byte value){
  switch(number){
  case DECK_A_FXSELECT:
    /*
       * エフェクターの種類を変更した場合、
     * つまみの位置がリセットされるため、
     * コントローラーのつまみの状態を送る
     */
    MIDI.sendControlChange(ifAssignMux0[7], currMux0Value[7]/8, CHANNEL_1);
    break;
  case DECK_B_FXSELECT:
    MIDI.sendControlChange(ifAssignMux1[7], currMux1Value[7]/8, CHANNEL_1);
    break;
  }
}


void ledRenew(){
  // LED出力
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, ledStatus_decka);   
  shiftOut(dataPin, clockPin, ledStatus_deckb);
  digitalWrite(latchPin, 1);
}

