
// AcsVisionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AcsVision.h"
#include "AcsVisionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAcsVisionDlg 对话框




CAcsVisionDlg::CAcsVisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAcsVisionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAcsVisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, mListLog);
}

BEGIN_MESSAGE_MAP(CAcsVisionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_GATE, &CAcsVisionDlg::OnNMDblclkTreeGate)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAcsVisionDlg 消息处理程序
BOOL CAcsVisionDlg::WStrToMStr(CStringW & strCS, std::string & sStr)
{
	std::string sRet;
	CStringW strW;
	strW = strCS;
	wchar_t wcTmp[MAX_PATH] = {0};
	wchar_t * wcpTmp = strW.GetBuffer(strW.GetLength() + 1);
	wcscpy_s(&wcTmp[0], sizeof(wcTmp) / sizeof(wcTmp[0]), wcpTmp);
	strW.ReleaseBuffer();
	wcTmp[sizeof(wcTmp) / sizeof(wcTmp[0]) - 1] = 0;
	char cTmp[MAX_PATH * 2] = {0};
	W2M(&wcTmp[0], &cTmp[0], sizeof(cTmp) / sizeof(cTmp[0]));
	cTmp[sizeof(cTmp) / sizeof(cTmp[0]) - 1] = 0;
	sRet = &cTmp[0];
	sStr = sRet;
	return TRUE;
}

BOOL CAcsVisionDlg::MStrToWStr(CStringW & strCS, std::string & sStr)
{
	std::string sRet;
	CStringW strW;
	strW = strCS;
	wchar_t wcTmp[1024] = {0};
	M2W(sStr.c_str(), &wcTmp[0], sizeof(wcTmp) / sizeof(wcTmp[0]));
	wcTmp[sizeof(wcTmp) / sizeof(wcTmp[0]) - 1] = TCHAR('\0');
	strCS = &wcTmp[0];
	return TRUE;
}

BOOL CAcsVisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	{
		gateStatusID[0] = IDC_STATIC_GATEA1;
		gateStatusID[1] = IDC_STATIC_GATEA2;
		gateStatusID[2] = IDC_STATIC_GATEB1;
		gateStatusID[3] = IDC_STATIC_GATEB2;
		gateStatusID[4] = IDC_STATIC_GATEC1;
		gateStatusID[5] = IDC_STATIC_GATEC2;
		gateStatusID[6] = IDC_STATIC_GATED1;
		gateStatusID[7] = IDC_STATIC_GATED2;
		gateStatusID[8] = IDC_STATIC_GATEE1;
		gateStatusID[9] = IDC_STATIC_GATEE2;
		gateStatusID[10] = IDC_STATIC_GATEF1;
		gateStatusID[11] = IDC_STATIC_GATEF2;
		gateStatusID[12] = IDC_STATIC_GATEG1;
		gateStatusID[13] = IDC_STATIC_GATEG2;
		gateStatusID[14] = IDC_STATIC_GATEH1;
		gateStatusID[15] = IDC_STATIC_GATEH2;
		gateStatusID[16] = IDC_STATIC_GATEJ1;
		gateStatusID[17] = IDC_STATIC_GATEJ2;
		gateStatusID[18] = IDC_STATIC_GATEK1;
		gateStatusID[19] = IDC_STATIC_GATEK2;
		for(int i=0;i<20;i++)
			gateStatusMap.insert(std::make_pair(i,0));
	}
	initTree();
	initData();

	initSqlServer();
	//==cardReader 读取 初始化
	m_cardReaderThreadHandle = NULL;
	cardReaderStart();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAcsVisionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAcsVisionDlg::OnPaint()
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
		CDialog::OnPaint();
	}
	CImage backImgae;
	backImgae.Load(_T("res//acs-pos.bmp"));
	showImage(backImgae,IDC_ACS_VISION);


	if(gateStatusMap.size()==0)
	{	
		for(int i=0;i<20;i++)
			gateStatusMap.insert(std::make_pair(i,0));
	}
	gateLampShow();
	//for(int i=0;i<20;i++)
	//{
	//	CImage redBack;
	//	redBack.Load(_T("res//green.png"));
	//	showImage(redBack,gateStatusID[i]);
	//}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAcsVisionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAcsVisionDlg::showImage(CImage image, UINT ID)
{
	CRect	rect;

	GetDlgItem(ID)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(ID)->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(ID);//获取控件句柄
	pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();//获取picture的DC
	pDc->SetStretchBltMode(COLORONCOLOR);
	image.Draw(pDc->m_hDC, rect);//将图片绘制到picture表示的区域内

	ReleaseDC(pDc);

}

void CAcsVisionDlg::gateLampShow(int gateId,int status)
{
	mLampChangeLock.Lock();
	std::map<int,int>::iterator gateOldStatuIte = gateOldStatusMap.find(gateId);
	if(gateOldStatuIte!=gateOldStatusMap.end())
	{
		if(gateOldStatuIte->second!=status)
		{
			gateOldStatuIte->second = status;
		}
		//else
		//{
		//	mLampChangeLock.Unlock();
		//	return;
		//}
	}
	else
	{
		gateOldStatusMap.insert(std::make_pair(gateId,status));
	}	
	CImage redLamp,greenLamp;
	redLamp.Load(_T("res//red.png"));
	greenLamp.Load(_T("res//green.png"));
	if(status==0){
		showImage(redLamp,gateStatusID[gateId]);
	}
	else if(status==1){
		showImage(greenLamp,gateStatusID[gateId]);
	}
	// change list tree
	std::map<int,gateInfoSt>::iterator iteInfo = mGateInfoMap.find(gateId);
	if(iteInfo!=mGateInfoMap.end())
	{
		CStringW gateNameW;
		MStrToWStr(gateNameW,iteInfo->second.gateName);
		CString gateName;
		gateName = cstringTool.CStrW2CStrT(gateNameW);
		if(status==0)
			InsertTree(iteInfo->second.strGroup,0,_T("-(close)"),gateName,gateName);
		else if(status==1)
			InsertTree(iteInfo->second.strGroup,0,_T("-(open)"),gateName,gateName);
	}

	mLampChangeLock.Unlock();
}

