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
   int exit = 0;

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

   //Set Quantum time
   setQuantum ( myMeta , myConfig );

   //Set PCB time scheduler thing
   PCBobj.setAlg ( schedAlg ); 

   /* Determine log output method
   Set the method in logging class
   Send Logger class the file path */
   getFileAndMethod ( myMeta , myConfig );

   //Initialize semaphores
   sem_init ( &PCBobj.HDDSem , 0 , HDDMax );
   sem_init ( &PCBobj.projSem , 0 , projMax );

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
   switch ( schedAlg )
   {
      //FIFO
   case 0:
      //Set the index to the beginning since its FIFO
      PCBobj.jobIndex = PCBobj.dataVec.begin ( );
      while ( !PCBobj.dataVec.empty ( ) )
      {
         processData ( myMeta , myConfig );

         /**
         *
         * Need to pop from the necessary index in the vector
         *
         */
         PCBobj.dataVec.erase ( PCBobj.jobIndex );
      }
      break;

      //PS
   case 1:
      //Initialize vector iterator
      PCBobj.jobIndex = PCBobj.dataVec.begin ( );
      //Data is erased within the processing
      processData ( myMeta , myConfig );

      //While the vector is not empty, process
      while ( exit == 0 )
      {
         //Find the application to begin with
         findPrioApplication ( );

         do
         {
            if ( DEBUG == 1 )
            {
               cout << "Code: " << PCBobj.jobIndex->code << "\n";
               cout << "Key: " << PCBobj.jobIndex->key << "\n";
               cout << "ConfigTime: " << PCBobj.jobIndex->configTime << "\n";
               cout << "MetaTime: " << PCBobj.jobIndex->metaTime << "\n";
               cout << "ProcessTime: " << PCBobj.jobIndex->processTime << "\n";
            }

            if ( PCBobj.dataVec.size ( ) == 1 ) {
               exit = 1;
               break;
            }
            processData ( myMeta , myConfig );

            /**
            *
            * Need to pop from the necessary index in the vector
            *
            */
            PCBobj.dataVec.erase ( PCBobj.jobIndex );

            PCBobj.endIndex--;
         } while ( PCBobj.jobIndex != ( PCBobj.endIndex ) );
         appCount--;

         processData ( myMeta , myConfig );

         /**
         *
         * Need to pop from the necessary index in the vector
         *
         */
         PCBobj.dataVec.erase ( PCBobj.jobIndex );
      }
      //Process any leftovers
      //processData ( myMeta , myConfig );
      break;

      //SJF
   case 2:
      //Initialize vector iterator
      PCBobj.jobIndex = PCBobj.dataVec.begin ( );
      //Data is erased within the processing
      processData ( myMeta , myConfig );

      while ( !PCBobj.dataVec.empty ( ) )
      {
         //Find the first application to begin with
         findShortestApp ( );

         do
         {
            processData ( myMeta , myConfig );

            /**
            *
            * Need to pop from the necessary index in the vector
            *
            */
            PCBobj.dataVec.erase ( PCBobj.jobIndex );

            PCBobj.endIndex--;
         } while ( PCBobj.jobIndex != PCBobj.endIndex + 1 );

         if ( PCBobj.dataVec.size ( ) == 1 )
         {
            processData ( myMeta , myConfig );

            /**
            *
            * Need to pop from the necessary index in the vector
            *
            */
            PCBobj.dataVec.erase ( PCBobj.jobIndex );
         }
      }

      break;

      //Round Robin
   case 3:
      int temp;
      bool success;
      //Initialize iterator
      PCBobj.jobIndex = PCBobj.dataVec.begin ( );

      processData ( myMeta , myConfig );
      //Erase simulation started
      PCBobj.dataVec.erase ( PCBobj.jobIndex );
      //cin >> temp;
      PCBobj.jobIndex++;

      while ( !PCBobj.dataVec.empty ( ) )
      {
         processComplete = 0;
         //Now find the first application to run
         success = findApplication ( );

         processData ( myMeta , myConfig );
         PCBobj.jobIndex++;

         if ( success == 1 )
         {
            while ( processComplete == 0 )
            {
               taskCount = 0; // Need to ensure its reset every iteration
                              // Just how this works

               if ( PCBobj.jobIndex->visited == true )
               {

                  if ( PCBobj.jobIndex->code != 'A' &&
                     ( PCBobj.jobIndex->key != "begin" ||
                        PCBobj.jobIndex->key != "finish" ) )
                     taskCount--;

                  if ( taskCount < 0 )
                     taskCount = 0;

                  PCBobj.jobIndex++;
               }
               else
               {
                  //If its less than or equal to 0 this has been processed
                  if ( PCBobj.jobIndex->processTime <= 0 )
                  {
                     PCBobj.jobIndex->visited = true;

                     PCBobj.jobIndex++;
                  }
                  //If the processTime is greater than or equal to the quantum
                  //Run the process for the quantum time and set the 
                  //algorithm to handle this accordingly
                  else if ( PCBobj.jobIndex->processTime >= quantum )
                  {
                     PCBobj.setAlg ( 3 );

                     processData ( myMeta , myConfig );

                     PCBobj.jobIndex->processTime -= quantum;
                     PCBobj.jobIndex++; 

                     taskCount++;
                  }
                  //Otherwise set the algorithm to run for the processTime 
                  //Because its less than the quantum and can run for its full
                  //Duration
                  else
                  {
                     PCBobj.setAlg ( 5 );

                     processData ( myMeta , myConfig );

                     PCBobj.jobIndex->processTime -= PCBobj.jobIndex->processTime;
                     PCBobj.jobIndex++;
                     taskCount++;
                  }

                  //If I reach the end of the process and all tasks are complete
                  //Erase this process from my vector 
                  if ( PCBobj.jobIndex->key == PCBobj.endIndex->key &&
                     taskCount == 0 )
                  {
                     //Everything in this application is done, end application
                     PCBobj.jobIndex = PCBobj.endIndex;
                     processData ( myMeta , myConfig );

                     //Erase application
                     PCBobj.dataVec.erase 
                     ( PCBobj.tempIndex , PCBobj.endIndex + 1 );

                     //Break out of loop to find next application to run
                     processComplete = 1;
                  }
                  else if ( PCBobj.jobIndex->code == PCBobj.endIndex->code )
                  {
                     //Reset to beginning of process to redo tasks not finished
                     PCBobj.jobIndex = PCBobj.tempIndex;
                  }
               }
            }
         }
         //If no application was found just process what is next
         else
         {
            processData ( myMeta , myConfig );
         }

         //Need to handle the last thing in the vector to not seg fault yay
         if ( PCBobj.dataVec.size ( ) == 1 )
         {
            PCBobj.jobIndex = PCBobj.dataVec.begin ( );
            processData ( myMeta , myConfig );
            PCBobj.dataVec.erase ( PCBobj.jobIndex );
         }
      }

      break;

   default:
      cout << "Error with switch code for scheduling algorithm...\n";
      break;

   }
}


