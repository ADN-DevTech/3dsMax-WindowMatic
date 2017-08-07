#pragma once
#include <maxapi.h>
#include <map>

#define MIMIKER Mimiker::getInstance()

class Mimiker :
	public ViewWindow
{
public:

	const wchar_t* GetName() override;
	
	HWND CreateViewWindow(HWND hParent, int x, int y, int w, int h) override;
	
	void DestroyViewWindow(HWND hWnd) override;
	
	ViewWindowMetaData GetMetaData() override;
	
	Class_ID ClassID() const override;

	static Mimiker* getInstance();

	void registerNewWindow(HWND foreign_window, bool);

private:
	static Mimiker* mimiker;
	static HWND foreign_window;
	static HWND current_parent;
	static HWND previous_parent;
	static bool is_internal_window;
	Mimiker();
	~Mimiker();
};

