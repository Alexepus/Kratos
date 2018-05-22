#pragma once
#include <array>

struct BUTTON
{
	CString Name;
	HMENU ResourceId;
	CRect Rect; // ������������� ������� ������ �� ������� �������� ���� ����
	HWND hWnd;
};

class CRegionWndButtons
{
	const int ButtonWidth = 80;
	const int ButtonOffset = 5;
	const int ButtonHeight = 25;

public:
	CRegionWndButtons();
	~CRegionWndButtons();
	void CreateButtonWindows(HWND hWndParent);
	void Resize(const RECT& parentRect);

	const int PanelWidth = ButtonWidth + 2 * ButtonOffset;

	BUTTON BtnAddNew;
	BUTTON BtnEdit;
	BUTTON BtnDelete;
	BUTTON BtnOnOff;
	BUTTON BtnView;
	BUTTON BtnCopy;
	BUTTON BtnUp;
	BUTTON BtnDown;
	std::array<BUTTON*, 8> Buttons;

private:
	void CreateButton(BUTTON& button, HWND hWndParent, RECT parentRect);
	void MoveButton(BUTTON& button, RECT parentRect);
};