bool Simulator::findApplication ( )
{
   //Variables
   int appCount = 0;
   bool success;

   //Now that simulation has started, determine which app to run
   for ( PCBobj.processIndex = PCBobj.dataVec.begin ( );
      PCBobj.processIndex != PCBobj.dataVec.end ( );
      ++PCBobj.processIndex )
   {
      //If A{begin} save the index
      if ( PCBobj.processIndex->code == 'A' &&
         PCBobj.processIndex->key == "begin" && 
         appCount == 0 )
      {
         PCBobj.jobIndex = PCBobj.processIndex;
         PCBobj.tempIndex = PCBobj.processIndex;
         appCount++;
      }

      //If A{finish} save the index
      if ( PCBobj.processIndex->code == 'A' &&
         PCBobj.processIndex->key == "finish" &&
         appCount == 1 )
      {
         //cout << "Found finish...\n";
         PCBobj.endIndex = PCBobj.processIndex;
         appCount++; // Dont want to continually access this
      }
   }
   
   if ( PCBobj.jobIndex->code == 'A' && 
      PCBobj.endIndex->code == 'A' )
      success = true;
   else
      success = false; 
}

/**
  * @function findShortestApp
  *
  * @details finds the iterator within the vector for the beginning of the 
  *            application that has the shortest amount of processes
*/
void Simulator::findShortestApp ( )
{
   //Now that simulation has started, determine which app to run
   for ( PCBobj.processIndex = PCBobj.dataVec.begin ( );
      PCBobj.processIndex != PCBobj.dataVec.end ( );
      ++PCBobj.processIndex )
   {
      //If A{begin} save the index
      if ( PCBobj.processIndex->code == 'A' &&
         PCBobj.processIndex->key == "begin" ) {
         PCBobj.tempIndex = PCBobj.processIndex;
         appCount++;
         //cout << "App count: " << appCount << "\n";
         //cout << "Found begin...\n";
      }

      //If the operation is I/O update count
      if ( PCBobj.processIndex->code == 'I' ||
         PCBobj.processIndex->code == 'O' || 
         PCBobj.processIndex->code == 'P' ||
         PCBobj.processIndex->code == 'M' ) {
         //cout << "Found process...\n";
         newProcessCount++;
      }

      //If A{finish} save the index
      if ( PCBobj.processIndex->code == 'A' &&
         PCBobj.processIndex->key == "finish" )
      {
         //cout << "Found finish...\n";
         PCBobj.temp2Index = PCBobj.processIndex;

         //If a new application is found to have less processes
         //Update the indices
         //shortProcessCount is initialized to 100 so the first run can 
         //Correctly update the count
         if ( newProcessCount < shortProcessCount )
         {
            //Set the processCount
            shortProcessCount = newProcessCount;

            //cout << "Process count: " << shortProcessCount << "\n";
            //cout << "NewPRocess Count: " << newProcessCount << "\n";

            //Set the iterators
            PCBobj.jobIndex = PCBobj.tempIndex;
            PCBobj.endIndex = PCBobj.temp2Index;
         }

         //Reset the count
         newProcessCount = 0;
         //cout << "Reset count...\n";
      }
   }

   PCBobj.processIndex = PCBobj.dataVec.begin ( );
   shortProcessCount = 100;
}

