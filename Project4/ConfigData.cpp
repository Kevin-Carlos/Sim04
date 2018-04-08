/**
* File: ConfigData.cpp
*
* Details: Implementation of the ConfigData.h class. This will concern file
*            read in of a config file
*
* Version: 1.00 Initial development
*            Kevin Carlos 2/2/18
*
* Notes: None
*
*/
#include "ConfigData.h"


/**
* Funciton: fileReadIn
*
* Details: Read in the file contents into a map for later use
*
* Notes: If file doesn't open correctly, exit program since this is command
*         line operated
*/
void ConfigData::fileReadIn ( string configFileName )
{
   //Declare variables
   ifstream    fin;
   string      temp ,
      data ,
      keyString ,
      keyVal;
   int         count = 0 ,
      HDDcount = 0 ,
      printCount = 0;

   //Open the file
   fin.open ( configFileName );

   //Check that the file opened
   if ( !fin )
   {
      std::cout << "Error opening file...\n";
      std::cout << "Exiting program...\n";
      exit ( 1 );
   }

   while ( !fin.eof ( ) )
   {
      //Jankiness
      if ( count == 0 )
      {
         getline ( fin , temp , '\n' );
         getline ( fin , temp , '\n' );
         count++;
      }

      //Collect data into map
      getline ( fin , keyString , ':' );
      getline ( fin , temp , ' ' );
      getline ( fin , data , '\n' );

      //Take the first word to be the actual key
      keyVal = keyString.substr ( 0 , keyString.find ( ' ' ) );
      //Set to lowercase
      transform (
         keyVal.begin ( ) ,
         keyVal.end ( ) ,
         keyVal.begin ( ) ,
         ::tolower );

      //Special case for memory block size
      if ( keyVal == "memory" )
      {
         //Need to read the next word
         keyVal =
            keyString.substr (
               keyString.find ( ' ' ) ,
               keyString.find ( ' ' ) );

         if ( keyVal == " block" )
            keyVal = "block";
         else
            keyVal = "memory";
      }

      if ( keyVal == "projector" )
      {
         keyVal = "projector";
         if ( printCount == 1 )
            keyVal = "projector quantity";
         printCount++;
      }

      if ( keyVal == "hard" )
      {
         keyVal = "hard drive";
         if ( HDDcount == 1 )
            keyVal = "hard drive quantity";
         HDDcount++;
      }

      //Special case for log file path
      it = configMap.find ( "log" );
      if ( it == configMap.end ( ) )
      {
         // Not found
      }
      else
         keyVal = "path";

      //Debug
      cout << "Key: " << keyVal << "\n";
      cout << "Data: " << data << "\n";

      //Insert into map
      configMap.insert ( pair<string , string> ( keyVal , data ) );

   }

   temp = ( "End" );
   configMap.erase ( temp );

   //Set the meta file
   it = configMap.find ( "file" );
   temp = it->second;
   //Get rid of \r
   metaFile = temp.substr ( 0 , temp.find ( '\r' ) );

   //Close the file
   fin.close ( );
}

string ConfigData::getMetaFile ( )
{
   return metaFile;
}
