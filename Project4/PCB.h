/**
* @file PCB.h
*
* @details Class definition for a process control block
*
* @version 1.00 Intiial development
*
* @author Kevin Carlos
*
* @notes none
*
*/

// Directives //////////////////////////////////////////////////////////////////
#ifndef __PCB_H
#define __PCB_H

#define START 1
#define READY 2
#define RUNNING 3
#define WAIT 4
#define EXIT 5

// Header files ////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;

struct Data {
   char     code;

   string   key;

   int      configTime ,
            metaTime;

   long     processTime;
};

// Class declaration ///////////////////////////////////////////////////////////
class PCB
{
public:
   Data data;

   //Handle operations
   void genHandler ( );
   void IOHandler ( );

   // Process state using the predefined START, Ready, Running, etc for ints
   void setProcessState ( int );
   int getProcessState ( );

   vector<Data> dataVec;
   vector<Data>::iterator processIndex;

private:
   int processState;
};

#endif