void CAcsVisionDlg::gateLampShow()
{
	for(int i=0;i<20;i++)
	{
		std::map<int,int>::iterator gateStatusIte = gateStatusMap.find(i);
		if(gateStatusIte!=gateStatusMap.end())
		{
			gateLampShow(i,gateStatusIte->second);	
		}
	}
}

std::string CAcsVisionDlg::GetCurrentPath()
{
	HMODULE module = GetModuleHandle(0); 
	char pFileName[MAX_PATH]; 
	GetModuleFileNameA(module, pFileName, MAX_PATH); 

	std::string csFullPath = pFileName; 
	int nPos = csFullPath.rfind( '\\' );
	if( nPos < 0 ) 
	{
		return ""; 
	}
	else 
	{
		return csFullPath.substr(0, nPos); 
	}
	return "";
}

void CAcsVisionDlg::initData()
{
	// gate status init
	//gateLampShow();

	//config init
	std::string configFile = GetCurrentPath()+"\\config.ini";
	CIniFile iniFile(configFile);
	std::vector<std::string>gateNameVec;
	iniFile.ReadSectionString("GATENAME",gateNameVec);
	int gateIndex =0;
	for(UINT i=0;i<gateNameVec.size();i++)
	{
		std::vector<std::string>gateGroupVec;
		StringSplit(gateNameVec[i],",",gateGroupVec);
		std::string groupName = gateGroupVec[0];
		for(UINT j =1;j<gateGroupVec.size();j++)
		{
			gateInfoSt oneGateInfo;
			oneGateInfo.strGroup = groupName;
			oneGateInfo.gateName = gateGroupVec[j];
			mGateInfoMap.insert(std::make_pair(gateIndex,oneGateInfo));
			gateNameMap.insert(std::make_pair(gateIndex++,gateGroupVec[j]));
			CStringW gateNameW;
			MStrToWStr(gateNameW,gateGroupVec[j]);
			CString gateName;
			gateName = cstringTool.CStrW2CStrT(gateNameW);
			InsertTree(groupName,0,_T("-(close)"),gateName,gateName);
		}
	}

    //===gate position
	std::vector<std::string>gatePosVec;
	iniFile.ReadSectionString("GATEPOSITION",gatePosVec);
	for(UINT i =0; i<gatePosVec.size();i++)
	{
		std::vector<std::string>oneGatePos;
		StringSplit(gatePosVec[i],",",oneGatePos);
		if(oneGatePos.size()!=3)
		{
			CString errorLog;
			errorLog.Format(_T("some error about gateposition configure id:%ud"),i);
			showLog(errorLog);
		}
		gatePosSt gatePos;
		gatePos.blockNumber =  std::atoi(oneGatePos[1].c_str());
		gatePos.bayNumber =  std::atoi(oneGatePos[2].c_str());
		m_GatePosMap.insert(std::make_pair(oneGatePos[0],gatePos));
	}

	//===sql server
	iniFile.ReadString("SQLSERVER","server",mSqlServerConn.sqlName);
	iniFile.ReadString("SQLSERVER","dataBase",mSqlServerConn.dataBaseName);
	iniFile.ReadString("SQLSERVER","userName",mSqlServerConn.userName);
	iniFile.ReadString("SQLSERVER","password",mSqlServerConn.password);
	iniFile.ReadString("SQLSERVER","hostName",m_sqlHostName);

	//=====gate status input
	std::vector<std::string>gateStatusVec;
	iniFile.ReadSectionString("GATESTATUS",gateStatusVec);
	for(UINT i =0;i<gateStatusVec.size();i++)
	{
		std::vector<std::string>oneGateStatus;
		StringSplit(gateStatusVec[i],",",oneGateStatus);
		gateSt gateStatusG;
		gateStatusG.gateName = oneGateStatus[0];
		gateStatusG.closeStatus = std::atoi(oneGateStatus[2].c_str());
		mGateInputMap.insert(std::make_pair(oneGateStatus[1],gateStatusG));
	}
	//=====gate action
	std::vector<std::string>gateActionVec;
	iniFile.ReadSectionString("GATEACTION",gateActionVec);
	for(UINT i=0;i<gateActionVec.size();i++)
	{
		std::vector<std::string>gateGVec;
		StringSplit(gateActionVec[i],",",gateGVec);
		mGateActionNameMap.insert(std::make_pair(gateGVec[0],gateGVec[1]));
	}
	//==cardReader 
	std::vector<std::string>cardReaderInVec;
	iniFile.ReadSectionString("CARDREADER_IN",cardReaderInVec);
	for(UINT i =0;i<cardReaderInVec.size();i++)
	{
		std::vector<std::string>cardReader;
		StringSplit(cardReaderInVec[i],",",cardReader);
		m_cardReaderInMap.insert(std::make_pair(cardReader[1],cardReader[0]));
	}
	std::vector<std::string>cardReaderOutVec;
	iniFile.ReadSectionString("CARDREADER_OUT",cardReaderOutVec);
	for(UINT i =0;i<cardReaderOutVec.size();i++)
	{
		std::vector<std::string>cardReader;
		StringSplit(gateStatusVec[i],",",cardReader);
		m_cardReaderOutMap.insert(std::make_pair(cardReader[1],cardReader[0]));
	}

}


void CAcsVisionDlg::StringSplit(const std::string& src, const std::string& separator, std::vector<std::string>& dest)
{
	dest.clear();
	std::string str = src;
	std::string substring;
	std::string::size_type start = 0, index;
	do
	{
		index = str.find_first_of(separator,start);
		if (index != std::string::npos)
		{    
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == std::string::npos) return;
		}
	}while(index != std::string::npos);
	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

void CAcsVisionDlg::initTree()
{
	CTreeCtrl* gateNameList =(CTreeCtrl*)GetDlgItem(IDC_TREE_GATE);
	//HICON icon;
	//icon=AfxGetApp()->LoadIcon(IDI_ICON1);
	//m_imagelist.Add(icon);  //把图标载入图像列表控件
	//gateNameList->SetImageList(&m_imagelist,TVSIL_NORMAL);  //为m_mytree设置一个图像列表，使CtreeCtrl的节点显示不同的图标
	//
	gateNameList->InsertItem(_T("gate"),TVI_ROOT);
	//hRoot = gateNameList->InsertItem(_T("gate"),0,0);
	m_GateItemMap.empty();
}

