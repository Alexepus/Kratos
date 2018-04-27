#include "stdafx.h"
#include "Main.h"
	
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
extern CMutex MutexThread;
extern CProgNewApp theApp;

CRegionWnd::CRegionWnd(CMainFrame* pMainFrame)
{
	m_pMainFrame = pMainFrame;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_XPS);
	m_pDlgParamReg = NULL;
	m_pListRegionWnd = new CListRegionWnd;
	int ScreenSizeX=GetSystemMetrics(SM_CXSCREEN);
	int ScreenSizeY=GetSystemMetrics(SM_CYSCREEN);
	int MinWindowSizeX=GetSystemMetrics(SM_CXMIN);
	int MinWindowSizeY=GetSystemMetrics(SM_CYMIN);
	CWinApp* App = AfxGetApp();
	m_rectWnd.left = App->GetProfileInt("SizeOfRegionWindow", "Left", 0);
	m_rectWnd.top = App->GetProfileInt("SizeOfRegionWindow", "Top", ScreenSizeY*2/3);
	m_rectWnd.right = App->GetProfileInt("SizeOfRegionWindow", "Right", ScreenSizeX);
	m_rectWnd.bottom = App->GetProfileInt("SizeOfRegionWindow", "Bottom", ScreenSizeY);

	if(m_rectWnd.right<ScreenSizeX/2) m_rectWnd.right=ScreenSizeX/2;
	if(m_rectWnd.bottom<5+GetSystemMetrics(SM_CYCAPTION)) m_rectWnd.bottom=5+GetSystemMetrics(SM_CYCAPTION);
	if(m_rectWnd.right-m_rectWnd.left<MinWindowSizeX) m_rectWnd.left=m_rectWnd.right-MinWindowSizeX;
	if(m_rectWnd.bottom-m_rectWnd.top<MinWindowSizeY) m_rectWnd.top=m_rectWnd.bottom-MinWindowSizeY;
	if(m_rectWnd.right-m_rectWnd.left>ScreenSizeX) m_rectWnd.left=m_rectWnd.right-ScreenSizeX;
	if(m_rectWnd.bottom-m_rectWnd.top>ScreenSizeY) m_rectWnd.top=m_rectWnd.bottom-ScreenSizeY;
	if(m_rectWnd.top<5-GetSystemMetrics(SM_CYCAPTION)) m_rectWnd.top=5-GetSystemMetrics(SM_CYCAPTION);

	RegisterRegionWndClass();
}

CRegionWnd::~CRegionWnd()
{
delete m_pListRegionWnd;
}

void CRegionWnd::Show()
{
	if (::IsWindow(m_hWnd))
	{
		ShowWindow(SW_SHOWNORMAL);
		::SetFocus(m_hWnd);
	}
	else
	{
		CreateEx(0, m_WC.lpszClassName, "Regions",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
			m_rectWnd.left, m_rectWnd.top,
			m_rectWnd.right - m_rectWnd.left,
			m_rectWnd.bottom - m_rectWnd.top,
			nullptr, nullptr);
	}
}

RECT CRegionWnd::GetWindowRect() const
{
	return m_rectWnd;
}

