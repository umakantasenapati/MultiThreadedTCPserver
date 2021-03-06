#ifndef myLog_H
#define myLog_H 

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;



class myLog;
extern myLog LOG;


const int LOG_WRITE  = ios::out;
const int LOG_APPEND = ios::app;

const int EXIT_MSG_SIZE = 512;
const int MAX_EXIT_CODES = 3;

class myLog : public ofstream
{

public:

   enum logLevels 
   {
      LEVEL_0,       // buffer all log messages
      LEVEL_1,       // buffer Level one, two and three log messages
      LEVEL_2,       // buffer Level two and three log messages
      LEVEL_3,       // buffer Level three log messages
      NOLOG_MODE     // do not print out any messages
   };
 
   myLog();
   myLog(const string&);
   myLog(const string&,enum logLevels);
   virtual ~myLog();

private:

   void initVars();
   void init(const string&);
   void init(const string&,int);
   void init();

   char* getExecTime();
   void  getExecTime(int*,int*);
   void  openLog(const string&,int);
   void  printHeader(int);

private:

   string logName;
   enum logLevels logLevel;
   time_t startTime;

};

const enum myLog::logLevels L0 = myLog::LEVEL_0;
const enum myLog::logLevels L1 = myLog::LEVEL_1;
const enum myLog::logLevels L2 = myLog::LEVEL_2;
const enum myLog::logLevels L3 = myLog::LEVEL_3;
const enum myLog::logLevels LQUIET = myLog::NOLOG_MODE;

#endif