void CAcsVisionDlg::InsertTree(std::string strGroup, int iState, CString strIdState, CString strName, CString schlId)
{
	std::map<std::string, HTREEITEM>::iterator iteItem = m_GateItemMap.find(strGroup);
	CTreeCtrl* gateTreeList = (CTreeCtrl*)GetDlgItem(IDC_TREE_GATE);
	if (iteItem != m_GateItemMap.end())
	{
		HTREEITEM hChildItem = gateTreeList->GetChildItem(iteItem->second);//遍历该子节点，修改状态或者添加设备
		while(hChildItem!=NULL)
		{
			TreeItemOgj *pItem = (TreeItemOgj*)gateTreeList->GetItemData(hChildItem);
			if (pItem->csGateName == schlId /*&& pItem->csIpcName == strName && pItem->iGroup == iGroup*/)
			{
				//已存在，修改状态，返回
				gateTreeList->SetItemText(hChildItem, strName + strIdState);
				return;
			}
			hChildItem = gateTreeList->GetNextItem(hChildItem, TVGN_NEXT);  
		} 
		HTREEITEM sub_son0=gateTreeList->InsertItem(strName + strIdState, 0, 0, iteItem->second, TVI_LAST);
		TreeItemOgj *pItem = new TreeItemOgj(); 
		pItem->iState = iState;
		pItem->csGateName = strName;
		pItem->strGroup = strGroup;
		gateTreeList->SetItemData(sub_son0, (DWORD)pItem);
		gateTreeList->Expand(iteItem->second, TVE_EXPAND); //只展开根结点下面一层.
	}
	else
	{
		if(strGroup!="")
		{
			CStringW cstrGroupW;
			MStrToWStr(cstrGroupW,strGroup);
			CString cstrGroup;
			cstrGroup = cstringTool.CStrW2CStrT(cstrGroupW);
			HTREEITEM hRoot=gateTreeList->InsertItem(cstrGroup, 0, 1, TVI_ROOT, TVI_LAST);
			HTREEITEM sub_son0=gateTreeList->InsertItem(strName + strIdState, 0, 0, hRoot, TVI_LAST);
			TreeItemOgj *pItem = new TreeItemOgj(); 
			pItem->iState = iState;
			pItem->csGateName = strName;
			pItem->strGroup = strGroup;
			gateTreeList->SetItemData(sub_son0, (DWORD)pItem);
			gateTreeList->Expand( hRoot, TVE_EXPAND); //只展开根结点下面一层.
			m_GateItemMap.insert(std::make_pair(strGroup, hRoot));
		}
	}
}

void CAcsVisionDlg::CleanTree()
{
	//m_TextDemoTree 先清理上次的先关资源
	CTreeCtrl* gateTreeList = (CTreeCtrl*)GetDlgItem(IDC_TREE_GATE);
	for(std::map<std::string, HTREEITEM>::iterator treeIte = m_GateItemMap.begin();treeIte!=m_GateItemMap.end();treeIte++)
	{
		HTREEITEM hChildItem = gateTreeList->GetChildItem(treeIte->second);//遍历该子节点，修改状态或者添加设备
		while(hChildItem!=NULL)
		{
			TreeItemOgj *pItem = (TreeItemOgj*)gateTreeList->GetItemData(hChildItem);
			delete pItem;
			hChildItem = gateTreeList->GetNextItem(hChildItem, TVGN_NEXT);  
		} 
	}
	gateTreeList->DeleteAllItems();
}
void  CAcsVisionDlg::DeleteItemData(CTreeCtrl* pCtrl,HTREEITEM hItem)   
{      
	if(pCtrl->ItemHasChildren(hItem))       
	{   
		HTREEITEM   hChildItem = pCtrl->GetChildItem(hItem);       
		while(hChildItem!=NULL)       
		{   
			DeleteItemData(pCtrl,hChildItem); //递归遍历孩子节点       
			hChildItem  = pCtrl->GetNextItem(hChildItem, TVGN_NEXT);       
		}       
	}  
	else
	{//删除绑定的相关变量 
		TreeItemOgj  *pItem = (TreeItemOgj*)pCtrl->GetItemData(hItem);
		delete pItem;
	}
}
void CAcsVisionDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CleanTree();
	// TODO: 在此处添加消息处理程序代码
}

void CAcsVisionDlg::OnNMDblclkTreeGate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CTreeCtrl* gateTreeList = (CTreeCtrl*)GetDlgItem(IDC_TREE_GATE);
	CPoint point;
	GetCursorPos(&point);//获得鼠标点击的位置
	gateTreeList->ScreenToClient(&point);//转化为客户坐标
	UINT uFlags;

	HTREEITEM CurrentItem;
	CurrentItem=gateTreeList->HitTest(point,&uFlags);//获得当前点击节点的ITEM
	TreeItemOgj *pItem = (TreeItemOgj*)gateTreeList->GetItemData(CurrentItem);
	std::string strGateName;
	CStringW wcsGateName;
	wcsGateName = cstringTool.CStrT2CStrW(pItem->csGateName);
	WStrToMStr(wcsGateName,strGateName);
	int gateIndex = gateIndexFind(strGateName);
	if(gateIndex !=-1)
	{
		gateOpen(gateIndex,strGateName);
	}
}

int CAcsVisionDlg::gateIndexFind(std::string strName) //can not find return -1
{
	int result = -1;
	for(std::map<int,std::string>::iterator ite = gateNameMap.begin();ite!=gateNameMap.end();ite++)	{
		if(ite->second==strName){
		result = ite->first;
		break;
		}
	}
	return result;
}

