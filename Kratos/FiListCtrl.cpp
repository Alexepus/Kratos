// FiListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"
#include "FiListCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern FARPROC EditNormalProc;
extern FARPROC EditNumberProc;

//////////////////////////////////////////////////////////////////////////////
//Sort Function
static int CALLBACK AlphabeticalOrder(LPARAM lParam1, LPARAM lParam2,
            LPARAM lParamSort)
 {
      // lParamSort contains a pointer to the list view control.
      // The lParam of an item is just its index.
      CFiListCtrl* pListCtrl = (CFiListCtrl*) lParamSort;

	  LV_FINDINFO a1;
	  LV_FINDINFO a2;

	  a1.flags = LVFI_PARAM;	a2.flags = LVFI_PARAM;
	  a1.lParam=lParam1;		a2.lParam=lParam2;	

      CString strItem1 = pListCtrl->GetItemText(lParam1,pListCtrl->ColumnSort );
      CString strItem2 = pListCtrl->GetItemText(lParam2,pListCtrl->ColumnSort );

      return strcmp(strItem1, strItem2);
 }

static int CALLBACK NumericalIncreaseSort(LPARAM lParam1, LPARAM lParam2,
            LPARAM lParamSort)
 {
 
      CFiListCtrl* pListCtrl = (CFiListCtrl*) lParamSort;

	  LV_FINDINFO a1;
	  LV_FINDINFO a2;

	  a1.flags = LVFI_PARAM;	a2.flags = LVFI_PARAM;
	  a1.lParam=lParam1;		a2.lParam=lParam2;	

      double b1 = atof(pListCtrl->GetItemText(pListCtrl->FindItem(&a1,-1),pListCtrl->ColumnSort ));
      double b2 = atof(pListCtrl->GetItemText(pListCtrl->FindItem(&a2,-1),pListCtrl->ColumnSort ));

      return (b1>b2);
 }
/////////////////////////////////////////////////////////////////////////////
//CFiEdit

CFiEdit::CFiEdit()
{
	m_x=0;m_y=0;
	m_cx=0;m_cy=0;
}

BEGIN_MESSAGE_MAP(CFiEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFiEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->x=m_x;
	lpwndpos->y=m_y;
	if (lpwndpos->cx<m_cx)lpwndpos->cx=m_cx;
	if (lpwndpos->cy<m_cy)lpwndpos->cy=m_cy;
	CEdit::OnWindowPosChanging(lpwndpos);
}

/////////////////////////////////////////////////////////////////////////////
// CFiListCtrl

CFiListCtrl::CFiListCtrl()
{
	
	m_bFullRowSel = FALSE;
	m_bClientWidthSel = TRUE;

	m_cxClient = 0;
	m_cxStateImageOffset = 1;
	ColumnSort=0;

	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_nEdit=-1;

}

CFiListCtrl::~CFiListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFiListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFiListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiListCtrl message handlers
void CFiListCtrl::SetStyle(DWORD NewStile)
{
	

	ASSERT(::IsWindow(m_hWnd ));
	DWORD a2 = GetWindowLong(m_hWnd,GWL_STYLE);
	if ((a2 & LVS_TYPEMASK) != NewStile)
		SetWindowLong(m_hWnd, GWL_STYLE , (a2 & ~LVS_TYPEMASK) | NewStile);

}

void CFiListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
	////////////////////
	// Не буду скрывать основу содрал у мелкомягких. LPCTSTR CFiListCtrl::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
	// тоже их изобретение.
	// Далее по тексту будут кометарии если их убрать, то проявится очень интересный контрол
	//
	//
	////////////////////


	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	UINT uiFlags = ILD_TRANSPARENT;
	CImageList* pImageList;
	int nItem = lpDrawItemStruct->itemID;
	BOOL bFocus = (GetFocus() == this);
	COLORREF clrTextSave, clrBkSave;
	COLORREF clrImage = m_clrBkgnd;
	static _TCHAR szBuff[MAX_PATH];
	LPCTSTR pszText;

	

// get item data

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.pszText = szBuff;
	lvi.cchTextMax = sizeof(szBuff);
	lvi.stateMask = 0xFFFF;     // get all state flags
	CFiListCtrl::GetItem(&lvi);

	BOOL bSelected = (bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
	bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

// set colors if item is selected

	CRect rcAllLabels;
	CFiListCtrl::GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);

	CRect rcLabel;
	CFiListCtrl::GetItemRect(nItem, rcLabel, LVIR_LABEL);

///////////////////////////////////////////////////////////////////////
// Это если через одну меняем цвет фона и текста
// тут можно добавить и шрифт, тогда вообще получается круто
// Об этом позднее
//	

	div_t div_result;
	//COLORREF d1;
	COLORREF newc;
	div_result = div( nItem, 2 );
   	if (div_result.rem == 0 )
	{
		d0=d1;
		newc=m_text1;
	}
	else
	{
		d0=d2;
		newc=m_text2;
	}