BEGIN_MESSAGE_MAP(CRegionWnd, CWnd)
	//{{AFX_MSG_MAP(CRegionWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_ADDNEW, OnButtonAddNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_ONOFF, OnButtonOnOff)	

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRegionWnd::OnButtonAddNew()
{
	CSingleLock sLock(&MutexThread);

	m_pDlgParamReg = new CDialogParamRegion((CWnd*)this);
	m_pDlgParamReg->m_pMainFrame = this->m_pMainFrame;
	if (!m_pDlgParamReg) { AfxMessageBox("Can't create dialog window"); return; }
	else
	{
		THRI_LOCK();

		CRegion* pReg = new CRegion;
		if (pReg == NULL)
		{
			AfxMessageBox("ERROR new Region");
			THRI_UNLOCK();
			return;
		}

		if (pReg != NULL) m_pMainFrame->m_Doc.m_ThrComm.pRegEdit = pReg;
		THRI_UNLOCK();
		if (pReg != NULL)
		{
			pReg->m_NewOrEdit = pReg->New;
			m_pDlgParamReg->m_pReg = pReg;
			::EnableWindow(m_pMainFrame->m_hWnd, FALSE);
			if (m_pDlgParamReg->DoModal() == IDOK)
			{
				SetRegionParametersFromDialog(pReg, m_pDlgParamReg);
				pReg->m_DataIn.LastEditTime = time(nullptr);

				m_pListRegionWnd->SetNewRegionItem(pReg);
				m_pMainFrame->m_Doc.m_NeedSave = m_pMainFrame->m_Doc.Need;
				if (m_pMainFrame->m_Doc.fpPrj)
				{
					CWaitCursor WCur;

					THRI_LOCK();
					fclose(m_pMainFrame->m_Doc.fpPrj);
					m_pMainFrame->m_Doc.fpPrj = fopen(m_pMainFrame->m_Doc.m_ProjectFile.FullPath, "wb+");
					if (!m_pMainFrame->m_Doc.fpPrj)
						m_pMainFrame->m_Doc.m_ThrComm.StopContinue = m_pMainFrame->m_Doc.m_ThrComm.Stop;
					else
					{
						SaveBinaryFile(m_pMainFrame->m_Doc.fpPrj);
						m_pMainFrame->m_Doc.m_ThrComm.fp = m_pMainFrame->m_Doc.fpPrj;
						m_pMainFrame->m_Doc.m_NeedSave = m_pMainFrame->m_Doc.NoNeed;
					}
					THRI_UNLOCK();
				} // end if(m_pMainFrame->m_Doc.fpPrj)			

				if (::IsWindow(this->m_pListRegionWnd->m_CommentsWnd.m_hWnd))
				{
					char str[32];
					sprintf(str, "Region %i", pReg->ID + 1);
					::SendMessage(m_pListRegionWnd->m_CommentsWnd.m_hWnd, WM_SETTEXT,
					              0, (LPARAM)str);
					::SendMessage(m_pListRegionWnd->m_CommentsWnd.m_hWndEdit, WM_SETTEXT,
					              0, (LPARAM)pReg->m_DataIn.Comments);
				}

				char TimeStr[64];
				SetNewTIME(&m_pMainFrame->m_Doc.m_ThrComm.TIME);
				TIME2Str(m_pMainFrame->m_Doc.m_ThrComm.TIME, TimeStr);
				::SendMessage(m_pMainFrame->m_hStatusBar, SB_SETTEXT,
				              2, (LPARAM)(LPSTR)TimeStr);

				theApp.m_pMainFrame->m_Doc.CheckDocType();
			} // end if(m_pDlgParamReg->DoModal()==IDOK)
			else { delete pReg; } // OK!

			m_pMainFrame->m_Doc.m_ThrComm.pRegEdit = NULL;
			::SetFocus(m_pListRegionWnd->m_hWnd);
			::EnableWindow(m_pMainFrame->m_hWnd, TRUE);
		} // end if(pReg!=NULL)
		delete m_pDlgParamReg;
		m_pDlgParamReg = NULL;
	} //end else if(m_pDlgParamReg != NULL)	
}

