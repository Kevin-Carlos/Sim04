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
#define FIFO 0
#define PS 1
#define SJF 2

class Simulator
{
public:
   //Object declarations
   PCB      PCBobj;
   Logger   log;

   //Variables
   int		maxMemory ,
            blockSize ,
            address = 0 ,
            flag = 0 ,
            HDDQuantity = 0 ,
            HDDMax ,
            projMax ,
            projQuantity = 0 ,
            schedAlg ,
            processCount = 0 ,
            newProcessCount = 0 ,
            shortProcessCount = 100 ,
            appCount = 0 ,
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
   void findPrioApplication ( );
   void findShortestApp ( );
   bool getAddress ( );
};
#endif