void CAcsVisionDlg::initSqlServer()
{
	//mSqlServerConn.sqlName = "MGWL-PC";
	//mSqlServerConn.dataBaseName = "Amadeus5";


	//mSqlServerConn.sqlName = "Z\\ZSQLEXPRESS";
	//mSqlServerConn.dataBaseName = "Sign";
	//mSqlServerConn.userName = "sa";
	//mSqlServerConn.password = "123";

	//mSqlServerConn.serverIp = "";
	//mSqlServerConn.serverIp = "";
//	std::string configFile = GetCurrentPath()+"\\config.ini";
	//CIniFile iniFile(configFile);

	mSqlServerConn.OnInitADOConn();

	//gate processID, 
	for(std::map<std::string,std::string>::iterator ite = mGateActionNameMap.begin();
		ite!=mGateActionNameMap.end();ite++)
	{
		//std::string sql = "select * from [Amadeus5].[dbo].[Process] where name = '";
		//sql += ite->second;
		//sql+"'";
		std::string sql = "select * from [Amadeus5].[dbo].[Process]";
		_RecordsetPtr m_pRst= mSqlServerConn.GetRecordSet((_bstr_t)sql.c_str());
		while(!m_pRst->adoEOF)
		{
			std::string processName = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)1)->Value;
			if(processName==ite->second)
			{
				std::string processId = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)0)->Value;
				mActionIdMap.insert(std::make_pair(ite->second,processId));
				break;
			}
			m_pRst->MoveNext();
		}
	}
	SetTimer(1,500,NULL); //get gate status
	//_RecordsetPtr m_pRst= mSqlServerConn.GetRecordSet("select * from [Sign].[dbo].[Amessage]");//.m_pConnection->Execute("select * from [Sign].[dbo].[Amessage]",&var, adCmdText);
	//
	//while(!m_pRst->adoEOF)
	//{
	////	m_pRst->MoveFirst();
	//	_variant_t var = m_pRst->GetFields()->GetItem((long)0)->Value;
	//	std::string result;
	//	if(var.vt!=VT_NULL)
	//		 result = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)1)->Value;
	//	else
	//		result = "";
	//	//std::string result = (LPCTSTR)(_bstr_t)m_pRst->GetCollect("ID");
	//	CString logStr;
	//	logStr.Format(_T("%s"),result.c_str());
	//	showLog(logStr);
	//	m_pRst->MoveNext();
	//}

	//mSqlServerConn.ExecuteSQL("insert into [Sign].[dbo].[Amessage] values('3478','z','y')");
}
void CAcsVisionDlg::gateOpen(int gateIndex,std::string gateName)
{
	//zpmc check 
	int checkResult = zpmcSysCheck(gateName);
	CString checkLog;
	if(checkResult!=0)
	{
		checkLog.Format(_T("gate open zpmc system audit not pass %s"),gateName.c_str());
		showLog(checkLog);
		return;
	}
	else
	{
		checkLog.Format(_T("gate open zpmc system audit pass %s"),gateName.c_str());
		showLog(checkLog);
	}
	std::map<std::string,std::string>::iterator ite = mGateActionNameMap.find(gateName);
	if(ite!=mGateActionNameMap.end())
	{
		std::map<std::string,std::string>::iterator iteIdFind = mActionIdMap.find(ite->second);
		if(iteIdFind!=mActionIdMap.end())
		{
			std::string sql = "	INSERT INTO [Amadeus5].[dbo].[QueueMSGAPI] ([DateCreated], [ServerName], [Cmd], [Msg], [Status], [Result]) SELECT GETDATE(),'"+
				m_sqlHostName+"','ExecuteProcess','<query><pID>"+iteIdFind->second+"</pID></query>',0,0";
			CString logStr;
			if(mSqlServerConn.ExecuteSQL(sql.c_str()))
			{
				gateLampShow(gateIndex,1);//open
				logStr.Format(_T("excutesql %s success"),sql.c_str());
			}
			else
			{
				logStr.Format(_T("excutesql %s failure"),sql.c_str());
			}
			showLog(logStr);
		}
		else
		{
			CString logStr;
			logStr.Format(_T("can not find process id gate: %s process: %s"),gateName.c_str(),ite->second.c_str());
			showLog(logStr);
		}
	}
	else
	{
		CString logStr;
		logStr.Format(_T("can not find the command gate:%s"),gateName.c_str());
		showLog(logStr);
	}
}
void CAcsVisionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//===== check gate status
	CDialog::OnTimer(nIDEvent);
	//return;
	// get gate status
	std::string sql = "select * from [Amadeus5].[dbo].[Input]";
	sqlLock.Lock();
	_RecordsetPtr m_pRst= mSqlServerConn.GetRecordSet((_bstr_t)sql.c_str());
	sqlLock.Unlock();
	while(!m_pRst->adoEOF)
	{
		std::string inputName = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)2)->Value; //Name
		std::map<std::string,gateSt>::iterator statusFindIte = mGateInputMap.find(inputName);
		if(statusFindIte!=mGateInputMap.end())
		{
			int statusID = std::atoi((TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)25)->Value); //LastEventType
			int statusResult=0;
			if(statusID!=statusFindIte->second.closeStatus)
				statusResult = 1;
			// find gateID
			int gateIndex  = gateIndexFind(statusFindIte->second.gateName);
			if(gateIndex!=-1)
				gateLampShow(gateIndex,statusResult);
		}
		m_pRst->MoveNext();
	}
}

