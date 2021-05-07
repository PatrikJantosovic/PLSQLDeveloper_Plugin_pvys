// PLSQLDeveloperPVYSPlugIn.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
// PLSQLFunctions.h get the whole API.
#include "PLSQLFunctions.h"
#include "PLSQLDeveloperPVYSPlugIn.h"

// Global variables
int siPlugInID;
int siAppHandle;
HWND siWindowHandle;

char* gcUserName;
char* gcPassword;
char* gcDatabase;
BOOL  gbConnected;
char gcConnectionString[100];
char gcConnDisplay[2000];
char gcVersionText[1000];

// This function receives a Plug - In ID from PL / SQL Developer and should return a description for
// the Plug-In. The returned description should be unique for your Plug-In and will be
// displayed in the Plug-In configuration dialog. The ID identifies your Plug-In and can be
// used in other callback functions.
char* IdentifyPlugIn(int ID)
{
	siPlugInID = ID;
	return scDesc;
}

// This function allows you to display an about dialog. You can decide to display a dialog
// yourself (in which case you should return an empty text) or just return the about text.  In
// PL/SQL Developer 3.1 there is an about button in the Plug-In configuration dialog.
char *About()
{
	sprintf_s(gcVersionText, "PL/SQL Developer PVYS Versioning Plugin - Version %s\n", scVersion);
	return gcVersionText;
}

// The PlugIn name (if defined) will be used for online updates, and as name for command window
// PlugIn commands. If you want your PlugIn to be handled by online updates, please contact
// support.  If this function is not defined, the PlugInName will be the dll filename.
//
// Available in version 700
char *PlugInName()
{
	return scPlugInName;
}

// The subname will be added to the PlugInName. Possible values are ‘Trial’ or ‘Beta’.
// Available in version 700
char *PlugInSubName()
{
	return scPlugInSubName;
}

// Delphi function PlugInShortName: PChar The short name is specifically for command window
// PlugIn commands. This allows you to specify a name that can be entered quickly.
//
// Available in version 700
char *PlugInShortName()
{
	return scPlugInShortName;
}

// All Callback to setup the popup menu items
void InitPopupMenu() {
	//// Called on Startup and Enabled in Config menu.
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Package", "PACKAGE+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Package Body", "PACKAGE BODY+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Type", "TYPE+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Type Body", "TYPE BODY+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a View", "VIEW+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Table", "TABLE+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Trigger", "TRIGGER+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Procedure", "PROCEDURE+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Function", "FUNCTION+");
	//IDE_CreatePopupItem(siPlugInID, siMenuPopup, "Popup Menu for on a Sequence", "SEQUENCE+");

	//// Set icons
	//std::string cIconPopup = std::string(SYS_RootDir()) + "\\icons\\Bulb.png";
	//IDE_CreateToolButton(siPlugInID, siMenuPopup, "", (char*)cIconPopup.c_str(), NULL);

}

// This function is called when the Plug-In is loaded into memory. You can use it to do some
// one-time initialization. PL/SQL Developer is not logged on yet and you can't use the
// callback functions, so you are limited in the things you can do.
void OnCreate() {
	// Create the array for the callback
	OnCreateInitPtrArray();
}

// This is the counterpart of the OnCreate. You can dispose of anything you created in the
// OnCreate.
void OnDestroy() {
	// Nothing to do. Yet.
	IDE_DebugLog("Called: OnDestroy()");
}

// This function is called when the user logs on to a different database or logs off. You can
// use the IDE_Connected and IDE_GetConnectionInfo callback to get information about the current
// connection.
void OnConnectionChange() {
	IDE_DebugLog("Called: OnConnectionChange()");
	//int rtn;
	//std::string cVersion;
	// Get Connection
	//if (IDE_Connected()) {
	//	gbConnected = true;
	//	IDE_GetConnectionInfo(&gcUserName, &gcPassword, &gcDatabase);
	//	sprintf_s(gcConnectionString, "%s@%s", gcUserName, gcDatabase);
	//	rtn = SQL_Execute("SELECT VERSION FROM v$instance");
	//	if (rtn == 0) {
	//		sprintf_s(gcConnDisplay, "%s (Oracle version %s)", gcConnectionString, SQL_Field(0));
	//	}
	//	else {
	//		sprintf_s(gcConnDisplay, "%s (Failed to get Oracle Version number)", gcConnectionString);
	//	}
	//}
	//else {
	//	gbConnected = false;
	//	sprintf_s(gcConnDisplay, "(Not Connected)");
	//}
}

