// ListRegionWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Main.h"

//#include <string.h>
//#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListRegionWnd
class CMainFrame;

CListRegionWnd::CListRegionWnd()
{
	char* s = "ListRegionWnd";
	strIniListRegionWnd = new char[strlen(s) + 1];
	sprintf(strIniListRegionWnd, "%s", s);
	char* str[] = {"Region","KE/BE","Anode","HV, [eV]","Start, [eV]","Finish, [eV]","Step, [eV]","N","n","Time, [sec]","Comments"};
	N_Column = sizeof(str)/sizeof(char*);
	strNameColumn = new char*[N_Column];
	for(int i=0; i<N_Column; ++i)
		{
		strNameColumn[i] = new char[strlen(str[i])+1];
		sprintf(strNameColumn[i],"%s", str[i]);
		}
	
	m_CommentsWnd.m_pMainFrame = this->m_pMainFrame;
	//N_Column = sizeof(strNameColumn)/sizeof(char*);
}

CListRegionWnd::~CListRegionWnd()
{
	for(int i=0; i<N_Column; ++i)  {	delete[] strNameColumn[i];}
	delete[] strNameColumn;
	delete[] strIniListRegionWnd;
}


BEGIN_MESSAGE_MAP(CListRegionWnd, CListCtrl)
	//{{AFX_MSG_MAP(CListRegionWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListRegionWnd message handlers

int CListRegionWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//AfxMessageBox("CListCtrl::OnCreate");
	//m_ImageIcon = new CImageList;
	m_ImageIcon.Create(16,16,TRUE,1,1);
	m_ImageIcon.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FOR_REG)));
	m_ImageIcon.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FOR_REG_NULL)));
	m_ImageIcon.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FOR_REG_ONOFF)));
	SetImageList(&m_ImageIcon, LVSIL_SMALL);

	CreateColumn();
	/*
	CWinApp* App = AfxGetApp();
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	LV_COLUMN col;
	memset(&col, 0, sizeof(LV_COLUMN));
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	col.fmt = LVCFMT_CENTER;
	for(int i=0; i<N_Column; ++i)
		{
		col.iSubItem = i;
		col.pszText = strNameColumn[i];
		col.cchTextMax = strlen(strNameColumn[i]);
		col.cx = (int) App->GetProfileInt(strIniListRegionWnd, strNameColumn[i], r.right/N_Column);
		InsertColumn(i, &col);	
		}
	*/
	
	return 0;
}


void CListRegionWnd::CreateColumn()
{
	CWinApp* App = AfxGetApp();
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	LV_COLUMN col;
	memset(&col, 0, sizeof(LV_COLUMN));
	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	col.fmt = LVCFMT_CENTER;
	for(int i=0; i<N_Column; ++i)
		{
		col.iSubItem = i;
		col.pszText = strNameColumn[i];
		col.cchTextMax = strlen(strNameColumn[i]);
		col.cx = (int) App->GetProfileInt(strIniListRegionWnd, strNameColumn[i], r.right/N_Column);
		InsertColumn(i, &col);	
		}
}




void CListRegionWnd::OnDestroy() 
{
	CListCtrl::OnDestroy();

	// TODO: Add your message handler code here
	m_ImageIcon.DeleteImageList();
	//AfxMessageBox("Destroy CtrlListWnd");
}

