// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "Spec.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::SetColor(int Color)
{

}



void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT uStyle = DFCS_BUTTONPUSH;
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	// If drawing selected, add the pushed style to DrawFrameControl.
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	  uStyle |= DFCS_PUSHED;
	CRect rect=lpDrawItemStruct->rcItem;
	// Draw the button frame.
	if(lpDrawItemStruct->itemState&ODS_FOCUS)
	{
	CBrush Brush(RGB(0,0,0));
	dc.FrameRect(&lpDrawItemStruct->rcItem,&Brush);
	rect.DeflateRect(1,1);
	}
	::DrawFrameControl(lpDrawItemStruct->hDC, &rect, 
	  DFC_BUTTON, uStyle);
	CString strText;
	GetWindowText(strText);

	// Draw the button text using the text color red.
	COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255,0,0));
	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
	  &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}
