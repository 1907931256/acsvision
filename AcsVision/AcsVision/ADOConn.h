#pragma once  
#include "stdafx.h"
#include <string>
class ADOConn  
{  
	// �������  
public:  
	//���һ��ָ��Connection�����ָ��:  
	_ConnectionPtr m_pConnection;  
	//���һ��ָ��Recordset�����ָ��:  
	_RecordsetPtr m_pRecordset;  
	// ���巽��  
public:  
	ADOConn();  
	virtual ~ADOConn();  
	// ��ʼ�����������ݿ�  
	void OnInitADOConn();  
	std::string sqlName,dataBaseName,userName,password,serverIp;

	// ִ�в�ѯ  
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);  
	// ִ��SQL��䣬Insert Update _variant_t  
	BOOL ExecuteSQL(_bstr_t bstrSQL);  
	void ExitConnect();  
};  