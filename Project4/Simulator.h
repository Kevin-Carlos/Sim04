/**
* File: Simulator.h
*
* Details: Runs the simulation timing processes
*
* Version 1.00 Initial
*
* Notes: None
*
*/

//Include Guards
#ifndef __SIMULATOR__
#define __SIMULATOR__

//
// Libraries
//
#include "MetaData.h"
#include "Logger.h"
#include "PCB.h"
#include <ctime>
#include <chrono>

//
// Preprocessor directives
//


//Enum scope for scheduling algorithm
enum Scheduler {
   FIFO = 0 ,
   PS = 1 ,
   SJF = 2
};

class Simulator
{
public:
   //Object declarations
   PCB      PCBobj;
   Logger   log;

   //Variables
   int		maxMemory ,
            blockSize ,
            address ,
            HDDQuantity = 0 ,
            HDDMax ,
            projMax ,
            projQuantity = 0 ,
            schedAlg ,
            count = 0;

   std::string processType;

   std::string stream;

   map<string , string>::iterator it;


   //Methods
   void runSim ( MetaData& , ConfigData& );
   void processData ( MetaData& , ConfigData& );
   void setData ( MetaData& , ConfigData& );
   void setScheduler ( MetaData& , ConfigData );
   void setMemoryBlockSize ( MetaData& , ConfigData );
   void setMaxMemory ( MetaData& , ConfigData );
   void setQuantities ( MetaData& , ConfigData );
   void getFileAndMethod ( MetaData& , ConfigData );
   bool getAddress ( int );
};
#endif