void CRegionWnd::OnButtonEdit()
{
	CSingleLock sLock(&MutexThread);

	BOOL RegNow;
	BOOL ReWriteFile = FALSE;
	BOOL ReDrawReg = TRUE;
	BOOL ViewReg = FALSE;
	int N_Item = (int) ::SendMessage(m_pListRegionWnd->m_hWnd, LVM_GETITEMCOUNT, 0, 0);
	if (N_Item == 0) ::MessageBox(m_pListRegionWnd->m_hWnd, "No any region", "Attention", MB_OK);
	else
	{
		int SelectedItem = m_pListRegionWnd->FindSelectedItem();
		if (SelectedItem == -1)
			::MessageBox(m_pListRegionWnd->m_hWnd, "Please select a region", "Attention", MB_OK);

		else
		{
			CRegion* pReg;
			for (pReg = CRegion::GetFirst(); pReg != NULL; pReg = CRegion::GetNext(pReg))
			{
				if (pReg->ID == SelectedItem)
				{
					THRI_LOCK();

					if ((pReg == m_pMainFrame->m_Doc.m_ThrComm.pRegNow) && (m_pMainFrame->m_StartStop == m_pMainFrame->Stop))
						RegNow = TRUE;
					else
					{
						m_pMainFrame->m_Doc.m_ThrComm.pRegEdit = pReg;
						RegNow = FALSE;
					}
					THRI_UNLOCK();
					break;
				}
			} // end for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))

			::EnableWindow(m_pMainFrame->m_hWnd, FALSE);
			if (!RegNow) // RegEdit == FALSE
			{
				m_pDlgParamReg = new CDialogParamRegion((CWnd*)this);
				m_pDlgParamReg->m_pMainFrame = this->m_pMainFrame;
				pReg->m_NewOrEdit = pReg->Edit;
				m_pDlgParamReg->m_pReg = pReg;
				if (m_pMainFrame->m_Doc.m_Graph.m_pDataAll == pReg->m_pDataOut)
				{
					ReDrawReg = TRUE;
					m_pMainFrame->m_Doc.m_Graph.m_pDataAll = NULL;
				}
				else { ReDrawReg = FALSE; }
				if (::IsWindow(m_pMainFrame->m_Doc.m_ViewWnd.m_hWnd))
				{
					if (pReg == m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pReg)
					{
						m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = NULL;
						m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
						ViewReg = TRUE;
					}
					else ViewReg = FALSE;
				}
				if (m_pDlgParamReg->DoModal() == IDOK)
				{
					if ((D2I(m_pDlgParamReg->m_KE_Start) != pReg->m_DataIn.KE_Start) || (D2I(m_pDlgParamReg->m_KE_End) != pReg->m_DataIn.KE_End) || (D2I(m_pDlgParamReg->m_Step) != pReg->m_DataIn.Step))
					{
						ReWriteFile = TRUE;
					}
					auto oldDataIn = pReg->m_DataIn;
					SetRegionParametersFromDialog(pReg, m_pDlgParamReg);
					if (memcmp(&oldDataIn, &pReg->m_DataIn, sizeof(DATA_IN)) != 0)
						pReg->m_DataIn.LastEditTime = time(nullptr); // Если реально было изменение, то обновляем время редактирования

					if (m_pMainFrame->m_Doc.fpPrj != NULL)
					{
						CWaitCursor WCur;
						m_pMainFrame->m_Doc.m_NeedSave = m_pMainFrame->m_Doc.Need;
						THRI_LOCK();
						if (ReWriteFile) // Перезапись всего файла если изменился диапазон или шаг региона
						{
							fclose(m_pMainFrame->m_Doc.fpPrj);
							m_pMainFrame->m_Doc.fpPrj = fopen(m_pMainFrame->m_Doc.m_ProjectFile.FullPath, "wb+");
							if (!m_pMainFrame->m_Doc.fpPrj)
								m_pMainFrame->m_Doc.m_ThrComm.StopContinue = m_pMainFrame->m_Doc.m_ThrComm.Stop;
							else
							{
								SaveBinaryFile(m_pMainFrame->m_Doc.fpPrj);
								m_pMainFrame->m_Doc.m_NeedSave = m_pMainFrame->m_Doc.NoNeed;
								m_pMainFrame->m_Doc.m_ThrComm.fp = m_pMainFrame->m_Doc.fpPrj;
							}
						}
						else //Перезаписать только параметры региона
						{
							SaveDataInToFile(m_pMainFrame->m_Doc.fpPrj, pReg);
						}
						THRI_UNLOCK();
						m_pMainFrame->m_Doc.m_NeedSave = m_pMainFrame->m_Doc.NoNeed;
					} // end else if(m_pMainFrame->m_Doc.fpPrj != NULL)

					if ((::IsWindow(this->m_pListRegionWnd->m_CommentsWnd.m_hWnd)))
					{
						char str[32];
						sprintf(str, "Region %i", pReg->ID + 1);
						::SendMessage(m_pListRegionWnd->m_CommentsWnd.m_hWnd, WM_SETTEXT,
						              0, (LPARAM)str);
						::SendMessage(m_pListRegionWnd->m_CommentsWnd.m_hWndEdit, WM_SETTEXT,
						              0, (LPARAM)pReg->m_DataIn.Comments);
					}

					char TimeStr[64];
					SetNewTIME(&m_pMainFrame->m_Doc.m_ThrComm.TIME);
					TIME2Str(m_pMainFrame->m_Doc.m_ThrComm.TIME, TimeStr);
					::SendMessage(m_pMainFrame->m_hStatusBar, SB_SETTEXT,
					              2, (LPARAM)(LPSTR)TimeStr);

					m_pListRegionWnd->UpdateItem(pReg);
				}
				//::EnableWindow(m_pMainFrame->m_hWnd, TRUE);
				if (ReDrawReg)
				{
					//m_pMainFrame->m_Doc.m_Graph.ReDrawAll();
					m_pMainFrame->m_Doc.m_Graph.m_pDataAll = pReg->m_pDataOut;
					m_pMainFrame->m_Doc.m_Graph.m_NDataAll = pReg->m_NDataOut;
					m_pMainFrame->m_Doc.m_Graph.ReDrawAll();
				}

				if (ViewReg) // if(ViewReg == TRUE)
				{
					m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = pReg->m_pDataOut;
					m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_NDataAll = pReg->m_NDataOut;
					m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
				}

				m_pMainFrame->m_Doc.m_ThrComm.pRegEdit = NULL;
				::SetFocus(m_pListRegionWnd->m_hWnd);
				delete m_pDlgParamReg;
			} // end if(!RegEdit)
			else // if(RegNow == TRUE)
			{
				::MessageBox(m_pListRegionWnd->m_hWnd,
				             "You can't edit this region now.\n", "Attention", MB_OK);
			}
			::EnableWindow(m_pMainFrame->m_hWnd, TRUE);
		} // end else if(SelectedItem != -1)
	} // end else if(N_Item != 0)
}

