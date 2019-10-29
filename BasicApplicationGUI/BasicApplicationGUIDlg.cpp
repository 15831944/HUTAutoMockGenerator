
// BasicApplicationGUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BasicApplicationGUI.h"
#include "BasicApplicationGUIDlg.h"
#include "afxdialogex.h"
#include "HUTAutoMockGenerator.h"
#include <iostream>
#include <ostream>
#include <string>
#include <filesystem>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBasicApplicationGUIDlg dialog



CBasicApplicationGUIDlg::CBasicApplicationGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BASICAPPLICATIONGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBasicApplicationGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TB_SOURCE_FILE, m_sourceFile);
	DDX_Text(pDX, IDC_TB_CLASS_NAME, m_className);
	DDX_Text(pDX, IDC_TB_DESTINATION_FOLDER, m_destinationFolder);
	DDX_Text(pDX, IDC_TB_OTHER_OPTIONS, m_otherOptions);
}

BEGIN_MESSAGE_MAP(CBasicApplicationGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PROCESS, &CBasicApplicationGUIDlg::OnBnClickedProcess)
	ON_BN_CLICKED(IDC_B_SOURCE_FILE, &CBasicApplicationGUIDlg::OnBnClickedBSourceFile)
	ON_BN_CLICKED(IDC_B_DESTINATION_FOLDER, &CBasicApplicationGUIDlg::OnBnClickedBDestinationFolder)
	ON_BN_CLICKED(IDC_B_RESET, &CBasicApplicationGUIDlg::OnBnClickedBReset)
	ON_EN_KILLFOCUS(IDC_TB_CLASS_NAME, &CBasicApplicationGUIDlg::OnEnChangeClassName)
	ON_EN_KILLFOCUS(IDC_TB_OTHER_OPTIONS, &CBasicApplicationGUIDlg::OnEnChangeClassName)
	
	ON_CBN_SELCHANGE(IDC_COMBO_LISTMODE, &CBasicApplicationGUIDlg::OnCbnSelchangeComboListmode)
END_MESSAGE_MAP()


// CBasicApplicationGUIDlg message handlers