void CAcsVisionDlg::showLog(CString strLog)
{
	SIZE nWidth;
	HDC hdc = ::GetDC(m_hWnd);
	GetTextExtentPoint32(hdc, strLog, strLog.GetLength(), &nWidth);
	int lWidth = nWidth.cx - nWidth.cy;

	if (lWidth > 200)
	{
		mListLog.SetHorizontalExtent(lWidth);
	}
	::ReleaseDC(m_hWnd , hdc) ;

	if (strLog.GetLength() > 900)
	{
		strLog = strLog.Left(900);
	}
	CTime CurTime = CTime::GetCurrentTime();
	CString StrTime;
	StrTime.Format(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d   "), CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());
	strLog += _T("\r\n");
	StrTime += strLog;
	//数据过长删除部分

	mListLog.AddString(StrTime);
	int nMaxLines = 2048;
	if (mListLog.GetCount() > nMaxLines)
	{
		mListLog.ResetContent();
	}

}

//opc 2017/11/13
void CAcsVisionDlg::opcInit()
{
	CIniFile iniFile(GetCurrentPath() + "\\config\\General.ini");
	iniFile.ReadString("OPCGROUOPSET","OPC_SERVER",mOpcServerName);
	iniFile.ReadString("RTGOPCSET","add_group",mOpcGroupName);

	//===RTG locl selection config
	readConfigIntString(iniFile,"RTGOPCSET","local_selection",mOpcRTGLocalSelectionMap);

	//==RTG block number config
	readConfigIntString(iniFile,"RTGOPCSET","block_number",mOpcRTGBlockNumberMap);

	//===RTG bay number config
	readConfigIntString(iniFile,"RTGOPCSET","bay_number",mOpcRTGBayNumberMap);


	StartReConnectOpc();
	StartGetOpcData();

	////READ ROS SET
	//readConfigIntString(iniFile,"OPCGROUPSET","ROS_SET",mOpcRosMap);
	////READ RMG SET
	//readConfigIntString(iniFile,"OPCGROUPSET","RMG_SET",mOpcRmgMap);
	////READ MODE SET
	//readConfigIntString(iniFile,"OPCSWITCHGROUP","MODE_SET",mOpcModeMap);
	////READ FEET SET
	//readConfigIntString(iniFile,"OPCZOOMFEETSET","FEET",mOpcFeetMap);
	////READ HEIGHT SET
	//iniFile.ReadString("OPCZOOMHEIGHTSET","HEIGHT",mOpcHeight);
	////READ RCCS FEET SET
	//readConfigIntString(iniFile,"OPCZOOMFEETSET","RCCSFEET",mOpcRccsFeetMap);
	////READ RCCS HEIGHT SET
	//iniFile.ReadString("OPCZOOMHEIGHTSET","RCCSHEIGHT",mOpcRccsHeight);
	////READ PTZ MANUAL SET
	//readConfigIntStringVec(iniFile,"OPCIPCITEM",mOpcPtzManualMap);

	////READ CONNECT ROS
	//std::string connectStr;
	//iniFile.ReadString("OPCGROUPSET","CONNECT_ROS",connectStr);
	//std::vector<std::string> connectStrVec;
	//StringSplit(connectStr,",",connectStrVec);
	//for(int i=0;i<connectStrVec.size();i++)
	//{
	//	mOpcConnetAlwaysRos.push_back(std::atoi(connectStrVec[i].c_str()));
	//}
	////====audio configure
	//readAudioConfigure();

}
void CAcsVisionDlg::readConfigIntString(CIniFile iniFile,std::string groupName,std::string valueName,OpcValueItemNameMap& valueMap)
{
	std::string setStr;
	iniFile.ReadString(groupName.c_str(),valueName.c_str(),setStr);
	if(setStr.length())
	{
		std::vector<std::string> setVec;
		StringSplit(setStr,",",setVec);
		for(int i=0;i<setVec.size();i++)
		{
			std::vector<std::string>valueVec;
			StringSplit(setVec[i],":",valueVec);
			int id = std::atoi(valueVec[0].c_str());
			valueMap.insert(std::make_pair(id,mOpcGroupName+"."+valueVec[1]));
		}
	}
}

void CAcsVisionDlg::readConfigIntStringVec(CIniFile iniFile,std::string groupName,OpcValueItemNameMap& valueMap)
{
	std::vector<std::string> setStrVec;
	iniFile.ReadSectionString(groupName.c_str(),setStrVec);
	for(std::vector<std::string>::iterator ite = setStrVec.begin();ite!=setStrVec.end();ite++)
	{
		std::vector<std::string>setVec;
		StringSplit(*ite,",",setVec);
		if(setVec.size()==2)
		{
			int id = std::atoi(setVec[0].c_str());
			valueMap.insert(std::make_pair(id,setVec[1]));
		}
	}
}
void CAcsVisionDlg::StartGetOpcData()
{
	m_bGetOpcData = TRUE;
	DWORD lpExitCode;
	if(GetExitCodeThread(m_GetOpcDataThread,&lpExitCode))
	{
		if(lpExitCode == STILL_ACTIVE)
			return;
	}
	m_GetOpcDataThread = (HANDLE)_beginthreadex(NULL,0,WorkThreadReadOpcData,(LPVOID)this,0,0);
	//AX_Thread::spawn(CAcsVisionDlg::WorkThreadReadOpcData, this, 0, 0, &m_GetOpcDataThread, 0, 0, 0);
}

void CAcsVisionDlg::StopGetOpcData()
{
	if(m_GetOpcDataThread)
	{
		m_bGetOpcData = FALSE;
		DWORD lpExitCode;
		if(GetExitCodeThread(m_GetOpcDataThread,&lpExitCode))
		{
			if(lpExitCode == STILL_ACTIVE)
			{	
				if(WAIT_OBJECT_0!=WaitForSingleObject(m_GetOpcDataThread,1000)){
				TerminateThread(m_GetOpcDataThread,-1);
				m_GetOpcDataThread = NULL;
				}
			}
		}
	}
}
UINT CAcsVisionDlg::WorkThreadReadOpcData(void *lpParam)
{
	CAcsVisionDlg* pThis = (CAcsVisionDlg*)lpParam;
	pThis->RunGetOpcData();
	return 0;
}

void CAcsVisionDlg::RunGetOpcData()
{
	//m_nPLCHeartBeat = GetSysTimeMicros()/1000;
	while(m_bGetOpcData)
	{
		//GetOpcDataInfo();
		ReadOpcDataProcess();
		Sleep(m_nRefresh);
	}
}

//void CAcsVisionDlg::GetOpcDataInfo()
//{
//
//	for( Clientmap::iterator iter = m_clients.begin(); iter != m_clients.end(); iter++)
//	{
//		iter->second->ReadOpcDataProcess();
//	}
//}

void CAcsVisionDlg::StartReConnectOpc()
{
	m_bReconnectOpc = TRUE;
	DWORD lpExitCode;
	if(GetExitCodeThread(m_GetOpcDataThread,&lpExitCode))
	{
		if(lpExitCode == STILL_ACTIVE)
			return;
	}
	m_GetOpcDataThread = (HANDLE)_beginthreadex(NULL,0,WorkThreadReadOpcData,(LPVOID)this,0,0);
}

UINT CAcsVisionDlg::WorkThreadReConnectOpc(void *lpParam)
{
	CAcsVisionDlg* pThis = (CAcsVisionDlg*)lpParam;
	pThis->RunReConnectOpc();
	return 0;

}
void CAcsVisionDlg::RunReConnectOpc()
{
	while(m_bReconnectOpc)
	{
		if(!mConnectOpc)
		{
			opcServerConnect();
		}
		Sleep(3000);
	}
}

void CAcsVisionDlg::StopReConnectOpc()
{
	if(m_ReConnectOpcThread)
	{
		m_bReconnectOpc = FALSE;
		mConnectOpc = FALSE;
		if(WAIT_OBJECT_0!=WaitForSingleObject(m_ReConnectOpcThread,1000))
		{
			TerminateThread(m_ReConnectOpcThread,-1);
			m_ReConnectOpcThread = NULL;
		}
	}
	//释放内存
	{
		if(mOpcServer)
		{
			delete mOpcServer;
			mOpcServer = NULL;
			mOpcGroup = NULL;
		}
		for(OpcItemMap::iterator iterItem = mOpcItemMap.begin();iterItem!=mOpcItemMap.end();iterItem++)
		{
			if(iterItem->second)
			{
				iterItem->second = NULL;
			}
		}
		mOpcItemMap.clear();
	}
}
void CAcsVisionDlg::ReadOpcDataProcess()
{
	if(mOpcServer->is_connected())
	{	
		m_blockRtgInfoLock.Lock();
		m_blockRtgInfoMap.clear();
		for(OpcValueItemNameMap::iterator iteRtg = mOpcRTGLocalSelectionMap.begin();iteRtg!=mOpcRTGLocalSelectionMap.end();iteRtg++)
		{
			blockRtgInfo oneRtgInfo;
			int rtgNum = iteRtg->first;
			int rtgBlockNum;
			getItemValue(mOpcRTGBlockNumberMap,rtgNum,rtgBlockNum);
			getItemValue(mOpcRTGLocalSelectionMap,rtgNum,oneRtgInfo.loclSelection);
			getItemValue(mOpcRTGBayNumberMap,rtgNum,oneRtgInfo.bayNumber);
			//==add current block rtg info
			blockRtgInfoMap::iterator iteblockRtgInfoMap = m_blockRtgInfoMap.find(rtgBlockNum);
			
			if(iteblockRtgInfoMap!=m_blockRtgInfoMap.end())
			{
				iteblockRtgInfoMap->second.push_back(oneRtgInfo);
			}
			else
			{
				blockRtgInfoVec blocRtgInfoVecIn;
				blocRtgInfoVecIn.push_back(oneRtgInfo);
				m_blockRtgInfoMap.insert(std::make_pair(rtgBlockNum,blocRtgInfoVecIn));
			}
		}
		m_blockRtgInfoLock.Unlock();
	}//connect 
	else
	{
		opcServerConnect();
		Sleep(5000);
	}
}

void CAcsVisionDlg::getItemValue(OpcValueItemNameMap itemNameMap,int index,int& value)
{
	bool opcReadStatus;
	value = 0;
	OpcValueItemNameMap::iterator iteItemName = itemNameMap.find(index);
	if(itemNameMap.end()==iteItemName)
	{
		CString errorInfo;
		errorInfo.Format(_T("can not find one name of rtg %d"),index);
		showLog(errorInfo);
		return;
	}
	OpcItemMap::iterator iteItem = mOpcItemMap.find(iteItemName->second);
	if(mOpcItemMap.end()==iteItem)
	{
		CString errorInfo;
		errorInfo.Format(_T("can not find one item of rtg %d"),index);
		showLog(errorInfo);
		return;
	}
	value = mOpcGroup->read_item(opcReadStatus,iteItem->second);
	if(opcReadStatus==false)
	{
		opcServerDisconnect();
		value =0;
	}
}

Item* CAcsVisionDlg::opcItemFind(std::string name)
{
	OpcItemMap::iterator iterResult = mOpcItemMap.find(name);
	if(iterResult!=mOpcItemMap.end())
	{
		return iterResult->second;
	}
	else
	{
		return NULL;
	}
}
void CAcsVisionDlg::opcServerDisconnect()
{
	//if(mOpcServer)
	//{
	//	delete mOpcServer;
	//	mOpcServer = NULL;
	//	mOpcGroup = NULL;
	//}
	for(OpcItemMap::iterator iterItem = mOpcItemMap.begin();iterItem!=mOpcItemMap.end();iterItem++)
	{
		if(iterItem->second)
		{
			iterItem->second = NULL;
		}
	}
	mOpcItemMap.clear();
	mOpcServer->disconnect();
	mOpcGroup = NULL;
	//mOpcServer = NULL;
	mConnectOpc = false;
}
void CAcsVisionDlg::opcServerConnect()
{
	if(mConnectOpc)
		mOpcServer->disconnect();

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	OPCServerInfo info ;
	//info.m_Description = _T("ZPMC OPCServer");
	//info.m_ProgID = _T("ZPMC.OPCServer.2");
	wchar_t* p = UTF8ToUnicode(mOpcServerName.c_str());
	CString serverNameInfo(p);
	info.m_ProgID = serverNameInfo;
	info.m_Description = serverNameInfo;
	free(p);

	CString	m_strNodeName;

	OPCServerInfo*		server_info;
	ServerInfoList		server_infos;
	CLSID			cat_id;
	CLSID			clsid;
	cat_id = CATID_OPCDAServer20;

	unsigned long const NEXT_COUNT = 100;

	IOPCServerList*	server_list = 0;
	COSERVERINFO	si;
	MULTI_QI	qi;

	si.dwReserved1 = 0;
	si.dwReserved2 = 0;
	si.pwszName = L"127.0.0.1";
	si.pAuthInfo = NULL;

	qi.pIID = &IID_IOPCServerList;
	qi.pItf = NULL;
	qi.hr = 0;

	char connectLog[MAX_STR_LEN];
	sprintf_s(connectLog,MAX_STR_LEN-1,"opc server %s connecting......!",mOpcServerName.c_str());
	//CAcsVisionDlg::GetInstance()->Showlog2Dlg(connectLog,CONNECT_PLC_ERR);

	HRESULT hr = CoCreateInstanceEx(
		CLSID_OPCServerList,
		NULL,
		CLSCTX_ALL,
		&si,
		1,
		&qi);
	if(FAILED(hr) || FAILED(qi.hr)){
		CString msg(_T("Error connecting to OPC 2.0 Server Browser."));
		if( !m_strNodeName.IsEmpty() )
			msg.Format(_T("Error connecting to OPC 2.0 Server Browser on %s."), (LPCTSTR)m_strNodeName);

		if( hr == REGDB_E_CLASSNOTREG )
		{
			CString msg(_T("Please install the OPC 2.0 Components."));
			if( !m_strNodeName.IsEmpty() )
				msg.Format(_T("Please install the OPC 2.0 Components on %s."), (LPCTSTR)m_strNodeName);
		}
		//if( FAILED(hr) )
		//	theDoc->ReportError(msg, hr);
		//else
		//	theDoc->ReportError(msg, qi.hr);
	}
	else{
		server_list = (IOPCServerList*)qi.pItf;
		IEnumGUID* enum_guid = NULL;
		hr = server_list->EnumClassesOfCategories(
			1,
			&cat_id,
			1,
			&cat_id,
			&enum_guid);
		if(SUCCEEDED(hr)){
			unsigned long count = 0;
			CLSID cls_id[NEXT_COUNT];

			do{
				hr = enum_guid->Next(NEXT_COUNT, cls_id, &count);
				for(unsigned int index = 0; index < count; index ++){
					LPOLESTR prog_id;
					LPOLESTR user_type;
					HRESULT hr2 = server_list->GetClassDetails(cls_id[index], &prog_id, &user_type);
					if(SUCCEEDED(hr2)){
						OPCServerInfo* info = new OPCServerInfo(prog_id, user_type, cls_id[index]);
						if(info){
							//info->m_NodeName = sz_node;
							server_infos.AddTail(info);
							server_info = info;
						}

						CString name;
						name.Format(_T("%s"),(LPCTSTR)info->m_ProgID);

						CoTaskMemFree(prog_id);
						CoTaskMemFree(user_type);
					}
				}
			}while(hr == S_OK);
			enum_guid->Release();
			server_list->Release();
		}
		else{
			CString msg(_T("EnumClassesOfCategories failed:"));
		}
	}
	mOpcServer->SetServerInfo(server_info);
	if(mOpcServer->connect())
	{
		mConnectOpc = true;
		CString strLog;
		strLog.Format(_T("opc server %s connect success!"),server_info->m_ProgID);
		showLog(strLog);
		//char connectLog[MAX_STR_LEN];
		//sprintf_s(connectLog,MAX_STR_LEN-1,"opc server %s connect success!",server_info->m_ProgID);
		//CAcsVisionDlg::GetInstance()->m_plog->TraceInfo(connectLog);
		//CAcsVisionDlg::GetInstance()->Showlog2Dlg(connectLog,CONNECT_PLC_SUC);

	}
	else
	{
		mConnectOpc = false;
		CString strLog;
		strLog.Format(_T("opc server %s connect failed!"),server_info->m_ProgID);
		showLog(strLog);
		//char connectLog[MAX_STR_LEN];
		//sprintf_s(connectLog,MAX_STR_LEN-1,"opc server %s connect failed!",server_info->m_ProgID);
		//CAcsVisionDlg::GetInstance()->m_plog->TraceInfo(connectLog);
		//CAcsVisionDlg::GetInstance()->Showlog2Dlg(connectLog,CONNECT_PLC_ERR);
		return;
	}
	////add opc item
	//==add local selection
	opcItemAdd(mOpcRTGLocalSelectionMap,1);
	//==add block number
	opcItemAdd(mOpcRTGBlockNumberMap,0);
	//==add bay_number
	opcItemAdd(mOpcRTGBayNumberMap,0);
	
	mConnectOpc = true;
}
void CAcsVisionDlg::opcItemAdd(OpcValueItemNameMap ItemNameMap,int mode)
{
	if(mOpcGroup==NULL)
	{
		mOpcGroup = new COPCGroup(mOpcServer);
		if(mOpcGroup){
			mOpcGroup->set_name("group");
			mOpcGroup->set_update_rate(100);
			mOpcGroup->set_dead_band(0);
			mOpcGroup->set_time_bias(0);
			mOpcGroup->set_local_id(0);
			mOpcGroup->set_active(true);

			mOpcGroup->parent = mOpcServer;
			mOpcServer->add_group(mOpcGroup);
		}
	}
	//===add item
	for(OpcValueItemNameMap::iterator itemIter = ItemNameMap.begin(); itemIter!=ItemNameMap.end();itemIter++)
	{
		Item* item = new Item;
		ASSERT(item);
		std::string nameStr = itemIter->second;
		wchar_t* p = UTF8ToUnicode(nameStr.c_str());
		CString itemName(p);
		item->name = itemName;
		free(p);
		item->access_path = "";
		item->native_type = 0;
		item->quality = OPC_QUALITY_GOOD;
		item->active = true;
		opcSingleItemAdd(item,nameStr);
	}
}

void CAcsVisionDlg::opcSingleItemAdd(Item* item,std::string name)
{
	if(mOpcGroup==NULL)
	{
		mOpcGroup = new COPCGroup(mOpcServer);
		if(mOpcGroup){
			mOpcGroup->set_name("group");
			mOpcGroup->set_update_rate(100);
			mOpcGroup->set_dead_band(0);
			mOpcGroup->set_time_bias(0);
			mOpcGroup->set_local_id(0);
			mOpcGroup->set_active(true);

			mOpcGroup->parent = mOpcServer;
			mOpcServer->add_group(mOpcGroup);
		}
	}
	if(mOpcServer != NULL)
	{
		COPCGroup* group = mOpcServer->get_current_group();
		if(group != NULL)
		{
			CString itemAddLog;
			if(group->add_item(item))
			{
				mOpcItemMap.insert(std::make_pair(name,item));
				char connectLog[MAX_STR_LEN];
				sprintf_s(connectLog,MAX_STR_LEN-1,"add item %s success!",name.c_str());
				itemAddLog.Format(_T("%s"),connectLog);
			}
			else
			{
				char connectLog[MAX_STR_LEN];
				sprintf_s(connectLog,MAX_STR_LEN-1,"add item %s failured!",name.c_str());
				itemAddLog.Format(_T("%s"),connectLog);
			}
			showLog(itemAddLog);
			//if(item)
			//	theDoc->UpdateAllViews(NULL, UPDATE_GROUP, (CObject*)group);
		}
	}
	else
	{
		char connectLog[MAX_STR_LEN];
		sprintf_s(connectLog,MAX_STR_LEN-1,"add group failured!",name.c_str());
		CString addGroupLog;
		addGroupLog.Format(_T("%s"),connectLog);
		showLog(addGroupLog);
	}
}
wchar_t * CAcsVisionDlg::UTF8ToUnicode(const char* str)
{
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 ); 
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
	memset(result,0,(textlen+1)*sizeof(wchar_t)); 
	MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen ); 
	return result; 
}

