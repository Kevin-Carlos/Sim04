/**
* File: Simulator.cpp
*
* Details: Implementation of Simulator
*
* Version: 1.00 Initial development
*
* Notes: None
*
*
*/
#include "Simulator.h"
#include <iostream>

#define DEBUG 0

/**
* @function runSim
*
* @parameters class objects for MetaData and ConfigData
*
* @details This function operates the entire OS program setting data
*            and running proccesses
*
*/
void Simulator::runSim ( MetaData &myMeta , ConfigData &myConfig )
{
   //Variables
   size_t size;

   //First create a vector of a struct called Data which holds all relevent info
   setData ( myMeta , myConfig );

   //Set a flag schedAlg to determine which scheduling algorithm to use
   setScheduler ( myMeta , myConfig );

   //Set the max memory 
   setMaxMemory ( myMeta , myConfig );

   //Set the memory to allocate to the correct address
   setMemoryBlockSize ( myMeta , myConfig );

   //Set HDD and projector quantity
   setQuantities ( myMeta , myConfig );

   /* Determine log output method
   Set the method in logging class
   Send Logger class the file path */
   getFileAndMethod ( myMeta , myConfig );

   //Start logging
   log.initTimer ( );

   /**
   * Determine the index to begin with based on the scheduling algorithm.
   * With FIFO, the first index will always be beginning + 1
   *
   * This function will call another function that will execute the processes
   * within at the correct index until the vector is empty.
   *
   */
   //Debug run thru the vector
   for ( int i = 0 , size = PCBobj.dataVec.size ( ); i < size; ++i )
   {
      cout << "Code: " << PCBobj.dataVec [ i ].code << "\n";
      cout << "Key: " << PCBobj.dataVec [ i ].key << "\n";
      cout << "ConfigTime: " << PCBobj.dataVec [ i ].configTime << "\n";
      cout << "MetaTime: " << PCBobj.dataVec [ i ].metaTime << "\n";
      cout << "ProcessTime: " << PCBobj.dataVec [ i ].processTime << "\n";
   }

}

