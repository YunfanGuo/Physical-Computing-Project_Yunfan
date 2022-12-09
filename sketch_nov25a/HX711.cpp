#include "hx711.h"

long HX711_Buffer = 0;
long Weight_Maopi = 0, Weight_Shiwu = 0;

#define GapValue 430

//****************************************************
//Initialising HX711
//****************************************************
void Init_Hx711()
{
  pinMode(HX711_SCK, OUTPUT);
  pinMode(HX711_DT, INPUT);
}


//****************************************************
//Get weight
//****************************************************
void Get_Maopi()
{
  Weight_Maopi = HX711_Read();
}

//****************************************************
//Weighing
//****************************************************
long Get_Weight()
{
  HX711_Buffer = HX711_Read();
  Weight_Shiwu = HX711_Buffer;
  Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
  Weight_Shiwu = (long)((float)Weight_Shiwu / GapValue);
  return Weight_Shiwu;
}

//****************************************************
//Read HX711
//****************************************************
unsigned long HX711_Read(void)	//Gain 128
{
  unsigned long count;
  unsigned char i;
  bool Flag = 0;

  digitalWrite(HX711_DT, HIGH);
  delayMicroseconds(1);

  digitalWrite(HX711_SCK, LOW);
  delayMicroseconds(1);

  count = 0;
  while (digitalRead(HX711_DT));
  for (i = 0; i < 24; i++)
  {
    digitalWrite(HX711_SCK, HIGH);
    delayMicroseconds(1);
    count = count << 1;
    digitalWrite(HX711_SCK, LOW);
    delayMicroseconds(1);
    if (digitalRead(HX711_DT))
      count++;
  }
  digitalWrite(HX711_SCK, HIGH);
  count ^= 0x800000;
  delayMicroseconds(1);
  digitalWrite(HX711_SCK, LOW);
  delayMicroseconds(1);

  return (count);
}
