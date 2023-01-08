// CppCompressionDecompressionDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CppCompressionDecompression.h"
#include "CppCompressionDecompressionDlg.h"
#include "afxdialogex.h"

#include"functions.h"
#include<fstream>
#include<vector>
#include<string>
#include<set>
#include<map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int out = 1;
int text = 1;


extern std::vector<std::string> beforeText0;//源文件文本
extern std::vector<std::string> compressionText;//压缩空格和注释后的文本
extern std::string compressionText2;//编码压缩文本（一行）
extern int nowOpen;
extern std::string decompressionTextBefore;//解压之前的文本
extern std::vector<std::string> decompressionTextAfter;//解压后的文本
extern std::vector<int> toBinary;


// CCppCompressionDecompressionDlg 对话框



CCppCompressionDecompressionDlg::CCppCompressionDecompressionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CPPCOMPRESSIONDECOMPRESSION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCppCompressionDecompressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCppCompressionDecompressionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CCppCompressionDecompressionDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESSION, &CCppCompressionDecompressionDlg::OnBnClickedButtonCompression)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESS2, &CCppCompressionDecompressionDlg::OnBnClickedButtonCompress2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, &CCppCompressionDecompressionDlg::OnBnClickedButtonOpen2)
	ON_BN_CLICKED(IDC_BUTTON_DECOMPRESSION, &CCppCompressionDecompressionDlg::OnBnClickedButtonDecompression)
	ON_BN_CLICKED(IDC_SAVE, &CCppCompressionDecompressionDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TEXT, &CCppCompressionDecompressionDlg::OnBnClickedButtonSaveText)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CCppCompressionDecompressionDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CCppCompressionDecompressionDlg 消息处理程序

BOOL CCppCompressionDecompressionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_static.SubclassDlgItem(IDC_STATIC, this);
	m_editFont.CreatePointFont(300, _T("宋体"));
	m_static.SetFont(&m_editFont);

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_COMPRESSION);
	pWnd->EnableWindow(false);
	pWnd = GetDlgItem(IDC_BUTTON_COMPRESS2);
	pWnd->EnableWindow(false);
	pWnd = GetDlgItem(IDC_SAVE);
	pWnd->EnableWindow(false);
	pWnd = GetDlgItem(IDC_BUTTON_DECOMPRESSION);
	pWnd->EnableWindow(false);

	dictInit();

	//输出关键字数量
	//std::string x = std::to_string(ttoc.size());
	//CString cx;
	//cx = x.c_str();
	//MessageBox(cx);

	//AllocConsole();//打开测试的输出窗口
	//freopen("CONOUT$", "w", stdout);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCppCompressionDecompressionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCppCompressionDecompressionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CCppCompressionDecompressionDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//输出关键字数量
	/*
	std::string s = std::to_string(int(a.size()));
	CString ss;
	ss = s.c_str();
	MessageBox(ss);*/
	
	CString strFileName;//保存路径
	CFileDialog opendlg(TRUE, _T("*"), /*_T("*.txt")*/NULL, OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		strFileName = opendlg.GetPathName();
		USES_CONVERSION;
		std::string filename(W2A(strFileName));
		std::ifstream ifs;
		ifs.open(filename, std::ios::in);
		std::string aline = "";
		std::string alllines = "";
		beforeText0.clear();
		while (!ifs.eof()) {
			getline(ifs, aline);
			beforeText0.push_back(aline);
			alllines += aline + "\r\n";
			aline = "";
		}
		CString Calllines;
		Calllines = alllines.c_str();
		SetDlgItemText(IDC_EDIT_BEFORE, Calllines);
		ifs.close();

		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_BUTTON_COMPRESS2);
		pWnd->EnableWindow(true);
		pWnd = GetDlgItem(IDC_BUTTON_COMPRESSION);
		pWnd->EnableWindow(true);
		pWnd = GetDlgItem(IDC_BUTTON_DECOMPRESSION);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_SAVE);
		pWnd->EnableWindow(false);



	}


}





void CCppCompressionDecompressionDlg::OnBnClickedButtonCompression()
{
	// TODO: 在此添加控件通知处理程序代码

	compression();
	std::string alllines = "";
	for (auto i : compressionText) {
		alllines += i + "\r\n";
	}
	CString Calllines;
	Calllines = alllines.c_str();
	SetDlgItemText(IDC_EDIT_AFTER, Calllines);
}


void CCppCompressionDecompressionDlg::OnBnClickedButtonCompress2()
{
	// TODO: 在此添加控件通知处理程序代码
	compression();
	//std::cout << compressionText2 << std::endl;
	compression2();
	CString CcompressionText2;
	CcompressionText2 = compressionText2.c_str();
	SetDlgItemText(IDC_EDIT_AFTER, CcompressionText2);

		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_BUTTON_COMPRESS2);
		pWnd->EnableWindow(true);
		pWnd = GetDlgItem(IDC_BUTTON_COMPRESSION);
		pWnd->EnableWindow(true);
		pWnd = GetDlgItem(IDC_BUTTON_DECOMPRESSION);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_SAVE);
		pWnd->EnableWindow(true);

}


void CCppCompressionDecompressionDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//FreeConsole();
	//释放控制台窗口程序
	CDialogEx::OnClose();
}