/**
  * @function findPrioApplication
  *
  * @details finds the iterator within the vector for the beginning of the 
  *            application that has the most IO processes
  *
  * @notes none
*/
void Simulator::findPrioApplication ( )
{
   //Variables
   int noIO = 0;

   //cout << "Finding...\n";
   //Now that simulation has started, determine which app to run
   for ( PCBobj.processIndex = PCBobj.dataVec.begin ( );
      PCBobj.processIndex != PCBobj.dataVec.end ( );
      ++PCBobj.processIndex )
   {
      //If A{begin} save the index
      if ( PCBobj.processIndex->code == 'A' &&
         PCBobj.processIndex->key == "begin" ) {
         PCBobj.tempIndex = PCBobj.processIndex;
         appCount++;
         //cout << "App count: " << appCount << "\n";
         //cout << "Found begin...\n";
      }

      //If the operation is I/O update count
      if ( PCBobj.processIndex->code == 'I' ||
         PCBobj.processIndex->code == 'O' ) {
         //cout << "Found I/O...\n";
         newProcessCount++;
      }

      //If A{finish} save the index
      if ( PCBobj.processIndex->code == 'A' &&
         PCBobj.processIndex->key == "finish" )
      {
         //cout << "Found finish...\n";
         PCBobj.temp2Index = PCBobj.processIndex;

         //If a new application is found to have more I/O, update the
         //correct indices
         if ( newProcessCount > processCount )
         {
            //Set the processCount
            processCount = newProcessCount;

            //cout << "Process count: " << processCount << "\n";
            //cout << "NewPRocess Count: " << newProcessCount << "\n";

            //Set the iterators
            PCBobj.jobIndex = PCBobj.tempIndex;
            PCBobj.endIndex = PCBobj.temp2Index;
         }

         //cout << "Process: " << processCount << "\n";
         if ( processCount == 0 )
            noIO = 1;

         //Reset the count
         newProcessCount = 0;
         //cout << "Reset count...\n";
      }
   }

   if ( appCount % 2 != 0 && noIO == 1 )
   {
      //cout << "Found no I/O...\n";
      PCBobj.jobIndex = PCBobj.tempIndex;
      PCBobj.endIndex = PCBobj.temp2Index;
      noIO = 0;
   }

   PCBobj.processIndex = PCBobj.dataVec.begin ( );
   processCount = 0;
}


