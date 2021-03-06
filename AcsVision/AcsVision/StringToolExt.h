#pragma once  
#include "stdafx.h"
class CStringToolExt  
{  
public:  
	CStringToolExt();  
	~CStringToolExt();  
public:  
	static CStringA CStrT2CStrA(const CString &cstrSrc);  
	static CStringW CStrT2CStrW(const CString &cstrSrc);  
	static CString CStrA2CStrT(const CStringA &cstrSrc);  
	static CString CStrW2CStrT(const CStringW &cstrSrc);  
private:  
	static CStringW CStrA2CStrW(const CStringA &cstrSrcA);  
	static CStringA CStrW2CStrA(const CStringW &cstrSrcW);  
};  