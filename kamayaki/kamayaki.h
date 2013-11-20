// MIDI Channel
#define CHANNEL_1 1

// Noise Reduce
#define VIBRATE_FILTER    15
#define DIGITAL_THRESHOLD 1000

// Numark MIXTRACK
#define DECK_A_PLAY     0x3B
#define DECK_B_PLAY     0x42
#define DECK_A_CUE      0x33
#define DECK_B_CUE      0x3C
#define DECK_A_SYNC     0x40
#define DECK_B_SYNC     0x47
#define DECK_A_SHUTTER  0x4A
#define DECK_B_SHUTTER  0x4C
#define DECK_A_SCRATCH  0x48
#define DECK_B_SCRATCH  0x50
#define DECK_A_VOLUME   0x08
#define DECK_B_VOLUME   0x09
#define DECK_A_PFL      0x65
#define DECK_B_PFL      0x66
#define DECK_A_EQ_HI    0x10
#define DECK_B_EQ_HI    0x11
#define DECK_A_EQ_MID   0x12
#define DECK_B_EQ_MID   0x13
#define DECK_A_EQ_LO    0x14
#define DECK_B_EQ_LO    0x15
#define DECK_A_FIL      0x21
#define DECK_B_FIL      0x22
#define DECK_A_JOG      0x19
#define DECK_B_JOG      0x18
#define DECK_A_JOGTOUCH 0x4E
#define DECK_B_JOGTOUCH 0x4D
#define DECK_A_FX       0x63
#define DECK_B_FX       0x64
#define DECK_A_DRYWET   0x1C
#define DECK_B_DRYWET   0x1F
#define DECK_A_FXAMOUNT 0x1D
#define DECK_B_FXAMOUNT 0x20
#define DECK_A_FXSELECT 0x1B
#define DECK_B_FXSELECT 0x1E
#define CROSSFADER      0x0A
#define HEAD_VOLUME     0x0B
#define HEAD_MIX        0x0C
#define MASTER_VOLUME   0x17

//MUX Control pins
#define CONTROL0 5    
#define CONTROL1 4
#define CONTROL2 3
#define CONTROL3 2

//MUX Input pins
#define MUX0 0
#define MUX1 1
#define MUX2 2

//JOG IO Pins
#define JOG_A_CLOCK 8
#define JOG_A_DATA  9
#define JOG_A_NCS   10
#define JOG_B_CLOCK 11
#define JOG_B_DATA  12
#define JOG_B_NCS   13

//PWM Pin
#define PWM_PIN 7
// MIN:7
#define INTERVAL_ON  10
#define INTERVAL_OFF 35

//LED Address
#define STOP     0x01
#define SYNC     0x02
#define FX       0x04
#define HEADON   0x08
#define PLAY     0x10
#define HEADOFF  0x20
#define NOASSIGN 0x40
#define SCH      0x80

// Port Type
// 0 => DIGITAL, 1 => ANALOG, 2=> UNUSE
//
const unsigned int portTypeMux0[16] = {
  //2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
  1,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2
    //2,2,2,2,2,0,1,1,2,2,2,2,2,2,2,2
};
const unsigned int portTypeMux1[16] = {
  1,1,0,0,0,0,0,0,0,2,2,2,2,2,2,2
    //2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};
const unsigned int portTypeMux2[16] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2
    //2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
};

//Pin Assign
const unsigned char ifAssignMux0[16] = {
  DECK_A_FXAMOUNT,
  DECK_A_DRYWET,
  DECK_A_FX,
  DECK_A_PFL,
  DECK_A_SHUTTER,
  DECK_A_PLAY,
  DECK_A_CUE,
  DECK_A_SYNC,
  DECK_A_SCRATCH,
  0,0,0,0,0,0,0
};
const unsigned char ifAssignMux1[16] = {
  DECK_B_FXAMOUNT,
  DECK_B_DRYWET,
  DECK_B_FX,
  DECK_B_PFL,
  DECK_B_SHUTTER,
  DECK_B_PLAY,
  DECK_B_CUE,
  DECK_B_SYNC,
  DECK_B_SCRATCH,
  0,0,0,0,0,0,0
};

const unsigned char ifAssignMux2[16] = {
  DECK_A_EQ_HI,
  DECK_A_EQ_MID,
  DECK_A_EQ_LO,
  DECK_A_FIL,
  DECK_B_EQ_HI,
  DECK_B_EQ_MID,
  DECK_B_EQ_LO,
  DECK_B_FIL,
  HEAD_VOLUME,
  HEAD_MIX,
  DECK_A_VOLUME,
  DECK_B_VOLUME,
  CROSSFADER,
  0,0,0
};

// シフトレジスタ用ピン
const int latchPin = 17;
const int clockPin = 18;
const int dataPin  = 19;