void CRegionWnd::OnButtonDelete()
{
	CSingleLock sLock(&MutexThread);

	BOOL RegNow;
	int N_Item = (int) ::SendMessage(m_pListRegionWnd->m_hWnd, LVM_GETITEMCOUNT, 0, 0);
	if (N_Item == 0) ::MessageBox(m_pListRegionWnd->m_hWnd, "There is no region.", "Attention", MB_OK);
	else
	{
		int SelectedItem = m_pListRegionWnd->FindSelectedItem();
		if (SelectedItem == -1)
			::MessageBox(m_pListRegionWnd->m_hWnd, "Please select a region.", "Attention", MB_OK);
		else
		{
			CRegion* pReg;
			for (pReg = CRegion::GetFirst(); pReg != NULL; pReg = CRegion::GetNext(pReg))
			{
				if (pReg->ID == SelectedItem) break;
			}

			THRI_LOCK();
			if ((pReg == m_pMainFrame->m_Doc.m_ThrComm.pRegNow) && (m_pMainFrame->m_StartStop == m_pMainFrame->Stop))
				RegNow = TRUE;
			else
			{
				m_pMainFrame->m_Doc.m_ThrComm.pRegEdit = pReg;
				RegNow = FALSE;
			}
			THRI_UNLOCK();

			::EnableWindow(m_pMainFrame->m_hWnd, FALSE);

			if (!RegNow)
			{
				char str[128];
				sprintf(str, "Are you sure you want\nto delete the region number %i ?", SelectedItem + 1);
				if (::MessageBox(m_pListRegionWnd->m_hWnd, str, "Attention", MB_YESNO) == IDYES)
				{
					if (::IsWindow(m_pMainFrame->m_Doc.m_ViewWnd.m_hWnd))
					{
						if (pReg->m_pDataOut == m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll)
						{
							m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = NULL;
							m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pReg = NULL;
							::SendMessage(m_pMainFrame->m_Doc.m_ViewWnd.m_hWnd, WM_CLOSE, 0, 0);
							m_pMainFrame->m_Doc.m_ViewWnd.DestroyWindow();
						}
					}
					CWaitCursor WCur;
					THRI_LOCK();
					if (m_pMainFrame->m_Doc.m_ThrComm.pRegNow == pReg)
						m_pMainFrame->m_Doc.m_ThrComm.pRegNow = NULL;
					if (m_pMainFrame->m_Doc.m_Graph.m_pDataAll == pReg->m_pDataOut)
					{
						m_pMainFrame->m_Doc.m_Graph.m_pDataAll = NULL;
						m_pMainFrame->m_Doc.m_Graph.ReDrawAll();
					}
					delete pReg;
					if (!CRegion::GetFirst())
						m_pMainFrame->m_Doc.m_ThrComm.StopContinue = m_pMainFrame->m_Doc.m_ThrComm.Stop;
					if (m_pMainFrame->m_Doc.fpPrj)
					{
						fclose(m_pMainFrame->m_Doc.fpPrj);
						m_pMainFrame->m_Doc.fpPrj = fopen(m_pMainFrame->m_Doc.m_ProjectFile.FullPath, "wb+");
						if (!m_pMainFrame->m_Doc.fpPrj)
							m_pMainFrame->m_Doc.m_ThrComm.StopContinue = m_pMainFrame->m_Doc.m_ThrComm.Stop;
						else
						{
							SaveBinaryFile(m_pMainFrame->m_Doc.fpPrj);
							m_pMainFrame->m_Doc.m_NeedSave = m_pMainFrame->m_Doc.NoNeed;
							m_pMainFrame->m_Doc.m_ThrComm.fp = m_pMainFrame->m_Doc.fpPrj;
						}
					}// end if(m_pMainFrame->m_Doc.fpPrj)
					THRI_UNLOCK();
					::SendMessage(m_pListRegionWnd->m_hWnd, LVM_DELETEITEM, (WPARAM)SelectedItem, 0);
					LV_ITEM item;
					item.iSubItem = 0;
					--N_Item;
					for (int i = 0; i<N_Item; ++i)
					{
						item.iItem = i;
						sprintf(str, "R%i", i + 1);
						item.pszText = str;
						::SendMessage(m_pListRegionWnd->m_hWnd, LVM_SETITEMTEXT, (WPARAM)i,
						              (LPARAM)(LV_ITEM FAR*) &item);
					}

					if ((::IsWindow(this->m_pListRegionWnd->m_CommentsWnd.m_hWnd)))
					{
						char str[32];
						sprintf(str, "No Comments");
						::SendMessage(m_pListRegionWnd->m_CommentsWnd.m_hWnd, WM_SETTEXT,
						              0, (LPARAM)str);
						::SendMessage(m_pListRegionWnd->m_CommentsWnd.m_hWndEdit, WM_SETTEXT,
						              0, (LPARAM)str);
					}

					char TimeStr[64];
					SetNewTIME(&m_pMainFrame->m_Doc.m_ThrComm.TIME);
					TIME2Str(m_pMainFrame->m_Doc.m_ThrComm.TIME, TimeStr);
					::SendMessage(m_pMainFrame->m_hStatusBar, SB_SETTEXT,
					              2, (LPARAM)(LPSTR)TimeStr);

				} // end if(::MessageBox(m_pListRegionWnd->m_hWnd, str, "Attention",MB_YESNO) == IDYES)

				m_pMainFrame->m_Doc.m_ThrComm.pRegEdit = NULL;
			}//end if(!RegNow) // if(RegNow == FALSE)

			else //if(RegNow == TRUE) //Нельзя удалять регион
			{
				::MessageBox(m_pListRegionWnd->m_hWnd,
				             "You can't delete this region now.\n", "Attention", MB_OK);
			}//Нельзя удалять регион

			::EnableWindow(m_pMainFrame->m_hWnd, TRUE);
			::SetFocus(m_pListRegionWnd->m_hWnd);

		} // end else if(SelectedItem != -1)			
	}
	theApp.m_pMainFrame->m_Doc.CheckDocType();
}

