#include <SPI.h>
#include <string.h>

byte recv[20];
byte response[9]={0x01,0x21,4,0x00,0x00,0x00,0x00,'\n','\r'};
byte sensor_fake[4] = {0x10,0x36,0x7f,0x6c};
volatile byte pos;
volatile boolean finish;
volatile boolean resp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MISO,OUTPUT);

  SPCR|=_BV(SPE);
  SPDR =0x00;
  pos=0;
  finish =false;
  resp = false;
  SPI.attachInterrupt();
}

ISR(SPI_STC_vect)
{
  byte c = SPDR;
  static byte oc=0;
  if(resp)
  {
    SPDR = response[++pos];
    if(pos==9)
    {
     resp =false; 
    }
    return;  
  }else
  {
   SPDR =0x00; 
   }
  if(pos<18&&!finish)
  {
    recv[pos++]=c;
    if((c==0xcc)&&(oc==0xff))
    {
      finish = true;
      SPDR = 0xff;  
    }
    
  }
  oc = c;
   
}

void loop() {
  // put your main code here, to run repeatedly:
  //Transfer Finished
  if((digitalRead(SS)==HIGH)&&finish&&!resp)
  {
    switch(recv[0])
    {
     case 0x01:
      movement();
      break;
     default:
      break;
    }

    finish = false;
    pos =0;
    SPDR =0x00;
  }
}
void movement()
{
  switch(recv[1])
  {
    case 0x20: // query sensor, need response
      memcpy(response+3,sensor_fake,4);
      SPDR = response[0];
      pos = 1;
      resp = true;
     break;
    default:
    return;
  }
}
