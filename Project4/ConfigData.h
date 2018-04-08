/**
* File: ConfigData.h
*
* Details: A header file containing a class for the datatypes of
*            configuration file
*
* Version 1.00 Initial development
*            Kevin Carlos 2/2/18
*
* Notes: none
*/

//
// Include libraries
//
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

#ifndef __CONFIGDATACLASS__
#define __CONFIGDATACLASS__

class ConfigData
{
public:
   //Variables
   std::string metaFile;

   //Data structures
   map<string , string> configMap;
   map<string , string>::iterator it;

   //Member methods
   void fileReadIn ( string );
   string getMetaFile ( );
};

#endif
