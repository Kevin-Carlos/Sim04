/**
* @file Timer.h
*
* Details: A class declaring methods for a timer
*
* Version: 1.00 Kevin Carlos
*          Initial development
*
* Notes: none
*/

// Directives //////////////////////////////////////////////////////////////////
#ifndef __TIMER_H
#define __TIMER_H

// Header files ////////////////////////////////////////////////////////////////
#include <sys/time.h>
#include <ctime>


class Timer
{
public:
   //Methods
   void initTimer ( );
   void start ( );
   void stop ( );
   long double elapsedSec ( );
   long double elapsedMicro ( );

private:
   //Variables
   struct timeval startTime , stopTime , duration;
   long double initTime , elapsedTime;
};

#endif
