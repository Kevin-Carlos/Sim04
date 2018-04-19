/**
* File: main.cpp
*
* Details: Driver file for the Sim02 project
*
* Version 1.00 Initial Development
*            Kevin Carlos 2/2/18
*
* Version 1.01 Editing for Project2
*
* Collaborators: Brian M., Nathan T.
*
*
* Notes: argv[1] should be the name of the config file, which will be passed
*            to the class to read in the data
*/

//
//Header files
//
#include "ConfigData.h"
#include "MetaData.h"
#include "Simulator.h"

int main ( int argc , char *argv [ ] )
{
   //Class Object declarations
   ConfigData myConfig;
   MetaData myMeta;
   Simulator sim;

   //Variables

   //Read in Config file and set data
   myConfig.fileReadIn ( argv [ 1 ] );

   //Read in metaData file
   myMeta.metaFileReadIn ( myConfig.getMetaFile ( ) );

   //Need to pass give Simulator both my configMap and the queues in meta
   //sim.getMetaData ( myMeta );
   sim.runSim ( myMeta , myConfig );
}