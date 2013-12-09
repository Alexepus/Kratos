// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma warning (disable:4786 4800)

#if !defined(AFX_STDAFX_H__4BEF5512_B27A_4DE3_84D4_6A2E0DF497D3__INCLUDED_)
#define AFX_STDAFX_H__4BEF5512_B27A_4DE3_84D4_6A2E0DF497D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Spec.h"
#include <math.h>
#include "LogToFile.h"

extern CSpecApp theApp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4BEF5512_B27A_4DE3_84D4_6A2E0DF497D3__INCLUDED_)
