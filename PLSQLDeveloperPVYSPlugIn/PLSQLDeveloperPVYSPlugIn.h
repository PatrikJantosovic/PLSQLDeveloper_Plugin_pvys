/*
	PLSQLDeveloperPVYSPlugIn.h
	Created from VSDemo project.

*/

/*
	PLSQLDeveloperVSDemoPlugIn.h
	This header file contain the variables, value, and static use all accross the Plug In.
	This is where you will change the Version Number, as well as any stored value, menu items ID.

*/

#pragma once

#if !defined(DEMOPLUGIN_H_INCLUDED_)
#define DEMOPLUGIN_H_INCLUDED_

char *const scPlugInName = "PL/SQL Developer PVYS Versioning PlugIn";
char *const scPlugInShortName = "PVYSPlugIn";

#ifdef _WIN64
char *const scDesc = "PVYS Versioning PlugIn - x64";
char *const scPlugInSubName = " v0.9 - 64 bits";
#else
char *const scDesc = "PVYS Versioning PlugIn - x86";
char *const scPlugInSubName = " v0.9 - 32 bits";
#endif
extern int siPlugInID;
extern int siAppHandle;
extern HWND siWindowHandle;

// version
char *const scVersion = "0.9.0";

extern char* gcUserName;
extern char* gcPassword;
extern char* gcDatabase;
extern BOOL  gbConnected;
extern char gcConnectionString[100];
extern char gcConnDisplay[2000];
extern char gcVersionText[1000];

#endif // !defined(DEMOPLUGIN_H_INCLUDED_)

int const siRibbonTab         = 1;
int const siMenuGlobal        = 2;

// PVYS actions according to java application
int const siAddVersion = 10;
int const siAddVersionStaged = 20;
int const siGetMessageCommit = 30;
int const siGetMessageTask = 40;
int const siGetMessageTaskLast = 50;
int const siGetPluginInfo = 60;

//int const siMenuPopup    = 90;