void Simulator::processData ( MetaData& , ConfigData& )
{
   //Variables
   const string   BEGIN = "begin";
   const string   FINISH = "finish";
   const string   ALLOCATE = "allocate";
   const string   HDD = "HDD";
   const string   PROJ = "PROJ";
   string         temp;
   int            flag = 0 ,
      num;
   bool           allocation;

   while ( !PCBobj.dataVec.empty ( ) )
   {

      switch ( PCBobj.dataVec.front ( ).code )
      {

      case 'S':

         if ( PCBobj.dataVec.front ( ).key == BEGIN )
         {
            stream = " - Simulator program starting";
            log.output ( stream );
         }
         else if ( PCBobj.dataVec.front ( ).key == FINISH )
         {
            stream = " - Simulator program ending";
            log.output ( stream );
         }
         break;

      case 'A':

         if ( PCBobj.dataVec.front ( ).key == BEGIN )
         {
            //Increment count to say correspond to process
            count++;
            stream = " - OS: preparing process " + to_string ( count );
            log.output ( stream );
            stream = " - OS: starting process " + to_string ( count );
            log.output ( stream );
         }
         else if ( PCBobj.dataVec.front ( ).key == FINISH )
         {
            stream = " - OS: removing process " + to_string ( count );
            log.output ( stream );
         }
         break;

      case 'P':

         stream = " - Process " + to_string ( count )
            + " - start processing action";
         log.output ( stream );

         //Pass to gen handler
         PCBobj.genHandler ( );

         stream = " - Process " + to_string ( count ) +
            " - end processing action";
         log.output ( stream );
         break;

      case 'I':
         //Set operation to input
         processType = "input";

         if ( PCBobj.dataVec.front ( ).key == "projector" )
            num = 0;
         else if ( PCBobj.dataVec.front ( ).key == "hard drive" )
            num = 1;
         else
            num = 100;

         switch ( num )
         {
         case 0:
            if ( projQuantity >= projMax )
               projQuantity = 0;
            temp = " on " + PROJ + " " + to_string ( projQuantity );
            projQuantity++;
            break;

         case 1:
            if ( HDDQuantity >= HDDMax )
               HDDQuantity = 0;
            temp = " on " + HDD + " " + to_string ( HDDQuantity );
            HDDQuantity++;
            break;

         default:
            temp = "";
            num = 100;
            break;
         }

         stream = " - Process " + to_string ( count ) + " - start " +
            PCBobj.dataVec.front ( ).key + " " + processType + temp;
         log.output ( stream );

         //Send this to the PCB to handle
         PCBobj.IOHandler ( );

         stream = " - Process " + to_string ( count ) + " - end " +
            PCBobj.dataVec.front ( ).key + " " + processType;
         log.output ( stream );

         break;

      case 'O':
         //Set operation to input
         processType = "output";

         if ( PCBobj.dataVec.front ( ).key == "projector" )
            num = 0;
         else if ( PCBobj.dataVec.front ( ).key == "hard drive" )
            num = 1;
         else
            num = 100;

         switch ( num )
         {
         case 0:
            if ( projQuantity >= projMax )
               projQuantity = 0;
            temp = " on " + PROJ + " " + to_string ( projQuantity );
            projQuantity++;
            break;

         case 1:
            if ( HDDQuantity >= HDDMax )
               HDDQuantity = 0;
            temp = " on " + HDD + " " + to_string ( HDDQuantity );
            HDDQuantity++;
            break;

         default:
            temp = "";
            num = 100;
            break;
         }

         stream = " - Process " + to_string ( count ) + " - start " +
            PCBobj.dataVec.front ( ).key + " " + processType + temp;
         log.output ( stream );

         //Send this to the PCB to handle
         PCBobj.IOHandler ( );

         stream = " - Process " + to_string ( count ) + " - end " +
            PCBobj.dataVec.front ( ).key + " " + processType;
         log.output ( stream );

         break;

      case 'M':

         //If memory allocate
         if ( PCBobj.dataVec.front ( ).key == ALLOCATE )
         {
            stream = " - Process " + to_string ( count ) +
               " - allocating memory";
            log.output ( stream );

            //Get the address
            allocation = getAddress ( flag );
            flag = 1; //So its not 0 from hereon

                      //Pass to gen handler
            PCBobj.genHandler ( );

            switch ( allocation )
            {

            case false:
               stream = " - Process " + to_string ( count ) +
                  " - memory could not be allocated. Max memory size reached.";
               log.output ( stream );
               break;

            case true:
               stream = " - Process " + to_string ( count ) +
                  " - memory allocated at 0x";
               log.outputMemory ( stream , address );
               break;

            default:
               std::cout << "Error occured in allocation switch...\n";
               break;
            }
         }
         else
         {
            stream = " - Process " + to_string ( count ) +
               " - start memory blocking";
            log.output ( stream );

            //Pass to gen handler
            PCBobj.genHandler ( );

            stream = " - Process " + to_string ( count ) +
               " - end memory blocking";
            log.output ( stream );
         }

         break;

      default:

         cout << "Error with the switch codes...\n";
         break;
      }

      /**
      *
      * Need to pop from the necessary index in the vector
      *
      */
      //PCBobj.dataVec.pop ( );

      if ( DEBUG == 1 )
      {
         /*cout << "Code: " << PCBobj.dataQueue.front( ).code << "\n";
         cout << "Key: " << PCBobj.dataQueue.front ( ).key << "\n";
         cout << "ConfigTime: " << PCBobj.dataQueue.front( ).configTime << "\n";
         cout << "MetaTime: " << PCBobj.dataQueue.front( ).metaTime << "\n";
         cout << "ProcessTime: "
         << PCBobj.dataQueue.front( ).processTime << "\n";*/
      }
   }
}

/**
* @function getAddress
*
* @parameters entered: a flag to tell the switch case whether or not
*               this function has been accessed before, need a better solution
*
* @details This function sets the memory and checks whether or not the memory
*            block size has reached the maximum memory capacity. It sets
*            the current address allocation to be used.
*
*/
bool Simulator::getAddress ( int entered )
{
   bool allocated;

   switch ( entered )
   {

   case false: //So first time called the address is 0
      address = 0;
      allocated = true;
      break;

   case true:
      address += blockSize;
      allocated = true;
      if ( blockSize > maxMemory )
      {
         address -= blockSize;
         allocated = false;
      }
      break;

   default:
      std::cout << "Error in getting the current address...\n";
      break;
   }

   return allocated;
}

/**
* @function setScheduler
*
* @parameters Takes in the class objects for MetaData and ConfigData
*
*
* @details Sets the scheduling algorithm used for the simulation
*            FIFO = 0, PS = 1, SJF = 2;
*/
void Simulator::setScheduler ( MetaData &myMeta , ConfigData myConfig )
{
   const string CPU = "cpu"; //lookup key for scheduling type
   string temp;

   //Grab the data of the key
   it = myConfig.configMap.find ( CPU );
   temp = it->second;

   //Determine the algorithm used
   if ( temp == "PS" )
      schedAlg = 1;
   else if ( temp == "SJF" )
      schedAlg = 2;
   else
      schedAlg = 0; //Default it to FIFO if nothing else
}

