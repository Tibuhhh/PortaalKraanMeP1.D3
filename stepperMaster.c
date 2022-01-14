/*

    Stepper Motor Library V1.1

        Door Tim van Rosse, 9/12/21

    Changelog: (Voor verdere versies)
    - init aangepast
    - stepperSetSpeed gemaakt
    - stepperGoto aangepast naar OCR ipv TOV
    - Simpele stepperSetHome zonder knop interrupt (dus nog niks waard)

*/

/*
    Lijst met aan te roepen functies:
    - setDir
    - stepperGoTo(pos)
    - stepperSetSpeed
    - stepperSetHome

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "stepperMaster.h"


float snelheid;
int posNu;

void initStepper(void)
{
    stepperLoc |= _BV(stepperPin1);
    stepperLoc |= _BV(stepperDir1);
    stepperLoc |= _BV(stepperPin2);
    stepperLoc |= _BV(stepperDir2);

    TCCR1A = 0;
    TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10);

//    TIMSK1 = (1 << TOIE1);



    sei();

}

static unsigned int stepsOplag[] = {
    0x186A0, 100, 200, 300, 400, 500, 600, 700, 800, 900 //1000, 2000

};



void stepperGoto(int pos, int dir, int mode)
{

    if(dir)
    {
        stepperPort |= (1 << stepperDir1);
        stepperPort &= ~(1 << stepperDir2);
    }
    else
    {
        stepperPort &= ~(1 << stepperDir1);
        stepperPort |= (1 << stepperDir2);
    }

    int stepsTogo = stepsOplag[pos] * 2 * mode;
    int wachten;

    for(int i = 0; i < stepsTogo; i++)
    {
        while(wachten != 1)
        {
           if(TIFR1 & (1 << OCF1A))
            {
                stepperPort ^= (1 << stepperPin1);
                stepperPort ^= (1 << stepperPin2);
                TCNT1 = 0;
                OCR1A = snelheid;
                wachten = 1;
                TIFR1 = (1 << OCF1A);

            }


        }
        wachten = 0;


    }
}

void stepperSetSpeed(float rps, int mode)
{
    float rekenwaarde = rps * 400 * mode;
    snelheid = 16000000 / rekenwaarde;
}

void stepperSetHome (int mode, int dir)
{
    stepperSetSpeed(0.5, mode);
    stepperGoto(0, dir, mode);
}
