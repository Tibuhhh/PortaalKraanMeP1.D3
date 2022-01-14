#ifndef STEPPERMASTER_H_INCLUDED
#define STEPPERMASTER_H_INCLUDED

#define stepperLoc DDRA
#define stepperPort PORTA
#define stepperDir1 PA1 //23, onderste
#define stepperPin1 PA0 //22

#define stepperDir2 PA3 //25
#define stepperPin2 PA2 //24


#define eindStopLoc DDRE
#define eindStopPort PINE
#define eindStopPinX PE4
#define eindStopPin2 PB6

#define rechts 0
#define links 1

#define heel 1
#define half 2
#define kwart 4
#define achtste 8

void initStepper(void);
void stepperGoto(int pos, int dir, int mode);
void stepperSetSpeed(float rps, int mode);
void stepperSetHome(int mode, int dir);


#endif // STEPPERMASTER_H_INCLUDED