void CRegionWnd::OnButtonView()
{
	int N_Item = (int) ::SendMessage(m_pListRegionWnd->m_hWnd, LVM_GETITEMCOUNT, 0, 0);
	if (N_Item == 0) ::MessageBox(m_pListRegionWnd->m_hWnd, "No any region", "Attention", MB_OK);
	else
	{
		int SelectedItem = m_pListRegionWnd->FindSelectedItem();
		if (SelectedItem == -1)
			::MessageBox(m_pListRegionWnd->m_hWnd, "Please select a region", "Attention", MB_OK);
		else
		{
			CRegion* pReg;
			for (pReg = CRegion::GetFirst(); pReg != NULL; pReg = CRegion::GetNext(pReg))
			{
				if (pReg->ID == SelectedItem) break;
			}

			m_pMainFrame->m_Doc.m_ViewWnd.m_pMainFrame = this->m_pMainFrame;
			sprintf(m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_cl_name_for_curve, "%s",
			        m_pMainFrame->m_Doc.m_Graph.m_cl_name_for_curve);

			if (::IsWindow(m_pMainFrame->m_Doc.m_ViewWnd.m_hWnd))
			{
				m_pMainFrame->m_Doc.m_ViewWnd.ShowWindow(SW_SHOWNORMAL);
				::SetFocus(m_pMainFrame->m_Doc.m_ViewWnd.m_hWnd);
			}
			else
			{
				CWinApp* App = AfxGetApp();
				m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.left = App->GetProfileInt("SizeOfViewWindow", "Left", 0);
				m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.top = App->GetProfileInt("SizeOfViewWindow", "Top", 0);
				m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.right = App->GetProfileInt("SizeOfViewWindow", "Right", CW_USEDEFAULT);
				m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.bottom = App->GetProfileInt("SizeOfViewWindow", "Bottom", CW_USEDEFAULT);

				m_pMainFrame->m_Doc.m_ViewWnd.CreateEx(0,
				                                       m_pMainFrame->m_Doc.m_ViewWnd.m_WC.lpszClassName, "View",
				                                       WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
				                                       m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.left,
				                                       m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.top,
				                                       m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.right - m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.left,
				                                       m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.bottom - m_pMainFrame->m_Doc.m_ViewWnd.m_rectWnd.top,
				                                       NULL, NULL);
			}

			if (pReg->m_DataIn.KE_BE == DATA_IN::EnergyType::KE)
				sprintf(m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_strCaption,
				        "Region %i ( KE, Anode: %s )", pReg->ID + 1, pReg->str.Name_h_nu);
			else
				sprintf(m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_strCaption,
				        "Region %i ( BE, Anode: %s )", pReg->ID + 1, pReg->str.Name_h_nu);

			m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pReg = pReg;
			m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_pDataAll = pReg->m_pDataOut;
			m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.m_NDataAll = pReg->m_NDataOut;
			m_pMainFrame->m_Doc.m_ViewWnd.m_ViewGraph.ReDrawAll();
		}// end else if(SelectedItem != -1)
	}// end else if(N_Item != 0)	
}

