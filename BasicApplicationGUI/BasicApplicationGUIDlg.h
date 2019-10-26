
// BasicApplicationGUIDlg.h : header file
//

#pragma once


// CBasicApplicationGUIDlg dialog
class CBasicApplicationGUIDlg : public CDialogEx
{
// Construction
public:
	CBasicApplicationGUIDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASICAPPLICATIONGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CString m_sourceFile;
	CString m_className;
	CString m_destinationFolder;
	CString m_otherOptions;

public:
	afx_msg void OnBnClickedProcess();
	afx_msg void OnBnClickedBSourceFile();
	afx_msg void OnBnClickedBDestinationFolder();
	afx_msg void OnBnClickedBReset();
	afx_msg void OnEnChangeClassName();
};
