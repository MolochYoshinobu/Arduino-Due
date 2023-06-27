#include <DueTimer.h>
const byte StartPin=3;
const byte StartBit=28;

const byte DataPin=5;
const byte DataBit=25;
uint32_t DataReg;
int DataLen=33;
uint32_t LongWordData=0x12345678;
uint32_t Data[32];
uint32_t DataSum=0;
uint32_t TrgState;
byte DataNo=0;
const int ClkTime=10;
boolean flg=false;
boolean OutPutflg=false;
byte cnt=0;
  
void setup() {
  int i;
  Serial.begin(115200);
  pinMode(StartPin,INPUT_PULLUP);
  pinMode(DataPin,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(StartPin),Start,FALLING);
  for(i=0;i<=31;i++){
    Data[i]=bitRead(LongWordData,31-i)<<DataBit;
    //Serial.println(Data[i],BIN);
  }
  DataReg=1<<DataBit;
  REG_PIOC_ODSR=DataReg;
  Data[32]=DataReg;
  TrgState=bitClear(REG_PIOC_PDSR,StartBit);
  
  //Serial.println(TrgState,BIN);
  Timer1.attachInterrupt(Timer1_handler);
  Timer1.start(ClkTime);
  
}

void Timer1_handler(void){
  if (flg==true){
    REG_PIOC_ODSR=Data[DataNo];
    DataNo++;
    if(DataNo==DataLen){
      DataNo=0;
      flg=false;
    }
   }
}
void Start(){
  delayMicroseconds(100);
  if (REG_PIOC_PDSR==TrgState){
  flg=true;
  }
}

void loop() {
  //if (REG_PIOC_PDSR==TrgState){
  //flg=true;
  //}
  // put your main code here, to run repeatedly:
  // Serial.println(digitalRead(StartPin));
  //delay(1000);
  
}
