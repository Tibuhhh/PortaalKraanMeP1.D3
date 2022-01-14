/*
 */

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "LiquidCrystalI2CDevice.h"
#include "i2c_master.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "stepperMaster.h"
#include "h_bridge.h"
//alle inlcudes die nodig zijn om code werkend te krijgen

#define CLK PB0 //53
#define DTL PB1 //52
#define LED1 PB7
#define LED2 PB6
#define LED3 PB5
#define LED4 PB4
#define KNOPA PD2 //19
//alle pin defines. kan bewerkt worden als dat nodig is

#define DEBOUNCE_TIME  100

#define stepMode kwart
//deze debounce define wordt niet gebruikt
int counter = 0;
//deze int counter wodt niet gebruikt
int huidig;
// de status die rotary op dit moment heeft
int laatst;
//laatst status Rotary zodat je weet welke kant hij is geweest
static int getal[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};


void uitvoering(int bevestigd)
{
    init_h_bridge();


    if (bevestigd == 0)
    {
        stepperSetHome(stepMode, links);
        // in deze if statement kan je alle mogelijke output schrijven

    }
     if (bevestigd == 1)
    {
         stepperGoto(1, rechts, stepMode);
         bewegen(2);

        stepperGoto(1, links, stepMode);
          // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 2)
    {
           stepperGoto(2, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(2, links, stepMode);
            // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 3)
    {
           stepperGoto(3, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(3, links, stepMode);
            // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 4)
    {
        stepperGoto(4, rechts, stepMode);

        bewegen(1);

        stepperGoto(4, links, stepMode);
         // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 5)
    {
           stepperGoto(5, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(5, links, stepMode);
            // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 6)
    {
          stepperGoto(6, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(6, links, stepMode);
            // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 7)
    {
            stepperGoto(7, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(7, links, stepMode);
              // in deze if statement kan je alle mogelijke output schrijven
    }

     if (bevestigd == 8)
    {
        stepperGoto(8, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(8, links, stepMode);
//          in deze if statement kan je alle mogelijke output schrijven

    }
        if (bevestigd == 9)
    {
        stepperGoto(9, rechts, stepMode);
        _delay_ms(1000);
        stepperGoto(9, links, stepMode);
         // in deze if statement kan je alle mogelijke output schrijven

    }

    // als je meer outputs wilt dan kan je de if statement gaan copypasten en de getallen veranderen



}

void lcdnummer(int nummer)
{
    initStepper();
    stepperSetSpeed(1, stepMode);

     uitvoering(getal[nummer]);
     //hier wordt het getal dat is bevestigd gegeven aan de array

}




void initLcd(void)
{
    i2c_master_init(I2C_SCL_FREQUENCY_100);


}

void init(void)
{
    DDRB &= ~_BV(CLK);
    DDRB &= ~_BV(DTL);
    DDRD &= ~_BV(KNOPA);
    // register input

    DDRB |= _BV(LED1);
    DDRB |= _BV(LED2);
    DDRB |= _BV(LED3);
    DDRB |= _BV(LED4);
    //output register

    PORTB |= _BV(LED1);
    PORTB |= _BV(LED2);
    PORTB |= _BV(LED3);
    PORTB |= _BV(LED4);
    PORTD |= _BV(KNOPA);
    // porten hoog zetten

    eindStopLoc &= !_BV(eindStopPinX);


    laatst = (PINB & _BV(CLK));
    //status rotary switch

}

int main(void)
{


    initLcd();
    LiquidCrystalDevice_t lcd1 = lq_init(0x27, 16, 2, LCD_5x8DOTS);
    lq_turnOnBacklight(&lcd1);
// functies van LCD display
     init();



     // functie naar de registers
      float displaygetal = 0;
      //getal die op de display te voorschijn komt
      float i = 0.5;
      // elke keer als de rotary naar rechts gaat +0.5
      float q = 1.5;
      // elke keer als de rotary naar links gaat +1.5
      int wasin;
      //int die ik gebruik voor de rotary knop om de status steeds te updaten


    while(1)
    {


        if (PIND & _BV(KNOPA))
        {
            if (wasin == 0)
            {
            lq_setCursor(&lcd1, 1, 1);
            lq_print(&lcd1, "Bevestigd");
            lcdnummer(displaygetal);
              //als de knop op de rotary wordt ingedrukt wordt er "BEVESTIGD" uitgebeeld
              // de variabel "displaynummer" wordt naar functie

            wasin = 1;

            }
        }
        else
        {

              wasin = 0;
        }


        huidig = (PINB & _BV(CLK));

        if (huidig != laatst)
        {
            if ((PINB & _BV(DTL)) != laatst)
            {
                displaygetal = displaygetal + i;
                // elke keer als de rotary naar rechts gaan wordt er per pulse +0.5 gedaan. De rotary geeft 2 pulsen bij een keer draaien van daar + 0.5
                 lq_setCursor(&lcd1, 1, 1);
              lq_print(&lcd1, "_________");
              //________wordt gedaan zodat "BEVESTIGD"niet gaat zitten glitchen


            }
            else
            {
                displaygetal = displaygetal - q;
                 // elke keer als de rotary naar rechts gaan wordt er per pulse -1.5 gedaan. De reden hiervoor is dat de rotary eerste een pulse naar rechts geeft en dan pas naar links. Dus eerst
                 //dus eerst +0.5 en daarna -1.5
                 lq_setCursor(&lcd1, 1, 1);
              lq_print(&lcd1, "_________");
                //________wordt gedaan zodat "BEVESTIGD"niet gaat zitten glitchen

            }
        }
        laatst = huidig;
        //status updaten rotary switch



        if (displaygetal >= 0)
        {
             if (displaygetal == 0)
        {
            lq_setCursor(&lcd1, 0, 1);
            lq_print(&lcd1, "Locatie: 0");

        }

        if (displaygetal == 1)
        {
            lq_setCursor(&lcd1, 0, 1);
            lq_print(&lcd1, "Locatie: 1");

        }
         if (displaygetal == 2)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 2");

        }
         if (displaygetal == 3)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 3");

        }
         if (displaygetal == 4)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 4");

        }
         if (displaygetal == 5)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 5");

        }
         if (displaygetal == 6)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 6");

        }
         if (displaygetal == 7)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 7");

        }
         if (displaygetal == 8)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 8");

        }
            if (displaygetal == 9)
        {
             lq_setCursor(&lcd1, 0, 1);
             lq_print(&lcd1, "Locatie: 9");

        }
        //als je meer getallen wilt toevoegen kan dat. je hoeft dan alleen maar de if statement te copypasten en de getallen te verandern


        }



    }


    return 0;
}




