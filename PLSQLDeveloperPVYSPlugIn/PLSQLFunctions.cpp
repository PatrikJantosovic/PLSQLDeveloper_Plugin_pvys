/*$File***********************************************************************

Module             PlSqlDevPlugIn

File               PlSqlDevFunctions.cpp

Contents           Pl/Sql Developer Plug-In Interface

Prototype File     PlSqlDevFunctions.h

Created            Vladimir Schneider
www.winpte.com
www.beyondedit.com

Sep 10, 2006

Copyright (c) 2006 Vladimir Schneider, All Rights Reserved

1.                 Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the
following conditions are met:

2.                 Neither the name of WinPTE, BeyondEdit nor the
names of its contributors may be used to endorse or
promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

******X******************X************************X************************X$*/

#include "stdafx.h"
#include "PlSqlFunctions.h"

#define MAX_PLSQLDEV_FUNCTIONS                    256

// structure defining the callback id, pointer holding the address, character representation for
// debugging and a dummy function which will print an error that the callback is not defined.
struct t_PlSqlDevFunc
{
	int m_nFuncID;                                // function id
	void **m_pFuncPtr;                            // pointer to location of function pointer
	const char *m_pszFuncDesc;                    // function description
	void *m_pFuncPtrUndef;                        // pointer to dummy in case callback is not defined
};

// trace callback invokations
int g_bTracePlSqlDevCalls = 1;

// trace value returned by IDE_GetChildHandle() before and after a plug-in function is called.
// Can be used to debug issues caused by the plug-in changing the selected window via the
// IDE_SelectWindow() callback.
int g_bTracePlSqlDevChildHandle = 1;

// this class is used to trace value of the IDE_GetChildHandle() before and after processing
// in the plug-in. Only used in times of trouble. :)
//
// constructor saves the window handle returned from the function.
// Destructor gets the value and TRACEs both values to the debug output
// if the global variable g_bTracePlSqlDevChildHandle is set.
//
// Just define a local variable and get the trace when the variable goes out of scope.
//
class PlSqlDevChildHandleTracer
{
public:
	inline PlSqlDevChildHandleTracer(const char *pszFunction = NULL)
	{
		m_pszFunction = pszFunction ? pszFunction : "<unknown>";
		m_hwndIn = g_bTracePlSqlDevChildHandle ? IDE_GetChildHandle() : NULL;
	}

	inline ~PlSqlDevChildHandleTracer()
	{
		if (g_bTracePlSqlDevChildHandle)
		{
			HWND hwndOut = g_bTracePlSqlDevChildHandle ? IDE_GetChildHandle() : NULL;
		}
	}

protected:
	HWND m_hwndIn;
	const char *m_pszFunction;

private:
};

//*****************************************************************************
//
// The following are used to eliminate overhead of checking a function pointer for
// the Callback on every call to the function while having the safety of not executing a NULL
// address, having the function index and possibly a text name when a pointer is not
// initialized, most likely due to a plug-in running on an earlier version of PL/SQL Dev that
// did not support this callback.
//
// Initially all functions point to dummy routines that just pass the index to
// UndefinedPlSqlDevCallback which handles all the tracing.
//
// As callback functions are initialized the pointers are set to the value passed by PL/SQL Dev.
//
//****X******************X************************X************************X***

// helper to define dummy callbacks
#define DEF_CALLBACK_UNDEFINED(n) int UndefinedPlSqlDevCallback_##n() { return UndefinedPlSqlDevCallback(n); }
#define REF_CALLBACK_UNDEFINED(n) UndefinedPlSqlDevCallback_##n

int UndefinedPlSqlDevCallback(int nFuncID);