void CAcsVisionDlg::cardReaderStart()
{
	cardReaderFlag= TRUE;
	DWORD lpExitCode;
	if(GetExitCodeThread(m_cardReaderThreadHandle,&lpExitCode))
	{
		if(lpExitCode == STILL_ACTIVE)
			return;
	}
	m_cardReaderThreadHandle = (HANDLE)_beginthreadex(NULL,0,WorkThreadCardReader,(LPVOID)this,0,0);
}

void CAcsVisionDlg::cardReaderStop()
{
	if(cardReaderFlag)
	{
		cardReaderFlag = FALSE;
		if(WAIT_OBJECT_0!=WaitForSingleObject(m_cardReaderThreadHandle,1000))
		{
			TerminateThread(m_cardReaderThreadHandle,-1);
			m_cardReaderThreadHandle = NULL;
		}
	}
}

UINT WINAPI CAcsVisionDlg::WorkThreadCardReader(LPVOID pParam)
{
	CAcsVisionDlg* pThis = (CAcsVisionDlg*)pParam;
	pThis->RunCardReaderProcess();
	return 0;
}

void CAcsVisionDlg::RunCardReaderProcess()
{
	CurrentTimegGet(m_lastTimeCardReaderInfoGet);
	Sleep(5000);
	CString cardReadLog;
	cardReadLog.Format(_T("card reader begin……"));
	showLog(cardReadLog);
	//m_lastTimeCardReaderInfoGet = "2017-10-20 11:35:27.000";
	while(cardReaderFlag)
	{
		cardReaderInfoGet();
		Sleep(4000);
	}
}