///////////////////////////////////////////////////////////////////////	

	rcAllLabels.left = rcLabel.left;
	if (m_bClientWidthSel && rcAllLabels.right<m_cxClient)
		rcAllLabels.right = m_cxClient;

	if (bSelected)
	{
		clrTextSave = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clrBkSave = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC->FillRect(rcAllLabels, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
///////////////////////////////////////////////////////////////////////	
//	тут рисуем наш фон от меломягких под коментарием
	//pDC->FillRect(rcAllLabels, &CBrush(m_clrTextBk));
		pDC->FillRect(rcAllLabels, &CBrush(d0));
///////////////////////////////////////////////////////////////////////	

// set color and mask for the icon

	if (lvi.state & LVIS_CUT)
	{
		clrImage = m_clrBkgnd;
		uiFlags |= ILD_BLEND50;
	}
	else if (bSelected)
	{
		clrImage = ::GetSysColor(COLOR_HIGHLIGHT);
		uiFlags |= ILD_BLEND50;
	}

// draw state icon

	UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
	if (nStateImageMask)
	{
		int nImage = (nStateImageMask>>12) - 1;
		pImageList = CFiListCtrl::GetImageList(LVSIL_STATE);
		if (pImageList)
		{
			pImageList->Draw(pDC, nImage,
				CPoint(rcItem.left, rcItem.top), ILD_TRANSPARENT);
		}
	}

// draw normal and overlay icon

	CRect rcIcon;
	CFiListCtrl::GetItemRect(nItem, rcIcon, LVIR_ICON);

	pImageList = CFiListCtrl::GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		if (rcItem.left<rcItem.right-1)
		{
			ImageList_DrawEx(pImageList->m_hImageList, lvi.iImage,
					pDC->m_hDC,rcIcon.left,rcIcon.top, 16, 16,
					m_clrBkgnd, clrImage, uiFlags | nOvlImageMask);
		}
	}

// draw item label

	CFiListCtrl::GetItemRect(nItem, rcItem, LVIR_LABEL);
	rcItem.right -= m_cxStateImageOffset;

	pszText = MakeShortString(pDC, szBuff,
				rcItem.right-rcItem.left, 2*OFFSET_FIRST);

	rcLabel = rcItem;
	rcLabel.left += OFFSET_FIRST;
	rcLabel.right -= OFFSET_FIRST;
///////////////////////////////////////////////////////////////////////	
// Тут ставим наш цвет, но только на первый элемент, если надо дольше 
//	то 
///////////////////////////////////////////////////////////////////////	
	COLORREF oldc=pDC->GetTextColor();
//	pDC->SetTextColor(newc);

	pDC->DrawText(pszText,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
/////////////////////////////////////////////////////////////////////////	
//	строку, что ниже перенести после следующего цикла 
	pDC->SetTextColor(oldc);


// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for(int nColumn = 1; CFiListCtrl::GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;

		int nRetLen = CFiListCtrl::GetItemText(nItem, nColumn,
						szBuff, sizeof(szBuff));
		if (nRetLen == 0)
			continue;

		pszText = MakeShortString(pDC, szBuff,
			rcItem.right - rcItem.left, 2*OFFSET_OTHER);

		UINT nJustify = DT_LEFT;

		if(pszText == szBuff)
		{
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nJustify = DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nJustify = DT_CENTER;
				break;
			default:
				break;
			}
		}

		rcLabel = rcItem;
		rcLabel.left += OFFSET_OTHER;
		rcLabel.right -= OFFSET_OTHER;

		pDC->DrawText(pszText, -1, rcLabel,
			nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
	}

// draw focus rectangle if item has focus

	if (lvi.state & LVIS_FOCUSED && bFocus)
		pDC->DrawFocusRect(rcAllLabels);

// set original colors if item was selected

	if (bSelected)
	{
		pDC->SetTextColor(clrTextSave);
		pDC->SetBkColor(clrBkSave);
	}
// Вот вроде и все. 

}