void Simulator::processData ( MetaData& myMeta , ConfigData& myConfig )
{
   //Variables
   const string   BEGIN = "begin";
   const string   FINISH = "finish";
   const string   ALLOCATE = "allocate";
   const string   HDD = "HDD";
   const string   PROJ = "PROJ";
   string         temp;
   int            num;
   bool           allocation;

   if ( DEBUG == 1 )
   {
      cout << "Code: " << PCBobj.jobIndex->code << "\n";
      cout << "Key: " << PCBobj.jobIndex->key << "\n";
      cout << "ConfigTime: " << PCBobj.jobIndex->configTime << "\n";
      cout << "MetaTime: " << PCBobj.jobIndex->metaTime << "\n";
      cout << "ProcessTime: " << PCBobj.jobIndex->processTime << "\n";
      cout << "Visited: " << PCBobj.jobIndex->visited << "\n";
   }

   //Run process only if it hasn't been visited before
   if ( PCBobj.jobIndex->visited == 1 )
   {
      // do nothing
   }
   else
   {
      switch ( PCBobj.jobIndex->code )
      {

      case 'S':

         if ( PCBobj.jobIndex->key == BEGIN )
         {
            stream = " - Simulator program starting";
            log.output ( stream );

            //Set job index to visit
            PCBobj.jobIndex->visited = true;
         }
         else if ( PCBobj.jobIndex->key == FINISH )
         {
            stream = " - Simulator program ending";
            log.output ( stream );
         }

         break;

      case 'A':

         if ( PCBobj.jobIndex->key == BEGIN )
         {
            //Increment count to say correspond to process
            count++;
            PCBobj.jobIndex->pid = count;
            stream = " - OS: preparing process " +
               to_string ( PCBobj.jobIndex->pid );
            log.output ( stream );
            stream = " - OS: starting process " +
               to_string ( PCBobj.jobIndex->pid );
            log.output ( stream );
         }
         else if ( PCBobj.jobIndex->key == FINISH )
         {
            PCBobj.jobIndex->pid = count;
            stream = " - OS: removing process " +
               to_string ( PCBobj.jobIndex->pid );
            log.output ( stream );
         }

         //Set job index to visit
         PCBobj.jobIndex->visited = true;

         break;

      case 'P':

         if ( PCBobj.jobIndex->pidVisit != true )
         {
            PCBobj.jobIndex->pid = count;
         }

         stream = " - Process " + to_string ( PCBobj.jobIndex->pid )
            + " - start processing action";
         log.output ( stream );

         //Pass to gen handler
         PCBobj.genHandler ( );

         stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) +
            " - end processing action";
         log.output ( stream );

         PCBobj.jobIndex->pidVisit = true;

         break;

      case 'I':

         if ( PCBobj.jobIndex->pidVisit != true )
         {
            PCBobj.jobIndex->pid = count;
         }

         //Set operation to input
         processType = "input";

         if ( PCBobj.jobIndex->key == "projector" )
            num = 0;
         else if ( PCBobj.jobIndex->key == "hard drive" )
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

         stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) + " - start "
            + PCBobj.jobIndex->key + " " + processType + temp;
         log.output ( stream );

         //Send this to the PCB to handle
         if ( PCBobj.jobIndex->key == "hard drive" )
            PCBobj.HDDHandler ( );
         else if ( PCBobj.jobIndex->key == "projector" )
            PCBobj.projHandler ( );
         else
            PCBobj.IOHandler ( );

         stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) + " - end " +
            PCBobj.jobIndex->key + " " + processType;
         log.output ( stream );

         PCBobj.jobIndex->pidVisit = true;

         break;

      case 'O':
         
         if ( PCBobj.jobIndex->pidVisit != true )
         {
            PCBobj.jobIndex->pid = count;
         }

         //Set operation to input
         processType = "output";

         if ( PCBobj.jobIndex->key == "projector" )
            num = 0;
         else if ( PCBobj.jobIndex->key == "hard drive" )
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

         stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) + " - start "
            + PCBobj.jobIndex->key + " " + processType + temp;
         log.output ( stream );

         //Send this to the PCB to handle
         if ( PCBobj.jobIndex->key == "hard drive" )
            PCBobj.HDDHandler ( );
         else if ( PCBobj.jobIndex->key == "projector" )
            PCBobj.projHandler ( );
         else
            PCBobj.IOHandler ( );

         stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) + " - end " +
            PCBobj.jobIndex->key + " " + processType;
         log.output ( stream );

         PCBobj.jobIndex->pidVisit = true;

         break;

      case 'M':
         
         if ( PCBobj.jobIndex->pidVisit != true )
         {
            PCBobj.jobIndex->pid = count;
         }

         //If memory allocate
         if ( PCBobj.jobIndex->key == ALLOCATE )
         {
            stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) +
               " - allocating memory";
            log.output ( stream );

            //Get the address
            allocation = getAddress ( );

            //Pass to gen handler
            PCBobj.genHandler ( );

            switch ( allocation )
            {

            case false:
               stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) +
                  " - memory could not be allocated. Max memory size reached.";
               log.output ( stream );
               break;

            case true:
               stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) +
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
            stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) +
               " - start memory blocking";
            log.output ( stream );

            //Pass to gen handler
            PCBobj.genHandler ( );

            stream = " - Process " + to_string ( PCBobj.jobIndex->pid ) +
               " - end memory blocking";
            log.output ( stream );
         }

         PCBobj.jobIndex->pidVisit = true;

         break;

      default:

         cout << "Error with the processing data switch codes...\n";
         break;
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
bool Simulator::getAddress ( )
{
   bool allocated;

   //To set the first address space to 0
   if ( address == 0 && flag == 0 )
   {
      allocated = true;
      flag = 1;
   }
   
   //To handle every other time
   else
   {
      address += blockSize;
      allocated = true;
      if ( blockSize > maxMemory )
      {
         address = 0;
         allocated = true;
      }
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
   temp = temp.substr ( 0 , temp.find ( '\r' ) ); //Get rid of \r

   //Determine the algorithm used
   if ( temp == "PS" )
      schedAlg = PS;
   else if ( temp == "SJF" )
      schedAlg = SJF;
   else if ( temp == "RR" )
      schedAlg = RR;
   else if ( temp == "STR" )
      schedAlg = STR;
   else
      schedAlg = FIFO; //Default it to FIFO if nothing else
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
  * @function setQuantum
  *
  * @parameters This function takes in the class objects of MetaData and
  *               ConfigData
  *
  * @details This function takes key value from the meta data to grab the
  *            data from the corresponding key value to set the quantum amount
  *            which will be used in the Round Robin scheduling algorithm
*/
void Simulator::setQuantum ( MetaData &myMeta , ConfigData myConfig )
{
   //Variables
   const string QUANTUM = "process";
   string   temp;

   it = myConfig.configMap.find ( QUANTUM );
   temp = it->second;
   quantum = stoi ( temp );
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
      //cout << "DataKey: " << PCBobj.data.key << "\n";
      myMeta.stringQueue.pop ( ); //Back to the entire string

                                  //Set metaTime
      PCBobj.data.metaTime = myMeta.intQueue.front ( );
      //cout << "MetaTime: " << PCBobj.data.metaTime << "\n";
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
      //cout << "ConfigTime: " << PCBobj.data.configTime << "\n";
      //Set the processTime
      PCBobj.data.processTime = PCBobj.data.configTime * PCBobj.data.metaTime;
      //std::cout << "ProcessTime: " << PCBobj.data.processTime << "\n";

      //Push all of this onto the dataQueue
      PCBobj.dataVec.push_back ( PCBobj.data );
   }
}

