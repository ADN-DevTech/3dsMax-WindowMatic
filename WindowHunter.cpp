#include "WindowHunter.h"


WindowHunter* WindowHunter::hunter = nullptr;
HWND WindowHunter::candidate = nullptr;


WindowHunter* WindowHunter::getWindowHunterInstance()
{
	return (hunter != nullptr) ? hunter : new WindowHunter();
}

WindowHunter::WindowHunter() :
	is_hunting(false),
	capturing(false),
	hunting_dialog(0)
{
	target_cursor= LoadCursor(nullptr, IDC_CROSS);
}

WindowHunter::~WindowHunter()
{}

bool WindowHunter::isHunting()
{
	return is_hunting;
}

void WindowHunter::sniffWindows(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT screee_point;
	HWND found_window;

	GetCursorPos(&screee_point);
	found_window = WindowFromPoint(screee_point);
	if(candidate != found_window)
	{
		reviewFoundWindow(candidate);
		candidate = found_window;
		highlightFoundWindow(candidate);
	}
}



void WindowHunter::highlightFoundWindow(HWND found_window)
{
	HGDIOBJ pen_bkp;
	HGDIOBJ brush_bkp;
	RECT window_rectangle;
	HDC found_context;
	HPEN highlight_pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	GetWindowRect(found_window, &window_rectangle);
	found_context = GetWindowDC(found_window);
	if(found_context)
	{
		pen_bkp = SelectObject(found_context, highlight_pen);
		brush_bkp = SelectObject(found_context, GetStockObject(HOLLOW_BRUSH));
		Rectangle(found_context,
			0,
			0,
			window_rectangle.right - window_rectangle.left,
			window_rectangle.bottom - window_rectangle.top);
		SelectObject(found_context, pen_bkp);
		SelectObject(found_context, brush_bkp);
		ReleaseDC(found_window, found_context);
	}

}

void WindowHunter::reviewFoundWindow(HWND found_window)
{
	InvalidateRect(found_window, nullptr, TRUE);
	UpdateWindow(found_window);
	RedrawWindow(found_window,
		nullptr,
		nullptr,
		RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	
}

HWND WindowHunter::startHunting(HWND starting_base)
{
	is_hunting = true;
	hunting_dialog = DialogBox(nullptr, 
								MAKEINTRESOURCE(IDD_HOUND), 
								starting_base, 
								huntingWindow);
	DebugPrint(_T("Found candidate: %p\n"), candidate);
	return candidate;
}

bool WindowHunter::isCapturing()
{
	return capturing;
}

void WindowHunter::switchCapturing()
{
	capturing = !capturing;
}




INT_PTR WindowHunter::huntingWindow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!HUNTER->capturing)
	{
		SetCapture(hWnd);
		HUNTER->switchCapturing();
	}

	switch(msg)
	{
	case WM_INITDIALOG: return true;
	
	case WM_MOUSEMOVE:
	{
		SetCursor(HUNTER->target_cursor);
		HUNTER->sniffWindows(hWnd, msg, wParam, lParam);
		break;
	}
		
	case WM_LBUTTONUP:
	{
		DebugPrint(_T("Button was released!!\n"));
		HUNTER->switchCapturing();
		ReleaseCapture();
		EndDialog(hWnd, wParam);
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
		return false;
	}
		

	defaut: return false;
	}



	return true;
}

