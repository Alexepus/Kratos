#include "StdAfx.h"
#include "RegionWndButtons.h"
#include "resource.h"


CRegionWndButtons::CRegionWndButtons()
{
	const int buttonsLeft = -ButtonWidth - ButtonOffset;
	SIZE stdSize = { ButtonWidth,  ButtonHeight };

	BtnAddNew = 
		{ 
			"Add New", 
			(HMENU)IDC_BUTTON_ADDNEW, 
			CRect(POINT{buttonsLeft, ButtonOffset }, stdSize),
			nullptr 
		};

	BtnEdit =
		{
			"Edit",
			(HMENU)IDC_BUTTON_EDIT,
			CRect(POINT{ buttonsLeft, BtnAddNew.Rect.bottom + ButtonOffset}, stdSize),
			nullptr
		};

	BtnDelete =
	{
		"Delete",
		(HMENU)IDC_BUTTON_DELETE,
		CRect(POINT{ buttonsLeft, BtnEdit.Rect.bottom + ButtonOffset }, stdSize),
		nullptr
	};

	BtnView =
	{
		"View",
		(HMENU)IDC_BUTTON_VIEW,
		CRect(POINT{ buttonsLeft, BtnDelete.Rect.bottom + ButtonOffset }, stdSize),
		nullptr
	};

	BtnOnOff =
	{
		"On/Off",
		(HMENU)IDC_BUTTON_ONOFF,
		CRect(POINT{ buttonsLeft, BtnView.Rect.bottom + ButtonOffset }, stdSize),
		nullptr
	};

	BtnUp =
	{
		"/\\",
		(HMENU)IDC_BUTTON_UP,
		CRect(POINT{ buttonsLeft, BtnOnOff.Rect.bottom + ButtonOffset }, SIZE { (ButtonWidth - ButtonOffset)/2,  ButtonHeight }),
		nullptr
	};

	BtnDown =
	{
		"\\/",
		(HMENU)IDC_BUTTON_DOWN,
		CRect(POINT{ BtnUp.Rect.right + ButtonOffset, BtnOnOff.Rect.bottom + ButtonOffset }, SIZE { (ButtonWidth - ButtonOffset) / 2,  ButtonHeight }),
		nullptr
	};
	Buttons = { &BtnAddNew, &BtnEdit, &BtnView, &BtnDelete, &BtnOnOff, &BtnUp, &BtnDown };
}

CRegionWndButtons::~CRegionWndButtons()
{
	for (auto b : Buttons)
		::DestroyWindow(b->hWnd);
}

void CRegionWndButtons::CreateButtonWindows(HWND hWndParent)
{
	RECT r;
	::GetClientRect(hWndParent, &r);

	for (auto b : Buttons)
		CreateButton(*b,hWndParent, r);	
}

void CRegionWndButtons::Resize(const RECT& parentRect)
{
	for (auto b : Buttons)
		MoveButton(*b, parentRect);
}

void CRegionWndButtons::CreateButton(BUTTON& button, HWND hWndParent, RECT parentRect)
{
	button.hWnd = ::CreateWindow("button", button.Name,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPSIBLINGS,
		parentRect.right + button.Rect.left, button.Rect.top, button.Rect.Width(), button.Rect.Height(),
		hWndParent, button.ResourceId, AfxGetInstanceHandle(), NULL);
}

void CRegionWndButtons::MoveButton(BUTTON& button, RECT parentRect)
{
	::MoveWindow(button.hWnd, parentRect.right + button.Rect.left, button.Rect.top, button.Rect.Width(), button.Rect.Height(), TRUE);
}
