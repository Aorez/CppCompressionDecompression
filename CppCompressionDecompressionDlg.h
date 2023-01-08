
// CppCompressionDecompressionDlg.h: 头文件
//

#pragma once


// CCppCompressionDecompressionDlg 对话框
class CCppCompressionDecompressionDlg : public CDialogEx
{
// 构造
public:
	CCppCompressionDecompressionDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPPCOMPRESSIONDECOMPRESSION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonCompression();
	afx_msg void OnBnClickedButtonCompress2();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonOpen2();
	afx_msg void OnBnClickedButtonDecompression();
	CFont m_editFont;
	CStatic m_static;
	CEdit* m_input;
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedButtonSaveText();
	afx_msg void OnBnClickedButtonReset();
};
