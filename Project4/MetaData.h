/**
* File: MetaData.h
*
* Details: a class to read in the meta data file and use the data
*
* Author: Kevin Carlos
*         1.00 Initial Development 2/7/18
*
* Notes: None
*
*/

#ifndef __METADATACLASS__
#define __METADATACLASS__

#include "ConfigData.h"
#include <queue>

class MetaData : public ConfigData
{
public:
   //Variables
   char     code;
   string   data ,
      key;
   int      cycleTime;

   //Data Structures
   queue<string>  stringQueue;
   queue<int>     intQueue;
   queue<char>    charQueue;

   //Methods
   void metaFileReadIn ( string );
};

#endif