// 256 Dummy Callback Functions
DEF_CALLBACK_UNDEFINED(0); DEF_CALLBACK_UNDEFINED(1); DEF_CALLBACK_UNDEFINED(2); DEF_CALLBACK_UNDEFINED(3); DEF_CALLBACK_UNDEFINED(4); DEF_CALLBACK_UNDEFINED(5); DEF_CALLBACK_UNDEFINED(6); DEF_CALLBACK_UNDEFINED(7); DEF_CALLBACK_UNDEFINED(8); DEF_CALLBACK_UNDEFINED(9); DEF_CALLBACK_UNDEFINED(10); DEF_CALLBACK_UNDEFINED(11); DEF_CALLBACK_UNDEFINED(12); DEF_CALLBACK_UNDEFINED(13); DEF_CALLBACK_UNDEFINED(14); DEF_CALLBACK_UNDEFINED(15);
DEF_CALLBACK_UNDEFINED(16); DEF_CALLBACK_UNDEFINED(17); DEF_CALLBACK_UNDEFINED(18); DEF_CALLBACK_UNDEFINED(19); DEF_CALLBACK_UNDEFINED(20); DEF_CALLBACK_UNDEFINED(21); DEF_CALLBACK_UNDEFINED(22); DEF_CALLBACK_UNDEFINED(23); DEF_CALLBACK_UNDEFINED(24); DEF_CALLBACK_UNDEFINED(25); DEF_CALLBACK_UNDEFINED(26); DEF_CALLBACK_UNDEFINED(27); DEF_CALLBACK_UNDEFINED(28); DEF_CALLBACK_UNDEFINED(29); DEF_CALLBACK_UNDEFINED(30); DEF_CALLBACK_UNDEFINED(31);
DEF_CALLBACK_UNDEFINED(32); DEF_CALLBACK_UNDEFINED(33); DEF_CALLBACK_UNDEFINED(34); DEF_CALLBACK_UNDEFINED(35); DEF_CALLBACK_UNDEFINED(36); DEF_CALLBACK_UNDEFINED(37); DEF_CALLBACK_UNDEFINED(38); DEF_CALLBACK_UNDEFINED(39); DEF_CALLBACK_UNDEFINED(40); DEF_CALLBACK_UNDEFINED(41); DEF_CALLBACK_UNDEFINED(42); DEF_CALLBACK_UNDEFINED(43); DEF_CALLBACK_UNDEFINED(44); DEF_CALLBACK_UNDEFINED(45); DEF_CALLBACK_UNDEFINED(46); DEF_CALLBACK_UNDEFINED(47);
DEF_CALLBACK_UNDEFINED(48); DEF_CALLBACK_UNDEFINED(49); DEF_CALLBACK_UNDEFINED(50); DEF_CALLBACK_UNDEFINED(51); DEF_CALLBACK_UNDEFINED(52); DEF_CALLBACK_UNDEFINED(53); DEF_CALLBACK_UNDEFINED(54); DEF_CALLBACK_UNDEFINED(55); DEF_CALLBACK_UNDEFINED(56); DEF_CALLBACK_UNDEFINED(57); DEF_CALLBACK_UNDEFINED(58); DEF_CALLBACK_UNDEFINED(59); DEF_CALLBACK_UNDEFINED(60); DEF_CALLBACK_UNDEFINED(61); DEF_CALLBACK_UNDEFINED(62); DEF_CALLBACK_UNDEFINED(63);
DEF_CALLBACK_UNDEFINED(64); DEF_CALLBACK_UNDEFINED(65); DEF_CALLBACK_UNDEFINED(66); DEF_CALLBACK_UNDEFINED(67); DEF_CALLBACK_UNDEFINED(68); DEF_CALLBACK_UNDEFINED(69); DEF_CALLBACK_UNDEFINED(70); DEF_CALLBACK_UNDEFINED(71); DEF_CALLBACK_UNDEFINED(72); DEF_CALLBACK_UNDEFINED(73); DEF_CALLBACK_UNDEFINED(74); DEF_CALLBACK_UNDEFINED(75); DEF_CALLBACK_UNDEFINED(76); DEF_CALLBACK_UNDEFINED(77); DEF_CALLBACK_UNDEFINED(78); DEF_CALLBACK_UNDEFINED(79);
DEF_CALLBACK_UNDEFINED(80); DEF_CALLBACK_UNDEFINED(81); DEF_CALLBACK_UNDEFINED(82); DEF_CALLBACK_UNDEFINED(83); DEF_CALLBACK_UNDEFINED(84); DEF_CALLBACK_UNDEFINED(85); DEF_CALLBACK_UNDEFINED(86); DEF_CALLBACK_UNDEFINED(87); DEF_CALLBACK_UNDEFINED(88); DEF_CALLBACK_UNDEFINED(89); DEF_CALLBACK_UNDEFINED(90); DEF_CALLBACK_UNDEFINED(91); DEF_CALLBACK_UNDEFINED(92); DEF_CALLBACK_UNDEFINED(93); DEF_CALLBACK_UNDEFINED(94); DEF_CALLBACK_UNDEFINED(95);
DEF_CALLBACK_UNDEFINED(96); DEF_CALLBACK_UNDEFINED(97); DEF_CALLBACK_UNDEFINED(98); DEF_CALLBACK_UNDEFINED(99); DEF_CALLBACK_UNDEFINED(100); DEF_CALLBACK_UNDEFINED(101); DEF_CALLBACK_UNDEFINED(102); DEF_CALLBACK_UNDEFINED(103); DEF_CALLBACK_UNDEFINED(104); DEF_CALLBACK_UNDEFINED(105); DEF_CALLBACK_UNDEFINED(106); DEF_CALLBACK_UNDEFINED(107); DEF_CALLBACK_UNDEFINED(108); DEF_CALLBACK_UNDEFINED(109); DEF_CALLBACK_UNDEFINED(110); DEF_CALLBACK_UNDEFINED(111);
DEF_CALLBACK_UNDEFINED(112); DEF_CALLBACK_UNDEFINED(113); DEF_CALLBACK_UNDEFINED(114); DEF_CALLBACK_UNDEFINED(115); DEF_CALLBACK_UNDEFINED(116); DEF_CALLBACK_UNDEFINED(117); DEF_CALLBACK_UNDEFINED(118); DEF_CALLBACK_UNDEFINED(119); DEF_CALLBACK_UNDEFINED(120); DEF_CALLBACK_UNDEFINED(121); DEF_CALLBACK_UNDEFINED(122); DEF_CALLBACK_UNDEFINED(123); DEF_CALLBACK_UNDEFINED(124); DEF_CALLBACK_UNDEFINED(125); DEF_CALLBACK_UNDEFINED(126); DEF_CALLBACK_UNDEFINED(127);
DEF_CALLBACK_UNDEFINED(128); DEF_CALLBACK_UNDEFINED(129); DEF_CALLBACK_UNDEFINED(130); DEF_CALLBACK_UNDEFINED(131); DEF_CALLBACK_UNDEFINED(132); DEF_CALLBACK_UNDEFINED(133); DEF_CALLBACK_UNDEFINED(134); DEF_CALLBACK_UNDEFINED(135); DEF_CALLBACK_UNDEFINED(136); DEF_CALLBACK_UNDEFINED(137); DEF_CALLBACK_UNDEFINED(138); DEF_CALLBACK_UNDEFINED(139); DEF_CALLBACK_UNDEFINED(140); DEF_CALLBACK_UNDEFINED(141); DEF_CALLBACK_UNDEFINED(142); DEF_CALLBACK_UNDEFINED(143);
DEF_CALLBACK_UNDEFINED(144); DEF_CALLBACK_UNDEFINED(145); DEF_CALLBACK_UNDEFINED(146); DEF_CALLBACK_UNDEFINED(147); DEF_CALLBACK_UNDEFINED(148); DEF_CALLBACK_UNDEFINED(149); DEF_CALLBACK_UNDEFINED(150); DEF_CALLBACK_UNDEFINED(151); DEF_CALLBACK_UNDEFINED(152); DEF_CALLBACK_UNDEFINED(153); DEF_CALLBACK_UNDEFINED(154); DEF_CALLBACK_UNDEFINED(155); DEF_CALLBACK_UNDEFINED(156); DEF_CALLBACK_UNDEFINED(157); DEF_CALLBACK_UNDEFINED(158); DEF_CALLBACK_UNDEFINED(159);
DEF_CALLBACK_UNDEFINED(160); DEF_CALLBACK_UNDEFINED(161); DEF_CALLBACK_UNDEFINED(162); DEF_CALLBACK_UNDEFINED(163); DEF_CALLBACK_UNDEFINED(164); DEF_CALLBACK_UNDEFINED(165); DEF_CALLBACK_UNDEFINED(166); DEF_CALLBACK_UNDEFINED(167); DEF_CALLBACK_UNDEFINED(168); DEF_CALLBACK_UNDEFINED(169); DEF_CALLBACK_UNDEFINED(170); DEF_CALLBACK_UNDEFINED(171); DEF_CALLBACK_UNDEFINED(172); DEF_CALLBACK_UNDEFINED(173); DEF_CALLBACK_UNDEFINED(174); DEF_CALLBACK_UNDEFINED(175);
DEF_CALLBACK_UNDEFINED(176); DEF_CALLBACK_UNDEFINED(177); DEF_CALLBACK_UNDEFINED(178); DEF_CALLBACK_UNDEFINED(179); DEF_CALLBACK_UNDEFINED(180); DEF_CALLBACK_UNDEFINED(181); DEF_CALLBACK_UNDEFINED(182); DEF_CALLBACK_UNDEFINED(183); DEF_CALLBACK_UNDEFINED(184); DEF_CALLBACK_UNDEFINED(185); DEF_CALLBACK_UNDEFINED(186); DEF_CALLBACK_UNDEFINED(187); DEF_CALLBACK_UNDEFINED(188); DEF_CALLBACK_UNDEFINED(189); DEF_CALLBACK_UNDEFINED(190); DEF_CALLBACK_UNDEFINED(191);
DEF_CALLBACK_UNDEFINED(192); DEF_CALLBACK_UNDEFINED(193); DEF_CALLBACK_UNDEFINED(194); DEF_CALLBACK_UNDEFINED(195); DEF_CALLBACK_UNDEFINED(196); DEF_CALLBACK_UNDEFINED(197); DEF_CALLBACK_UNDEFINED(198); DEF_CALLBACK_UNDEFINED(199); DEF_CALLBACK_UNDEFINED(200); DEF_CALLBACK_UNDEFINED(201); DEF_CALLBACK_UNDEFINED(202); DEF_CALLBACK_UNDEFINED(203); DEF_CALLBACK_UNDEFINED(204); DEF_CALLBACK_UNDEFINED(205); DEF_CALLBACK_UNDEFINED(206); DEF_CALLBACK_UNDEFINED(207);
DEF_CALLBACK_UNDEFINED(208); DEF_CALLBACK_UNDEFINED(209); DEF_CALLBACK_UNDEFINED(210); DEF_CALLBACK_UNDEFINED(211); DEF_CALLBACK_UNDEFINED(212); DEF_CALLBACK_UNDEFINED(213); DEF_CALLBACK_UNDEFINED(214); DEF_CALLBACK_UNDEFINED(215); DEF_CALLBACK_UNDEFINED(216); DEF_CALLBACK_UNDEFINED(217); DEF_CALLBACK_UNDEFINED(218); DEF_CALLBACK_UNDEFINED(219); DEF_CALLBACK_UNDEFINED(220); DEF_CALLBACK_UNDEFINED(221); DEF_CALLBACK_UNDEFINED(222); DEF_CALLBACK_UNDEFINED(223);
DEF_CALLBACK_UNDEFINED(224); DEF_CALLBACK_UNDEFINED(225); DEF_CALLBACK_UNDEFINED(226); DEF_CALLBACK_UNDEFINED(227); DEF_CALLBACK_UNDEFINED(228); DEF_CALLBACK_UNDEFINED(229); DEF_CALLBACK_UNDEFINED(230); DEF_CALLBACK_UNDEFINED(231); DEF_CALLBACK_UNDEFINED(232); DEF_CALLBACK_UNDEFINED(233); DEF_CALLBACK_UNDEFINED(234); DEF_CALLBACK_UNDEFINED(235); DEF_CALLBACK_UNDEFINED(236); DEF_CALLBACK_UNDEFINED(237); DEF_CALLBACK_UNDEFINED(238); DEF_CALLBACK_UNDEFINED(239);
DEF_CALLBACK_UNDEFINED(240); DEF_CALLBACK_UNDEFINED(241); DEF_CALLBACK_UNDEFINED(242); DEF_CALLBACK_UNDEFINED(243); DEF_CALLBACK_UNDEFINED(244); DEF_CALLBACK_UNDEFINED(245); DEF_CALLBACK_UNDEFINED(246); DEF_CALLBACK_UNDEFINED(247); DEF_CALLBACK_UNDEFINED(248); DEF_CALLBACK_UNDEFINED(249); DEF_CALLBACK_UNDEFINED(250); DEF_CALLBACK_UNDEFINED(251); DEF_CALLBACK_UNDEFINED(252); DEF_CALLBACK_UNDEFINED(253); DEF_CALLBACK_UNDEFINED(254); DEF_CALLBACK_UNDEFINED(255);