void CAcsVisionDlg::cardReaderInfoGet()
{

	//std::string sql = "select * from [Amadeus5].[dbo].[LOGt]";// where Date >"+m_lastTimeCardReaderInfoGet;
   std::string sql = "select * from [Amadeus5].[dbo].[LOGt] WHERE Date > '"+m_lastTimeCardReaderInfoGet+"'";
	//==update time 
	CurrentTimegGet(m_lastTimeCardReaderInfoGet);
	sqlLock.Lock();
	_RecordsetPtr m_pRst= mSqlServerConn.GetRecordSet((_bstr_t)sql.c_str());
	sqlLock.Unlock();
#ifdef DEBUG_MODE
	if(m_pRst->adoEOF)
	{
		CString queryLog;
		queryLog.Format(_T("can not find any log after sql:%s time:%s"),sql.c_str(),m_lastTimeCardReaderInfoGet.c_str());
		showLog(queryLog);
	}
#endif
	while(!m_pRst->adoEOF)
	{
		//==get cardReader Name
		std::string From_Name = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)3)->Value; //Name
		// if from_name exit
		std::map<std::string,std::string>::iterator cardReaderIte = m_cardReaderInMap.find(From_Name);
		if(cardReaderIte==m_cardReaderInMap.end())
		{
			cardReaderIte = m_cardReaderOutMap.find(From_Name);
			if(cardReaderIte==m_cardReaderOutMap.end())
			{
				m_pRst->MoveNext();
				CString findLog;
				findLog.Format(_T("can not find card reader %s"),From_Name.c_str());
				showLog(findLog);
				continue;
			}
		}
		//== get Desc3
		std::string Desc3 = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)6)->Value;// card holder name
		//== get result 1：accept 3:refuse
		std::string Trn_Type = (TCHAR*)(_bstr_t)m_pRst->GetFields()->GetItem((long)2)->Value; //Name
		if(Trn_Type=="1"){ //门禁系统允许进门
			int gateIndex = gateIndexFind(cardReaderIte->second);
			CString gateIdLogStr;
			if(gateIndex !=-1){
				gateIdLogStr.Format(_T("gate open id:%d name:%s"),gateIndex,cardReaderIte->second.c_str());
				gateOpen(gateIndex,cardReaderIte->second);
			}
			else{
			
				gateIdLogStr.Format(_T("can not find the id of gate:%s"),cardReaderIte->second.c_str());
				showLog(gateIdLogStr);
			}
		}
		CString logStr ;
		logStr.Format(_T("cardReader:%s people name:%s result:%s"),From_Name.c_str(),Desc3.c_str(),Trn_Type.c_str());
		showLog(logStr);
		m_pRst->MoveNext();
	}
}

