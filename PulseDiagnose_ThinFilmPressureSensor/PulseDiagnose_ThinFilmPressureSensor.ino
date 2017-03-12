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
int vpress_max=1023;
int vpress_min=0;
int vpress_max_cur=0;
int vpress_min_cur=1023;
int dbg_x=400;
int dbg_y=30;

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
  
// Clear the screen and draw the frame
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 306, 479, 319);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* Pulse Diagnose Thin Film Pressure Sensor *", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("Developed by Wang Jiang (version 0.1)", CENTER, 307);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 479, 305);
  myGLCD.setBackColor(0, 0, 0);
  for (int i=9; i<470; i+=10)
    myGLCD.drawLine(i, 305, i, 300);
  for (int i=19; i<300; i+=10)
    myGLCD.drawLine(0, i, 5, i);
  myGLCD.setColor(255,0,0);

  while (1){
    myGLCD.printNumI(Vpress, dbg_x, dbg_y, 4, ' ');
    myGLCD.printNumI(vpress_max, dbg_x, dbg_y+10, 4, ' ');
    myGLCD.printNumI(vpress_min, dbg_x, dbg_y+20, 4, ' ');
    myGLCD.printNumI(vpress_max_cur, dbg_x, dbg_y+30, 4, ' ');
    myGLCD.printNumI(vpress_min_cur, dbg_x, dbg_y+40, 4, ' ');
    myGLCD.printNumI(x1, dbg_x, dbg_y+50, 4, ' ');
    myGLCD.printNumI(y1, dbg_x, dbg_y+60, 4, ' ');
    x++;
    PressureSensorRead(); 
    vpress_max_cur=Vpress>vpress_max_cur? Vpress: vpress_max_cur;
    vpress_min_cur=Vpress<vpress_min_cur? Vpress: vpress_min_cur;

    if(x==478){
      x=6;
      x1=6;
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(0, 14, 479, 305);      
      myGLCD.setColor(0, 0, 255);
      myGLCD.drawRect(0, 14, 479, 305);
//      vpress_max=vpress_max_cur;
//      vpress_min=vpress_min_cur;
      vpress_max_cur=Vpress;
      vpress_min_cur=Vpress;
      // Draw crosshairs
      myGLCD.setColor(0, 0, 255);
      myGLCD.setBackColor(0, 0, 0);
      for (int i=9; i<470; i+=10)
        myGLCD.drawLine(i, 305, i, 300);
      for (int i=19; i<300; i+=10)
        myGLCD.drawLine(0, i, 5, i);
      myGLCD.setColor(255,0,0);
    }
    y= (int)((Vpress/(vpress_max-vpress_min))*(pixel_max-pixel_min));
    myGLCD.drawLine(x,y,x1,y1);
    x1=x;
    y1=y;
  }
}
void PressureSensorRead(){
  int v_press[8], v_press_sum=0;
  for (int i=0; i<8;i++){
    v_press[i] = analogRead(PressurePin);    
    v_press_sum=v_press_sum+v_press[i];
  }
  Vpress=v_press_sum/8.0;
//    Vpress = analogRead(PressurePin);    
//    delay(50);    

}
