#include "ADOConn.h"  
ADOConn::ADOConn(void)  
{  
}  

ADOConn::~ADOConn(void)  
{  
}  
// ��ʼ�����������ݿ�  
void ADOConn::OnInitADOConn()  
{  
	// ��ʼ��OLE/COM�⻷��  
	::CoInitialize(NULL);  
	try  
	{  
		// ����Connection����  
		m_pConnection.CreateInstance("ADODB.Connection");  
		// ���������ַ�����������BSTR�ͻ���_bstr_t����  
/*		std::string connectStrSet = "Provider=SQLOLEDB; Server="+sqlName+";Database="+dataBaseName+"; uid="+userName+
			"; pwd="+password+";"; 192.168.1.141\\*/ 
		//std::string connectStrSet = "Provider=SQLOLEDB.1;Data Sourcer=192.168.1.180,1433;Integrated Security=SSPI;Persist Security Info=False;User ID=sa;Password=123;Initial Catalog=Sign";//;Data Source="+sqlName;
		
		//std::string connectStrSet ="driver={SQL Server};Server=Z\\ZSQLEXPRESS;DATABASE=Sign;UID=sa;PWD=123";
		std::string connectStrSet ="driver={SQL Server};Server="+sqlName+";DATABASE="+dataBaseName+";UID="+userName+";PWD="+password;	
		_bstr_t strConnect = (_bstr_t)connectStrSet.c_str();
		
		//m_pConnection->Open(strConnect,"","",adModeUnknown);  
		m_pConnection->Open(strConnect,userName.c_str(),password.c_str(),adModeUnknown);  
	}  
	// ��׽�쳣  
 	catch(_com_error e)  
	{  
		// ��ʾ������Ϣ  
		AfxMessageBox(e.Description());
		//AfxMessageBox(_T("dataBase connect failed!"));
	}  
}  

_RecordsetPtr& ADOConn::GetRecordSet(_bstr_t bstrSQL)  
{  
	try  
	{  
		//�������ݿ⣬���connectionΪ������������  
		if(m_pConnection==NULL)  
			OnInitADOConn();  
		//������¼������  
		m_pRecordset.CreateInstance(__uuidof(Recordset));  
		//ȡ�ñ��еļ�¼  
		//m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),  
		//	adOpenDynamic,adLockOptimistic,adCmdText);  
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),  
			adOpenKeyset,adLockOptimistic,adCmdText);  
	}  
	catch(_com_error e)  
	{  
		e.Description();  
	}  
	//���ؼ�¼��  
	return m_pRecordset;  
}  



// ִ��SQL��䣬Insert Update _variant_t  
BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)  
{  
	// _variant_t RecordsAffected;  
	try  
	{  
		// �Ƿ��Ѿ��������ݿ�  
		if(m_pConnection == NULL)  
			OnInitADOConn();  

		// Connection�����Execute����:(_bstr_t CommandText,  
		// VARIANT * RecordsAffected, long Options )  
		// ����CommandText�������ִ���ͨ����SQL���  
		// ����RecordsAffected�ǲ�����ɺ���Ӱ�������,  
		// ����Options��ʾCommandText�����ͣ�adCmdText-�ı����adCmdTable-����  
		// adCmdProc-�洢���̣�adCmdUnknown-δ֪  
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);  
		return true;  
	}  
	catch(_com_error e)  
	{  
		AfxMessageBox(e.Description());  
		return false;  
	}  
}  
void ADOConn::ExitConnect()  
{  
	// �رռ�¼��������  
	if (m_pRecordset != NULL)  
		m_pRecordset->Close();  
	m_pConnection->Close();  
	// �ͷŻ���  
	::CoUninitialize();  
}  
