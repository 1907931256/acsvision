#pragma once  
#include "stdafx.h"
#include <string>
class ADOConn  
{  
	// 定义变量  
public:  
	//添加一个指向Connection对象的指针:  
	_ConnectionPtr m_pConnection;  
	//添加一个指向Recordset对象的指针:  
	_RecordsetPtr m_pRecordset;  
	// 定义方法  
public:  
	ADOConn();  
	virtual ~ADOConn();  
	// 初始化—连接数据库  
	void OnInitADOConn();  
	std::string sqlName,dataBaseName,userName,password,serverIp;

	// 执行查询  
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);  
	// 执行SQL语句，Insert Update _variant_t  
	BOOL ExecuteSQL(_bstr_t bstrSQL);  
	void ExitConnect();  
};  