void CCppCompressionDecompressionDlg::OnBnClickedButtonOpen2()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strFileName;//保存路径
	CFileDialog opendlg(TRUE, _T("*"), /*_T("*.txt")*/NULL, OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		strFileName = opendlg.GetPathName();
		USES_CONVERSION;
		std::string filename(W2A(strFileName));
		std::ifstream ifs;
		ifs.open(filename, std::ios::in | std::ios::binary);
		//unsigned char uc;
		decompressionTextBefore = "";
		while (!ifs.eof()) {
			unsigned char uc;
			ifs.read((char*)&uc, sizeof(uc));
			//std::cout << "haha\n";
			//std::cout << int(uc) << std::endl;
			decompressionTextBefore += std::to_string((int)uc) + " ";
		}
		//不知道为何读取二进制文件后最后一个数字编码重复，所以要剔除掉
		decompressionTextBefore.pop_back();
		while (decompressionTextBefore.back() != ' ') {
			decompressionTextBefore.pop_back();
		}

		
		CString Calllines;
		Calllines = decompressionTextBefore.c_str();
		SetDlgItemText(IDC_EDIT_BEFORE, Calllines);
		ifs.close();

		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_BUTTON_COMPRESS2);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_BUTTON_COMPRESSION);
		pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_BUTTON_DECOMPRESSION);
		pWnd->EnableWindow(true);
		pWnd = GetDlgItem(IDC_SAVE);
		pWnd->EnableWindow(false);

	}
	//std::cout << decompressionTextBefore << std::endl;
}


void CCppCompressionDecompressionDlg::OnBnClickedButtonDecompression()
{
	// TODO: 在此添加控件通知处理程序代码
	decompression();
	std::string decompressionStr = "";
	for (int i = 0; i < decompressionTextAfter.size(); i++) {
		decompressionStr += decompressionTextAfter[i] + "\r\n";
	}
	CString CdecompressionStr;
	CdecompressionStr = decompressionStr.c_str();
	SetDlgItemText(IDC_EDIT_AFTER, CdecompressionStr);

}


void CCppCompressionDecompressionDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码

	CString m_saveFilePath;
	//打开文件，获取文件路径名
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.lpszTitle = L"请选择保存路径";
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);

	if (pItemIDList)
	{
		if (SHGetPathFromIDList(pItemIDList, szPath))
		{
			m_saveFilePath = szPath;
			m_saveFilePath = m_saveFilePath + L"\\";
		}
		
		USES_CONVERSION;
		std::string fp(W2A(m_saveFilePath));
		fp += "out" + std::to_string(out);
		std::ofstream ofs;
		ofs.open(fp, std::ios::out | std::ios::binary);
		unsigned char uc;
		for (int i = 0; i < toBinary.size(); i++) {
			uc = toBinary[i];
			ofs.write((char*)&uc, sizeof(uc));
		}
		ofs.close();
		MessageBox(L"已保存到" + m_saveFilePath + L"out" + (std::to_string(out)).c_str());
		out++;

		//use IMalloc interface for avoiding memory leak  
		IMalloc* pMalloc;
		if (SHGetMalloc(&pMalloc) != NOERROR)
		{
			TRACE(_T("Can't get the IMalloc interface\n"));
		}

		pMalloc->Free(pItemIDList);
		if (pMalloc)
			pMalloc->Release();
		UpdateData(FALSE);
	}
}


void CCppCompressionDecompressionDlg::OnBnClickedButtonSaveText()
{
	// TODO: 在此添加控件通知处理程序代码

		CString Stmp;
		//获取
		m_input = (CEdit*)(this->GetDlgItem(IDC_EDIT_AFTER));
		//获取输入的sn
		m_input->GetWindowText(Stmp);
		if (Stmp.IsEmpty()) {
			MessageBox(L"文本框内无内容！");
			return;
		}


	CString m_saveFilePath;
	//打开文件，获取文件路径名
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.lpszTitle = L"请选择保存路径";
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);

	if (pItemIDList)
	{
		if (SHGetPathFromIDList(pItemIDList, szPath))
		{
			m_saveFilePath = szPath;
			m_saveFilePath = m_saveFilePath + L"\\";
		}
		CFile File;
		CString file_path;
		CString string;
		Stmp.Replace(L"\r\n", L"\n");

		File.Open(file_path, CFile::modeCreate | CFile::modeWrite);

		CStdioFile  file(m_saveFilePath + L"text" + std::to_string(text).c_str() + L".txt", CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite);

		file.WriteString(Stmp);
		file.Close();
		MessageBox(L"已保存到" + m_saveFilePath + L"text" + std::to_string(text).c_str() + L".txt");
		text++;

		//use IMalloc interface for avoiding memory leak  
		IMalloc* pMalloc;
		if (SHGetMalloc(&pMalloc) != NOERROR)
		{
			TRACE(_T("Can't get the IMalloc interface\n"));
		}

		pMalloc->Free(pItemIDList);
		if (pMalloc)
			pMalloc->Release();
		UpdateData(FALSE);
	}



}


void CCppCompressionDecompressionDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT_BEFORE, L"");
	SetDlgItemText(IDC_EDIT_AFTER, L"");

	beforeText0.clear();
	compressionText.clear();
	compressionText2.clear();
	nowOpen = 0;
	decompressionTextBefore.clear();
	decompressionTextAfter.clear();
	toBinary.clear();

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_COMPRESSION);
	pWnd->EnableWindow(false);
	pWnd = GetDlgItem(IDC_BUTTON_COMPRESS2);
	pWnd->EnableWindow(false);
	pWnd = GetDlgItem(IDC_SAVE);
	pWnd->EnableWindow(false);
	pWnd = GetDlgItem(IDC_BUTTON_DECOMPRESSION);
	pWnd->EnableWindow(false);

	out = 1;
	text = 1;

}
