#pragma once
struct BUTTON
{
	CString Name;
	HMENU ResourceId;
	CRect Rect; // Относительные размеры кнопки от правого верхнего угла окна
	HWND hWnd;
};

class CRegionWndButtons
{
	const int ButtonWidth = 80;
	const int ButtonOffset = 5;
	const int ButtonHeight = 25;

public:
	CRegionWndButtons();
	void CreateButtonWindows(HWND hWndParent);
	void Resize(const RECT& parentRect);

	const int PanelWidth = ButtonWidth + 2 * ButtonOffset;

	BUTTON BtnAddNew;
	BUTTON BtnEdit;
	BUTTON BtnDelete;
	BUTTON BtnOnOff;
	BUTTON BtnView;
	BUTTON BtnUp;
	BUTTON ButtonDown;

private:
	void CreateButton(BUTTON& button, HWND hWndParent, RECT parentRect);
	void MoveButton(BUTTON& button, RECT parentRect);
};

