#include <UTFT.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Arduino Uno / 2009:
// -------------------
// Standard Arduino Uno/2009 shield            : <display model>,A5,A4,A3,A2
// DisplayModule Arduino Uno TFT shield        : <display model>,A5,A4,A3,A2
//
// Arduino Mega:
// -------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega      : <display model>,38,39,40,41
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ILI9481,38,39,40,41);
int PressurePin = A0;
float Vpress =0;
int pixel_max=304;
int pixel_min=15;
int vpress_max=580;
int vpress_min=495;
int vpress_max_cur=0;
int vpress_min_cur=1023;
int dbg_x=400;
int dbg_y=30;
float heart_rate=0;
float heart_cnt=0;
int silent_cnt=0;
int rec_pkg=0;
long timer_cnt=0;

float band_guard =0.2;
void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

}

void loop()
{
  int x=6;
  int y=159;
  int x1=6;
  int y1=159;
  int x2=6;
  int y2=159;
  int i=2;

// Clear the screen and draw the frame
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 306, 479, 319);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* Pulse Diagnose Pulse Sensor *", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("Developed by Wang Jiang (version 0.3)", CENTER, 307);
  myGLCD.setFont(BigFont);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 479, 305);
  myGLCD.setColor(255,0,0);

  while (1){
    x=x+1;
    PressureSensorRead(); 
    if(x>478){
      x=6;
      x1=6;
      i=i>10?1:i;
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(0, 14, 479, 305);      
      myGLCD.setColor(0, 0, 255);
      myGLCD.drawRect(0, 14, 479, 305);
//      vpress_max=(vpress_max_cur+(vpress_max_cur-vpress_min_cur)*band_guard)>1013? 1023 : (vpress_max_cur+(vpress_max_cur-vpress_min_cur)*band_guard);
//      vpress_min=(vpress_min_cur-(vpress_max_cur-vpress_min_cur)*band_guard)<0? 0 : (vpress_min_cur-(vpress_max_cur-vpress_min_cur)*band_guard);
      vpress_max_cur=Vpress;
      vpress_min_cur=Vpress;
      // Draw crosshairs
      myGLCD.setColor(255,0,0);
    }
    Vpress=Vpress>vpress_max? vpress_max: Vpress;
    Vpress=Vpress<vpress_min? vpress_min: Vpress;
    y= (int)(pixel_max-((Vpress-vpress_min)/(vpress_max-vpress_min))*(pixel_max-pixel_min));
//    Serial.print(y);
//    Serial.print("\n");      
    myGLCD.drawLine(x,y,x1,y1);
    myGLCD.setColor(0, 0, 255);
//    myGLCD.drawCircle(x,,i);
    myGLCD.drawLine(x-i,y-i,x+i,y+i);
    myGLCD.drawLine(x-i,y+i,x+i,y-i);
    myGLCD.setColor(255,0,0);
    if((silent_cnt==0)&&(y>=y1)&&(y1<=y2)&&(y1<=150)){
      heart_cnt++;
      silent_cnt=10;
    } else if(silent_cnt!=0){
      silent_cnt--;    
    }
    myGLCD.printNumI(heart_cnt, dbg_x, dbg_y+70, 4, ' ');
    x2=x1;
    y2=y1;
    x1=x;
    y1=y;
  }
}

void PressureSensorRead(){

  int v_press[10], v_press_sum=0;
  for (int i=0; i<10;i++){
    v_press[i] = analogRead(PressurePin);    
    v_press_sum=v_press_sum+v_press[i];
//    delay(1);
  }
  Vpress=v_press_sum/10.0;
  rec_pkg++;
//  Vpress = analogRead(PressurePin);    
}

