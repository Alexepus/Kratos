#if !defined(AFX_DLGABOUT_H__BEC07955_D54F_46F8_8EAD_D51469E687BF__INCLUDED_)
#define AFX_DLGABOUT_H__BEC07955_D54F_46F8_8EAD_D51469E687BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAbout.h : header file
//
#include "resource.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog

class CDlgAbout : public CDialog
{
// Construction
public:
	CDlgAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_DLG_ABOUT };
	CString	m_StaticBuildDate;
	CString	m_StaticMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAbout)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL GetFileVersion(CString &fileVersion, CString &fileCreateDateTime, CString &peTimestamp);
	CStatic m_StaticVer;
	CString m_StaticFileCreateDate;
	CString GetPeTimeStamp(HANDLE fp);
	
	struct _IMAGE_FILE_HEADER {
		WORD  Machine;
		WORD  NumberOfSections;
		DWORD TimeDateStamp;
		DWORD PointerToSymbolTable;
		DWORD NumberOfSymbols;
		WORD  SizeOfOptionalHeader;
		WORD  Characteristics;
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGABOUT_H__BEC07955_D54F_46F8_8EAD_D51469E687BF__INCLUDED_)