LPCTSTR CFiListCtrl::MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset)
{
	static const _TCHAR szThreeDots[] = _T("...");

	int nStringLen = lstrlen(lpszLong);

	if(nStringLen == 0 ||
		(pDC->GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	static _TCHAR szShort[MAX_PATH];

	lstrcpy(szShort,lpszLong);
	int nAddLen = pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i = nStringLen-1; i > 0; i--)
	{
		szShort[i] = 0;
		if((pDC->GetTextExtent(szShort, i).cx + nOffset + nAddLen)
			<= nColumnLen)
		{
			break;
		}
	}

	lstrcat(szShort, szThreeDots);
	return(szShort);
}

void CFiListCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	// TODO: Add your control notification handler code here
	
	LV_DISPINFO* pDispInfo=(LV_DISPINFO*)pNMHDR;

	//Это нашел гдето в недрах примерах или MSDN
	// Может кому будет интересно
	//LV_ITEM		 *plvItem = &pDispInfo->item;

    CPoint posMouse;

    GetCursorPos(&posMouse);

    ScreenToClient(&posMouse);

	LV_HITTESTINFO htest;
	//LVHITTESTINFO htest;

	htest.pt=posMouse;

	htest.flags=0;
 
    CRect rcItem;
	
	CFiListCtrl::SubItemHitTest(&htest);

    HWND hWnd=(HWND)SendMessage(LVM_GETEDITCONTROL);
    ASSERT(hWnd!=NULL);
    VERIFY(m_LVEdit.SubclassWindow(hWnd));
	
		
	//char ss[64];
	//sprintf(ss,"htest.iItem = %i\n\nhtest.iSubItem = %i",htest.iItem, htest.iSubItem);
	//AfxMessageBox(ss);	
	//??????????
/*	
	HWND hWndMainParent;
	hWndMainParent = ::GetParent(::GetParent(this->m_hWnd) );
	if(hWndMainParent == AfxGetMainWnd()->m_hWnd)
		{//AfxMessageBox("MainWnd");
		EditNumberProc = (FARPROC) ::SetWindowLong(m_LVEdit.m_hWnd, GWL_WNDPROC, (LONG) EditSuperNumberProc);
		}
	else //(hWndMainParent == AfxGetMainWnd()->m)
		{//AfxMessageBox("Region");
		::SetWindowLong(m_LVEdit.m_hWnd, GWL_WNDPROC, (LONG) EditSuperNumberProc);
		}
*/
	
	//EditNumberProc = (FARPROC) ::SetWindowLong(m_LVEdit.m_hWnd, GWL_WNDPROC, (LONG) EditSuperNumberProc);
	//::SetWindowLong(m_LVEdit.m_hWnd, GWL_WNDPROC, (LONG) EditSuperNumberProc);

	CFiListCtrl::GetSubItemRect(htest.iItem,htest.iSubItem,LVIR_LABEL,rcItem);

	
	if (htest.iSubItem==0)
	{
		m_LVEdit.m_x=rcItem.left;
		m_LVEdit.m_cx=rcItem.Width();
	}
	else
	{
		m_LVEdit.m_x=rcItem.left+2;
		m_LVEdit.m_cx=rcItem.Width()-2;
	}
	m_LVEdit.m_y=rcItem.top;
	m_LVEdit.m_cy=rcItem.Height();
	
	//::MoveWindow(m_LVEdit.m_hWnd,rcItem.left, rcItem.top, rcItem.right-rcItem.left,
	//	rcItem.bottom-rcItem.top,TRUE);
	m_LVEdit.MoveWindow(rcItem,true);
	m_LVEdit.SetWindowText(CFiListCtrl::GetItemText(htest.iItem,htest.iSubItem));
	m_nEdit=htest.iSubItem;	
    *pResult=0;
	
}

void CFiListCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString sEdit;
	int HV;
	float Fi;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
		sEdit=pDispInfo->item.pszText;

	if( ::SendMessage(m_LVEdit.m_hWnd, EM_GETMODIFY, 0,0))
	{
		if(!sEdit.IsEmpty())
		{
			switch (m_nEdit)
			{
			case 0:
				HV=atoi(sEdit);
				if (HV<1)
					HV=1;
				else if (HV>255)
					HV=255;
				sEdit.Format("%i",HV);
				break;
			case 1:
				Fi=(float)atof(sEdit);
				if (Fi>20.)
					Fi=20.;
				else if (Fi<-40)
					Fi=-40;
				sEdit.Format("%g",Fi);
			}
		}
			CFiListCtrl::SetItemText(pDispInfo->item.iItem,m_nEdit,sEdit);
			
	}

	m_nEdit=-1;
  VERIFY(m_LVEdit.UnsubclassWindow()!=NULL);
	CFiListCtrl::SetItemState(pDispInfo->item.iItem,0,LVNI_FOCUSED|LVNI_SELECTED);
	
	*pResult = 0;
}


void CFiListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnLButtonDown(nFlags, point);
	CFiListCtrl::SendMessage(LVM_EDITLABEL ,(WPARAM)HitTest(point, NULL),0);
	// Или так, 
	//EditLabel(HitTest(point, NULL));
   
}

void CFiListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	ColumnSort=pNMListView->iSubItem;	
	bool NeedAPassage=TRUE;
CString str1, str2;
for (int Passage=1; (Passage<256)&&NeedAPassage; Passage++)
{
	NeedAPassage=FALSE;
	for (int i=0; i<256-Passage; i++)
	{
		str1=GetItemText(i,ColumnSort);
		str2=GetItemText(i+1,ColumnSort);
		if(((atof(str1)>atof(str2))&&!(str1.IsEmpty()||str2.IsEmpty()))||(str1.IsEmpty()&&!str2.IsEmpty()))
		{
			NeedAPassage=TRUE;
			str1=GetItemText(i,0);str2=GetItemText(i,1);
			SetItemText(i,0,GetItemText(i+1,0));SetItemText(i,1,GetItemText(i+1,1));
			SetItemText(i+1,0,str1);SetItemText(i+1,1,str2);
		}
	}
}
	*pResult = 0;
}
