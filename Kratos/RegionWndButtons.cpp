#include "stdafx.h"
#include "RegionWndButtons.h"
#include "resource.h"


CRegionWndButtons::CRegionWndButtons()
{
	cx = 80;
	cy = 25;
	AddNew.Name = "Add New";
	Edit.Name = "Edit";
	Delete.Name = "Delete";
	OnOff.Name = "On/Off";
	View.Name = "View";
}

void CRegionWndButtons::CreateButtonWindows(HWND hWndParent)
{
	RECT r;
	::GetClientRect(hWndParent, &r);
	AddNew.hWnd = ::CreateWindow("button", AddNew.Name,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPSIBLINGS,
		r.right - cx - 5, 5,
		cx, cy,
		hWndParent, (HMENU)IDC_BUTTON_ADDNEW, AfxGetInstanceHandle(), NULL);

	Edit.hWnd = ::CreateWindow("button", Edit.Name,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPSIBLINGS,
		r.right - cx - 5, 5 * 2 + cy,
		cx, cy,
		hWndParent, (HMENU)IDC_BUTTON_EDIT, AfxGetInstanceHandle(), NULL);
	Delete.hWnd = ::CreateWindow("button", Delete.Name,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPSIBLINGS,
		r.right - cx - 5, 5 * 3 + 2 * cy,
		cx, cy,
		hWndParent, (HMENU)IDC_BUTTON_DELETE, AfxGetInstanceHandle(), NULL);
	View.hWnd = ::CreateWindow("button", View.Name,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPSIBLINGS,
		r.right - cx - 5, 5 * 4 + 3 * cy,
		cx, cy,
		hWndParent, (HMENU)IDC_BUTTON_VIEW, AfxGetInstanceHandle(), NULL);
	OnOff.hWnd = ::CreateWindow("button", OnOff.Name,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPSIBLINGS,
		r.right - cx - 5, 5 * 5 + 3 * cy,
		cx, cy,
		hWndParent, (HMENU)IDC_BUTTON_ONOFF, AfxGetInstanceHandle(), NULL);
}
