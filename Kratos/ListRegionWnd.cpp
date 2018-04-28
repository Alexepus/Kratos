// ListRegionWnd.cpp : implementation file
//
#include "stdafx.h"
#include "Main.h"
#include "CppLinq.h"

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
	m_iniSectionName = "ListRegionWnd";
	char* str[] = {"Region","KE/BE","Anode","HV, [eV]","Start, [eV]","Finish, [eV]","Step, [eV]","N","n","Time, [sec]","Priority","Comments"};
	N_Column = sizeof(str)/sizeof(char*);
	for (int i = 0; i < N_Column; ++i)
		m_columnsNames.push_back(str[i]);
	
	m_CommentsWnd.m_pMainFrame = this->m_pMainFrame;
}

void CListRegionWnd::SaveColumnsWidthsToIni()
{
	CWinApp* App = AfxGetApp();
	for (int i = 0; i < N_Column; ++i)
	{
		int width = GetColumnWidth(i);
		App->WriteProfileInt(m_iniSectionName.GetString(),
			m_columnsNames[i].GetString(), width);
	}
}

void CListRegionWnd::Create(CWnd* parentCWnd, RECT controlPosition)
{
	CreateEx(WS_EX_CLIENTEDGE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS,
		controlPosition, parentCWnd, IDC_REPORT_REGION);

	DWORD ExStyle = (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, ExStyle);
}

CListRegionWnd::~CListRegionWnd()
{
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
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListRegionWnd message handlers

bool CListRegionWnd::IsItemDisabledAtIndex(int index)
{
	auto reg =  CRegion::GetAtPosition(index);
	if (reg != nullptr && reg->m_DataIn.Off)
		return true;
	return false;
}

int CListRegionWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ImageIcon.Create(16,16,TRUE,1,1);
	m_ImageIcon.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FOR_REG)));
	m_ImageIcon.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FOR_REG_NULL)));
	m_ImageIcon.Add(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FOR_REG_ONOFF)));
	SetImageList(&m_ImageIcon, LVSIL_SMALL);

	CreateColumns();
		return 0;
}

void CListRegionWnd::CreateColumns()
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
		col.pszText = const_cast<LPSTR>(m_columnsNames[i].GetString());
		col.cchTextMax = m_columnsNames[i].GetLength();
		col.cx = (int) App->GetProfileInt(m_iniSectionName.GetString(), m_columnsNames[i].GetString(), r.right/N_Column);
		InsertColumn(i, &col);	
	}
}

void CListRegionWnd::OnDestroy() 
{
	CListCtrl::OnDestroy();
	m_ImageIcon.DeleteImageList();
}

BOOL CListRegionWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
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
			int SelectedItem = FindSelectedItem();
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
				pReg->UpdateStrValues();
				UpdateItem(pReg);
			}
		}
		else if(wID == IDC_BUTTON_COMMENTS)
			{
			char str[32];
			int SelectedItem = FindSelectedItem();
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
					"Comments",
					WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
					r.left, r.top, (r.right - r.left), (r.bottom - r.top),
					this->m_hWnd, NULL);
				}// end if(!::IsWindow(this->m_CommentsWnd.m_hWnd))
			sprintf(str, "Region %i", SelectedItem+1);
			::SendMessage(m_CommentsWnd.m_hWnd, WM_SETTEXT, 0, (LPARAM) str);
				
			for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
				{ if(pReg->ID == SelectedItem) break;}
			::SendMessage(m_CommentsWnd.m_hWndEdit, WM_SETTEXT, 0, 
										(LPARAM) pReg->m_DataIn.Comments);
			if(::IsIconic(m_CommentsWnd.m_hWnd) ) ::ShowWindow(m_CommentsWnd.m_hWnd, SW_RESTORE);
			::SetForegroundWindow(m_CommentsWnd.m_hWnd);
			}// end else if(wID == IDC_BUTTON_COMMENTS)
		}
	return CListCtrl::OnCommand(wParam, lParam);
}

void CListRegionWnd::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		// This is the prepaint stage for an item. Here's where we set the
		// item's text color. Our return value will tell Windows to draw the
		// item itself, but it will use the new color we set here.
		// Store the color back in the NMLVCUSTOMDRAW struct.
		auto itemIndex = pLVCD->nmcd.dwItemSpec;
		if (IsItemDisabledAtIndex(itemIndex))
			pLVCD->clrText = RGB(127, 127, 127);

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;	
	}
}

void CListRegionWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
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
	CListCtrl::OnRButtonDown(nFlags, point);

	LVHITTESTINFO TestInfo;
	TestInfo.pt = point;
	int res = ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) &TestInfo);

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
	if(res==-1) 
		::AppendMenu(hMenu, MF_DISABLED | MF_GRAYED	 | MF_STRING, IDC_BUTTON_RESET, "Reset Data");
	else ::AppendMenu(hMenu, MF_ENABLED | MF_STRING, IDC_BUTTON_RESET, "Reset Data");

	::ClientToScreen(this->m_hWnd, &pt);
	::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, 
		pt.x, pt.y, 0, this->m_hWnd, NULL);
}

void CListRegionWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
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

void CListRegionWnd::SetNewRegionItem(CRegion* pReg)
{
	LV_ITEM item;
	memset(&item, 0, sizeof(LV_ITEM));
	item.iItem = pReg->ID;
	item.iImage = 0;
	item.mask = (LVIF_TEXT | LVIF_STATE | LVIF_IMAGE);
	item.stateMask = (LVIS_SELECTED | LVIS_FOCUSED);
	::SendMessage(m_hWnd, LVM_INSERTITEM, 0, (LPARAM)&item);

	UpdateItem(pReg);	
}

void CListRegionWnd::UpdateItem(CRegion* pReg)
{
	LV_ITEM item;
	memset(&item, 0, sizeof(LV_ITEM));
	item.iItem = pReg->ID;
	item.iSubItem = 0;
	char s[5];
	sprintf(s, "R%i", pReg->ID + 1);
	item.pszText = s;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 1;
	item.pszText = pReg->str.KE_BE;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 2;
	item.pszText = pReg->str.Name_h_nu;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 3;
	item.pszText = pReg->str.HV;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 4;
	item.pszText = pReg->str.KE_Start;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 5;
	item.pszText = pReg->str.KE_End;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 6;
	item.pszText = pReg->str.Step;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 7;
	item.pszText = pReg->str.N_;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 8;
	item.pszText = pReg->str.Curr_N;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 9;
	item.pszText = pReg->str.Time;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 10;
	item.pszText = pReg->str.Priority;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	item.iSubItem = 11;
	char *Comments = "Comments", *NullComments = "";
	if (strcmp(pReg->str.Comments, Comments))
		item.pszText = pReg->str.Comments;
	else
		item.pszText = NullComments;
	::SendMessage(m_hWnd, LVM_SETITEMTEXT, (WPARAM)(int)pReg->ID, (LPARAM) &item);

	SetOnOffIcon(pReg);
}



int CListRegionWnd::FindSelectedItem()
{
	UINT N_Item = GetItemCount();
	UINT mask = (LVIS_SELECTED | LVIS_FOCUSED);
	for (UINT i = 0; i<N_Item; ++i)
	{
		UINT state = (UINT) ::SendMessage(m_hWnd, LVM_GETITEMSTATE, (LPARAM)(int)i, (WPARAM)mask);
		if ((state & LVIS_SELECTED) && (state & LVIS_FOCUSED)) 
			return (int)i;
	}
	return -1;
}

std::vector<CRegion*> CListRegionWnd::GetSelectedRegions()
{
	std::vector<CRegion*> result;
	auto posPointer = GetFirstSelectedItemPosition();
	while(posPointer)
	{
		int index = GetNextSelectedItem(posPointer);
		CRegion* reg;
		if (reg = CRegion::GetAtPosition(index))
			result.push_back(reg);
	}
	return result;
}

void CListRegionWnd::SelectItems(std::vector<CRegion*> regions)
{
	auto prevSelected = GetSelectedRegions();
	for(auto r: regions)
		SetItemState(r->ID, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	auto toDeselect = Linq::from(prevSelected).except(Linq::from(regions)).toVector();
	for (auto r : toDeselect)
		SetItemState(r->ID, ~LVIS_SELECTED, LVIS_SELECTED);
}

void CListRegionWnd::SetIconForReg(CRegion* pReg, int Image)
{
	if (pReg == nullptr) 
		return;
	LV_ITEM item;
	memset(&item, 0, sizeof(LV_ITEM));
	item.iItem = pReg->ID;
	item.iSubItem = 0;
	item.iImage = Image;
	item.mask = LVIF_IMAGE;
	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, LVM_SETITEM, 0, (LPARAM)&item);
}

void CListRegionWnd::SetOnOffIcon(CRegion* pReg)
{
	SetIconForReg(pReg, pReg->m_DataIn.Off? 2 : 0);
}
