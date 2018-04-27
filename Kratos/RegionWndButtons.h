#pragma once
struct BUTTON
{
	CString Name;
	HMENU ResourceId;
	CRect Rect;
	HWND hWnd;
};

class CRegionWndButtons
{
public:
	CRegionWndButtons();
	void CreateButtonWindows(HWND hWndParent);

	int cx;
	int cy;
	BUTTON AddNew;
	BUTTON Edit;
	BUTTON Delete;
	BUTTON OnOff;
	BUTTON View;

private:
	void CreateButton(BUTTON button, HWND hWndParent);
};

