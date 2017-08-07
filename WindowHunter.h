#pragma once
#include <windows.h>
#include "resource.h"
#include "dbgprint.h"

#define HUNTER WindowHunter::getWindowHunterInstance() 

class WindowHunter
{
public:
	HWND startHunting(HWND starting_base);
	bool isHunting();
	static WindowHunter* getWindowHunterInstance();

private:
	WindowHunter();
	~WindowHunter();
	void sniffWindows(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void highlightFoundWindow(HWND found_window);
	void reviewFoundWindow(HWND candidate);

	static WindowHunter* hunter;

	HCURSOR target_cursor;
	static HWND candidate;
	bool is_hunting;
	bool capturing;

	bool isCapturing();


	void switchCapturing();


	long hunting_dialog;



	static INT_PTR CALLBACK huntingWindow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