// This function will be called with an Index ranging from 1 to 99. For every Index you can
// return a string that creates a new menu-item in PL/SQL Developer.
char* CreateMenuItem(int nIndex) {
	int plsqlVersion = SYS_Version();

	if (plsqlVersion < 1200) {
		if (nIndex == siMenuGlobal) { return "PVYS Plugin"; }
		if (nIndex == siAddVersion) { return "PVYS Plugin / Add Version"; }
		if (nIndex == siAddVersionStaged) { return "PVYS Plugin / Add Version to Staged"; }
		if (nIndex == siGetMessageCommit) { return "PVYS Plugin / Get Changes for Commit"; }
		if (nIndex == siGetMessageTask) { return "PVYS Plugin / Get Changes for Task"; }
		if (nIndex == siGetMessageTaskLast) { return "PVYS Plugin / Get Changes for Task (last commit)"; }
		if (nIndex == siGetPluginInfo) { return "PVYS Plugin / Plugin info"; }
	}
	else { // PL/SQL Developer 12 use a Ribbon and no longer menu.
		if (nIndex == siRibbonTab) { return "TAB=PVYS Plugin [tabindex=9]"; }
		if (nIndex == siMenuGlobal) { return "GROUP=PVYS Plugin"; }
		if (nIndex == siAddVersion) { return "LARGEITEM=Add Version"; }
		if (nIndex == siAddVersionStaged) { return "LARGEITEM=Add Version to Staged"; }
		if (nIndex == siGetMessageCommit) { return "LARGEITEM=Get Changes for Commit"; }
		if (nIndex == siGetMessageTask) { return "LARGEITEM=Get Changes for Task"; }
		if (nIndex == siGetMessageTaskLast) { return "LARGEITEM=Get Changes for Task (last commit)"; }
		if (nIndex == siGetPluginInfo) { return "LARGEITEM=Plugin info"; }
		//if (nIndex == siMenuSysVersion) { return "SUBITEM=SYS_Version()"; }
	}
	return "";
}

// Function for Menu Icons. Called from OnActivate(). Separated from there for lisibility.
void setMenuIcons() {
	// Set images for Menu items/Ribbon
	std::string cIconAddVersion = std::string(SYS_RootDir()) + "\\icons\\Check.png";
	std::string cIconAddVersionStaged = std::string(SYS_RootDir()) + "\\icons\\CheckIt.png";
	std::string cIconGetMessageCommit = std::string(SYS_RootDir()) + "\\icons\\WindowOptions.png";
	std::string cIconGetMessageTask = std::string(SYS_RootDir()) + "\\icons\\Show1.png";
	std::string cIconGetMessageTaskLast = std::string(SYS_RootDir()) + "\\icons\\Show2.png";
	std::string cIconGetPluginInfo = std::string(SYS_RootDir()) + "\\icons\\Help2.png";

	// Setting icon only for the Version 12 - Ribbon. No need on version 11 - Menu
	//if (SYS_Version() >= 1200) {
	//	IDE_CreateToolButton(siPlugInID, siAddVersion, "", (char*)cIconAddVersion.c_str(), NULL);
	//
    //}
	IDE_CreateToolButton(siPlugInID, siAddVersion, "", (char*)cIconAddVersion.c_str(), NULL);
	IDE_CreateToolButton(siPlugInID, siAddVersionStaged, "", (char*)cIconAddVersionStaged.c_str(), NULL);
	IDE_CreateToolButton(siPlugInID, siGetMessageCommit, "", (char*)cIconGetMessageCommit.c_str(), NULL);
	IDE_CreateToolButton(siPlugInID, siGetMessageTask, "", (char*)cIconGetMessageTask.c_str(), NULL);
	IDE_CreateToolButton(siPlugInID, siGetMessageTaskLast, "", (char*)cIconGetMessageTaskLast.c_str(), NULL);
	IDE_CreateToolButton(siPlugInID, siGetPluginInfo, "", (char*)cIconGetPluginInfo.c_str(), NULL);

}

// OnActivate gets called after OnCreate. However, when OnActivate is called PL/SQL Developer
// and the Plug-In are fully initialized. This function is also called when the Plug-In is
// enabled in the configuration dialog. A good point to enable/disable menus.
void OnActivate() {
	IDE_DebugLog("Called: OnActivate()");
	// Get Handle on the App and the main window and store them
	siAppHandle = IDE_GetAppHandle();
	siWindowHandle = IDE_GetWindowHandle();
	InitPopupMenu();
	OnConnectionChange();
	// To force menu to appear
	IDE_SetMenuVisible(siPlugInID, siMenuGlobal, true);
	setMenuIcons();
}

