#include <stdio.h>
#include<string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "bitfont.h"
#include "pcd8544.h"

/*
3V3 <--> VCC
GND <--> GND

GPIO12 <--> DIN
GPIO14 <--> CLK

GPIO15 <--> CE
GPIO25 <--> DC
GPIO4 <--> RST
GPIO16 <--> BL

frame[y][x]

*/


void app_main() {


  init();
  enable();





framesetInit();

//---------------------Drawing-Area-----------------

drawRectToFrame(47,83);
setCursor(0,24);
plotTriangleToFrame(2,45,80,44);

plotCircleToFrame(10);
setCursor(0,0);
printToFrame("Hello");
display();



//---------------------------------------------------


  disable();




}
