/**
* File: MetaData.cpp
*
* Details: Implementation of the MetaData class
*
* Author: Kevin Carlos
*         1.00 Initial development
*         2/7/18
*
* Notes: None
*
*/


//
// Includes
//
#include "MetaData.h"

//
// Pre-Processor directives
//
#define __DEBUG 0

void MetaData::metaFileReadIn ( string fileName )
{
   //Variable declarations
   ifstream    fin;

   string      temp ,
      temp2;

   int         index1 ,
      index2 ,
      endCond = 0;

   size_t      pos;

   const string END = "End";
   const string BEGIN = "begin";
   const string HARD = "hard";

   //Open the file
   fin.open ( fileName );

   //If it doesnt open exit program
   if ( !fin )
   {
      cout << "Error opening meta data file...\n";
      cout << "Check file path...\n";
      cout << "Exiting program...\n";
      exit ( 1 );
   }

   //read in first line since will always be the start
   getline ( fin , temp , '\n' );

   for ( ;; )
   {
      //Read in process
      fin >> data;

      //If end of file has been reached
      /*if ( data == END )
      break;*/

      //Special case :: hard drive
      pos = data.find ( HARD );
      if ( pos != string::npos )
      {
         fin >> temp;
         temp2 = data + ' ' + temp;
         data = temp2;
         code = data [ 0 ];
      }
      else
         //Take the code
         code = data [ 0 ];

      //Get the key with the indices
      //Set indices
      index1 = data.find ( '{' );
      index2 = data.find ( '}' );
      key = data.substr ( index1 + 1 , index2 - 2 );

      //Time
      index1 = data.find ( ';' );
      temp = data.substr ( ( index2 + 1 ) , ( index1 - 1 ) );

      //If time = .
      if ( temp == END )
         endCond = 1;

      if ( endCond == 1 )
         cycleTime = 0;
      else
         cycleTime = stoi ( temp );

      if ( endCond == 1 )
         break;

      //Store data to use
      charQueue.push ( code );
      stringQueue.push ( data );
      stringQueue.push ( key );
      intQueue.push ( cycleTime );

      //Debug
      if ( __DEBUG == 1 )
      {
         cout << "Data: " << data << "\n";
         cout << "Char: " << code << "\n";
         cout << "Key: " << key << "\n";
         cout << "Time: " << cycleTime << "\n";
      }
   }
}
