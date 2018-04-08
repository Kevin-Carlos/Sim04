/**
* File Timer.cpp
*
* Details Implementation of the Timer class
*
* Version 1.00 Kevin  Carlos
*         Initial development
*
* Notes none
*/

// Precompiler Directives //////////////////////////////////////////////////////
#ifndef __TIMER_CPP
#define __TIMER_CPP

// Header Files ////////////////////////////////////////////////////////////////
#include "Timer.h"
#include <iostream>

/**
* @function start
*
* @parameters none
*
* @details Sets an initial time of when the timer is started using the current
*            time of day to initialize it
*
*/
void Timer::start ( )
{
   gettimeofday ( &startTime , NULL );
   initTime = ( startTime.tv_sec * 1000000 + startTime.tv_usec );
}

/**
* @function stop
*
* @parameters none
*
* @details Stops the timer and sets the seconds and micro seconds of the timer
*/
void Timer::stop ( )
{
   gettimeofday ( &stopTime , NULL );
   duration.tv_sec = ( stopTime.tv_sec - startTime.tv_sec );
   duration.tv_usec = ( stopTime.tv_usec - startTime.tv_usec );
}

/**
* @function elapsedSec
*
* @parameters none
*
* @details Creates a variable called elapsed time and returns it with the
*            the correct precision
*
*/
long double Timer::elapsedSec ( )
{
   gettimeofday ( &duration , NULL );
   elapsedTime =
      ( ( duration.tv_sec * 1000000 + duration.tv_usec ) - initTime );
   elapsedTime = elapsedTime / 1000000; // To get it into seconds

   return elapsedTime;
}

/**
* @function elapsedMicro
*
* @parameters none
*
* @details Creates a variable called elapsed time and returns it with the
*            the correct precision, in this case in microseconds
*
*/
long double Timer::elapsedMicro ( )
{
   gettimeofday ( &duration , NULL );
   elapsedTime =
      ( duration.tv_sec * 1000000 + duration.tv_usec - initTime );
   return elapsedTime;
}

#endif