BOOL CListRegionWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
int wNotifyCode = HIWORD(wParam); // notification code 
int wID = LOWORD(wParam);         // item, control, or accelerator identifier 
HWND hwndCtl = (HWND) lParam;      // handle of control 
HWND hWnd;
	if(wNotifyCode == 0 && hwndCtl == 0)
		{
		
		if(wID == IDC_BUTTON_ADDNEW || wID == IDC_BUTTON_EDIT || wID == IDC_BUTTON_DELETE 
				|| wID == IDC_BUTTON_VIEW || wID == IDC_BUTTON_ONOFF)
			{
			hWnd = ::GetDlgItem(::GetParent(this->m_hWnd), wID);
			::SendMessage(hWnd, BM_CLICK, 0, 0);
			}
		else if(wID == IDC_BUTTON_RESET)
		{
			char str[128];
			int i;
			CRegion* pReg;
			int SelectedItem = FindSelectedItem(this->m_hWnd);
			for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
				{ if(pReg->ID == SelectedItem) break;}
			sprintf(str,"Are you sure you want\nto reset all measured data ?");
			if(::MessageBox(this->m_hWnd, str, "Attention",MB_YESNO) == IDYES)
			{
				for(i=0; i<pReg->m_NDataOut; ++i) 
					{
					pReg->m_pDataOut[i].x = pReg->m_DataIn.KE_Start + i*pReg->m_DataIn.Step;
					pReg->m_pDataOut[i].y = 0;
					SaveDataToFile(m_pMainFrame->m_Doc.fpPrj, pReg, i, &pReg->m_pDataOut[i]);
					}
				pReg->m_NDataOutCurr = 0;
				pReg->m_DataIn.Curr_N = 0;
				SaveDataInToFile(m_pMainFrame->m_Doc.fpPrj, pReg);
				sprintf(pReg->str.Curr_N, "%i", pReg->m_DataIn.Curr_N);
				UpdateTextItem(m_hWnd, pReg);
			}
		}
		else if(wID == IDC_BUTTON_COMMENTS)
			{
			//AfxMessageBox("IDC_BUTTON_COMMENTS");
			char str[32];
			int SelectedItem = FindSelectedItem(this->m_hWnd);
			CRegion* pReg;

			if(!::IsWindow(this->m_CommentsWnd.m_hWnd))
				{
				RECT r;
				CWinApp* App = AfxGetApp();
				r.left = App->GetProfileInt("SizeOfCommentsWindow", "Left", 0);
				r.top = App->GetProfileInt("SizeOfCommentsWindow", "Top", 0);
				r.right = App->GetProfileInt("SizeOfCommentsWindow", "Right", CW_USEDEFAULT);
				r.bottom = App->GetProfileInt("SizeOfCommentsWindow", "Bottom", CW_USEDEFAULT);
				
				m_CommentsWnd.CreateEx(0, 
					m_pMainFrame->m_pHideWnd->m_pBigClientWnd->m_class_name, 
					//m_pMainFrame->m_Doc.m_ViewWnd.m_WC.lpszClassName,
					"Comments",
					//WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE ,
					WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
					//WS_POPUPWINDOW | WS_VISIBLE |  ES_MULTILINE | ES_AUTOVSCROLL ,
					//200,100,400,200,
					r.left, r.top, (r.right - r.left), (r.bottom - r.top),
					this->m_hWnd, NULL);

				//::SendMessage();
				}// end if(!::IsWindow(this->m_CommentsWnd.m_hWnd))
			sprintf(str, "Region %i", SelectedItem+1);
			::SendMessage(m_CommentsWnd.m_hWnd, WM_SETTEXT, 0, (LPARAM) str);
				
			for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
				{ if(pReg->ID == SelectedItem) break;}
			::SendMessage(m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 0, 
										(LPARAM) pReg->m_DataIn.Comments);
			if(::IsIconic(m_CommentsWnd.m_hWnd) ) ::ShowWindow(m_CommentsWnd.m_hWnd, SW_RESTORE);
			::SetForegroundWindow(m_CommentsWnd.m_hWnd);
			//::SetFocus(m_CommentsWnd.m_hWnd);
			}// end else if(wID == IDC_BUTTON_COMMENTS)
		}
	return CListCtrl::OnCommand(wParam, lParam);
}

void CListRegionWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("OnRButtonUp");
/*
	LVHITTESTINFO TestInfo;
	TestInfo.pt = point;
	int res;
	res = ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) &TestInfo);
//	if(res == -1) sprintf(s,"%s","No any");
//	else sprintf(s," Item = %i\n\n SubItem = %i",TestInfo.iItem, TestInfo.iSubItem);

	POINT pt = point;
	RECT r;
	::GetWindowRect(m_hWnd, &r);
	HMENU hMenu = ::CreatePopupMenu();
	::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_ADDNEW, "Add New");
	::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_EDIT, "Edit");
	::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_DELETE, "Delete");
	::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_VIEW, "View");
	::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_ONOFF, "On/Off");
	::ClientToScreen(this->m_hWnd, &pt);
	::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, 
		r.left+point.x, r.top+point.y, 0, this->m_hWnd, NULL);
*/	
	CListCtrl::OnRButtonUp(nFlags, point);
}

void CListRegionWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnRButtonDown(nFlags, point); // Сам сюда вставил

	LVHITTESTINFO TestInfo;
	TestInfo.pt = point;
	int res;
	res = ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) &TestInfo);

	UINT style;
	CRegion* pReg;
	
	if( (res!=-1) && (::IsWindow(this->m_CommentsWnd) ))
		{
		char str[32];
		sprintf(str, "Region %i", res+1);
		::SendMessage(m_CommentsWnd.m_hWnd, WM_SETTEXT, 0, (LPARAM) str);
				
		for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
			{ if(pReg->ID == res) break;}
		::SendMessage(m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 0, 
										(LPARAM) pReg->m_DataIn.Comments);
		}	
	
	for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
		{if(pReg->ID == res) break;}
	//if((pReg==m_pMainFrame->m_Doc.m_ThrComm.pRegNow) && (m_pMainFrame->m_StartStop==m_pMainFrame->Stop)) 
	
	if(res == -1 || (pReg == m_pMainFrame->m_Doc.m_ThrComm.pRegNow && m_pMainFrame->m_StartStop==m_pMainFrame->Stop) )
		style = MF_DISABLED | MF_GRAYED	 | MF_STRING;
	else style = MF_ENABLED | MF_STRING;

	POINT pt = point;
	RECT r;
	::GetWindowRect(m_hWnd, &r);
	HMENU hMenu = ::CreatePopupMenu();
	::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_ADDNEW, "Add New");
	::AppendMenu(hMenu, style, IDC_BUTTON_EDIT, "Edit");
	::AppendMenu(hMenu, style, IDC_BUTTON_DELETE, "Delete");
	if(res==-1) 
		::AppendMenu(hMenu, MF_DISABLED | MF_GRAYED	 | MF_STRING, IDC_BUTTON_VIEW, "View");
	else ::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_VIEW, "View");
	::AppendMenu(hMenu, style, IDC_BUTTON_ONOFF, "On/Off");
//	if(res==-1) 
//		::AppendMenu(hMenu, MF_DISABLED | MF_GRAYED	 | MF_STRING, IDC_BUTTON_COMMENTS, "Comments");
//	else ::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_COMMENTS, "Comments");
	if(res==-1) 
		::AppendMenu(hMenu, MF_DISABLED | MF_GRAYED	 | MF_STRING, IDC_BUTTON_RESET, "Reset Data");
	else ::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_RESET, "Reset Data");

	::ClientToScreen(this->m_hWnd, &pt);
	::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, 
		//r.left+point.x, r.top+point.y, 0, this->m_hWnd, NULL);
		pt.x, pt.y, 0, this->m_hWnd, NULL);
	
	
		
	//CListCtrl::OnRButtonDown(nFlags, point);
	//AfxMessageBox("OnRButtonDown");
}

void CListRegionWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LVHITTESTINFO TestInfo;
	TestInfo.pt = point;
	int res;
	res = ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) &TestInfo);

	CRegion* pReg;
	
	if( (res!=-1) && (::IsWindow(this->m_CommentsWnd.m_hWnd) ))
		{
		char str[32];
		sprintf(str, "Region %i", res+1);
		::SendMessage(m_CommentsWnd.m_hWnd, WM_SETTEXT, 0, (LPARAM) str);
				
		for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
			{ if(pReg->ID == res) break;}
		::SendMessage(m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 0, 
										(LPARAM) pReg->m_DataIn.Comments);
		}	
	

	
	CListCtrl::OnLButtonDown(nFlags, point);
}


void CListRegionWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_pMainFrame->m_pRegionWnd!=NULL)
		m_pMainFrame->m_pRegionWnd->SendMessage(WM_COMMAND,(BN_CLICKED<<16)|IDC_BUTTON_EDIT,0);
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}