void CRegionWnd::OnButtonOnOff()
{
	CSingleLock sLock(&MutexThread);

	int N_Item = (int) ::SendMessage(m_pListRegionWnd->m_hWnd, LVM_GETITEMCOUNT, 0, 0);
	if (N_Item == 0) ::MessageBox(m_pListRegionWnd->m_hWnd, "No any region", "Attention", MB_OK);
	else
	{
		int SelectedItem = m_pListRegionWnd->FindSelectedItem();
		if (SelectedItem == -1)
			::MessageBox(m_pListRegionWnd->m_hWnd, "Please select a region", "Attention", MB_OK);
		else
		{

			CRegion* pReg;
			for (pReg = CRegion::GetFirst(); pReg != NULL; pReg = CRegion::GetNext(pReg))
			{
				if (pReg->ID == SelectedItem) break;
			}
			if (pReg == m_pMainFrame->m_Doc.m_ThrComm.pRegNow && m_pMainFrame->m_StartStop == m_pMainFrame->Stop)
				::MessageBox(m_pListRegionWnd->m_hWnd,
				             "You can't OFF this region now.\n", "Attention", MB_OK);
			else
			{
				CWaitCursor WCur;
				::EnableWindow(m_pMainFrame->m_hWnd, FALSE);
				THRI_LOCK();
				if (pReg->m_DataIn.Off == FALSE)
				{
					pReg->m_DataIn.Off = TRUE;
					m_pListRegionWnd->SetOnOffIcon(pReg);
				}
				else
				{
					pReg->m_DataIn.Off = FALSE;
					m_pListRegionWnd->SetOnOffIcon(pReg);
				}
				if (m_pMainFrame->m_Doc.fpPrj) SaveDataInToFile(m_pMainFrame->m_Doc.fpPrj, pReg);
				THRI_UNLOCK();

				char TimeStr[64];
				SetNewTIME(&m_pMainFrame->m_Doc.m_ThrComm.TIME);
				TIME2Str(m_pMainFrame->m_Doc.m_ThrComm.TIME, TimeStr);
				::SendMessage(m_pMainFrame->m_hStatusBar, SB_SETTEXT,
				              2, (LPARAM)(LPSTR)TimeStr);

				::EnableWindow(m_pMainFrame->m_hWnd, TRUE);
			}

		}// end else if(SelectedItem != -1)
	}// end else if(N_Item != 0)
	::SetFocus(m_pListRegionWnd->m_hWnd);
}

