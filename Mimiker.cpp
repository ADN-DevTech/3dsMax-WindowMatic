#include "Mimiker.h"
#include <dbgprint.h>

Mimiker* Mimiker::mimiker = nullptr;;
HWND Mimiker::foreign_window = nullptr;
HWND Mimiker::current_parent = nullptr;
HWND Mimiker::previous_parent = nullptr;
bool Mimiker::is_internal_window = false;

Mimiker::Mimiker()
{}


Mimiker::~Mimiker()
{}


const wchar_t* Mimiker::GetName()
{
	return _T("Foreign Window");
}

HWND Mimiker::CreateViewWindow(HWND hParent, int x, int y, int h, int w)
{
	if (foreign_window != nullptr)
	{
		current_parent = hParent;
		SetParent(foreign_window, hParent);
		MoveWindow(foreign_window, x, y, w, h, TRUE);
		SetWindowPos(foreign_window,nullptr, x, y, h, w, SWP_NOMOVE | SWP_NOZORDER);
		ShowWindow(foreign_window, SW_RESTORE);
		return foreign_window;
	}

	return GetCOREInterface()->GetActiveViewExp().GetHWnd();
}

void Mimiker::DestroyViewWindow(HWND hWnd)
{
	if(!is_internal_window)
	{
		DebugPrint(_T("This is an internal window\n"));
		DestroyWindow(hWnd);
	}
	else
	{
		DebugPrint(_T("This is EXTERNAL WINDOW, thus restoring\n"));
		SetParent(hWnd, previous_parent);
	}
	
}

ViewWindowMetaData Mimiker::GetMetaData()
{
	// Here we want only one instance of this window,
	// yet it would be interesting to "clone" a window
	// into all 3ds Max ViewPorts
	return ViewWindowMetaData(ViewWindowMetaData::SINGLETON);
}

Class_ID Mimiker::ClassID() const
{
	// created with gencid.exe
	return Class_ID(0x13dc67cd, 0x58833ef2);
}

Mimiker* Mimiker::getInstance()
{
	return (mimiker != nullptr) ? mimiker : new Mimiker();
}

void Mimiker::registerNewWindow(HWND foreign_window, bool is_internal)
{
	Mimiker::is_internal_window = is_internal;
	Mimiker::foreign_window = foreign_window;
	previous_parent = GetParent(Mimiker::foreign_window);

	if (Mimiker::foreign_window == nullptr)
	{
		DebugPrint(_T("Could not aquire the necessary windows: "));		
		DebugPrint(_T("PARENT = %p, FOREING = %p"), 
			previous_parent, foreign_window);
		return; // TODO: Check why kannot aquire the previous parent of the target window

	}

}