// Helper function that reads data from clipboard using win32 api
std::string GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
		return "";

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr) {
		CloseClipboard();
		return "";
	}

	// Lock the handle to get the actual text pointer
    char * pText = static_cast<char*>(GlobalLock(hData));
	if (pText == nullptr) {
		GlobalUnlock(hData);
		CloseClipboard();
		return "";
	}

	// Save text in a string class instance
	std::string text(pText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

void RunSubProcess(std::string command) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//conversion from string
	LPSTR cmd = const_cast<char*>(command.c_str());

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		cmd,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,  // DONT FLASH THE TERMINAL WINDOWS
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

// This function is called when a user selected a menu-item created with the CreateMenuItem
// function and the Index parameter has the value (1 to 99) it is related to.
void OnMenuClick(int nIndex) {
	IDE_DebugLog("Called: OnMenuClick()");
	std::string clipboard = GetClipboardText();
	std::string pathToJar = "\"" + std::string(SYS_RootDir()) + "PlugIns\\VersionControl.jar\"";
	
	if (nIndex == siAddVersion) {
		if (clipboard.empty() || clipboard.at(0) != '[') {
			MessageBox(NULL, "Task ID in clipboard expected!", "PVYS Plugin", MB_ICONINFORMATION);
			return;
		}
		std::string command = "java -jar " + pathToJar + " add -t=\"" + clipboard + "\"";
		RunSubProcess(command);
	}
	else if (nIndex == siAddVersionStaged) {
		if (clipboard.empty() || clipboard.at(0) != '[') {
			MessageBox(NULL, "Task ID in clipboard expected!", "PVYS Plugin", MB_ICONINFORMATION);
			return;
		}
		std::string command = "java -jar " + pathToJar + " add -s=\"true\" -t=\"" + clipboard + "\"";
		RunSubProcess(command);
	}
	else if (nIndex == siGetMessageCommit) {
		if (clipboard.empty() || clipboard.at(0) == '[') {
			MessageBox(NULL, "Commit hash in clipboard expected!", "PVYS Plugin", MB_ICONINFORMATION);
			return;
		}
		std::string command = "java -jar " + pathToJar + " getc -c=\"" + clipboard + "\"";
		RunSubProcess(command);
	}
	else if (nIndex == siGetMessageTask) {
		if (clipboard.empty() || clipboard.at(0) != '[') {
			MessageBox(NULL, "Task ID in clipboard expected!", "PVYS Plugin", MB_ICONINFORMATION);
			return;
		}
		std::string command = "java -jar " + pathToJar + " get -t=\"" + clipboard + "\"";
		RunSubProcess(command);
	}
	else if (nIndex == siGetMessageTaskLast) {
		if (clipboard.empty() || clipboard.at(0) != '[') {
			MessageBox(NULL, "Task ID in clipboard expected!", "PVYS Plugin", MB_ICONINFORMATION);
			return;
		}
		std::string command = "java -jar " + pathToJar + " get -l=\"true\" -t=\"" + clipboard + "\"";
		RunSubProcess(command);
	}
	else if (nIndex == siGetPluginInfo) {
		MessageBox(NULL, "Non-official, not really supported versioning plugin. Full readme here: https://github.com/PatrikJantosovic/PLSQLDeveloper_Plugin_pvys#readme", "PVYS Plugin", MB_ICONINFORMATION);
	}
	//else if (nIndex == siMenuGetHelp) {
	//	IDE_ShowHTML("https://github.com/PatrikJantosovic/PLSQLDeveloper_Plugin_pvys#readme", "", "PL/SQL Developer Forums","");
	//} 
	//else if (nIndex == siMenuPopup) {
	//	char* cType;
	//	char *cOwner;
	//	char *cName;
	//	char *cSubType;

	//	// Get Data regarding the object selected.
	//	IDE_GetPopupObject(&cType, &cOwner, &cName, &cSubType);

	//	sprintf_s(Msg, "You did right-click on the following object: %s.%s (%s)", cOwner, cName, cType);
	//	MessageBox(NULL, Msg, "Demo Plugin", MB_ICONEXCLAMATION);
	//}

}

// If the Plug-In has a configure dialog you could use this function to activate it. This will
// allow a user to configure your Plug-In using the configure button in the Plug-In
// configuration dialog.
void Configure() {
	IDE_DebugLog("Called: OnConfigure()");
	//MessageBox(NULL, "Nothing to configure", "Demo Plugin", MB_ICONEXCLAMATION);

}

