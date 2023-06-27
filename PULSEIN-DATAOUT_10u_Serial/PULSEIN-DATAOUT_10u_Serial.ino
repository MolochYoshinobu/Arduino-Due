#include <DueTimer.h>
const byte StartPin=3;
const byte StartBit=28;

const byte DataPin=5;
const byte DataBit=25;
uint32_t DataReg;
uint32_t TrgState;
uint32_t data[1];
  
void setup() {
  pinMode(StartPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(StartPin),Start,FALLING);
  DataReg=1<<DataBit;
  REG_PIOC_ODSR=DataReg;
  TrgState=bitClear(REG_PIOC_PDSR,StartBit);
  
  data[0]=0xFF555555;
  Serial.begin(100000);
  
}

void Start(){
  delayMicroseconds(100);
  if (REG_PIOC_PDSR==TrgState){
  Serial.write((byte*)data,4);
}
}

void loop() {
  
}
