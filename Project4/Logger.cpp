/**
* @file Logger.cpp
*
* @author Kevin Carlos
*
* @version 1.00
*
* @date 3/26/18
*
* @details This file implements the Logger class which will handle all output
*
*/


// Header Files ////////////////////////////////////////////////////////////////
#include "Logger.h"

//Constructor to initalize settings if need be
Logger::Logger ( )
{
   fileOutput = 1;
   IOutput = 1;
}

//Destructor to dump the stringstream
Logger::~Logger ( )
{
   if ( fileOutput )
      sendFileStream ( );
}

//Start the timer
void Logger::initTimer ( )
{
   timer.start ( );
}

/**
* @function setMethod
*
* @parameters std::string arg, this is the string passed in to determine
*               which method will be the output
*
*/
void Logger::setFilePath ( std::string file )
{
   filePath = file;
}

/**
* @function setOutputMethod
*
* @parameters method: a string to be compared to set the output method
*
*/
void Logger::setOutputMethod ( std::string method )
{
   outputMethod = method;
   if ( outputMethod == "Log to File" )
   {
      fileOutput = 1;
      IOutput = 0;
   }
   else if ( outputMethod == "Log to Monitor" )
   {
      fileOutput = 0;
      IOutput = 1;
   }
   else //Default will be Log to Both
   {
      fileOutput = 1;
      IOutput = 1;
   }
}

/**
* @function output
*
* @parameters log: the built message to output to file or console
*/
void Logger::output ( std::string log )
{
   if ( IOutput )
      std::cout << fixed << setprecision ( 6 ) << setw ( 8 )
      << timer.elapsedSec ( ) << log << std::endl;
   if ( fileOutput )
      fout << fixed << setprecision ( 6 ) << setw ( 8 )
      << timer.elapsedSec ( ) << log << std::endl;
}

/**
* @function outputMemory
*
* @parameters
*      log: the built message to output to file or console
*      address: the current address that is being allocated
*
* @details a special purpose output function for memory to manipulate its
*            precision and output
*
*/
void Logger::outputMemory ( std::string log , int address )
{
   if ( IOutput )
      std::cout << fixed << setprecision ( 6 ) << setw ( 8 )
      << timer.elapsedSec ( ) << log << setfill ( '0' ) << setw ( 8 )
      << hex << uppercase << address << std::endl;
   if ( fileOutput )
      fout << fixed << setprecision ( 6 ) << setw ( 8 )
      << timer.elapsedSec ( ) << log << setfill ( '0' ) << setw ( 8 )
      << hex << uppercase << address << std::endl;
}

/**
* @function sendFileStream
*
* @parameters none
*
* @details The function called from the destructor to dump the stringstream
*            buffer if outputting to a file
*/
void Logger::sendFileStream ( )
{
   outputToFile.open ( filePath );
   if ( outputToFile.good ( ) )
   {
      outputToFile << fout.str ( );
      outputToFile.close ( );
   }
}