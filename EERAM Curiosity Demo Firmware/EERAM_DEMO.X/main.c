/*
 * © 2019 Microchip Technology Inc. and its subsidiaries.
 * 
 * Subject to your compliance with these terms, you may use Microchip software and any
 * derivatives exclusively with Microchip products. It is your responsibility to comply 
 * with third party license terms applicable to your use of third party software 
 * (including open source software) that may accompany Microchip software.
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF 
 * NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL 
 * OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR 
 * THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL 
 * LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
 * OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include <stdbool.h>

#define POT_CHANNEL 0x04

#define RECORDSIZE 0x0800
#define START_ADDRESS 0x0000 
#define TIMEOUTAT50HZ 100

void LED0();void LED1();void LED2();void LED3();void LED4();void LED5();
void LED6();void LED7();void LED8();void LED9();void LED10();void LED11();
void LEDRecordOn(); void LEDPlaybackOn();void LEDRecordOff(); void LEDPlaybackOff();
void TMR0_CustomStuff(void);
void TMR4_CustomStuff(void);

#define L0 LED0();
#define L1 LED1();
#define L2 LED2();
#define L3 LED3();
#define L4 LED4();
#define L5 LED5();
#define L6 LED6();
#define L7 LED7();
#define L8 LED8();
#define L9 LED9();
#define L10 LED10();
#define L11 LED11();

#define RECON LEDRecordOn();
#define RECOFF LEDRecordOff();
#define PLAYON LEDPlaybackOn();
#define PLAYOFF LEDPlaybackOff();              
            
void wait_custom()
{
    static uint32_t j=0;
    for (j=0;j<16000;j++);
}

#define WAIT wait_custom();

I2C_EERAM_T eeram;
uint32_t i=0;

uint16_t readValue=0;

uint8_t buffer[8]={0xAB,0xCD,0xEF,0,0,0,0,0};
 
bool isMoving = false;

enum states {record, playback, hold, def} ;    

    uint8_t pressedOnce=0; 
    uint8_t liftOnce=0; uint8_t waitForButton=1;
    uint8_t currentState = playback;
    uint16_t writeAddress = START_ADDRESS;
    uint16_t playbackAddress =START_ADDRESS;
    uint8_t logData = 3;
    uint8_t readData =3 ;
    uint8_t timeout =0;
    uint32_t previousConversion12 =55, currentConversion12 =66, currentConversion=0;
    
    void lightUp(uint8_t data){
          switch (data){
                      case 0: L0; break;
                      case 1: L1; break;
                      case 2: L2; break;
                      case 3: L3; break;
                      case 4: L4; break;
                      case 5: L5; break;
                      case 6: L6; break;
                      case 7: L7; break;
                      case 8: L8; break;
                      case 9: L9; break;
                      case 10: L10; break;
                      case 11: L11; break;    
              case 12: break;
                    }   
    }
    
    
    uint8_t count[2];
    uint8_t firstRead=1;
    uint16_t readLimit=0;
    uint8_t releaseFirstRead=0;
    
uint8_t flushBuffer[32]={13,13,13,13,13,13,13,13, 13,13,13,13,13,13,13,13, 13,13,13,13,13,13,13,13, 13,13,13,13,13,13,13,13};
void main(void)
{    
    uint32_t k=0;
    uint16_t blink=0;  
    
    SYSTEM_Initialize();
    I2C_EERAM_Initialize(&eeram);
       
    ADC_SelectChannel(POT_CHANNEL);
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    
    TMR0_SetInterruptHandler(TMR0_CustomStuff); 
    
    I2C_EERAM_AutoStoreEnableBitSet	(&eeram, 1);
    
    TRISBbits.TRISB5=0;   
    TRISBbits.TRISB7=0;
   
    TRISCbits.TRISC4=1;
    
    currentState = playback; logData=0; readData=1; pressedOnce=0; liftOnce=0;
  
    while (1)
    {
        if (logData==1)
        {
            playbackAddress=START_ADDRESS;            
            LEDRecordOn();
            LEDPlaybackOff();
            
            count[0]=(writeAddress&0xFF00)>>8;
            count[1]=(writeAddress&0x00FF);
            
            I2C_EERAM_ByteWrite	   ( &eeram, buffer[0], writeAddress);
            I2C_EERAM_SequentialWrite	   ( &eeram, count, 2, 0x199E);
            if (writeAddress >= RECORDSIZE)
            {
                writeAddress = START_ADDRESS;          
            }    
            else writeAddress+=1;
            logData = 0;
        }
        
        if (readData==1)
        {
            writeAddress=START_ADDRESS;
            if(firstRead==1)
            {
                I2C_EERAM_RandomAddressRead ( &eeram, count, 2, 0x199E);                
                readLimit=(((uint16_t)count[0])<<8)|count[1];
                firstRead=0;                
                
                LEDPlaybackOff();    
                for (blink=0;blink<2000;blink++)
                {
                    L0 L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11
                }
                LEDPlaybackOn();  
            }
            LEDRecordOff();
            LEDPlaybackOn();
        
            I2C_EERAM_RandomAddressRead  (&eeram, buffer, 1, playbackAddress);
            lightUp(buffer[0]);            
            
            playbackAddress+=1;
            if (playbackAddress == RECORDSIZE || playbackAddress==readLimit) 
            {               
                playbackAddress =START_ADDRESS;
                LEDPlaybackOff();
                for (blink=0;blink<2000;blink++)
                {
                    L0 L1 L2 L3 L4 L5 L6 L7 L8 L9 L10 L11
                }
                LEDPlaybackOn();
            }
            readData = 0;
        }
    }
}

void TMR0_CustomStuff(void)
{
    if (currentState == record)
    {               
        pressedOnce=0;
        buffer[0]=(ADC_GetConversion(POT_CHANNEL)/5461);
        lightUp(buffer[0]);                  
       
        if (releaseFirstRead==1)
        {
            firstRead=1;
            releaseFirstRead=0;
        }
         if (PORTCbits.RC4==1 && liftOnce==1) 
             currentState=playback;
         if (PORTCbits.RC4==1) liftOnce=1;  
         logData = 1;                  
    }
    
    if (currentState==playback)
    {          
         liftOnce=0;

         if (PORTCbits.RC4==0 && pressedOnce==1)
         {
             currentState=record;
             releaseFirstRead=1;

         }
         if (PORTCbits.RC4==0) 
             pressedOnce=1;  
        readData=1;                    
    }
}

void LED0(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
   
    TRISC&=0b11011111;
    LATC  |=0b00100000;  
    
    TRISA&=0b11101111;
    LATA  &=0b11101111; 
};
void LED1(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISC&=0b11011011;
    LATC  |=0b00100000;
};
void LED2(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISC&=0b11011111;
    LATC  |=0b00100000;
    
    TRISA&=0b11111011;
    LATA  &=0b11111011;    
};
void LED3(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISC&=0b11011111;
    LATC  |=0b11011111;
    
    TRISA&=0b11101111;
    LATA  |=0b00010000;    
};
void LED4(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISC&=0b11011111;
    TRISC&=0b11111011;
    LATC |=0b00000100;
};
void LED5(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISA&=0b11111011;
    LATA|=  0b00000100;  
    
    TRISC&=0b11011111;
    LATC&=  0b11011111;   
};
void LED6(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISA&=0b11101111;
    LATA  |=0b00010000;
    
    TRISC&=0b11111011;
    LATC &=0b11111011;   
};
void LED7(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISA&=0b11101111;
    LATA  &=0b11101111;
    
    TRISC&=0b11111011;
    LATC  |=0b00000100;    
};
void LED8(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISA&=0b11101111;
    TRISA&=0b11111011;
    LATA |=0b00010000;
};
void LED9(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISA&=0b11101111;
    TRISA&=0b11111011;
    LATA |=0b00000100;
};
void LED10(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISC&=0b11111011;
    LATC  |=0b00000100;
    
    TRISA&=0b11111011;
    LATA  &=0b11111011;    
};
void LED11(){
    TRISC=0xFF; TRISA=0xFF; LATC=0x00; LATA=0x00; 
    
    TRISA&=0b11111011;
    LATA  |=0b00000100;
    
    TRISC&=0b11111011;
    LATC  &=0b11111011;    
};

void LEDRecordOn(){ //b5
    LATBbits.LATB5=1;
};

void LEDRecordOff(){ //b5
    LATBbits.LATB5=0;
};


void LEDPlaybackOn(){ //b7
    LATBbits.LATB7=1;
    
};

void LEDPlaybackOff(){ //b7
    LATBbits.LATB7=0;
    
};