void CAcsVisionDlg::CurrentTimegGet(std::string& timeStr)
{
	CTime CurTime = CTime::GetCurrentTime();
	char timeBuffer[255]={'\0'};
	sprintf_s(timeBuffer,sizeof(timeBuffer)-1,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond());
	timeStr = timeBuffer;
}

int CAcsVisionDlg::zpmcSysCheck(std::string gateNmae)//gate open check 0:ok else:no
{
	int result =0;
	CString checkLog;
	gatePosSt gatePos;
	std::map<std::string,gatePosSt>::iterator iteGatePos = m_GatePosMap.find(gateNmae);
	if(iteGatePos==m_GatePosMap.end())
	{
		checkLog.Format(_T("can not find the position gate:%s"),gateNmae.c_str());
		showLog(checkLog);
		return result;
	}
	result = 1; //默认不开门
	gatePos = iteGatePos->second;
	m_blockRtgInfoLock.Lock();
	blockRtgInfoMap::iterator rtgStatus = m_blockRtgInfoMap.find(gatePos.blockNumber);
	if(rtgStatus!=m_blockRtgInfoMap.end())
	{
		if(rtgStatus->second.size()==1)// 一台rtg
		{
			if(rtgStatus->second[0].loclSelection==1)
				result = 0;
		}
		else if(rtgStatus->second.size()==2)
		{
			if((rtgStatus->second[0].loclSelection==1)||(rtgStatus->second[1].loclSelection==1))
				result = 0;
		}
	}
	return result;
}