/*FUNC: 1*/ int(*SYS_Version)();
/*FUNC: 2*/ char *(*SYS_Registry)();
/*FUNC: 3*/ char *(*SYS_RootDir)();
/*FUNC: 4*/ char *(*SYS_OracleHome)();
/*FUNC: 5*/ char *(*SYS_OCIDLL)();
/*FUNC: 6*/ BOOL *(*SYS_OCI8Mode)();
/*FUNC: 7*/ BOOL *(*SYS_XPStyle)();
/*FUNC: 8*/ char* (*SYS_TNSNAMES)(char *Param);
/*FUNC: 10*/ void(*IDE_MenuState)(int ID, int Index, BOOL Enabled);
/*FUNC: 11*/ BOOL(*IDE_Connected)();
/*FUNC: 12*/ void(*IDE_GetConnectionInfo)(char **Username, char **Password, char **Database);
/*FUNC: 13*/ void(*IDE_GetBrowserInfo)(char **ObjectType, char **ObjectOwner, char **ObjectName);
/*FUNC: 14*/ int(*IDE_GetWindowType)();
/*FUNC: 15*/ int(*IDE_GetAppHandle)();
/*FUNC: 16*/ HWND(*IDE_GetWindowHandle)();
/*FUNC: 17*/ HWND(*IDE_GetClientHandle)();
/*FUNC: 18*/ HWND(*IDE_GetChildHandle)();
/*FUNC: 19*/ void(*IDE_Refresh)();
/*FUNC: 20*/ void(*IDE_CreateWindow)(int WindowType, char *Text, BOOL Execute);
/*FUNC: 21*/ BOOL(*IDE_OpenFile)(int WindowType, char *Filename);
/*FUNC: 22*/ BOOL(*IDE_SaveFile)();
/*FUNC: 23*/ char *(*IDE_Filename)();
/*FUNC: 24*/ void(*IDE_CloseFile)();
/*FUNC: 25*/ void(*IDE_SetReadOnly)(BOOL ReadOnly);
/*FUNC: 26*/ BOOL(*IDE_GetReadOnly)();
/*FUNC: 27*/ BOOL(*IDE_ExecuteSQLReport)(char *SQL, char *Title, BOOL Updateable);
/*FUNC: 28*/ BOOL(*IDE_ReloadFile)();
/*FUNC: 29*/ void(*IDE_SetFilename)(char *Filename);
/*FUNC: 30*/ char *(*IDE_GetText)();
/*FUNC: 31*/ char *(*IDE_GetSelectedText)();
/*FUNC: 32*/ char *(*IDE_GetCursorWord)();
/*FUNC: 33*/ HWND(*IDE_GetEditorHandle)();
/*FUNC: 34*/ BOOL(*IDE_SetText)(char *Text);
/*FUNC: 35*/ BOOL(*IDE_SetStatusMessage)(char *Text);
/*FUNC: 36*/ BOOL(*IDE_SetErrorPosition)(int Line, int Col);
/*FUNC: 37*/ void(*IDE_ClearErrorPositions)();
/*FUNC: 38*/ int(*IDE_GetCursorWordPosition)();
/*FUNC: 39*/ BOOL(*IDE_Perform)(int Param);
/*FUNC: 60*/ char *(*IDE_GetCustomKeywords)();
/*FUNC: 61*/ void(*IDE_SetCustomKeywords)(char *Keywords);
/*FUNC: 62*/ void(*IDE_SetKeywords)(int ID, int Style, char *Keywords);
/*FUNC: 63*/ void(*IDE_ActivateKeywords)();
/*FUNC: 64*/ void(*IDE_RefreshMenus)(int ID);
/*FUNC: 65*/ void(*IDE_SetMenuName)(int ID, int Index, char *Name);
/*FUNC: 66*/ void(*IDE_SetMenuCheck)(int ID, int Index, BOOL Enabled);
/*FUNC: 67*/ void(*IDE_SetMenuVisible)(int ID, int Index, BOOL Enabled);
/*FUNC: 68*/ char *(*IDE_GetMenulayout)();
/*FUNC: 69*/ void *(*IDE_CreatePopupItem)(int ID, int Index, char *Name, char *ObjectType);
/*FUNC: 70*/ BOOL(*IDE_SetConnection)(char *Username, char *Password, char *Database);
/*FUNC: 71*/ int(*IDE_GetObjectInfo)(char *AnObject, char **ObjectType, char **ObjectOwner, char **ObjectName, char **SubObject);
/*FUNC: 72*/ char *(*IDE_GetBrowserItems)(char *Node, BOOL GetItems);
/*FUNC: 73*/ void(*IDE_RefreshBrowser)(char *Node);
/*FUNC: 74*/ int(*IDE_GetPopupObject)(char **ObjectType, char **ObjectOwner, char **ObjectName, char **SubObject);
/*FUNC: 75*/ char *(*IDE_GetPopupBrowserRoot)();
/*FUNC: 76*/ void(*IDE_RefreshObject)(char *ObjectType, char *ObjectOwner, char *ObjectName, int Action);
/*FUNC: 77*/ BOOL(*IDE_FirstSelectedObject)(char *ObjectType, char *ObjectOwner, char *ObjectName, char *SubObject);
/*FUNC: 78*/ BOOL(*IDE_NextSelectedObject)(char *ObjectType, char *ObjectOwner, char *ObjectName, char *SubObject);
/*FUNC: 79*/ char *(*IDE_GetObjectSource)(char *ObjectType, char *ObjectOwner, char *ObjectName);
/*FUNC: 80*/ int(*IDE_GetWindowCount)();
/*FUNC: 81*/ BOOL(*IDE_SelectWindow)(int Index);
/*FUNC: 82*/ BOOL(*IDE_ActivateWindow)(int Index);
/*FUNC: 83*/ BOOL(*IDE_WindowIsModified)();
/*FUNC: 84*/ BOOL(*IDE_WindowIsRunning)();
/*FUNC: 90*/ void(*IDE_SplashCreate)(int ProgressMax);
/*FUNC: 91*/ void(*IDE_SplashHide)();
/*FUNC: 92*/ void(*IDE_SplashWrite)(char *s);
/*FUNC: 93*/ void(*IDE_SplashWriteLn)(char *s);
/*FUNC: 94*/ void(*IDE_SplashProgress)(int Progress);
/*FUNC: 95*/ char *(*IDE_TemplatePath)();
/*FUNC: 96*/ BOOL(*IDE_ExecuteTemplate)(char *Template, BOOL NewWindow);
/*FUNC: 97*/ char *(*IDE_GetConnectAs)();
/*FUNC: 98*/ BOOL(*IDE_SetConnectionAs)(char *Username, char *Password, char *Database, char *ConnectAs);
/*FUNC: 100*/ char *(*IDE_GetFileOpenMenu)(int MenuIndex, int *WindowType);
/*FUNC: 101*/ BOOL(*IDE_CanSaveWindow)();
/*FUNC: 102*/ void(*IDE_OpenFileExternal)(int WindowType, char *Data, char *FileSystem, char *Tag, char *Filename);
/*FUNC: 103*/ char *(*IDE_GetFileTypes)(int WindowType);
/*FUNC: 104*/ char *(*IDE_GetDefaultExtension)(int WindowType);
/*FUNC: 105*/ char *(*IDE_GetFiledata)();
/*FUNC: 106*/ void(*IDE_FileSaved)(char *FileSystem, char *FileTag, char *Filename);
/*FUNC: 107*/ BOOL(*IDE_ShowHTML)(char *Url, char *Hash, char *Title, char *ID);
/*FUNC: 108*/ BOOL(*IDE_RefreshHTML)(char *Url, char *ID, BOOL            BringToFront);
/*FUNC: 109*/ char *(*IDE_GetProcEditExtension)(char *oType);
/*FUNC: 110*/ BOOL(*IDE_GetWindowObject)(char **ObjectType, char **ObjectOwner, char **ObjectName, char **SubObject);
/*FUNC: 120*/ void(*IDE_KeyPress)(int Key, int Shift);
/*FUNC: 121*/ int(*IDE_GetMenuItem)(char *MenuName);
/*FUNC: 122*/ BOOL(*IDE_SelectMenu)(int MenuItem);
/*FUNC: 130*/ char *(*IDE_TranslationFile)();
/*FUNC: 131*/ char *(*IDE_TranslationLanguage)();
/*FUNC: 132*/ char *(*IDE_GetTranslatedMenuLayout)();
/*FUNC: 140*/ BOOL(*IDE_SaveRecoveryFiles)();
/*FUNC: 141*/ int(*IDE_GetCursorX)();
/*FUNC: 142*/ int(*IDE_GetCursorY)();
/*FUNC: 143*/ void(*IDE_SetCursor)(int X, int Y);
/*FUNC: 144*/ int(*IDE_SetBookmark)(int Index, int X, int Y);
/*FUNC: 145*/ void(*IDE_ClearBookmark)(int Index);
/*FUNC: 146*/ void(*IDE_GotoBookmark)(int Index);
/*FUNC: 147*/ BOOL(*IDE_GetBookmark)(int Index, int X, int Y);
/*FUNC: 148*/ char *(*IDE_TabInfo)(int Index);
/*FUNC: 149*/ int(*IDE_TabIndex)(int Index);
/*FUNC: 150*/ void(*IDE_CreateToolButton)(int ID, int Index, char *Name, char *BitmapFile, HBITMAP BitmapHandle);
/*FUNC: 150*/ /* Old version: void(*IDE_CreateToolButton)(int ID, int Index, char *Name, char *BitmapFile, int BitmapHandle); */
/*FUNC: 160*/ int(*IDE_BeautifierOptions)();
/*FUNC: 161*/ BOOL(*IDE_BeautifyWindow)();
/*FUNC: 162*/ char *(*IDE_BeautifyText)(char *S);
/*FUNC: 165*/ BOOL(*IDE_ObjectAction)(char *Action, char *ObjectType, char *ObjectOwner, char *ObjectName);
/*FUNC: 166*/ BOOL(*IDE_ShowDialog)(char *Dialog, char *Param);
/*FUNC: 173*/ void(*IDE_DebugLog)(char *Msg);
/*FUNC: 174*/ char* (*IDE_GetParamString)(char *Name);
/*FUNC: 175*/ BOOL(*IDE_GetParamBool)(char *Name);
/*FUNC: 180*/ void(*IDE_CommandFeedback)(int FeedbackHandle, char *S);
/*FUNC: 190*/ int(*IDE_ResultGridRowCount)();
/*FUNC: 191*/ int(*IDE_ResultGridColCount)();
/*FUNC: 192*/ char* (*IDE_ResultGridCell)(int Col, int Row);
/*FUNC: 200*/ BOOL(*IDE_Authorized)(char *Category, char *Name, char *SubName);
/*FUNC: 201*/ BOOL(*IDE_WindowAllowed)(int WindowType, BOOL ShowErrorMessage);
/*FUNC: 202*/ BOOL(*IDE_Authorization)();
/*FUNC: 203*/ char* (*IDE_AuthorizationItems)(char *Category);
/*FUNC: 204*/ void(*IDE_AddAuthorizationItem)(int PlugInID, char *Name);
/*FUNC: 210*/ char* (*IDE_GetPersonalPrefSets)();
/*FUNC: 211*/ char* (*IDE_GetDefaultPrefSets)();
/*FUNC: 212*/ BOOL(*IDE_GetPrefAsString)(int PlugInID, char * PrefSet, char *Name, char *Default);
/*FUNC: 213*/ int(*IDE_GetPrefAsInteger)(int PlugInID, char * PrefSet, char *Name, BOOL Default);
/*FUNC: 214*/ BOOL(*IDE_GetPrefAsBool)(int PlugInID, char * PrefSet, char *Name, BOOL Default);
/*FUNC: 215*/ BOOL(*IDE_SetPrefAsString)(int PlugInID, char *PrefSet, char *Name, char *Value);
/*FUNC: 216*/ BOOL(*IDE_SetPrefAsInteger)(int PlugInID, char *PrefSet, char *Name, int Value);
/*FUNC: 217*/ BOOL(*IDE_SetPrefAsBool)(int PlugInID, char *PrefSet, char *Name, BOOL Value);
/*FUNC: 218*/ char* (*IDE_GetGeneralPref)(char *Name);
/*FUNC: 219*/ BOOL(*IDE_PlugInSetting)(int PlugInID, char *Setting, char *Value);
/*FUNC: 220*/ int(*IDE_GetProcOverloadCount)(char *Owner, char *PackageName, char *ProcedureName);
/*FUNC: 221*/ int(*IDE_SelectProcOverloading)(char *Owner, char *PackageName, char *ProcedureName);
/*FUNC: 230*/ char* (*IDE_GetSessionValue)(char *Name);
/*FUNC: 40*/ int(*SQL_Execute)(char *SQL);
/*FUNC: 41*/ int(*SQL_FieldCount)();
/*FUNC: 42*/ BOOL(*SQL_Eof)();
/*FUNC: 43*/ int(*SQL_Next)();
/*FUNC: 44*/ char *(*SQL_Field)(int Field);
/*FUNC: 45*/ char *(*SQL_FieldName)(int Field);
/*FUNC: 46*/ int(*SQL_FieldIndex)(char *Name);
/*FUNC: 47*/ int(*SQL_FieldType)(int Field);
/*FUNC: 48*/ char *(*SQL_ErrorMessage)();
/*FUNC: 50*/ BOOL(*SQL_UsePlugInSession)(int PlugInID);
/*FUNC: 51*/ void(*SQL_UseDefaultSession)(int PlugInID);
/*FUNC: 52*/ BOOL(*SQL_CheckConnection)();
/*FUNC: 53*/ char* (*SQL_GetDBMSGetOutput)();
/*FUNC: 54*/ void(*SQL_SetVariable)(char *Name, char *Value);
/*FUNC: 55*/ char* (*SQL_GetVariable)(char *Name);
/*FUNC: 56*/ void(*SQL_ClearVariables)();