/**
* @function getFileAndMethod
*
* @parameters Takes in the class objects for MetaData and ConfigData
*
* @details Sets the log file output path and the log output method using
*            the information from MetaData and ConfigData
*/
void Simulator::getFileAndMethod ( MetaData &myMeta , ConfigData myConfig )
{
   const string PATH = "path";
   const string LOG = "log";
   string temp;

   //Grab and send file path
   it = myConfig.configMap.find ( PATH );
   temp = it->second;
   temp = temp.substr ( 0 , temp.find ( '\r' ) );

   log.setFilePath ( temp );

   //Grab and send output method
   it = myConfig.configMap.find ( LOG );
   temp = it->second;
   log.setOutputMethod ( temp );

}

/**
* @function setMemoryBlockSize
*
* @parameters Takes in the class objects for MetaData and ConfigData
*
* @details sets the memory block size
*/
void Simulator::setMemoryBlockSize ( MetaData &myMeta , ConfigData myConfig )
{
   const string BLOCK = "block"; // Key value for memory block size
   string temp;
   it = myConfig.configMap.find ( BLOCK );
   temp = it->second;
   blockSize = stoi ( temp );
}


/**
* @function setMaxMemory
*
* @parameters Takes in the class objects for MetaData and ConfigData
*
* @details setMaxMemory This function sets the max amount of memory for use
*
*/
void Simulator::setMaxMemory ( MetaData &myMeta , ConfigData myConfig )
{
   const string SYSTEM = "system"; //Key value in map for memory
   string temp;
   it = myConfig.configMap.find ( SYSTEM );
   temp = it->second;
   maxMemory = stoi ( temp );
}


/**
* @function setQuantities
*
* @parameters Takes in the class objects for MetaData and ConfigData
*
* @details This function sets the quantities for hard drive and projector
*            to count which one is in use
*/
void Simulator::setQuantities ( MetaData &myMeta , ConfigData myConfig )
{
   const string HDDNUM = "hard drive quantity";
   const string PROJNUM = "projector quantity";
   string temp;

   //Set HDD
   it = myConfig.configMap.find ( HDDNUM );
   temp = it->second;
   HDDMax = stoi ( temp );

   //Set Projector
   it = myConfig.configMap.find ( PROJNUM );
   temp = it->second;
   projMax = stoi ( temp );
}

/**
* @function setData
*
* @parameters This function takes in the class objects or MetaData and
*               ConfigData
*
* @details This function takes key value from the meta data to grab the
*            configTime of the corresponding key value to process the total
*            time a certain metaData item will take. This information is all
*            put into a struct called Data to be used later
*
*/
void Simulator::setData ( MetaData &myMeta , ConfigData &myConfig )
{
   //Variables
   string   temp ,
      temp2;

   while ( !myMeta.intQueue.empty ( ) )
   {
      //Set code
      PCBobj.data.code = myMeta.charQueue.front ( );
      //cout << "DataCode: " << PCBobj.data.code << "\n";
      myMeta.charQueue.pop ( );

      //Set metaKey
      myMeta.stringQueue.pop ( ); //Get it to a key value, not the entire string
      PCBobj.data.key = myMeta.stringQueue.front ( );
      //std::cout << "DataKey: " << PCBobj.data.key << "\n";
      myMeta.stringQueue.pop ( ); //Back to the entire string

                                  //Set metaTime
      PCBobj.data.metaTime = myMeta.intQueue.front ( );
      //std::cout << "MetaTime: " << PCBobj.data.metaTime << "\n";
      myMeta.intQueue.pop ( );

      //Set configTime for corresponding metaTime
      if ( PCBobj.data.key == "begin" || PCBobj.data.key == "finish" )
         temp = "null";
      else if ( PCBobj.data.key == "run" )
         temp = "processor";
      else if ( PCBobj.data.key == "allocate" || PCBobj.data.key == "block" )
         temp = "memory";
      else
         temp = PCBobj.data.key;

      if ( temp == "null" )
         PCBobj.data.configTime = 0;
      else {
         it = myConfig.configMap.find ( temp );
         temp2 = it->second;
         PCBobj.data.configTime = stoi ( temp2 );
      }
      //std::cout << "ConfigTime: " << PCBobj.data.configTime << "\n";
      //Set the processTime
      PCBobj.data.processTime = PCBobj.data.configTime * PCBobj.data.metaTime;
      //std::cout << "ProcessTime: " << PCBobj.data.processTime << "\n";

      //Push all of this onto the dataQueue
      PCBobj.dataVec.push_back ( PCBobj.data );
   }
}

