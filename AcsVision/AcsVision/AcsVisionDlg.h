
// AcsVisionDlg.h : 头文件
//
#pragma once

#include <map>
#include <vector>
#include <string>
#include "IniFile.h"
#include "StringToolExt.h"

#include "opcserver.h"
#include <algorithm>

const CLSID CLSID_OPCServerList = {0x13486D51,0x4821,0x11D2,{0xA4,0x94,0x3C,0xB3,0x06,0xC1,0x00,0x00}};
#define __GUID_DEFINED__
#include "OPCDa_Cats.c"

#include "ADOConn.h"
#include "afxwin.h"

//#define DEBUG_MODE

#define M2W(mStrNull, pWBuf, nWSize) MultiByteToWideChar(CP_ACP, 0, mStrNull, -1, pWBuf, nWSize)//string to wstring
#define W2M(wStrNull, pMBuf, nMSize) WideCharToMultiByte(CP_ACP, 0, wStrNull, -1, pMBuf, nMSize, NULL, NULL)//wstring to string

typedef struct TreeItemOgj
{
	CString csGateName;
	int iState;
	std::string strGroup;
}TreeItemOgj;

typedef struct gateSt
{
	std::string gateName;
	int closeStatus;
}gateSt;

typedef struct gateInfoSt
{
	std::string strGroup;
	std::string gateName;
}gateInfoSt;

typedef struct blockRtgInfo
{
	int rtgNun;
	int bayNumber;
	int loclSelection;
}blockRtgInfo; //当前blok的rtg信息
typedef std::vector<blockRtgInfo>blockRtgInfoVec;
typedef std::map<int,blockRtgInfoVec>blockRtgInfoMap;
typedef struct gatePosSt
{
	int blockNumber;
	int bayNumber;
}gatePosSt;

//opc 
//==2017/11/13 OPC SET nanSha
#define m_nRefresh 500
#define MAX_STR_LEN 1024
typedef std::map<std::string,Item*> OpcItemMap; //OPC Item (name,item)e 
typedef std::map<int,std::string>OpcValueItemNameMap; 
typedef struct RTGInfoSt
{
	std::string localSelectionName;
	std::string blockNumber;
	std::string bayNumber;
}RTGInfoSt;

// CAcsVisionDlg 对话框
class CAcsVisionDlg : public CDialog
{
// 构造
public:
	CAcsVisionDlg(CWnd* pParent = NULL);	// 标准构造函数

	CStringToolExt  cstringTool; 
	BOOL WStrToMStr(CStringW & strCS, std::string & sStr);
	BOOL MStrToWStr(CStringW & strCS, std::string & sStr);
	void StringSplit(const std::string& src, const std::string& separator, std::vector<std::string>& dest);

	CImageList  m_imagelist;
	std::map<int,std::string>gateNameMap;//id,gateName
	std::map<int,gateInfoSt>mGateInfoMap;
	std::map<std::string, HTREEITEM> m_GateItemMap;
	CCriticalSection mLampChangeLock;
	void showImage(CImage image, UINT ID);
	void showLog(CString strLog);
	std::string GetCurrentPath();
	void initData();
	void initTree();
	void CleanTree();
	void DeleteItemData(CTreeCtrl* pCtrl,HTREEITEM hItem);   
	void InsertTree(std::string strGroup, int iState, CString strIdState, CString strName, CString schlId);
	int gateIndexFind(std::string strName);
	void changeGateStatus();
//= gate status
	UINT gateStatusID[20];
	std::map<int,int>gateStatusMap,gateOldStatusMap; //gate,status
	void gateLampShow(int gateId,int status);
	void gateLampShow();

//===sql server connect
	CCriticalSection sqlLock;
	ADOConn mSqlServerConn;
	std::string sqlServerIp,dataBaseName,dataBasePassword,dataBaseUserName;
	void initSqlServer();

//===gate control
  std::string m_sqlHostName;
  std::map<std::string,std::string>mGateActionNameMap; //gate name, action name 
  std::map<std::string,std::string>mActionIdMap; //gate action process name,id
  std::map<std::string,gateSt>mGateInputMap;//gate name, status input, close satus
  std::map<std::string,gatePosSt>m_GatePosMap;//gate name ,position
  void gateOpen(int gateIndex,std::string gateName);
 
//===card reader
  bool cardReaderFlag;
  std::string m_lastTimeCardReaderInfoGet;
  std::map<std::string,std::string>m_cardReaderInMap;//进门读卡器分布 name，gate name
  std::map<std::string,std::string>m_cardReaderOutMap;//出门读卡器分布 name， gate name
  void cardReaderStart();
  void cardReaderStop();
  HANDLE m_cardReaderThreadHandle;
  static UINT WINAPI WorkThreadCardReader(LPVOID pParam);
  void RunCardReaderProcess();
  void cardReaderInfoGet();
  void CurrentTimegGet(std::string& timeStr);

//===opc data read
  std::string mOpcServerName,mOpcGroupName;
  COPCServer* mOpcServer;
  COPCGroup* mOpcGroup;
  OpcItemMap mOpcItemMap; //OPC Item (name,item)
  OpcValueItemNameMap mOpcRTGLocalSelectionMap,mOpcRTGBayNumberMap,mOpcRTGBlockNumberMap;  //mRosMap :ros对应的item,mOpcRmgMap
  bool m_bGetOpcData ; 
  bool m_bReconnectOpc,mConnectOpc;
  HANDLE m_ReConnectOpcThread;
  HANDLE m_GetOpcDataThread;
  bool mOpcFlag;

  void opcInit(); //load opc 
  void readConfigIntString(CIniFile iniFile,std::string groupName,std::string valueName,OpcValueItemNameMap& valueMap);
  void readConfigIntStringVec(CIniFile iniFile,std::string groupName,OpcValueItemNameMap& valueMap);
 // void GetOpcDataInfo();
  void RunGetOpcData();
  void StartReConnectOpc();
  void RunReConnectOpc();
  void StopReConnectOpc();
  void StartGetOpcData();
  void StopGetOpcData();
  void opcServerConnect();
  void opcServerDisconnect();
  static UINT WINAPI WorkThreadReadOpcData(void *lpParam);
  static UINT WINAPI WorkThreadReConnectOpc(void *lpParam);
  wchar_t * UTF8ToUnicode( const char* str );
  void opcItemAdd(OpcValueItemNameMap ItemNameMap,int mode=0); 
  void opcSingleItemAdd(Item* item,std::string name);
  void ReadOpcDataProcess();
  Item* opcItemFind(std::string name);
  void OpcSwitchScreen(int nGroup, int mode, int iSwitchGroup);
  void OpcIpcZoomOperation(int nGroup ,int height, int feet,int mode,int iSwitchGroup);
  void getItemValue(OpcValueItemNameMap itemNameMap,int index,int& value);
//==block  
 blockRtgInfoMap m_blockRtgInfoMap;
 CCriticalSection m_blockRtgInfoLock;//信息更新锁

 //== zpmc system check
 int zpmcSysCheck(std::string gateNmae); //gate open check 0:ok else:no

// 对话框数据
	enum { IDD = IDD_ACSVISION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnNMDblclkTreeGate(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox mListLog;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
