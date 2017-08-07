//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: Denis Grigor
//***************************************************************************/

#include "WindowMatic.h"

#include <mouseman.h>



#define WindowMatic_CLASS_ID	Class_ID(0x932cb0fa, 0x34f8e91b)


class WindowMatic : public UtilityObj 
{
public:
		
	//Constructor/Destructor
	WindowMatic();
	virtual ~WindowMatic();

	virtual void DeleteThis() { }
	
	virtual void BeginEditParams(Interface *ip,IUtil *iu);
	virtual void EndEditParams(Interface *ip,IUtil *iu);

	virtual void Init(HWND hWnd);
	virtual void Destroy(HWND hWnd);
	
	// Singleton access
	static WindowMatic* GetInstance() { 
		static WindowMatic theWindowMatic;
		return &theWindowMatic; 
	}

private:
	void processTheWindow(HWND window, bool);

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND   hPanel;
	IUtil* iu;
};


class WindowMaticClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 	{ return WindowMatic::GetInstance(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return UTILITY_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return WindowMatic_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("WindowMatic"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetWindowMaticDesc() { 
	static WindowMaticClassDesc WindowMaticDesc;
	return &WindowMaticDesc; 
}




//--- WindowMatic -------------------------------------------------------
WindowMatic::WindowMatic()
	: hPanel(nullptr)
	, iu(nullptr)
{
	
}

WindowMatic::~WindowMatic()
{

}

void WindowMatic::BeginEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = iu;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		DlgProc,
		GetString(IDS_PARAMS),
		0);
}
	
void WindowMatic::EndEditParams(Interface* ip,IUtil*)
{
	this->iu = nullptr;
	ip->DeleteRollupPage(hPanel);
	hPanel = nullptr;
}

void WindowMatic::Init(HWND /*handle*/)
{

}

void WindowMatic::Destroy(HWND /*handle*/)
{

}

INT_PTR CALLBACK WindowMatic::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND target_window = nullptr;
	bool is_internal_window = false;
	if(HUNTER->isHunting())
	{
		SetCursor(LoadCursor(nullptr, IDC_CROSS));
	}
	switch (msg) 
	{
		case WM_INITDIALOG:
		{
			WindowMatic::GetInstance()->Init(hWnd);
			break;
		}

		case WM_DESTROY:
		{
			WindowMatic::GetInstance()->Destroy(hWnd);
			break;
		}

		case WM_COMMAND:
		{
			switch (wParam)
			{
				case IDC_TARGET_BTN:
				{
					if (IsDlgButtonChecked(hWnd,IDC_CHECK_INTERN) != BST_CHECKED)
					{
						ShowWindow(GetCOREInterface()->GetMAXHWnd(), SW_MINIMIZE);
						is_internal_window = true;
					}
					
					WindowMatic::GetInstance()->processTheWindow(HUNTER->startHunting(hWnd), is_internal_window);
					
					if (is_internal_window)
					{
						ShowWindow(GetCOREInterface()->GetMAXHWnd(), SW_RESTORE);
					}
					
					break;
				}

			}
			break;
		}
			


		case WM_LBUTTONDOWN:
		{
			break;
		}
		case WM_LBUTTONUP:
		{
			break;
		}
		case WM_MOUSEMOVE:
			break;

		default:
			return 0;
	}
	return 1;
}

void WindowMatic::processTheWindow(HWND window, bool is_internal)
{
	MIMIKER->registerNewWindow(window, is_internal);
	GetCOREInterface()->RegisterViewWindow(MIMIKER);
}