BOOL CRegionWnd::RegisterRegionWndClass()
{
	memset(&m_WC, 0, sizeof(WNDCLASS));
	m_WC.lpszClassName = "RegionWindow";
	m_WC.lpfnWndProc = (WNDPROC) ::DefWindowProc;
	m_WC.style = CS_HREDRAW | CS_VREDRAW;
	m_WC.hInstance = AfxGetInstanceHandle();
	m_WC.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	m_WC.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	m_WC.hbrBackground = (HBRUSH) ::GetStockObject(LTGRAY_BRUSH);
	m_WC.lpszMenuName = NULL;
	if( !::RegisterClass(&m_WC) ) {AfxMessageBox("No registered RegionWndClass"); return FALSE;}
	return TRUE;
}

void CRegionWnd::OnDestroy() 
{
	if (m_pDlgParamReg)
	{
		if (::IsWindow(m_pDlgParamReg->m_hWnd))
		{
			m_pDlgParamReg->DestroyWindow();
			delete m_pDlgParamReg;
			m_pDlgParamReg = nullptr;
		}
	}

	CWnd::OnDestroy();
}

int CRegionWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWnd::SetIcon(m_hIcon, TRUE);			// Set big icon
	m_Buttons.CreateButtonWindows(m_hWnd);
	
	RECT r;
	::GetClientRect(this->m_hWnd, &r);

	RECT rl;
	rl.left = rl.top = 0;
	rl.right = r.right - 2*5 - m_Buttons.cx;
	rl.bottom = r.bottom;
	
	m_pListRegionWnd->m_pMainFrame = this->m_pMainFrame;
	m_pListRegionWnd->Create(this, rl);
	
	if(::IsWindow(m_pListRegionWnd->m_hWnd))
	{
		CRegion* pReg = NULL;
		for(pReg=CRegion::GetFirst(); pReg!=NULL; pReg=CRegion::GetNext(pReg))
			m_pListRegionWnd->SetNewRegionItem(pReg);
	}
	
	return 0;
}

void CRegionWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	RECT r;
	::GetClientRect(this->m_hWnd, &r);
	::MoveWindow(m_Buttons.AddNew.hWnd, r.right - m_Buttons.cx - 5, 5,
	             m_Buttons.cx, m_Buttons.cy, TRUE);
	::MoveWindow(m_Buttons.Edit.hWnd, r.right - m_Buttons.cx - 5, 5*2+m_Buttons.cy,
	             m_Buttons.cx, m_Buttons.cy, TRUE);
	::MoveWindow(m_Buttons.Delete.hWnd, r.right - m_Buttons.cx - 5, 5*3+2*m_Buttons.cy,
	             m_Buttons.cx, m_Buttons.cy, TRUE);
	::MoveWindow(m_Buttons.View.hWnd, r.right - m_Buttons.cx - 5, 5*4+3*m_Buttons.cy,
	             m_Buttons.cx, m_Buttons.cy, TRUE);
	::MoveWindow(m_Buttons.OnOff.hWnd, r.right - m_Buttons.cx - 5, 5*5+4*m_Buttons.cy,
	             m_Buttons.cx, m_Buttons.cy, TRUE);
	::MoveWindow(m_pListRegionWnd->m_hWnd, 0, 0,
	             r.right - 2*5 - m_Buttons.cx, r.bottom, TRUE);

	if(!theApp.m_pMainFrame->m_bSynchronousResize)
	{
		CWnd::GetWindowRect(&theApp.m_pMainFrame->m_pRegionWnd->m_rectWnd);
		theApp.m_pMainFrame->m_bSynchronousResize=TRUE;
		theApp.m_pMainFrame->m_pDxpsDlg->MoveWindow(&theApp.m_pMainFrame->m_pRegionWnd->m_rectWnd,FALSE);
		theApp.m_pMainFrame->m_bSynchronousResize=FALSE;
	}	
}

void CRegionWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	::SetFocus(m_pListRegionWnd->m_hWnd);	
}

void CRegionWnd::OnClose() 
{
	if(::IsWindow(this->m_hWnd))
	{
		if(!::IsIconic(this->m_hWnd) && !::IsZoomed(this->m_hWnd))
		{
			::GetWindowRect(this->m_hWnd, &m_rectWnd);
			CWinApp* App = AfxGetApp();
			if( m_rectWnd.left > -20 && m_rectWnd.top > -20 )
			{
				App->WriteProfileInt("SizeOfRegionWindow", "Left", m_rectWnd.left);
				App->WriteProfileInt("SizeOfRegionWindow", "Top", m_rectWnd.top);
				App->WriteProfileInt("SizeOfRegionWindow", "Right", m_rectWnd.right);
				App->WriteProfileInt("SizeOfRegionWindow", "Bottom", m_rectWnd.bottom);
			}
			m_pListRegionWnd->SaveColumnsWidthsToIni();
		}
	} // end if(::IsWindow(this->m_hWnd))
	
	if( (::IsWindow(this->m_pListRegionWnd->m_CommentsWnd) ) 
		&& (!::IsIconic(this->m_pListRegionWnd->m_CommentsWnd) ))
	{
		::SendMessage(this->m_pListRegionWnd->m_CommentsWnd, WM_CLOSE, 0, 0);
	}
	CWnd::OnClose();
}

void CRegionWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	if(bShow==SW_SHOW||bShow==SW_SHOWNORMAL)
		theApp.m_pMainFrame->m_pDxpsDlg->ShowWindow(SW_HIDE);
}

void CRegionWnd::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	
	if(!theApp.m_pMainFrame->m_bSynchronousResize)
	{
		CWnd::GetWindowRect(&m_rectWnd);
		theApp.m_pMainFrame->m_bSynchronousResize=TRUE;
		theApp.m_pMainFrame->m_pDxpsDlg->MoveWindow(&m_rectWnd,FALSE);
		theApp.m_pMainFrame->m_bSynchronousResize=FALSE;
	}
}