// sorted copies of unsorted entries
t_PlSqlDevFunc *g_PlSqlDevFuncRefs[MAX_PLSQLDEV_FUNCTIONS];

// unsorted entries of callback functions
t_PlSqlDevFunc g_PlSqlDevFuncList[] =
{
	{ 1, (void **)&SYS_Version, "SYS_Version",                                     REF_CALLBACK_UNDEFINED(1) }
	,{ 2, (void **)&SYS_Registry, "SYS_Registry",                                   REF_CALLBACK_UNDEFINED(2) }
	,{ 3, (void **)&SYS_RootDir, "SYS_RootDir",                                     REF_CALLBACK_UNDEFINED(3) }
	,{ 4, (void **)&SYS_OracleHome, "SYS_OracleHome",                               REF_CALLBACK_UNDEFINED(4) }
	,{ 5, (void **)&SYS_OCIDLL, "SYS_OCIDLL",                                       REF_CALLBACK_UNDEFINED(5) }
	,{ 6, (void **)&SYS_OCI8Mode, "SYS_OCI8Mode",                                   REF_CALLBACK_UNDEFINED(6) }
	,{ 7, (void **)&SYS_XPStyle, "SYS_XPStyle",                                     REF_CALLBACK_UNDEFINED(7) }
	,{ 8, (void **)&SYS_TNSNAMES, "SYS_TNSNAMES",                                   REF_CALLBACK_UNDEFINED(8) }
	,{ 10, (void **)&IDE_MenuState, "IDE_MenuState",                                 REF_CALLBACK_UNDEFINED(10) }
	,{ 11, (void **)&IDE_Connected, "IDE_Connected",                                 REF_CALLBACK_UNDEFINED(11) }
	,{ 12, (void **)&IDE_GetConnectionInfo, "IDE_GetConnectionInfo",                 REF_CALLBACK_UNDEFINED(12) }
	,{ 13, (void **)&IDE_GetBrowserInfo, "IDE_GetBrowserInfo",                       REF_CALLBACK_UNDEFINED(13) }
	,{ 14, (void **)&IDE_GetWindowType, "IDE_GetWindowType",                         REF_CALLBACK_UNDEFINED(14) }
	,{ 15, (void **)&IDE_GetAppHandle, "IDE_GetAppHandle",                           REF_CALLBACK_UNDEFINED(15) }
	,{ 16, (void **)&IDE_GetWindowHandle, "IDE_GetWindowHandle",                     REF_CALLBACK_UNDEFINED(16) }
	,{ 17, (void **)&IDE_GetClientHandle, "IDE_GetClientHandle",                     REF_CALLBACK_UNDEFINED(17) }
	,{ 18, (void **)&IDE_GetChildHandle, "IDE_GetChildHandle",                       REF_CALLBACK_UNDEFINED(18) }
	,{ 19, (void **)&IDE_Refresh, "IDE_Refresh",                                     REF_CALLBACK_UNDEFINED(19) }
	,{ 20, (void **)&IDE_CreateWindow, "IDE_CreateWindow",                           REF_CALLBACK_UNDEFINED(20) }
	,{ 21, (void **)&IDE_OpenFile, "IDE_OpenFile",                                   REF_CALLBACK_UNDEFINED(21) }
	,{ 22, (void **)&IDE_SaveFile, "IDE_SaveFile",                                   REF_CALLBACK_UNDEFINED(22) }
	,{ 23, (void **)&IDE_Filename, "IDE_Filename",                                   REF_CALLBACK_UNDEFINED(23) }
	,{ 24, (void **)&IDE_CloseFile, "IDE_CloseFile",                                 REF_CALLBACK_UNDEFINED(24) }
	,{ 25, (void **)&IDE_SetReadOnly, "IDE_SetReadOnly",                             REF_CALLBACK_UNDEFINED(25) }
	,{ 26, (void **)&IDE_GetReadOnly, "IDE_GetReadOnly",                             REF_CALLBACK_UNDEFINED(26) }
	,{ 27, (void **)&IDE_ExecuteSQLReport, "IDE_ExecuteSQLReport",                   REF_CALLBACK_UNDEFINED(27) }
	,{ 28, (void **)&IDE_ReloadFile, "IDE_ReloadFile",                               REF_CALLBACK_UNDEFINED(28) }
	,{ 29, (void **)&IDE_SetFilename, "IDE_SetFilename",                             REF_CALLBACK_UNDEFINED(29) }
	,{ 30, (void **)&IDE_GetText, "IDE_GetText",                                     REF_CALLBACK_UNDEFINED(30) }
	,{ 31, (void **)&IDE_GetSelectedText, "IDE_GetSelectedText",                     REF_CALLBACK_UNDEFINED(31) }
	,{ 32, (void **)&IDE_GetCursorWord, "IDE_GetCursorWord",                         REF_CALLBACK_UNDEFINED(32) }
	,{ 33, (void **)&IDE_GetEditorHandle, "IDE_GetEditorHandle",                     REF_CALLBACK_UNDEFINED(33) }
	,{ 34, (void **)&IDE_SetText, "IDE_SetText",                                     REF_CALLBACK_UNDEFINED(34) }
	,{ 35, (void **)&IDE_SetStatusMessage, "IDE_SetStatusMessage",                   REF_CALLBACK_UNDEFINED(35) }
	,{ 36, (void **)&IDE_SetErrorPosition, "IDE_SetErrorPosition",                   REF_CALLBACK_UNDEFINED(36) }
	,{ 37, (void **)&IDE_ClearErrorPositions, "IDE_ClearErrorPositions",             REF_CALLBACK_UNDEFINED(37) }
	,{ 38, (void **)&IDE_GetCursorWordPosition, "IDE_GetCursorWordPosition",         REF_CALLBACK_UNDEFINED(38) }
	,{ 39, (void **)&IDE_Perform, "IDE_Perform",                                     REF_CALLBACK_UNDEFINED(39) }
	,{ 60, (void **)&IDE_GetCustomKeywords, "IDE_GetCustomKeywords",                 REF_CALLBACK_UNDEFINED(60) }
	,{ 61, (void **)&IDE_SetCustomKeywords, "IDE_SetCustomKeywords",                 REF_CALLBACK_UNDEFINED(61) }
	,{ 62, (void **)&IDE_SetKeywords, "IDE_SetKeywords",                             REF_CALLBACK_UNDEFINED(62) }
	,{ 63, (void **)&IDE_ActivateKeywords, "IDE_ActivateKeywords",                   REF_CALLBACK_UNDEFINED(63) }
	,{ 64, (void **)&IDE_RefreshMenus, "IDE_RefreshMenus",                           REF_CALLBACK_UNDEFINED(64) }
	,{ 65, (void **)&IDE_SetMenuName, "IDE_SetMenuName",                             REF_CALLBACK_UNDEFINED(65) }
	,{ 66, (void **)&IDE_SetMenuCheck, "IDE_SetMenuCheck",                           REF_CALLBACK_UNDEFINED(66) }
	,{ 67, (void **)&IDE_SetMenuVisible, "IDE_SetMenuVisible",                       REF_CALLBACK_UNDEFINED(67) }
	,{ 68, (void **)&IDE_GetMenulayout, "IDE_GetMenulayout",                         REF_CALLBACK_UNDEFINED(68) }
	,{ 69, (void **)&IDE_CreatePopupItem, "IDE_CreatePopupItem",                     REF_CALLBACK_UNDEFINED(69) }
	,{ 70, (void **)&IDE_SetConnection, "IDE_SetConnection",                         REF_CALLBACK_UNDEFINED(70) }
	,{ 71, (void **)&IDE_GetObjectInfo, "IDE_GetObjectInfo",                         REF_CALLBACK_UNDEFINED(71) }
	,{ 72, (void **)&IDE_GetBrowserItems, "IDE_GetBrowserItems",                     REF_CALLBACK_UNDEFINED(72) }
	,{ 73, (void **)&IDE_RefreshBrowser, "IDE_RefreshBrowser",                       REF_CALLBACK_UNDEFINED(73) }
	,{ 74, (void **)&IDE_GetPopupObject, "IDE_GetPopupObject",                       REF_CALLBACK_UNDEFINED(74) }
	,{ 75, (void **)&IDE_GetPopupBrowserRoot, "IDE_GetPopupBrowserRoot",             REF_CALLBACK_UNDEFINED(75) }
	,{ 76, (void **)&IDE_RefreshObject, "IDE_RefreshObject",                         REF_CALLBACK_UNDEFINED(76) }
	,{ 77, (void **)&IDE_FirstSelectedObject, "IDE_FirstSelectedObject",             REF_CALLBACK_UNDEFINED(77) }
	,{ 78, (void **)&IDE_NextSelectedObject, "IDE_NextSelectedObject",               REF_CALLBACK_UNDEFINED(78) }
	,{ 79, (void **)&IDE_GetObjectSource, "IDE_GetObjectSource",                     REF_CALLBACK_UNDEFINED(79) }
	,{ 80, (void **)&IDE_GetWindowCount, "IDE_GetWindowCount",                       REF_CALLBACK_UNDEFINED(80) }
	,{ 81, (void **)&IDE_SelectWindow, "IDE_SelectWindow",                           REF_CALLBACK_UNDEFINED(81) }
	,{ 82, (void **)&IDE_ActivateWindow, "IDE_ActivateWindow",                       REF_CALLBACK_UNDEFINED(82) }
	,{ 83, (void **)&IDE_WindowIsModified, "IDE_WindowIsModified",                   REF_CALLBACK_UNDEFINED(83) }
	,{ 84, (void **)&IDE_WindowIsRunning, "IDE_WindowIsRunning",                     REF_CALLBACK_UNDEFINED(84) }
	,{ 90, (void **)&IDE_SplashCreate, "IDE_SplashCreate",                           REF_CALLBACK_UNDEFINED(90) }
	,{ 91, (void **)&IDE_SplashHide, "IDE_SplashHide",                               REF_CALLBACK_UNDEFINED(91) }
	,{ 92, (void **)&IDE_SplashWrite, "IDE_SplashWrite",                             REF_CALLBACK_UNDEFINED(92) }
	,{ 93, (void **)&IDE_SplashWriteLn, "IDE_SplashWriteLn",                         REF_CALLBACK_UNDEFINED(93) }
	,{ 94, (void **)&IDE_SplashProgress, "IDE_SplashProgress",                       REF_CALLBACK_UNDEFINED(94) }
	,{ 95, (void **)&IDE_TemplatePath, "IDE_TemplatePath",                           REF_CALLBACK_UNDEFINED(95) }
	,{ 96, (void **)&IDE_ExecuteTemplate, "IDE_ExecuteTemplate",                     REF_CALLBACK_UNDEFINED(96) }
	,{ 97, (void **)&IDE_GetConnectAs, "IDE_GetConnectAs",                           REF_CALLBACK_UNDEFINED(97) }
	,{ 98, (void **)&IDE_SetConnectionAs, "IDE_SetConnectionAs",                     REF_CALLBACK_UNDEFINED(98) }
	,{ 100, (void **)&IDE_GetFileOpenMenu, "IDE_GetFileOpenMenu",                     REF_CALLBACK_UNDEFINED(100) }
	,{ 101, (void **)&IDE_CanSaveWindow, "IDE_CanSaveWindow",                         REF_CALLBACK_UNDEFINED(101) }
	,{ 102, (void **)&IDE_OpenFileExternal, "IDE_OpenFileExternal",                   REF_CALLBACK_UNDEFINED(102) }
	,{ 103, (void **)&IDE_GetFileTypes, "IDE_GetFileTypes",                           REF_CALLBACK_UNDEFINED(103) }
	,{ 104, (void **)&IDE_GetDefaultExtension, "IDE_GetDefaultExtension",             REF_CALLBACK_UNDEFINED(104) }
	,{ 105, (void **)&IDE_GetFiledata, "IDE_GetFiledata",                             REF_CALLBACK_UNDEFINED(105) }
	,{ 106, (void **)&IDE_FileSaved, "IDE_FileSaved",                                 REF_CALLBACK_UNDEFINED(106) }
	,{ 107, (void **)&IDE_ShowHTML, "IDE_ShowHTML",                                   REF_CALLBACK_UNDEFINED(107) }
	,{ 108, (void **)&IDE_RefreshHTML, "IDE_RefreshHTML",                             REF_CALLBACK_UNDEFINED(108) }
	,{ 109, (void **)&IDE_GetProcEditExtension, "IDE_GetProcEditExtension",           REF_CALLBACK_UNDEFINED(109) }
	,{ 110, (void **)&IDE_GetWindowObject, "IDE_GetWindowObject",                     REF_CALLBACK_UNDEFINED(110) }
	,{ 120, (void **)&IDE_KeyPress, "IDE_KeyPress",                                   REF_CALLBACK_UNDEFINED(120) }
	,{ 121, (void **)&IDE_GetMenuItem, "IDE_GetMenuItem",                             REF_CALLBACK_UNDEFINED(121) }
	,{ 122, (void **)&IDE_SelectMenu, "IDE_SelectMenu",                               REF_CALLBACK_UNDEFINED(122) }
	,{ 130, (void **)&IDE_TranslationFile, "IDE_TranslationFile",                     REF_CALLBACK_UNDEFINED(130) }
	,{ 131, (void **)&IDE_TranslationLanguage, "IDE_TranslationLanguage",             REF_CALLBACK_UNDEFINED(131) }
	,{ 132, (void **)&IDE_GetTranslatedMenuLayout, "IDE_GetTranslatedMenuLayout",     REF_CALLBACK_UNDEFINED(132) }
	,{ 140, (void **)&IDE_SaveRecoveryFiles, "IDE_SaveRecoveryFiles",                 REF_CALLBACK_UNDEFINED(140) }
	,{ 141, (void **)&IDE_GetCursorX, "IDE_GetCursorX",                               REF_CALLBACK_UNDEFINED(141) }
	,{ 142, (void **)&IDE_GetCursorY, "IDE_GetCursorY",                               REF_CALLBACK_UNDEFINED(142) }
	,{ 143, (void **)&IDE_SetCursor, "IDE_SetCursor",                                 REF_CALLBACK_UNDEFINED(143) }
	,{ 144, (void **)&IDE_SetBookmark, "IDE_SetBookmark",                             REF_CALLBACK_UNDEFINED(144) }
	,{ 145, (void **)&IDE_ClearBookmark, "IDE_ClearBookmark",                         REF_CALLBACK_UNDEFINED(145) }
	,{ 146, (void **)&IDE_GotoBookmark, "IDE_GotoBookmark",                           REF_CALLBACK_UNDEFINED(146) }
	,{ 147, (void **)&IDE_GetBookmark, "IDE_GetBookmark",                             REF_CALLBACK_UNDEFINED(147) }
	,{ 148, (void **)&IDE_TabInfo, "IDE_TabInfo",                                     REF_CALLBACK_UNDEFINED(148) }
	,{ 149, (void **)&IDE_TabIndex, "IDE_TabIndex",                                   REF_CALLBACK_UNDEFINED(149) }
	,{ 150, (void **)&IDE_CreateToolButton, "IDE_CreateToolButton",                   REF_CALLBACK_UNDEFINED(150) }
	,{ 160, (void **)&IDE_BeautifierOptions, "IDE_BeautifierOptions",                 REF_CALLBACK_UNDEFINED(160) }
	,{ 161, (void **)&IDE_BeautifyWindow, "IDE_BeautifyWindow",                       REF_CALLBACK_UNDEFINED(161) }
	,{ 162, (void **)&IDE_BeautifyText, "IDE_BeautifyText",                           REF_CALLBACK_UNDEFINED(162) }
	,{ 165, (void **)&IDE_ObjectAction, "IDE_ObjectAction",                           REF_CALLBACK_UNDEFINED(165) }
	,{ 166, (void **)&IDE_ShowDialog, "IDE_ShowDialog",                               REF_CALLBACK_UNDEFINED(166) }
	,{ 173, (void **)&IDE_DebugLog, "IDE_DebugLog",                                   REF_CALLBACK_UNDEFINED(173) }
	,{ 174, (void **)&IDE_GetParamString, "IDE_GetParamString",                       REF_CALLBACK_UNDEFINED(174) }
	,{ 175, (void **)&IDE_GetParamBool, "IDE_GetParamBool",                           REF_CALLBACK_UNDEFINED(175) }
	,{ 180, (void **)&IDE_CommandFeedback, "IDE_CommandFeedback",                     REF_CALLBACK_UNDEFINED(180) }
	,{ 190, (void **)&IDE_ResultGridRowCount, "IDE_ResultGridRowCount",               REF_CALLBACK_UNDEFINED(190) }
	,{ 191, (void **)&IDE_ResultGridColCount, "IDE_ResultGridColCount",               REF_CALLBACK_UNDEFINED(191) }
	,{ 192, (void **)&IDE_ResultGridCell, "IDE_ResultGridCell",                       REF_CALLBACK_UNDEFINED(192) }
	,{ 200, (void **)&IDE_Authorized, "IDE_Authorized",                               REF_CALLBACK_UNDEFINED(200) }
	,{ 201, (void **)&IDE_WindowAllowed, "IDE_WindowAllowed",                         REF_CALLBACK_UNDEFINED(201) }
	,{ 202, (void **)&IDE_Authorization, "IDE_Authorization",                         REF_CALLBACK_UNDEFINED(202) }
	,{ 203, (void **)&IDE_AuthorizationItems, "IDE_AuthorizationItems",               REF_CALLBACK_UNDEFINED(203) }
	,{ 204, (void **)&IDE_AddAuthorizationItem, "IDE_AddAuthorizationItem",           REF_CALLBACK_UNDEFINED(204) }
	,{ 210, (void **)&IDE_GetPersonalPrefSets, "IDE_GetPersonalPrefSets",             REF_CALLBACK_UNDEFINED(210) }
	,{ 211, (void **)&IDE_GetDefaultPrefSets, "IDE_GetDefaultPrefSets",               REF_CALLBACK_UNDEFINED(211) }
	,{ 212, (void **)&IDE_GetPrefAsString, "IDE_GetPrefAsString",                     REF_CALLBACK_UNDEFINED(212) }
	,{ 213, (void **)&IDE_GetPrefAsInteger, "IDE_GetPrefAsInteger",                   REF_CALLBACK_UNDEFINED(213) }
	,{ 214, (void **)&IDE_GetPrefAsBool, "IDE_GetPrefAsBool",                         REF_CALLBACK_UNDEFINED(214) }
	,{ 215, (void **)&IDE_SetPrefAsString, "IDE_SetPrefAsString",                     REF_CALLBACK_UNDEFINED(215) }
	,{ 216, (void **)&IDE_SetPrefAsInteger, "IDE_SetPrefAsInteger",                   REF_CALLBACK_UNDEFINED(216) }
	,{ 217, (void **)&IDE_SetPrefAsBool, "IDE_SetPrefAsBool",                         REF_CALLBACK_UNDEFINED(217) }
	,{ 218, (void **)&IDE_GetGeneralPref, "IDE_GetGeneralPref",                       REF_CALLBACK_UNDEFINED(218) }
	,{ 219, (void **)&IDE_PlugInSetting, "IDE_PlugInSetting",                         REF_CALLBACK_UNDEFINED(219) }
	,{ 220, (void **)&IDE_GetProcOverloadCount, "IDE_GetProcOverloadCount",           REF_CALLBACK_UNDEFINED(220) }
	,{ 221, (void **)&IDE_SelectProcOverloading, "IDE_SelectProcOverloading",         REF_CALLBACK_UNDEFINED(221) }
	,{ 230, (void **)&IDE_GetSessionValue, "IDE_GetSessionValue",                     REF_CALLBACK_UNDEFINED(230) }
	,{ 40, (void **)&SQL_Execute, "SQL_Execute",                                     REF_CALLBACK_UNDEFINED(40) }
	,{ 41, (void **)&SQL_FieldCount, "SQL_FieldCount",                               REF_CALLBACK_UNDEFINED(41) }
	,{ 42, (void **)&SQL_Eof, "SQL_Eof",                                             REF_CALLBACK_UNDEFINED(42) }
	,{ 43, (void **)&SQL_Next, "SQL_Next",                                           REF_CALLBACK_UNDEFINED(43) }
	,{ 44, (void **)&SQL_Field, "SQL_Field",                                         REF_CALLBACK_UNDEFINED(44) }
	,{ 45, (void **)&SQL_FieldName, "SQL_FieldName",                                 REF_CALLBACK_UNDEFINED(45) }
	,{ 46, (void **)&SQL_FieldIndex, "SQL_FieldIndex",                               REF_CALLBACK_UNDEFINED(46) }
	,{ 47, (void **)&SQL_FieldType, "SQL_FieldType",                                 REF_CALLBACK_UNDEFINED(47) }
	,{ 48, (void **)&SQL_ErrorMessage, "SQL_ErrorMessage",                           REF_CALLBACK_UNDEFINED(48) }
	,{ 50, (void **)&SQL_UsePlugInSession, "SQL_UsePlugInSession",                   REF_CALLBACK_UNDEFINED(50) }
	,{ 51, (void **)&SQL_UseDefaultSession, "SQL_UseDefaultSession",                 REF_CALLBACK_UNDEFINED(51) }
	,{ 52, (void **)&SQL_CheckConnection, "SQL_CheckConnection",                     REF_CALLBACK_UNDEFINED(52) }
	,{ 53, (void **)&SQL_GetDBMSGetOutput, "SQL_GetDBMSGetOutput",                   REF_CALLBACK_UNDEFINED(53) }
	,{ 54, (void **)&SQL_SetVariable, "SQL_SetVariable",                             REF_CALLBACK_UNDEFINED(54) }
	,{ 55, (void **)&SQL_GetVariable, "SQL_GetVariable",                             REF_CALLBACK_UNDEFINED(55) }
	,{ 56, (void **)&SQL_ClearVariables, "SQL_ClearVariables",                       REF_CALLBACK_UNDEFINED(56) }

	// end marker
	,{ 0, NULL, NULL, NULL }
};

