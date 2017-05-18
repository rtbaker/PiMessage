#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <wiringPi.h>
#include "a_fuentes.h"
#include "a_text_functions.h"


// Pin setup
int latchPin = 27; //LT
int clockPin = 28;//SK
int dataPin =  29; //R1

int en_74138 = 21;
int la_74138 = 22;
int lb_74138 = 23;
int lc_74138 = 24;
int ld_74138 = 25;

// Various vars
#define COL_PIXEL     64
#define ROW_PIXEL     16

char *message = "Merry Xmas and a Happy New Year !";

void display_matrix(unsigned char *BMP);
void setup();

unsigned long long previousMillis = 0; // last time update
int offset = 0;

int main (int argc, char *argv[])
{
    if (argc > 1) message = argv[1];
    // Setup the Wiring Pi Library
    wiringPiSetup ();
    
    // Our own setup
    setup();

    while (1){
       display_matrix(buffer);
       
       struct timeval tv;

       gettimeofday(&tv, NULL);

       unsigned long long currentMillis =
           (unsigned long long)(tv.tv_sec) * 1000 +
           (unsigned long long)(tv.tv_usec) / 1000;
       
       if(currentMillis - previousMillis > 80) {
           previousMillis = currentMillis;  
           offset--;
  
           int nostring = strlen(message) * 8 * -1;
           if (offset < nostring) offset = 64;
       }
       
       fillCanvas(0);
       textToCanvas(tinyfont_8x8,8,8,message,0,offset);
       translateCanvasToBuffer();

      // delayMicroseconds(1000);
    }

    // And finish ?
    return 0 ;
}


/*
 * Support functions
 */
/* This function is called once at start up ----------------------------------*/
void setup()
{
   // Dot matrix setup
    pinMode(latchPin,OUTPUT);
    pinMode(clockPin,OUTPUT);
    pinMode(dataPin,OUTPUT);
  
    pinMode(en_74138,OUTPUT);
    pinMode(la_74138,OUTPUT);
    pinMode(lb_74138,OUTPUT);
    pinMode(lc_74138,OUTPUT);
    pinMode(ld_74138,OUTPUT);
        
    digitalWrite(en_74138, LOW);
    
    fillCanvas(0);
    testLines();
    //checkerCanvas();
    translateCanvasToBuffer();
}

void shiftOut(unsigned char dataOut)
{
    int i;
    for(i=0;i<=7;i++) {
        digitalWrite(clockPin,LOW);
           delayMicroseconds(5);
        if(dataOut & (0x01<<i))  digitalWrite(dataPin,HIGH);
        else  digitalWrite(dataPin,LOW);
             delayMicroseconds(5);
        digitalWrite(clockPin,HIGH);
             delayMicroseconds(5);
    }
}

void display_matrix(unsigned char *BMP)
{
    //Display count
    unsigned int dis_cnt=4; //256;
    unsigned int i;
    unsigned int ROW_xPixel = 0;
        
    for(i=0;i<dis_cnt*16;i++)
    {
        // What to display
        unsigned char Col_num_1=~BMP[(COL_PIXEL/8)*ROW_xPixel];
        unsigned char Col_num_2=~BMP[(COL_PIXEL/8)*ROW_xPixel+1];
        unsigned char Col_num_3=~BMP[(COL_PIXEL/8)*ROW_xPixel+2];
        unsigned char Col_num_4=~BMP[(COL_PIXEL/8)*ROW_xPixel+3];
        unsigned char Col_num_5=~BMP[(COL_PIXEL/8)*ROW_xPixel+4];
        unsigned char Col_num_6=~BMP[(COL_PIXEL/8)*ROW_xPixel+5];
        unsigned char Col_num_7=~BMP[(COL_PIXEL/8)*ROW_xPixel+6];
        unsigned char Col_num_8=~BMP[(COL_PIXEL/8)*ROW_xPixel+7];
                
        // Display off
        //      PIN_MAP[en_74138].gpio_peripheral->BSRR = PIN_MAP[en_74138].gpio_pin; //digitalWrite(en_74138, HIGH);
  
        //Col scanning
        shiftOut(Col_num_1);
        shiftOut(Col_num_2);
        shiftOut(Col_num_3);
        shiftOut(Col_num_4);
        shiftOut(Col_num_5);
        shiftOut(Col_num_6);
        shiftOut(Col_num_7);
        shiftOut(Col_num_8);
  
        // toggle the latch
        digitalWrite(latchPin, LOW);
        //      delayMicroseconds(20);
        digitalWrite(latchPin, HIGH);

                
        //Row scanning
        // AVR Port Operation 
        // PORTD = ((ROW_xPixel << 3 ) & 0X78) | (PORTD & 0X87);//Write PIN  la_74138 lb_74138 lc_74138 ld_74138
        if ((ROW_xPixel & 0x1)) digitalWrite(la_74138, HIGH);
        else digitalWrite(la_74138, LOW);
                
        if ((ROW_xPixel & 0x2)) digitalWrite(lb_74138, HIGH);
        else digitalWrite(lb_74138, LOW);
                
        if ((ROW_xPixel & 0x4)) digitalWrite(lc_74138, HIGH);
        else digitalWrite(lc_74138, LOW);
                
        if ((ROW_xPixel & 0x8)) digitalWrite(ld_74138, HIGH);
        else digitalWrite(ld_74138, LOW);
                

        // Display on
        //      PIN_MAP[en_74138].gpio_peripheral->BRR = PIN_MAP[en_74138].gpio_pin; //digitalWrite(en_74138, LOW);
        // Next row
        if(ROW_xPixel==15) ROW_xPixel=0; else ROW_xPixel++;
                
       // delayMicroseconds(1000);
    }
  
}
                