/**
* @file Logger.h
*
* @author Kevin Carlos
*
* @version 1.00
*
* @date 3/26/18
*
* @details This is the class definition file for logging output
*
*/

// Include Guards //////////////////////////////////////////////////////////////
#ifndef __LOGGER
#define __LOGGER

// Header Files ////////////////////////////////////////////////////////////////
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "ConfigData.h"
#include "Timer.h"

class Logger
{
public:

   //Constructor
   Logger ( );

   //Destructor
   ~Logger ( );

   //Methods
   void setFilePath ( std::string );
   void setOutputMethod ( std::string );
   void initTimer ( );
   void output ( std::string );
   void outputMemory ( std::string , int );
   void sendFileStream ( );

private:
   //Variables
   int                  fileOutput;
   int                  IOutput;
   std::string          filePath ,
      outputMethod;
   ofstream             outputToFile;
   std::ostringstream   fout;

   //Objects
   Timer                timer;
};

#endif