int UndefinedPlSqlDevCallback(int nFuncID)
{
	char ErrMsg[500];
	sprintf_s(ErrMsg, "Callback Undefined %d, %s", nFuncID, (g_PlSqlDevFuncRefs[nFuncID] ? g_PlSqlDevFuncRefs[nFuncID]->m_pszFuncDesc : ""));
	MessageBox(NULL,ErrMsg, "UndefinedPlSqlDevCallback", MB_ICONERROR);
	return 0;
}

//*****************************************************************************
//
//    Plug-In Exported Functions
//
// TRACE() is a macro used to output text to the debug console of MS Dev IDE. You can remove
// this or change it to whatever your fancy.
//
// PlSqlDevChildHandleTracer is used to trace the plug-in changing the selected window with
// IDE_SelectWindow. This is only used for diagnostics and removing it won't affect the
// operation of the functions.
//
// All calls are directed to theApp.... which is a global variable for the MFC Application
// class. Change the call to your implementation or even add the implementation right in these
// functions.
//
// The only function that should not require modification is RegisterCallback because it
// initializes the callback pointers based on the g_PlSqlDevFuncRefs array.
//
//****X******************X************************X************************X***

// There are several functions in PL/SQL Developer that you can use from your Plug-In. With
// this function you can get access to the callback functions you need.  The Index is related to
// a specific callback function while the Addr parameter holds the address to this function.
void RegisterCallback(int nIndex, void *pvAddr)
{
	// This function should not need modification. It is driven by the table of callback
	// function definitions. All additional functions should be added to the g_PlSqlDevFuncRefs
	// table.
	if (nIndex > 0 && nIndex < MAX_PLSQLDEV_FUNCTIONS && g_PlSqlDevFuncRefs[nIndex])
	{
		*g_PlSqlDevFuncRefs[nIndex]->m_pFuncPtr = pvAddr;
	}
}

void OnCreateInitPtrArray() {
	t_PlSqlDevFunc *p;

	// copy dense unsorted list to sparse sorted one
	for (p = g_PlSqlDevFuncList; p->m_nFuncID; p++)
	{
		g_PlSqlDevFuncRefs[p->m_nFuncID] = p;
	}

	// make all callback functions point to their undefined defaults
	for (p = g_PlSqlDevFuncList; p->m_nFuncID; p++)
	{
		*p->m_pFuncPtr = p->m_pFuncPtrUndef;
	}

}

