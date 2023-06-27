#include <DueTimer.h>
const byte ReadPin=3;
const byte ReadBit=28;
 
const byte DebugPin=5;
const byte DebugBit=25;
uint32_t DebugReg;
uint32_t DataSum=0;
uint32_t TrgState;
boolean Trgflg=false;
uint32_t StartBitReg;

uint32_t LongWordData=0x12345678;
const byte DataPin=6;
const byte DataBit=24;
uint32_t DataReg;
boolean OutPutflg=false;

byte DataNo=0;
uint32_t Data[33];
int DataLen=32;
const int ClkTime=10;



  
void setup() {
  Serial.begin(115200);
  pinMode(ReadPin,INPUT_PULLUP);
  pinMode(DebugPin,OUTPUT);
  pinMode(DataPin,OUTPUT);
  DebugReg=1<<DebugBit;
  DataReg=1<<DataBit;
  StartBitReg=DataReg;
  REG_PIOC_ODSR=DebugReg | DataReg;
  TrgState=bitClear(REG_PIOC_PDSR,ReadBit); 

  for(int i=0;i<=31;i++){
    Data[i]=DebugReg | (bitRead(LongWordData,31-i)<<DataBit);
    //Serial.println(Data[i],BIN);
  }
  
  Data[32]=DebugReg | DataReg;
  //Serial.println(Data[32],BIN);

  Timer4.attachInterrupt(Timer4_handler);
  Timer4.start(ClkTime);
  
  attachInterrupt(digitalPinToInterrupt(ReadPin),Start,FALLING);
}
 
void Start(){
  if (REG_PIOC_PDSR!=TrgState){
  return;
  }
  noInterrupts();
  delayMicroseconds(3);
  delayMicro(24);
  REG_PIOC_ODSR=StartBitReg;
  DataSum=bitWrite(DataSum,31-DataNo,bitRead(REG_PIOC_PDSR,ReadBit));
  DataNo++;
  for(int i=0;i<=30;i++){
    delayMicroseconds(9);
    delayMicro(10);
    REG_PIOC_ODSR=REG_PIOC_PDSR^DebugReg;
    DataSum=bitWrite(DataSum,31-DataNo,bitRead(REG_PIOC_PDSR,ReadBit));
    DataNo++;
  }
  //Serial.println(DataSum,BIN);
   
  DataSum=0;
  DataNo=0;
  //Trgflg=false;
  OutPutflg=true;
  interrupts();
  }
  
static inline void delayMicro(uint32_t n){
    if (n == 0) return;
    //uint32_t n = usec * (VARIANT_MCK / 3000000);
    asm volatile(
        "L_%=_delayMicro:"       "\n\t"
        "subs   %0, #1"                 "\n\t"
        "bne    L_%=_delayMicro" "\n"
        : "+r" (n) :
    );
}
void Timer4_handler(void){
  if (OutPutflg==true){
    
    REG_PIOC_ODSR=Data[DataNo];

    DataNo=DataNo+1;
    if(DataNo>DataLen){
      DataNo=0;
      Trgflg=false;
      OutPutflg=false;
    }
}
}
void loop() {
}
