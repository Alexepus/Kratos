// MassSpecDoc.h : interface of the CMassSpecDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASSSPECDOC_H__8FFCCC3D_56B4_4279_8DBA_DA3687C1BB51__INCLUDED_)
#define AFX_MASSSPECDOC_H__8FFCCC3D_56B4_4279_8DBA_DA3687C1BB51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <vector>
#include <afxmt.h>

#define MAX_MASS_COUNT 50
class CSpecView;

class CMassTable
{
public:
	double Mass;
	bool Checked;
	char Comments[64]; //Для окна проекта (м.б. и калибровки)
	std::vector<double> vTime;
	std::vector<double> vIntensity;
	double IntensityMax;
	bool bIntensityMaxChanged;
};

class CMassSpecDoc : public CDocument
{
protected: // create from serialization only
	CMassSpecDoc();
	DECLARE_DYNCREATE(CMassSpecDoc)

// Attributes
public:
	bool m_bMeasurementInProgress;
	std::vector<CMassTable> m_MassTable;
	std::vector<double> m_vRefTemp;
	std::vector<double> m_vMeasTemp;
	int m_ScanTime;		//in msec
	int m_PassedCommonTime;	//in msec
	CCriticalSection m_DocModifyCS;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMassSpecDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool IsEmpty();
	CSpecView* GetView();
	void FillCurrentTemperatures();
	void SetModifiedFlag(bool Modified=true);
	CString PrintEasyPlotData();
	void SaveDefData();
	void SaveEasyPlot(char *FileName);
	void InitDoc();
	virtual ~CMassSpecDoc();
	void CopyToClipboard();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMassSpecDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASSSPECDOC_H__8FFCCC3D_56B4_4279_8DBA_DA3687C1BB51__INCLUDED_)
