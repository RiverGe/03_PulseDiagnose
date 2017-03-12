#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

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
int vpress_max=550;
int vpress_min=495;
int zoom_in_f=6.0;
int vpress_max_cur=0;
int vpress_min_cur=1023;
int dbg_x=400;
int dbg_y=30;
int heart_rate=0;
int heart_cnt=0;
int rec_pkg=0;
float y_avg=0;
float y_sum=0;

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
  myGLCD.print("Developed by Wang Jiang (version 0.1)", CENTER, 307);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 479, 305);
//  myGLCD.setBackColor(0, 0, 0);
//  for (int i=9; i<470; i+=10)
//    myGLCD.drawLine(i, 305, i, 300);
//  for (int i=19; i<300; i+=10)
//    myGLCD.drawLine(0, i, 5, i);
  myGLCD.setColor(255,0,0);

  while (1){
//    myGLCD.printNumI(Vpress, dbg_x, dbg_y, 4, ' ');
//    myGLCD.printNumI(vpress_max, dbg_x, dbg_y+10, 4, ' ');
//    myGLCD.printNumI(vpress_min, dbg_x, dbg_y+20, 4, ' ');
//    myGLCD.printNumI(vpress_max_cur, dbg_x, dbg_y+30, 4, ' ');
//    myGLCD.printNumI(vpress_min_cur, dbg_x, dbg_y+40, 4, ' ');
//    myGLCD.printNumI(x1, dbg_x, dbg_y+50, 4, ' ');
//    myGLCD.printNumI(y1, dbg_x, dbg_y+60, 4, ' ');
    x=x+1;
    PressureSensorRead(); 
    vpress_max_cur=Vpress>vpress_max_cur? Vpress: vpress_max_cur;
    vpress_min_cur=Vpress<vpress_min_cur? Vpress: vpress_min_cur;

    if(x>478){
      x=6;
      x1=6;
//      i++;
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
//     myGLCD.setColor(0, 0, 255);
//      myGLCD.setBackColor(0, 0, 0);
//      for (int i=9; i<470; i+=10)
//        myGLCD.drawLine(i, 305, i, 300);
//      for (int i=19; i<300; i+=10)
//        myGLCD.drawLine(0, i, 5, i);
      myGLCD.setColor(255,0,0);
    }
    Vpress=Vpress>vpress_max? vpress_max: Vpress;
    Vpress=Vpress<vpress_min? vpress_min: Vpress;
    y= (int)(pixel_max-((Vpress-vpress_min)/(vpress_max-vpress_min))*(pixel_max-pixel_min));
//    myGLCD.printNumI(y, dbg_x, dbg_y+70, 4, ' ');
    y_sum=y_sum+y;
    y_avg=y_sum/rec_pkg;
    
    myGLCD.drawLine(x,y,x1,y1);
    myGLCD.setColor(0, 0, 255);
    myGLCD.drawCircle(x,y_avg,i);
    myGLCD.drawLine(x-i,y-i,x+i,y+i);
    myGLCD.drawLine(x-i,y+i,x+i,y-i);
    myGLCD.setColor(255,0,0);
    if((y>y1)&&(y1<y2)&&(y<(y_avg-20))){
      heart_cnt++;
    }
    if(rec_pkg%350==0){
      heart_rate=heart_cnt*6;
      heart_cnt=0;
    }  
    myGLCD.printNumI(heart_rate, dbg_x, dbg_y+70, 4, ' ');
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
    delay(1);
  }
  Vpress=v_press_sum/10.0;
  rec_pkg++;
//  Vpress = analogRead(PressurePin);    

}