BOOL CBasicApplicationGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CComboBoxEx* obj = (CComboBoxEx*)GetDlgItem(IDC_COMBO_LISTMODE);
	obj->SetCurSel(0);

	CStatic* staticText = (CStatic*)GetDlgItem(IDC_STATIC_SOURCE_FILE);
	staticText->SetWindowTextA(staticSourceFileText);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBasicApplicationGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBasicApplicationGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBasicApplicationGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBasicApplicationGUIDlg::OnBnClickedProcess()
{
	Mode mode = GetCurrentMode();
	UpdateData(true);
	if (m_sourceFile.IsEmpty())
	{
		MessageBox("Source file field should not be empty.", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	else if (m_destinationFolder.IsEmpty())
	{
		MessageBox("Destination folder field should not be empty.", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	if (mode == Mode::SINGLE)
	{
		if (m_className.IsEmpty())
		{
			MessageBox("Class name field should not be empty.", "Error", MB_OK | MB_ICONERROR);
			return;
		}
		AfterProcess(Process(m_sourceFile, m_className, m_destinationFolder, m_otherOptions));
	}
	else if (mode == Mode::ALL_IN_FOLDER)
	{
		AfterProcess(TraverseDirectory(m_sourceFile, m_className, m_destinationFolder, m_otherOptions));
	}
	else
	{
		MessageBox("This mode is not supported yet", "Info", MB_OK | MB_ICONINFORMATION);
	}
}

int CBasicApplicationGUIDlg::TraverseDirectory(CString source, CString className, CString destination, CString others, bool isSubFolder)
{
	int res = 0;
	for (const auto & entry : std::filesystem::directory_iterator(source.GetBuffer()))
	{
		if (entry.is_regular_file())
		{
			CString pt(entry.path().extension().c_str());
			pt.MakeLower();
			if (pt.Compare(".h") == 0 || pt.Compare(".hpp") == 0)
			{
				res = res | Process(CString(entry.path().c_str()), className, destination, others);
			}
		}
		else if (entry.is_directory() && isSubFolder)
		{
			CString pt(entry.path().extension().c_str());
			res = res | TraverseDirectory(pt, className, destination, others);
		}
	}
	return res;
}


void CBasicApplicationGUIDlg::OnBnClickedBSourceFile()
{
	Mode mode = GetCurrentMode();
	if (mode == Mode::SINGLE)
	{
		const TCHAR szFilter[] = _T("Header Files (*.h;*.hpp)|*.h;*.hpp||");
		CFileDialog dlg(true, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
		if (dlg.DoModal() == IDOK)
		{
			m_sourceFile = dlg.GetPathName();
		}
	}
	else if (mode == Mode::ALL_IN_FOLDER)
	{
		CFolderPickerDialog dlg(nullptr, 0, this);
		if (dlg.DoModal() == IDOK)
		{
			m_sourceFile = dlg.GetFolderPath();
		}
	}
	UpdateData(false);
}


void CBasicApplicationGUIDlg::OnBnClickedBDestinationFolder()
{
	CFolderPickerDialog dlg(nullptr, 0, this);
	if (dlg.DoModal() == IDOK)
	{
		m_destinationFolder = dlg.GetFolderPath();
	}
	UpdateData(false);
}


void CBasicApplicationGUIDlg::OnBnClickedBReset()
{
	m_sourceFile = "";
	m_className = "";
	m_destinationFolder = "";
	m_otherOptions = "";
	UpdateData(false);
}

void CBasicApplicationGUIDlg::OnEnChangeClassName()
{
	CEdit* obj = (CEdit*)GetDlgItem(IDC_TB_CLASS_NAME);
	obj->GetWindowTextA(m_className);
	obj = (CEdit*)GetDlgItem(IDC_TB_SOURCE_FILE);
	obj->GetWindowTextA(m_sourceFile);
	obj = (CEdit*)GetDlgItem(IDC_TB_DESTINATION_FOLDER);
	obj->GetWindowTextA(m_destinationFolder);
	obj = (CEdit*)GetDlgItem(IDC_TB_OTHER_OPTIONS);
	obj->GetWindowTextA(m_otherOptions);
	UpdateData(false);
}

CBasicApplicationGUIDlg::Mode CBasicApplicationGUIDlg::GetCurrentMode()
{
	CComboBoxEx* obj = (CComboBoxEx*)GetDlgItem(IDC_COMBO_LISTMODE);
	return (Mode)obj->GetCurSel();
}

int CBasicApplicationGUIDlg::Process(CString source, CString className, CString destination, CString others)
{
	int argc = 3;
	int sourceLength = sourceFileTag.length() + source.GetLength() + 1;
	int classLength = classTag.length() + className.GetLength() + 1;
	int destinationLength = destinationFolderTag.length() + destination.GetLength() + 1;
	int otherOptionsLength = 0;
	if (!others.IsEmpty())
	{
		argc++;
		otherOptionsLength = others.GetLength() + 1;
	}
	char** argv = new char* [argc];
	argv[0] = new char[sourceLength];
	argv[1] = new char[classLength];
	argv[2] = new char[destinationLength];
	memset(argv[0], 0, sourceLength);
	memset(argv[1], 0, classLength);
	memset(argv[2], 0, destinationLength);
	memcpy(argv[0], sourceFileTag.c_str(), sourceFileTag.length());
	memcpy(&argv[0][sourceFileTag.length()], source.GetBuffer(), source.GetLength());
	memcpy(argv[1], classTag.c_str(), classTag.length());
	memcpy(&argv[1][classTag.length()], className.GetBuffer(), className.GetLength());
	memcpy(argv[2], destinationFolderTag.c_str(), destinationFolderTag.length());
	memcpy(&argv[2][destinationFolderTag.length()], destination.GetBuffer(), destination.GetLength());

	if (argc = 4)
	{
		argv[3] = new char[otherOptionsLength];
		memset(argv[3], 0, otherOptionsLength);
		memcpy(argv[3], others.GetBuffer(), otherOptionsLength - 1);
	}
	
	int res = HUTAutoMockGenerator(argc, argv);

	delete[] argv[0];
	delete[] argv[1];
	delete[] argv[2];
	if (argc = 4)
	{
		delete[] argv[3];
	}
	delete[] argv;

	return res;
}

void CBasicApplicationGUIDlg::AfterProcess(int res)
{
	if (res == 0)
	{
		MessageBox("Successfully generated mocks", "Success", MB_OK | MB_USERICON);
		OnBnClickedBReset();
	}
	else
	{
		MessageBox("Failed to generate mocks.", "Error", MB_OK | MB_ICONERROR);
	}
}

void CBasicApplicationGUIDlg::OnCbnSelchangeComboListmode()
{
	Mode mode = GetCurrentMode();
	CEdit* obj = (CEdit*)GetDlgItem(IDC_TB_CLASS_NAME);
	if (mode == Mode::ALL_IN_FOLDER)
	{
		CStatic* staticText = (CStatic*)GetDlgItem(IDC_STATIC_SOURCE_FILE);
		staticText->SetWindowTextA(staticSourceFolderText);
		m_sourceFile = "";
		m_className = "all";
		obj->EnableWindow(false);
	}
	else if (mode == Mode::SINGLE)
	{
		CStatic* staticText = (CStatic*)GetDlgItem(IDC_STATIC_SOURCE_FILE);
		staticText->SetWindowTextA(staticSourceFileText);
		m_sourceFile = "";
		m_className = "";
		obj->EnableWindow(true);
	}
	UpdateData(false);
}
