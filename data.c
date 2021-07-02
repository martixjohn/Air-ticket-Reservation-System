/*
	data.c
	数据处理
*/
#include "header.h"
extern HWND hwndLogIn;
/*
	函数调用前请使用有效检查函数，检验有效性合法性
*/
/*-----------USER ACCOUNT相关-------------*/
/*
*   函数: DataUAValid(USERACCOUNT ua)
*
*   目标: 检查指定用户账户数据中用户名和密码合法性
*
*   输入：
*		ua		用户账户
*	返回：
*		BOOL	判断合法性TRUE or FALSE
* 
*/
BOOL DataUAValid(USERACCOUNT ua)
{
	int lenU = lstrlen(ua.szUsername);
	int lenP = lstrlen(ua.szPassword);
	if (!(lenU >= 5 && lenU <= LIMIT_USERNAME)) return FALSE;
	if (!(lenP >= 5 && lenP <= LIMIT_PASSWORD)) return FALSE;
	if (DataValidCharcterUNorPW(ua.szUsername) && DataValidCharcterUNorPW(ua.szPassword))
	{
		return TRUE;
	}
	return FALSE;
}

/*
*   函数: DataGetValidUA_UOPStd(TCHAR szInfo[128])
*
*   目标: 获得合法的用户账号密码提示信息，并存放至指定szBuf
*
*   输入：
*		szInfo	提示信息存放处
*	返回：
*		BOOL	判断是否正确执行
*
*/
BOOL DataGetValidUA_UOPStd(TCHAR szInfo[128])
{
	StringCchPrintf(szInfo, 128, 
		TEXT("合法的账号密码：                      \n仅由字母，数字组成，限制%d到%d位以内\n"), 5, LIMIT_USERNAME);
	return TRUE;
}

/*
*   函数: DataGetValidUA_SelfInfoStd(TCHAR szInfo[128])
*
*   目标: 获得合法的用户个人信息的提示信息，并存放至指定szBuf
*
*   输入：
*		szInfo	提示信息存放处
*	返回：
*		BOOL	判断是否正常运行
*
*/
BOOL DataGetValidUA_SelfInfoStd(TCHAR szInfo[128])
{
	StringCchPrintf(szInfo, 128, 
		TEXT("合法的个人信息：                      \n范围是18字符以内\n不得含有: \",\"\"\\\"\n身份证由18位数字构成\n"));
	return TRUE;
}

/*
*   函数: DataValidUNorPW(TCHAR* szInfo)
*
*   目标: 检查指定用户账户数据中的用户或密码某一项的合法性
*
*   输入：
*		szInfo	szUsername or szPassword
*	返回：
*		BOOL	判断合法性TRUE or FALSE
*
*/
BOOL DataValidCharcterUNorPW(const TCHAR* szInfo)
{
	int i;
	if (szInfo[0] == TEXT('\0')) return FALSE;
	for (i = 0; szInfo[i] != TEXT('\0'); i++)
	{
		if (!(
			(szInfo[i] >= TEXT('0') && szInfo[i] <= TEXT('9')) ||
			(szInfo[i] >= TEXT('a') && szInfo[i] <= TEXT('z')) ||
			(szInfo[i] >= TEXT('A') && szInfo[i] <= TEXT('Z'))
			)
		)
		{
			if (szInfo[i] == TEXT('\b')) continue;
			return FALSE;
		}
	}
	return TRUE;
}

/*
*   函数: DataValidCharcterSelfInfo(TCHAR* szInfo, UA_SETINFO ua_setinfo)
*
*   目标: 检查指定用户账户数据中的SelfInfo某一项的合法性
*
*   输入：
*		szInfo			用户数据
*		ua_setinfo		指定szInfo类型
*	返回：
*		BOOL	判断合法性TRUE or FALSE
*
*/
BOOL DataValidCharcterSelfInfo(const TCHAR* szInfo, UA_SETINFO ua_setinfo)
{
	int i;
	if (szInfo[0] == TEXT('\0')) return FALSE;
	switch (ua_setinfo)
	{
	case UA_SETINFO_IDCARD:
		for (i = 0; szInfo[i] != TEXT('\0'); i++)
		{
			if (szInfo[i] < TEXT('0') || szInfo[i] > TEXT('9'))
			{
				return FALSE;
			}
		}
		if (i != 18) return FALSE;
		break;
	case UA_SETINFO_NICKNAME:case UA_SETINFO_NAME:
		for (i = 0; szInfo[i] != TEXT('\0'); i++)
		{
			if (szInfo[i] == TEXT(',') || szInfo[i] == TEXT('\\'))
			{
				return FALSE;
			}
		}
		break;
	}

	return TRUE;
}

/*
*   函数: DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o)
*
*   目标: 在内存中读取或更改用户账户(含管理账户)，作为程序当前账户，该函数管理当前账户信息
*
*   输入：
*		pUa		指向UA（用户账户信息）的指针变量
*		ua_o	操作方式：读/取
*	返回：
*		BOOL	判断程序运行状态
*/
BOOL DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o)
{
	//保存在该函数内
	static USERACCOUNT ua = { TEXT("\0"),TEXT("\0"),0,0 };
	switch (ua_o)
	{
	case UA_GET:
		*pUa = ua;
		break;
	case UA_CHANGE:
		if (pUa == NULL) return FALSE;
		ua = *pUa;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/*
*   函数: DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o)
*
*   目标: 在内存中读取或保存用户账户(含管理账户)链表，该函数存储头指针
*
*   输入：
*		pUaN	数据链表
*		uan_o	操作方式：读/取 UALIST_GET or _CHANGE		(管理员后缀_ADMIN)
*	返回：
*		BOOL	判断程序运行状态
*/
BOOL DataAccount_LinkedListMem(LPUSERACCOUNTNODE* ppUaN, UAL_OPERATE ual_o)
{
	static LPUSERACCOUNTNODE pUaN_Mem;
	static LPUSERACCOUNTNODE pUaN_admin_Mem;
	switch (ual_o)
	{
	case UALIST_GET:
		*ppUaN = pUaN_Mem;
		break;
	case UALIST_CHANGE:
		if (ppUaN == NULL) return FALSE;
		pUaN_Mem = *ppUaN;
		break;
	case UALIST_GET_ADMIN:
		*ppUaN = pUaN_admin_Mem;
		break;
	case UALIST_CHANGE_ADMIN:
		if (ppUaN == NULL) return FALSE;
		pUaN_admin_Mem = *ppUaN;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

/*
*   函数: DataAccount_LinkedListFile(UALIST_FILE ual_f)
*
*   目标: 将用户数据链表从内存中读取到文件或从文件读取到内存中的链表
*
*
*   输入：
*		ual_f	读取方式/方向
*				普通用户UALLIST_TO_FILE FILE_TO_UALLIST
*				管理人员 _ADMIN
*	返回：
*		BOOL	判断程序运行状态
*/
BOOL DataAccount_LinkedListFile(UALIST_FILE ual_f)
{
	FILE* fp = NULL;
	errno_t err;
	LPUSERACCOUNTNODE pUaN = NULL, pUaNOld = NULL, pUaN_head = NULL;
	TCHAR szBuf[2] = TEXT("\0");
	size_t len;
	long userStart = 0L, userEnd = 0L;
	size_t userLen;
	TCHAR szUserData[256] = TEXT("\0");
	switch (ual_f)
	{
	case UALIST_TO_FILE:
		if ((err = fopen_s(&fp, URL_UA, TEXT("wb+"))) != 0)
		{
			ErrorReport(ERR_FPOPEN_FAIL | ERR_IN_DataAccount_LinkedListFile);
			return FALSE;
		}
		if (fp == NULL) return FALSE;
		DataAccount_LinkedListMem(&pUaN, UALIST_GET);
		while (pUaN != NULL)
		{
			DataAccountFormat(&(pUaN->ua), szUserData, UA_TO_TEXT, &len);
			fwrite(szUserData, sizeof(TCHAR), len, fp);
			pUaN = pUaN->next;
		}

		fclose(fp);
		break;
	case UALIST_FROM_FILE:
		//文件不存在
		if ((err = fopen_s(&fp, URL_UA, TEXT("rb"))) != 0)
		{
			//创建文件
			if ((err = fopen_s(&fp, URL_UA, TEXT("wb+"))) != 0)
			{
				ErrorReport(ERR_FPOPEN_FAIL | ERR_IN_DataAccount_LinkedListFile);
				return FALSE;
			}
			
		}
		if (fp == NULL) return FALSE;
		
		while (!feof(fp))
		{
			fread(szBuf, sizeof(TCHAR), 1, fp);
			//防止越界
			if (feof(fp)) {
				fclose(fp);
				break;
			}

			//读取到一个用户数据
			if (szBuf[0] == TEXT('{'))
			{
				//记录{文件指针位置,注意宽字符2
				userStart = ftell(fp) - FCOUNT;
				//开辟内存空间
				if ((pUaN = malloc(sizeof(USERACCOUNTNODE))) == NULL)
				{
					ErrorReport(ERR_MEM_CREATE_FAIL | ERR_IN_DataAccount_LinkedListFile);
					return FALSE;
				}
				if (pUaN_head == NULL)
				{
					pUaN_head = pUaNOld = pUaN;
					
				}
				pUaN->ua.ut = UT_USER;//用户类型设置
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//计算字符串长度
				fseek(fp, userStart, SEEK_SET);
				//读取到文字
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//格式化，保存到ua
				if (pUaN != NULL)
				{
					DataAccountFormat(&(pUaN->ua), szUserData, TEXT_TO_UA, &userLen);
					pUaNOld->next = pUaN;
					pUaNOld = pUaN;
				}
				
			}
		}
		if (pUaN != NULL)
		{
			pUaN->next = NULL;
		}
		
		fclose(fp);
		//保存头指针
		DataAccount_LinkedListMem(&pUaN_head, UALIST_CHANGE);
		break;
	case UALIST_TO_FILE_ADMIN:
		if ((err = fopen_s(&fp, URL_ADMIN, TEXT("wb+"))) != 0)
		{
			ErrorReport(ERR_FPOPEN_FAIL | ERR_IN_DataAccount_LinkedListFile);
			return FALSE;
		}
		if (fp == NULL) return FALSE;
		DataAccount_LinkedListMem(&pUaN, UALIST_GET_ADMIN);
		while (pUaN != NULL)
		{
			DataAccountFormat(&(pUaN->ua), szUserData, UA_TO_TEXT, &len);
			fwrite(szUserData, sizeof(TCHAR), len, fp);
			pUaN = pUaN->next;
		}

		fclose(fp);
		break;
	case UALIST_FROM_FILE_ADMIN:
		//文件不存在
		if ((err = fopen_s(&fp, URL_ADMIN, TEXT("rb"))) != 0)
		{
			//创建文件
			if ((err = fopen_s(&fp, URL_UA, TEXT("wb+"))) != 0)
			{
				ErrorReport(ERR_FPOPEN_FAIL | ERR_IN_DataAccount_LinkedListFile);
				return FALSE;
			}

		}
		if (fp == NULL) return FALSE;

		while (!feof(fp))
		{
			fread(szBuf, sizeof(TCHAR), 1, fp);
			//防止越界
			if (feof(fp)) {
				fclose(fp);
				break;
			}

			//读取到一个用户数据
			if (szBuf[0] == TEXT('{'))
			{
				//记录{文件指针位置,注意宽字符2
				userStart = ftell(fp) - FCOUNT;
				//开辟内存空间
				if ((pUaN = malloc(sizeof(USERACCOUNTNODE))) == NULL)
				{
					ErrorReport(ERR_MEM_CREATE_FAIL | ERR_IN_DataAccount_LinkedListFile);
					return FALSE;
				}
				if (pUaN_head == NULL)
				{
					pUaN_head = pUaNOld = pUaN;

				}
				
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//计算字符串长度
				fseek(fp, userStart, SEEK_SET);
				//读取到文字
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//格式化，保存到ua
				if (pUaN != NULL)
				{
					DataAccountFormat(&(pUaN->ua), szUserData, TEXT_TO_UA, & userLen);
					DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_ADMINPER, &(pUaN->ua));
					if (szBuf[0] == TEXT('0'))
					{
						pUaN->ua.ut = UT_ADMIN;
					}
					else
					{
						pUaN->ua.ut = UT_SUPERADMIN;
					}
					pUaNOld->next = pUaN;
					pUaNOld = pUaN;
				}

			}
		}
		if (pUaN != NULL)
		{
			pUaN->next = NULL;
		}

		fclose(fp);
		//保存头指针
		DataAccount_LinkedListMem(&pUaN_head, UALIST_CHANGE_ADMIN);
		break;
	}
	return TRUE;
}

/*
*   函数: DataUAFile(UA_FILE u_f)
*
*   目标: 普通用户使用，处理普通用户数据库文件，从指定的UA和操作方式，注意没有查找相同功能，由其他函数实现查找
*			
*   输入：
*		u_f		说明操作方式
*			UA_ADDTO_FILE					将指定单个普通账户添加到文件 (文件追加形式)
*			UA_UPDATE_FILE					信息进行更新
* 
*		ua		待操作的用户账户
*	返回：
*		BOOL	判断程序运行状态
*/
BOOL DataUAFile(UA_FILE u_f, LPUSERACCOUNT pUa)
{
	FILE* fp = NULL;
	errno_t err;
	TCHAR szBuf[256] = {TEXT('\0')};
	size_t uLen = 0;
	LPUSERACCOUNTNODE pUaN = NULL, pUaN_head = NULL;

	//从内存获得用户信息,存到临时变量ua
	//DataUserAccount(&ua, UA_GET);
	switch (u_f)
	{
	case UA_ADDTO_FILE:
		//直接写文件，无需再经过链表
		//打开文件,无则创建
		if ((err = fopen_s(&fp, URL_UA, TEXT("ab+"))) != 0)
		{
			ErrorReport(ERR_FPOPEN_FAIL | ERR_IN_DataUAFile);
			return FALSE;
		}
		if (fp == NULL) return FALSE;
		//将数据格式化到szBuf以便文件保存
		if (!DataAccountFormat(pUa, szBuf, UA_TO_TEXT, &uLen))
		{
			//此时说明程序异常
			fclose(fp);
			return FALSE;
		}
		fwrite(szBuf, uLen, FCOUNT, fp);
		fclose(fp);
		return TRUE;
		break;

	case UA_UPDATE_FILE:
		//打开文件,无则错误
		err = fopen_s(&fp, URL_UA, TEXT("ab+"));
		if (err != 0) {
			return FALSE;
			break;
		}
		if (fp == NULL) return FALSE;
		fclose(fp);

		//文件读入内存中链表
		DataAccount_LinkedListFile(FILE_TO_UALIST);
		DataAccount_LinkedListMem(&pUaN_head, UALIST_GET);
		pUaN = pUaN_head;
		while (pUaN != NULL)
		{
			//和链表中数据进行对比
			if (lstrcmp(pUa->szUsername, pUaN->ua.szUsername) == 0)
			{
				//相同则直接覆盖
				pUaN->ua = *pUa;
				break;
			}
			pUaN = pUaN->next;
		}
		//文件保存
		DataAccount_LinkedListFile(UALIST_TO_FILE);
		//更新当前用户信息
		if (pUaN != NULL)
		{
			DataUserAccount(&(pUaN->ua), UA_CHANGE);
		}
		
		break;
	}
	return TRUE;
}


/*
*   函数: DataGetUserType()
*
*   目标: 获得返回当前用户的类型
*

*	返回：
*		USERTYPE
*/
USERTYPE DataGetUserType()
{
	USERACCOUNT ua;
	DataUserAccount(&ua, UA_GET);
	return ua.ut;
}

//用户账户搜索结果存取，返回用户数目，错误返回-1
int DataAccountSearchRet(LPUSERACCOUNTNODE pUaNodeArr[MAX_SEARCH_UA_ID], UAL_SEARCHRET_MEM ual_sr_m)
{
	static LPUSERACCOUNTNODE pUaNodeArr_Mem[MAX_SEARCH_UA_ID] = { NULL };
	int i;
	switch (ual_sr_m)
	{
	case UAL_SR_GET:
	case UAL_SR_SET:
		for (i = 0; i < MAX_SEARCH_UA_ID; i++)
		{
			if (ual_sr_m == UAL_SR_GET)
			{
				if ((pUaNodeArr[i] = pUaNodeArr_Mem[i]) == 0) break;
			}
			else if (ual_sr_m == UAL_SR_SET)
			{
				if (i == 0) ClearPointerArrToZero(pUaNodeArr_Mem, MAX_SEARCH_UA_ID);
				if ((pUaNodeArr_Mem[i] = pUaNodeArr[i]) == 0) break;

			}
		}
		return i;
		break;
	default:
		return -1;
	}

}

//获取指定用户的订单数量,调用前请开辟用户订单内存
size_t DataAccount_GetOrderNum(LPUSERACCOUNT pUa)
{
	LPORDERINFONODE pOINode = NULL;
	size_t num = 0;
	DataOrderLinkedListMem(&pOINode, OILIST_GET_HEADNODE);
	while (pOINode != NULL)
	{
		if (lstrcmp(pUa->szUsername, pOINode->oi.szUsername) == 0)
		{
			num++;
		}
		pOINode = pOINode->next;
	}
	return num;
}

/*
*   函数: DataSearchAccount_File(USERACCOUNT uaTrgt)
*
*   目标: 从数据库文件中查找是否有目标用户或管理员的数据，只查询用户名和密码是否存在或匹配
*
*   输入：
*		fp					指定的文件指针
*		uaTrgt				目标用户
*	返回：
*		UA_SEARCHRET类型	搜索结果
*			
*/
UA_SEARCHRET DataAccountSearch_File(USERACCOUNT uaTrgt)
{
	FILE* fp = NULL;
	errno_t err;//错误代码
	TCHAR szBuf[2] = { TEXT('\0') };
	TCHAR szUserData[256] = { TEXT('\0') };
	TCHAR szUsername[20] = { TEXT('\0') };
	TCHAR szPassword[20] = { TEXT('\0') };
	long userStart = 0L, userEnd = 0L;
	size_t userLen = 0;
	int i = 0, j = 0;
	USERACCOUNT ua;

	//分别在用户和管理员数据中检索
	for (j = 0; j < 2; j++)
	{
		switch (j)
		{
		case 0:
			err = fopen_s(&fp, URL_UA, TEXT("rb"));
			break;
		case 1:
			err = fopen_s(&fp, URL_ADMIN, TEXT("rb"));
			break;
		}
		if (err != 0)
		{
			return UA_FOUNDFILEERR;
		}
		if (fp == NULL) return UA_FOUNDFILEERR;
		while (!feof(fp))
		{
			fread(szBuf, sizeof(TCHAR), 1, fp);
			//防止越界
			if (feof(fp)) {
				fclose(fp);
				break;
			}
			//读取到一个用户数据
			if (szBuf[0] == TEXT('{'))
			{
				//记录{文件指针位置,注意宽字符2
				userStart = ftell(fp) - FCOUNT;
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//计算字符串长度
				fseek(fp, userStart, SEEK_SET);
				//读取到文字
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//格式化，保存到ua
				DataAccountFormat(&ua, szUserData, TEXT_TO_UA, &userLen);
				//ID相同
				if (lstrcmp(ua.szUsername, uaTrgt.szUsername) == 0)
				{
					//密码相同
					if (lstrcmp(ua.szPassword, uaTrgt.szPassword) == 0)
					{
						fclose(fp);
						switch (j)
						{
						case 0:
							return UA_FOUND;//普通用户
						case 1:
							return UA_FOUND_ADMIN;//管理员
						}
						
					}
					else
					{
						fclose(fp);
						return UA_FOUND_ERR_PW;
					}
				}
			}
		}
		fclose(fp);
	}

	return UA_NOTFOUND;
}

/*
*   函数: DataAccountSearchFileToMem(USERACCOUNT uaTrgt)
*
*   目标: 从数据库文件中查找是否有目标用户或管理员的数据，若查询到，将自动存入内存
*			内存数据由DataUserAccount管理(当前)
*
*   输入：
*		fp					指定的文件指针
*		uaTrgt				目标用户
*	返回：
*		UA_SEARCHRET类型	搜索结果
*
*/
UA_SEARCHRET DataAccountSearchFileToMem(USERACCOUNT uaTrgt)
{
	FILE* fp = NULL;
	errno_t err;//错误代码
	TCHAR szBuf[2] = TEXT("\0");
	TCHAR szUserData[256] = TEXT("\0");
	TCHAR szUsername[20] = TEXT("\0");
	TCHAR szPassword[20] = TEXT("\0");
	long userStart = 0L, userEnd = 0L;
	size_t userLen = 0;
	int i = 0, j = 0;
	USERACCOUNT ua = {0};

	//分别在用户和管理员数据中检索
	for (j = 0; j < 2; j++)
	{
		switch (j)
		{
		case 0:
			err = fopen_s(&fp, URL_UA, TEXT("rb"));
			break;
		case 1:
			err = fopen_s(&fp, URL_ADMIN, TEXT("rb"));
			break;
		}
		if ((err != 0 || fp == NULL) && j == 1)
		{
			return UA_FOUNDFILEERR;
		}
		if (fp == NULL) continue;
		while (!feof(fp))
		{
			fread(szBuf, sizeof(TCHAR), 1, fp);
			//防止越界
			if (feof(fp)) {
				fclose(fp);
				break;
			}
			//读取到一个用户数据
			if (szBuf[0] == TEXT('{'))
			{
				//记录{文件指针位置,注意宽字符2
				userStart = ftell(fp) - FCOUNT;
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//计算字符串长度
				fseek(fp, userStart, SEEK_SET);
				//读取到文字
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//格式化，保存到ua
				DataAccountFormat(&ua, szUserData, TEXT_TO_UA, &userLen);
				//ID相同
				if (lstrcmp(ua.szUsername, uaTrgt.szUsername) == 0)
				{
					//密码相同
					if (lstrcmp(ua.szPassword, uaTrgt.szPassword) == 0)
					{
						fclose(fp);
						switch (j)
						{
						case 0:
							ua.ut = UT_USER;
							DataUserAccount(&ua, UA_CHANGE);
							return UA_FOUND;//普通用户
						case 1:
							DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_ADMINPER, &ua);
							if (szBuf[0] == TEXT('1'))
							{
								ua.ut = UT_SUPERADMIN;
							}
							else
							{
								ua.ut = UT_ADMIN;
							}
							DataUserAccount(&ua, UA_CHANGE);
							//注意，管理员只有nickname
							return UA_FOUND_ADMIN;//管理员
						}

					}
					else
					{
						fclose(fp);
						return UA_FOUND_ERR_PW;
					}
				}
			}
		}
		fclose(fp);
	}

	return UA_NOTFOUND;
}

/*
*   函数: DataFormatAccount(LPUSERACCOUNT pUa, TCHAR* szBuf, DATA_FORMAT d_f, size_t *pLength)
*
*   目标: 将单个数据格式化，用于文件保存或读取，TEXT转换成UA结构体或是相反
*
*   输入：
*		pUa		指向USERACCOUNT结构体的指针变量
*		szBuf	TEXT
*		d_f		转换方向
*		pLength	TEXT长度(编码默认，不一定按CHAR计算)，TEXT传入UA时需指定该长度，UA传入TEXT时自动赋值返回
*	返回：
*		BOOL	判断程序运行状态
*/

BOOL DataAccountFormat(LPUSERACCOUNT pUa, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
{
	/*
		文件中数据组织形式：
					iDataPos=1       2     ...
		UA TEXT:	{[USERNAME],[PASSWORD],[NICKNAME,NAME,IDCARD]},...
					ADMIN:					IDCARD为0:ADMIN 1:SUPERADMIN
	*/
	TCHAR szTemp[LIMIT_ACCOUNT] = {TEXT('\0')};
	size_t i = 0, j = 0, k = 0;
	size_t iStart = 0, iEnd = 0;
	size_t iDataPos = 0;
	BOOL fVaild = FALSE;
	switch (d_f)
	{
	case UA_TO_TEXT:
		StringCchPrintf(szBuf, LIMIT_ACCOUNT, TEXT("{[%s][%s][%s]}"),pUa->szUsername,pUa->szPassword,pUa->szSelfInfo);
		*pLength = lstrlen(szBuf);//返回长度
		return TRUE;
		break;
	case TEXT_TO_UA:
		//先清空
		ClearUAStructToZero(pUa);
		while (i<*pLength && szBuf[i] != TEXT('\0'))
		{
			if (szBuf[i] == TEXT('['))
			{
				iStart = i + 1;
				iDataPos++;
			}
			if (szBuf[i] == TEXT(']'))
			{
				fVaild = TRUE;
				iEnd = i - 1;
				for (j = iStart, k=0; j <= iEnd; j++, k++)
				{
					szTemp[k] = szBuf[j];
				}
				szTemp[k] = TEXT('\0');
				if (iStart > iEnd) return FALSE;//说明为空
				if (iDataPos == 1)
					StringCchCopy(pUa->szUsername, iEnd - iStart + 2, szTemp);//注意\0算入其中
				if (iDataPos == 2)
					StringCchCopy(pUa->szPassword, iEnd - iStart + 2, szTemp);
				if (iDataPos == 3)
					StringCchCopy(pUa->szSelfInfo, iEnd - iStart + 2, szTemp);
			}
			i++;
		}
		if (!fVaild) return FALSE;
		return TRUE;
		break;
	default:
		return FALSE;
	}
}


/*
*   函数: DataAccount_GetSelfInfoFromUA(TCHAR* szBuf, UA_GETINFO ua_getinfo, LPUSERACCOUNT pUa)
*
*   目标: 从指定用户账户(普通用户)结构体截取info
*
*   输入：
*		_In_  pUa			指向USERACCOUNT结构体的指针变量
*		_Out_ szDest			输出的目标
*		_In_  ua_getinfo	指定输出的目标
*				birth格式 2000-12-11
*				sex格式   男/女
*		
*	返回：
*		BOOL	判断截取不到，是否存在
*/
BOOL DataAccount_GetSelfInfoFromUA(TCHAR* szDest, UA_GETINFO ua_getinfo, LPUSERACCOUNT pUa)
{
	/*
		nickname name ID
	*/

	TCHAR szInfo[128] = TEXT("\0");//存放所有信息
	TCHAR szTemp[128] = TEXT("\0");
	TCHAR* pszInfo = pUa->szSelfInfo;
	int i = 0, j = 0;
	size_t pos = 0;
	int nicknameStart = 0, nicknameEnd = 0;
	int nameStart = 0, nameEnd = 0;
	int IDStart = 0, IDEnd = 0;
	int iStart = 0, iEnd = 0;
	lstrcpy(szInfo, pUa->szSelfInfo);
	if (pUa->szSelfInfo[0] == TEXT('\0')) return FALSE;
	while (*pszInfo != TEXT('\0') && i < 128)
	{
		if (*pszInfo == TEXT(','))
		{
			pos++;//当前,位置
			switch (pos)
			{
			case 1:
				nicknameEnd = i - 1;
				nameStart = i + 1;

				break;
			case 2:
				nameEnd = i - 1;
				IDStart = i + 1;
				break;
			}
			
		}
		i++;
		pszInfo++;
		
	}
	IDEnd = i - 1;
	switch (ua_getinfo)
	{
	case UA_GETINFO_NICKNAME:
		iStart = nicknameStart;
		iEnd = nicknameEnd;
		break;
	case UA_GETINFO_NAME:
		iStart = nameStart;
		iEnd = nameEnd;
		break;
	case UA_GETINFO_IDCARD:
		iStart = IDStart;
		iEnd = IDEnd;
		break;
	case UA_GETINFO_BIRTH:
		iStart = IDStart + 6;
		iEnd = IDStart + 13;
		break;
	case UA_GETINFO_SEX:
		iStart = IDStart + 16;
		iEnd = IDStart + 16;
		break;

	}
	//不存在该项

	if (iStart > iEnd)
	{
		lstrcpy(szDest, TEXT("\0"));
		return FALSE;
	}
	for (j = 0, i = iStart; i <= iEnd; i++, j++)
	{
		szTemp[j] = szInfo[i];
	}
	szTemp[j] = TEXT('\0');
	switch (ua_getinfo)
	{
	case UA_GETINFO_BIRTH:
		for (i = 0; i <= 3; i++)
			szDest[i] = szTemp[i];
		szDest[4] = TEXT('-');
		szDest[5] = szTemp[4];
		szDest[6] = szTemp[5];
		szDest[7] = TEXT('-');
		szDest[8] = szTemp[6];
		szDest[9] = szTemp[7];
		szDest[10] = TEXT('\0');
		break;
	case UA_GETINFO_SEX:
		szDest[0] = szTemp[0];
		szDest[1] = szTemp[1];
		if (atoi(szDest) % 2 == 0)
			szDest[0] = TEXT('女');
		else szDest[0] = TEXT('男');
		szDest[1] = TEXT('\0');
		break;
	default:
		lstrcpy(szDest, szTemp);
	}
	/*
		xxxxxx  year month  day     x  x  sex  x
		012345  6789 10 11 12 13	14 15  16  17
				0123 4  5  6  7     8  9  10
				0123-5  6 -8  9
	*/

	return TRUE;
}

/*
*   函数: DataAccount_SetSelfInfoFromUA(TCHAR* szBuf, UA_SETINFO ua_setinfo, LPUSERACCOUNT pUa)
*
*   目标: 从指定个人信息类型，将szBuf中的数据修改到UA
*
*   输入：
*		_Out_  pUa			指向USERACCOUNT结构体的指针变量
*		_In_ szBuf			修改内容
*		_In_  ua_setinfo	指定修改的数据

*
*	返回：
*		BOOL	判断程序运行状态
*/
BOOL DataAccount_SetSelfInfoToUA(TCHAR* szBuf, UA_SETINFO ua_setinfo, LPUSERACCOUNT pUa)
{
	TCHAR szInfo[128] = TEXT("\0");
	TCHAR szNickName[19] = TEXT("\0");
	TCHAR szName[19] = TEXT("\0");
	TCHAR szIDcard[19] = TEXT("\0");
	DataAccount_GetSelfInfoFromUA(szNickName, UA_GETINFO_NICKNAME, pUa);
	DataAccount_GetSelfInfoFromUA(szName, UA_GETINFO_NAME, pUa);
	DataAccount_GetSelfInfoFromUA(szIDcard, UA_GETINFO_IDCARD, pUa);
	switch (ua_setinfo)
	{
	case UA_SETINFO_NICKNAME:
		StringCchPrintf(szInfo, 128, TEXT("%s,%s,%s"), szBuf, szName, szIDcard);
		lstrcpy(pUa->szSelfInfo, szInfo);
		break;
	case UA_SETINFO_NAME:
		StringCchPrintf(szInfo, 128, TEXT("%s,%s,%s"), szNickName, szBuf, szIDcard);
		lstrcpy(pUa->szSelfInfo, szInfo);
		break;
	case UA_SETINFO_IDCARD:
		StringCchPrintf(szInfo, 128, TEXT("%s,%s,%s"), szNickName, szName, szBuf);
		lstrcpy(pUa->szSelfInfo, szInfo);
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}

/*-----------航班数据相关-------------*/
/*
{[出发地][目的地][6,21,19,30][6,22,10,0][航班ID][飞机编号][价格][票数][其他说明如经济舱]}
	256字符空间
*/

/*
*   函数: DataFlightLinkedListMem(LPFLIGHTDATANODE pFlNode_Head, FLLIST_OPERATE fl_o)
*
*   目标: 在内存中保存和读取飞机动态链表数据，调用此函数进行当前链表数据的获取和修改
*
*   输入：
*		pFlNode_Head		指向链表的头指针的指针，首节点
*		fll_o				说明指向的链表的操作方式
*			FLLIST_GET_HEADNODE		从内存中获取链表，头指针保存在pFl_HeadNode
			FLLIST_CHANGE_HEADNODE	将pFl_HeadNode指向的链表覆盖保存，同时删除内存中以前链表
*	返回：
*		size_t		返回链表节点数即航班数据总数
*/
IDDATA_NUM DataFlightLinkedListMem(LPFLIGHTDATANODE* ppFlNode_Head, FLLIST_OPERATE fll_o)
{
	static LPFLIGHTDATANODE pFlNode_Mem = NULL;
	IDDATA_NUM iListNum = 0;//保存链表数
	LPFLIGHTDATANODE pFlNode = NULL;
	LPFLIGHTDATANODE pFlNode_next = NULL;
	switch (fll_o)
	{
		//读取链表
	case FLLIST_GET_HEADNODE:
		pFlNode = *ppFlNode_Head = pFlNode_Mem;
		while (pFlNode != NULL)
		{
			iListNum++;
			pFlNode = pFlNode->next;
		}
		break;
		//改变链表
	case FLLIST_CHANGE_HEADNODE:
		pFlNode = pFlNode_Mem;
		//保存传进来的链表
		pFlNode = pFlNode_Mem = *ppFlNode_Head;
		//记录数目
		while (pFlNode != NULL)
		{
			iListNum++;
			pFlNode = pFlNode->next;
		}
		break;
	}
	
	return iListNum;
}

/*
*   函数: DataFlightLinkedListFile(FL_FILE fl_file)
*
*   目标: 将航班数据从内存读取到文件或从文件中读取到内存，内存数据由DataFlightLinkedListMem管理
*
*   输入：
*		fl_file		指定操作方式
*			FLLIST_TO_FILE			从内存覆盖写入磁盘文件
*			FLLIST_FROM_FILE		从磁盘文件中全部读取到内存
*	返回：
*		BOOL		判断程序运行状态
*/
BOOL DataFlightLinkedListFile(FL_FILE fl_file)
{
	TCHAR szBuf[LIMIT_FLIGHTDATA] = TEXT("\0");
	size_t uLen;
	FILE* fp = NULL;
	errno_t err;
	long iStart, iEnd;
	LPFLIGHTDATANODE pFlNode_head = NULL;
	LPFLIGHTDATANODE pFlNode = NULL, pFlNode_new = NULL;
	size_t iItemLen;
	long test = 0L;
	switch (fl_file)
	{
	case FLLIST_TO_FILE:
		DataFlightLinkedListMem(&pFlNode, FLLIST_GET_HEADNODE);
		if ((err = fopen_s(&fp, URL_DATA, TEXT("wb+"))) != 0)
		{
			return FALSE;
		}
		if (fp == NULL) return FALSE;
		while (pFlNode!=NULL)
		{
			DataFlightFormat(&pFlNode->fl, szBuf, FL_TO_TEXT, &uLen);
			fwrite(szBuf, sizeof(szBuf), 1, fp);
			pFlNode = pFlNode->next;
		}
		fclose(fp);
		break;
	case FLLSIT_FROM_FILE:
		if ((err = fopen_s(&fp, URL_DATA, TEXT("rb"))) != 0)
		{
			return FALSE;
		}
		
		if (fp == NULL) return FALSE;
		
		while (!feof(fp))
		{
			fread(szBuf, sizeof(TCHAR), 1, fp);
			if (feof(fp)) break;
			
			if (szBuf[0] == TEXT('{'))
			{
				test = ftell(fp);
				pFlNode_new = (LPFLIGHTDATANODE)malloc(sizeof(FLIGHTDATANODE));
				if (pFlNode_new == NULL)
				{
					ErrorReport(ERR_MEM_CREATE_FAIL + ERR_IN_DataFlightLinkedListFile);
					return FALSE;
				}
				if (pFlNode == NULL)
				{
					//首次
					pFlNode_head = pFlNode_new;
				}
				else
				{
					pFlNode->next = pFlNode_new;
				}
				iStart = ftell(fp) - FCOUNT;
				//READ TO szBuf
				while (szBuf[0] != TEXT('}'))
				{
					fread(szBuf, sizeof(TCHAR), 1, fp);
					test = ftell(fp);
				}
				iEnd = ftell(fp) - FCOUNT;
				fseek(fp, iStart, SEEK_SET);
				test = ftell(fp);
				iItemLen = (iEnd - iStart + FCOUNT) / FCOUNT;
				fread(szBuf, sizeof(TCHAR), iItemLen, fp);
				//文本到结构体
				DataFlightFormat(&(pFlNode_new->fl), szBuf, TEXT_TO_FL, &iItemLen);
				//重置szBuf
				ClearStrToZero(szBuf, iItemLen);

				//保存上一个地址
				pFlNode = pFlNode_new;
			}
		}
		if (pFlNode_new != NULL)
			pFlNode_new->next = NULL;
		fclose(fp);
		//存储到内存
		DataFlightLinkedListMem(&pFlNode_head, FLLIST_CHANGE_HEADNODE);
		
		break;
	}
	return TRUE;
}

/*
*   函数: DataGetValidFlightInfoStd(TCHAR szInfo[128])
*
*   目标: 获得合法的航班信息的提示信息，并存放至指定szBuf
*
*   输入：
*		szInfo	提示信息存放处
*	返回：
*		BOOL	判断是否正确执行
*
*/
BOOL DataGetValidFlightInfoStd(TCHAR szInfo[128])
{
	StringCchPrintf(szInfo, 128, 
		TEXT("合法的航班信息：                 \n1. 出发地目的地、航空公司及航班编号不含有空格，不超过15字符\n2. 时间、票数为整数\n3. 票价只保留1位小数，不超过5位整数\n4. 数据合理"));
	return TRUE;
}

/*
*   函数: DataValidCharacterFlightIDOrDepartOrDest(TCHAR* szInfo)
*
*   目标: 检查指定航班数据中的szFlightID,szDepart,szDest合法性
*   输入：
*		szBuf	指定的szBuf
*	返回：
*		BOOL	判断合法性TRUE or FALSE
*
*/
BOOL DataValidCharacterFlightIDOrDepartOrDest(const TCHAR* szBuf)
{
	int i;
	for (i = 0; i < 32 && szBuf[i] != TEXT('\0'); i++)
	{
		if (szBuf[i] == TEXT(' '))
		{
			return FALSE;
			break;
		}
	}
	return TRUE;
}


/*
*   函数: DataFlightFormat(LPFLIGHTDATA pFl, TCHAR* szBuf, DATA_FORMAT d_f, size_t *pLength)
*
*   目标: 将单个数据格式化，用于文件保存或读取，TEXT转换成FL结构体或是相反
*
*   输入：
*		pFl		指向FLIGHTDATA结构体的指针变量
*		szBuf	TEXT
*		d_f		转换方向
*			FL_TO_TEXT		256 存储TCHAR
*			TEXT_TO_FL
*		pLength	TEXT长度，字符个数(编码默认，不一定按CHAR计算)，TEXT传入FL时需指定该长度，FL传入TEXT时自动赋值返回
*	返回：
*		BOOL	判断程序运行状态
*	注释：
*		包含{[],[],...}
*/
BOOL DataFlightFormat(LPFLIGHTDATA pFl, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
{
	/*
	*					  1 2  3  4   1  2  3 4
		{[出发地][目的地][6,21,19,30][6,22,10,0][航班ID][飞机编号][价格][票数][其他][唯一ID]}
			1		2		3			4			5		6		7	  8		9		10
	*/
	UINT i = 0, j = 0 ,k = 0, l = 0;
	UINT iPosItem = 0, iPosInnerItem = 0;
	UINT iStart = 0, iEnd = 0;
	UINT iItemStart = 0, iItemEnd = 0;
	TCHAR szItem[128] = TEXT("\0");
	TCHAR szInnerItem[5] = TEXT("\0");
	switch (d_f)
	{
	case FL_TO_TEXT:
		StringCchPrintf(szBuf, LIMIT_FLIGHTDATA, 
			TEXT("{[%s][%s][%d,%d,%d,%d][%d,%d,%d,%d][%s][%s][%.1lf][%d][%s][%d]}"),
			pFl->szDepart, pFl->szDest,
			pFl->dtDepart.month, pFl->dtDepart.day, pFl->dtDepart.hr, pFl->dtDepart.min,
			pFl->dtDest.month, pFl->dtDest.day, pFl->dtDest.hr, pFl->dtDest.min,
			pFl->szFlightID, pFl->szPlaneID, pFl->dPrice, pFl->iAmount, pFl->szType, pFl->id
		);
		*pLength = lstrlen(szBuf);
		break;
	case TEXT_TO_FL:
		i = 0;
		iPosItem = 0;
		while (i < *pLength && szBuf[i] != TEXT('\0'))
		{
			if (szBuf[i] == TEXT('['))
			{
				ClearStrToZero(szItem, 128);
				ClearStrToZero(szInnerItem, 3);
				iPosItem++;
				iItemStart = i + 1;
			}
			if(szBuf[i] == TEXT(']'))
			{
				iItemEnd = i - 1;
				for (j = iItemStart, k = 0; j <= iItemEnd; j++, k++)
				{
					szItem[k] = szBuf[j];
				}
				szItem[k] = TEXT('\0');//只保存[]内
				switch (iPosItem)
				{
				case 1:
					StringCchPrintf(pFl->szDepart, LIMIT_SZDEPARTDEST, szItem);
					break;
				case 2:
					StringCchPrintf(pFl->szDest, LIMIT_SZDEPARTDEST, szItem);
					break;
				case 5:
					StringCchPrintf(pFl->szFlightID, LIMIT_SZFLIGHTID, szItem);
					break;
				case 6:
					StringCchPrintf(pFl->szPlaneID, LIMIT_SZFLIGHTID, szItem);
					break;
				case 7:
					pFl->dPrice = atof(szItem);
					break;
				case 8:
					pFl->iAmount = atoi(szItem);
					break;
				case 9:
					StringCchPrintf(pFl->szType, LIMIT_SZFLIGHTID, szItem);
					break;
				case 10:
					pFl->id = atoi(szItem);
					break;
				case 3:case 4://dtDepart,dtDest
					for (iPosInnerItem = 0, iStart = j = 0; ; j++)
					{
						//InnerItem分隔
						//旧结束位置，新的开始位置
						if (szItem[j] == TEXT(',') || szItem[j] == TEXT('\0'))
						{
							iPosInnerItem++;
							iEnd = j - 1;
							for (l = 0, k = iStart; k <= iEnd; k++, l++)
							{
								szInnerItem[l] = szItem[k];
							}
							szInnerItem[l] = TEXT('\0');
							switch (iPosItem)
							{
								//dtDepart
							case 3:
								switch (iPosInnerItem)
								{
								case 1:
									pFl->dtDepart.month = atoi(szInnerItem);
									break;
								case 2:
									pFl->dtDepart.day = atoi(szInnerItem);
									break;
								case 3:
									pFl->dtDepart.hr = atoi(szInnerItem);
									break;
								case 4:
									pFl->dtDepart.min = atoi(szInnerItem);
									break;
								}
								
								break;
								//dtDest
							case 4:
								switch (iPosInnerItem)
								{
								case 1:
									pFl->dtDest.month = atoi(szInnerItem);
									break;
								case 2:
									pFl->dtDest.day = atoi(szInnerItem);
									break;
								case 3:
									pFl->dtDest.hr = atoi(szInnerItem);
									break;
								case 4:
									pFl->dtDest.min = atoi(szInnerItem);
									break;
								}
								break;
							}
							//InnerItem分隔结束
						iStart = j + 1;
						}//if结束
						if (szItem[j] == TEXT('\0')) break;
					}
					break;//dtDepart,dtDest结束
					
				}
			}//if TEXT(']')结束
			i++;
		}//while结束
		break;
	}
	return TRUE;
}

//IDDATA数据为0表示没有
/*
*	函数：DataFlightSearch_ReturnIDDATA(FL_SEARCHDATA fl_sd, IDDATA pIdData[MAX_SEARCH_FLIGHT_ID])
*	
*	目标：从指定的查询数据中搜索航班数据并返回
*	
*	输入：
*		_In_  fl_sd		查询数据来源，包含数据和数据查询标志
*		_Out_ IDDATA	返回查询的航班ID
* 
*	返回：FL_SEARCHRET	搜索结果码
*					FL_NOTFOUND, FL_FOUND, FL_ERRFOUND
*/
FL_SEARCHRET DataFlightSearch_ReturnIDDATA(FL_SEARCHDATA fl_sd, IDDATA IdData[MAX_SEARCH_FLIGHT_ID])
{
	LPFLIGHTDATANODE pFlNode = NULL, pFlNode_head = NULL;
	//存放搜索结果ID
	IDDATA idSearchRet[MAX_SEARCH_FLIGHT_ID] = { 0 };
	//存放临时搜索结果ID
	IDDATA idSearchRetTemp1[MAX_SEARCH_FLIGHT_ID] = { 0 };
	IDDATA idSearchRetTemp2[MAX_SEARCH_FLIGHT_ID] = { 0 };
	IDDATA idSearchRetTemp3[MAX_SEARCH_FLIGHT_ID] = { 0 };
	IDDATA idSearchRetBuf[4][MAX_SEARCH_FLIGHT_ID] = { 0 };
	size_t iIdCount = 0;
	size_t i = 0;
	size_t iNumNeeded = 0;
	size_t iSearchNeeded[3] = {0};
	BOOL fFound = FALSE;
	BOOL fSearchNeeded[4] = { FALSE };
	TCHAR szCityTrgt_Depart[LIMIT_SZDEPARTDEST] = TEXT("\0");
	TCHAR szCityTrgt_Dest[LIMIT_SZDEPARTDEST] = TEXT("\0");
	TCHAR szCity_Depart[LIMIT_SZDEPARTDEST] = TEXT("\0");
	TCHAR szCity_Dest[LIMIT_SZDEPARTDEST] = TEXT("\0");
	TCHAR szType[LIMIT_FLIGHTOTHER] = TEXT("\0");
	//数据置空
	ClearIDDATAARRToZero(IdData);
	//提取fl_sd数据
	DataFlight_GetCityName(szCityTrgt_Depart, fl_sd.szDepart);
	DataFlight_GetCityName(szCityTrgt_Dest, fl_sd.szDest);
	//从内存中获取链表头指针
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);
	pFlNode = pFlNode_head;
	if (pFlNode == NULL)
	{
		return FL_ERRFOUND;
	}
	//查询DepartAndDest(必选),第一次查询，空格查询
	iIdCount = 0;
	while (pFlNode != NULL)
	{
		
		DataFlight_GetCityName(szCity_Depart, pFlNode->fl.szDepart);
		DataFlight_GetCityName(szCity_Dest, pFlNode->fl.szDest);

		if ((lstrcmp(szCityTrgt_Depart, szCity_Depart) == 0) &&
			(lstrcmp(szCityTrgt_Dest, szCity_Dest) == 0))
		{
			//保存ID
			idSearchRetBuf[0][iIdCount] = pFlNode->fl.id;
			iIdCount++;//记录数目
			fFound = TRUE;
		}
		pFlNode = pFlNode->next;
	}
	if (!fFound) return FL_NOTFOUND;

	//查询其他项，并与第一次查询数据合并
	if (((fl_sd.fl_searchoption) & 0x00000001) == FL_SEARCHOPTION_DEPARTTIME)
	{
		fSearchNeeded[1] = TRUE;
		pFlNode = pFlNode_head;
		iIdCount = 0;
		while (pFlNode != NULL)
		{
			if (pFlNode->fl.dtDepart.month == atoi(fl_sd.szDepartMonth)&&
				pFlNode->fl.dtDepart.day == atoi(fl_sd.szDepartDay))
			{
				//保存ID
				idSearchRetTemp1[iIdCount] = pFlNode->fl.id;
				iIdCount++;
				fFound = TRUE;
			}
			pFlNode = pFlNode->next;
		}
		//合并到buf
		IDDATAFoundCommon(idSearchRetBuf[1], idSearchRetTemp1, idSearchRetBuf[0]);
	}

	if (((fl_sd.fl_searchoption) & 0x00000010) == FL_SEARCHOPTION_DESTTIME)
	{
		fSearchNeeded[2] = TRUE;
		pFlNode = pFlNode_head;
		iIdCount = 0;
		while (pFlNode != NULL)
		{
			if (pFlNode->fl.dtDest.month == atoi(fl_sd.szDestMonth) &&
				pFlNode->fl.dtDest.day == atoi(fl_sd.szDestDay))
			{
				//保存ID
				idSearchRetTemp2[iIdCount] = pFlNode->fl.id;
				iIdCount++;
				fFound = TRUE;
			}
			pFlNode = pFlNode->next;
		}
		IDDATAFoundCommon(idSearchRetBuf[2], idSearchRetTemp2, idSearchRetBuf[0]);
	}

	if (((fl_sd.fl_searchoption) & 0x00000100) == FL_SEARCHOPTION_TYPE)
	{
		fSearchNeeded[3] = TRUE;
		pFlNode = pFlNode_head;
		iIdCount = 0;
		while (pFlNode != NULL)
		{
			if (lstrcmp(fl_sd.szType, pFlNode->fl.szType) == 0)
			{
				//保存ID
				idSearchRetTemp3[iIdCount] = pFlNode->fl.id;
				iIdCount++;
				fFound = TRUE;
			}
			pFlNode = pFlNode->next;
		}
		IDDATAFoundCommon(idSearchRetBuf[3], idSearchRetTemp3, idSearchRetBuf[0]);
	}
	if (!fFound) return FL_NOTFOUND;
	
	//若查找到，搜索结果合并
	for (i = 0,iNumNeeded = 0 ; i < 4 ; i++)
	{
		if (fSearchNeeded[i] == TRUE)
		{
			iNumNeeded++;				//记录需要合并的数组数量
			iSearchNeeded[iNumNeeded-1] = i;//记录需要合并的数组下标
		}
	}
	//只搜索Depart Dest或包含其他任意一项(已经合并)
	if (iNumNeeded == 0 || iNumNeeded == 1)
	{
		for (i = 0; i < MAX_SEARCH_FLIGHT_ID; i++)
		{
			if (idSearchRetBuf[iSearchNeeded[0]][i] == 0) break;
			idSearchRet[i] = idSearchRetBuf[iSearchNeeded[0]][i];
		}
	}
	//包含其他任意两项
	else if (iNumNeeded == 2)
	{
		IDDATAFoundCommon(idSearchRet, idSearchRetBuf[iSearchNeeded[0]], idSearchRetBuf[iSearchNeeded[1]]);
	}
	//全部包含
	else
	{
		IDDATAFoundCommon(idSearchRetBuf[0], idSearchRetBuf[1], idSearchRetBuf[2]);
		IDDATAFoundCommon(idSearchRet, idSearchRetBuf[0], idSearchRetBuf[3]);
	}

	for (i = 0; i < MAX_SEARCH_FLIGHT_ID; i++)
	{
		if (idSearchRet[i] == 0) break;
		IdData[i] = idSearchRet[i];
	}
	return FL_FOUND;
}

/*
*	函数：DataFlight_GetPNode(IDDATA id)
*
*	目标：返回指定的链表节点ID对对应的指针
*
*	输入：
*		id					节点id
*
*	返回：LPFLIGHTDATANODE	节点指针
*/
LPFLIGHTDATANODE DataFlight_GetPNode(IDDATA id)
{
	LPFLIGHTDATANODE pFlNode_head = NULL;
	LPFLIGHTDATANODE pFlNode = NULL;
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);
	pFlNode = pFlNode_head;
	while (pFlNode != NULL)
	{
		if (pFlNode->fl.id == id)
		{
			break;
		}
		pFlNode = pFlNode->next;
	}
	return pFlNode;
}

/*
*	函数：DataFlight_GetCityName(TCHAR* szDst, const TCHAR* szSrc)
*
*	目标：从源字符串截取城市名字
*
*	输入：
*		szDst		目标字符串
*		szSrc		源字符串
*
*	返回：BOOL		判断运行状态
*/
BOOL DataFlight_GetCityName(TCHAR* szDst, const TCHAR* szSrc)
{
	int i = 0;
	while (*szSrc != TEXT(' ') && *szSrc != TEXT('\0') && i < LIMIT_SZDEPARTDEST - 1)
	{
		*szDst++ = *szSrc++;
		i++;
	}
	*szDst = TEXT('\0');
	return TRUE;
}

/*
*	函数：DataFlight_GetCityName(TCHAR* szDst, const TCHAR* szSrc)
*
*	目标：从源字符串(szDepart/szDest)截取机场名字
*
*	输入：
*		szDst		目标字符串
*		szSrc		源字符串
*
*	返回：BOOL		判断运行状态
*/

BOOL DataFlight_GetAirportName(TCHAR* szDst, const TCHAR* szSrc)
{
	int i = 0;
	while (*szSrc != TEXT(' ') && i < LIMIT_SZDEPARTDEST)
	{
		szSrc++;
		i++;
	}
	szSrc++;
	while (*szSrc != TEXT('\0') && i < LIMIT_SZDEPARTDEST)
	{
		*szDst++ = *szSrc++;
		i++;
	}
	*szDst = TEXT('\0');
	return TRUE;
}
/*
*	函数：DataFlightIDDATA_Store(IDDATA iddataSrc, size_t iLen)
*
*	目标：临时存储航班IDDATA数组，以便接下来的数据操作（如显示）
*
*	输入：
*			_In_	IDDATA数组
*			size_t	数组长度，不得大于MAX_SEARCH_FLIGHT_ID
*
*	返回：BOOL		判断运行状态
*/
BOOL DataFlightIDDATA_Store(IDDATA iddataSrc[], size_t iLen)
{
	static IDDATA iddata[MAX_SEARCH_FLIGHT_ID] = {0};
	size_t i;
	for (i = 0; i < iLen; i++)
	{
		iddata[i] = iddataSrc[i];
	}
	return TRUE;
}

/*
*	函数：DataValidDateText(const TCHAR* szMonth, const TCHAR* szDay)
*
*	目标：判断时间文本是否合法
*
*	输入：
*			时间文本
*
*	返回：BOOL		TRUE合法，FALSE不合法
*/
BOOL DataValidDateText(const TCHAR* szMonth, const TCHAR* szDay)
{
	int monthDayMax[2][13] = { 
		{ 0,31,28,31,30,31,30,31,31,30,31,30,31 }, 
		{ 0,31,29,31,30,31,30,31,31,30,31,30,31 }
	};
	int iMonth = atoi(szMonth),
		iDay = atoi(szDay);
	time_t t;
	struct tm s_tm;
	int year = 0;
	if (*szMonth == TEXT('\0') || *szDay == TEXT('\0')) return FALSE;
	time(&t);
	localtime_s(&s_tm, &t);
	year = s_tm.tm_year + 1900;
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) year = 1;
	else year = 0;
	if (iMonth > 12 || iMonth <= 0) return FALSE;
	else
	{
		if (iDay > monthDayMax[year][iMonth] || iDay <= 0) return FALSE;
	}
	return TRUE;
}

/*
*	函数：DataFlight_CreateNULLFlight()
*
*	目标：创建新的航班节点（创建的节点数据为空，待填充）
*
*	输入：无
*
*	返回：LPFLIGHTDATANODE		新节点的指针
*/

LPFLIGHTDATANODE DataFlight_CreateNULLFlight()
{
	LPFLIGHTDATANODE pFlNode_head = NULL;
	LPFLIGHTDATANODE pFlNode_new = NULL;
	IDDATA idNew = 0;
	//从内存中获取头指针
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);

	//创建
	pFlNode_new = (LPFLIGHTDATANODE)malloc(sizeof(FLIGHTDATANODE));
	if (pFlNode_new == NULL)
	{
		ErrorReport(ERR_IN_DataFlight_CreateNULLFlight | ERR_MEM_CREATE_FAIL);
		return NULL;
	}
	//没有可供分配的航班ID，即达到上限
	if ((idNew = DataFlight_GetAvailableID()) == 0)
	{
		return NULL;
	}
	//初始化
	pFlNode_new->fl.id = idNew;
	pFlNode_new->fl.dPrice = 0;
	pFlNode_new->fl.dtDepart.month = 0;
	pFlNode_new->fl.dtDepart.day = 0;
	pFlNode_new->fl.dtDepart.hr = 0;
	pFlNode_new->fl.dtDepart.min = 0;
	pFlNode_new->fl.dtDest.month = 0;
	pFlNode_new->fl.dtDest.day = 0;
	pFlNode_new->fl.dtDest.hr = 0;
	pFlNode_new->fl.dtDest.min = 0;
	pFlNode_new->fl.iAmount = 0;
	lstrcpy(pFlNode_new->fl.szDepart, TEXT(" "));
	lstrcpy(pFlNode_new->fl.szDest, TEXT(" "));
	lstrcpy(pFlNode_new->fl.szFlightID, TEXT(" "));
	lstrcpy(pFlNode_new->fl.szPlaneID, TEXT("\0"));
	lstrcpy(pFlNode_new->fl.szType, TEXT("\0"));
	pFlNode_new->next = NULL;
	while (pFlNode_head->next != NULL)
	{
		pFlNode_head = pFlNode_head->next;
	}
	//连接到链表
	pFlNode_head->next = pFlNode_new;
	return pFlNode_new;
}

/*
*	函数：DataFlight_GetAvailableID()
*
*	目标：获取可用的ID以便分配航班节点,无则返回0
*
*	输入：
*			无
*
*	返回：IDDATA	可用的ID，无则为0
*/
IDDATA DataFlight_GetAvailableID()
{
	LPFLIGHTDATANODE pFlNode_head = NULL, pFlNode = NULL;
	IDDATA iddata[MAX_SEARCH_FLIGHT_ID] = { 0 };
	IDDATA_NUM i = 0;
	IDDATA_NUM ret = 0;
	IDDATA_NUM sameNum = 0;
	//从内存中获取头指针
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);
	pFlNode = pFlNode_head;
	while (pFlNode != NULL)
	{
		//读取所有的已分配ID
		iddata[i] = pFlNode->fl.id;
		pFlNode = pFlNode->next;
		i++;
	}
	//穷举是否有该ID
	for (ret = IDDATA_BEGIN; ret <= IDDATA_END; ret++)
	{
		for (i = 0, sameNum = 0; i < MAX_SEARCH_FLIGHT_ID && iddata[i] != 0; i++)
		{
			//相同的数量
			if (ret == iddata[i])
			{
				sameNum++;
			}
		}
		//没有相同的，跳出循环
		if (sameNum == 0) break;
	}
	
	if (sameNum != 0) ret = 0;//始终有相同的
	return ret;
}
/*
	订单数据
	{[USERNAME][ID][ORDERNUM]}
*/

/*
*   函数: DataOrder(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
*
*   目标: 在内存中存取当前用户的订单数据（链表），使用前请记得释放不必要的堆内存
*
*   输入：
*		pOINode_Head		指向链表的头指针的指针，首节点
*		oil_o				说明指向的链表的操作方式
*			OILIST_GET_HEADNODE		从内存中获取链表，头指针保存在pOI_HeadNode
*			OILIST_CHANGE_HEADNODE	将pOI_HeadNode指向的链表覆盖保存，同时删除内存中以前链表
*	返回：
*		size_t		返回链表节点数即订单总数
*/
IDDATA_NUM DataUserOrder(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
{
	static LPORDERINFONODE pOINode_Mem = NULL;
	IDDATA_NUM iListNum = 0;//保存链表数
	LPORDERINFONODE pOINode = NULL;
	LPORDERINFONODE pOINode_next = NULL;
	switch (oil_o)
	{
		//读取链表
	case OILIST_GET_HEADNODE:
		pOINode = *ppOINode_Head = pOINode_Mem;
		while (pOINode != NULL)
		{
			iListNum++;
			pOINode = pOINode->next;
		}
		break;
		//改变链表
	case OILIST_CHANGE_HEADNODE:
		pOINode = pOINode_Mem;
		//保存传进来的链表
		pOINode = pOINode_Mem = *ppOINode_Head;
		//记录数目
		while (pOINode != NULL)
		{
			iListNum++;
			pOINode = pOINode->next;
		}
		break;
	}

	return iListNum;
}

/*
*
*	函数：DataOrder_CreateSingleOrder(IDDATA idData)
*	
*	目标：为当前的用户创建新的用户订单，并保存至所有订单链表和该用户订单链表
* 
*	输入：
*		idData			航班节点ID
* 
*	返回：
*		BOOL			成功创建TRUE
* 
*/
BOOL DataOrder_CreateSingleOrder(IDDATA idData)
{
	USERACCOUNT ua;
	ORDERINFO oi_new;//当前添加的
	LPORDERINFONODE pOINode_New;
	time_t timeNow;
	struct tm tm_tm;
	int year, month, day, hr, min, sec;
	LPORDERINFONODE pOINode_All;		//所有订单数据
	LPORDERINFONODE pOINode_UserAll;	//当前用户的所有订单数据
	LPORDERINFONODE pOINode;			//临时指针

	LPFLIGHTDATANODE pFlNode;			//操作的航班
	//获取当前用户数据
	DataUserAccount(&ua, UA_GET);

	//航班数据操作
	pFlNode = DataFlight_GetPNode(idData);
	pFlNode->fl.iAmount--;                   //该节点的票数立即减少
	//订单数据操作
	oi_new.fldata = pFlNode->fl;
	time(&timeNow);
	localtime_s(&tm_tm, &timeNow);
	year = tm_tm.tm_year + 1900;
	month = tm_tm.tm_mon + 1;
	day = tm_tm.tm_mday;
	hr = tm_tm.tm_hour;
	min = tm_tm.tm_min;
	sec = tm_tm.tm_sec;
	lstrcpy(oi_new.szUsername, ua.szUsername);
	StringCchPrintf(oi_new.szOrderNumber, 20, TEXT("%04d%02d%02d%02d%02d%02d%05d"), year, month, day, hr, min, sec, idData);
	/*注：文件保存时，仅进行所有订单数据链表的保存即可*/

	//获取所有订单头节点
	DataOrderLinkedListMem(&pOINode_All, OILIST_GET_HEADNODE);

	//如果内存没有链表
	if (pOINode_All == NULL)
	{
		//创建新的节点
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;//置空
		pOINode_All = pOINode_New;
		DataOrderLinkedListMem(&pOINode_All, OILIST_CHANGE_HEADNODE);
	}
	else
	{
		//内存存在链表，跳转到尾节点
		pOINode = pOINode_All;
		while (pOINode->next != NULL)
		{
			pOINode = pOINode->next;
		}
		//创建新的节点
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;
		//节点链接到所有数据
		pOINode->next = pOINode_New;
	}


	//获取所有该用户订单头节点
	DataUserOrder(&pOINode_UserAll, OILIST_GET_HEADNODE);
	if (pOINode_UserAll == NULL)
	{
		//创建新的节点
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;
		pOINode_UserAll = pOINode_New;
		DataUserOrder(&pOINode_UserAll, OILIST_CHANGE_HEADNODE);
	}
	else
	{
		pOINode = pOINode_UserAll;
		while (pOINode->next != NULL)
		{
			pOINode = pOINode->next;
		}
		//创建新的节点
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;
		//节点链接到用户所有订单数据
		pOINode->next = pOINode_New;
	}


	return TRUE;
}

/*
*   函数: DataOrder_GetUserOrder()
*
*   目标: 获取用户的所有订单数据，保存到用户链表DataUserOrder，用于初始化
*
*   输入：
*		pOINode_Head		指向链表的头指针的指针，首节点
*		oil_o				说明指向的链表的操作方式
*			OILIST_GET_HEADNODE		从内存中获取链表，头指针保存在pOI_HeadNode
*			OILIST_CHANGE_HEADNODE	将pOI_HeadNode指向的链表覆盖保存，同时删除内存中以前链表
*	返回：
*		BOOL		判断是否执行成功
*/
BOOL DataOrder_GetUserOrderToMem()
{
	LPORDERINFONODE pOINode_User = NULL, pOINode_User_Old = NULL;
	LPORDERINFONODE pOINode_User_Head = NULL;
	LPORDERINFONODE pOINode_All = NULL;
	USERACCOUNT ua;
	//获取用户数据,主要获取用户名
	DataUserAccount(&ua, UA_GET);
	//从所有订单中获取该用户订单
	//获取所有订单
	DataOrderLinkedListMem(&pOINode_All, OILIST_GET_HEADNODE);
	if (pOINode_All == NULL)
	{
		//不存在，默认用户数据为NULL
		return FALSE;
	}
	else
	{
		while (pOINode_All != NULL)
		{
			//查到与当前用户名相同
			if (lstrcmp(ua.szUsername, pOINode_All->oi.szUsername) == 0)
			{
				//堆内存开辟
				pOINode_User = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
				//开辟失败
				if (pOINode_User == NULL)
				{
					ErrorReport(ERR_IN_DataOrder_GetUserOrderToMem | ERR_MEM_CREATE_FAIL);
					return FALSE;
				}
				//开辟成功

				//第一次查到
				if (pOINode_User_Head == NULL)
				{
					pOINode_User_Head = pOINode_User_Old = pOINode_User;
				}
				else
				{	

					pOINode_User_Old->next = pOINode_User;
					pOINode_User->next = NULL;
					//保存该节点
					pOINode_User_Old = pOINode_User;
				}
				pOINode_User->oi = pOINode_All->oi;
			}
			//进入下一个节点（所有订单）
			pOINode_All = pOINode_All->next;
		}
		if (pOINode_User != NULL)
		{
			pOINode_User->next = NULL;
		}

		DataUserOrder(&pOINode_User_Head, OILIST_CHANGE_HEADNODE);
	}
	return TRUE;
}
/*
*   函数: DataOrderLinkedListMem(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
*
*   目标: 在内存中保存和读取所有订单动态链表数据，调用此函数进行当前链表数据的获取和修改，使用前请对尾节点next置空
*
*   输入：
*		pOINode_Head		指向链表的头指针的指针，首节点
*		oil_o				说明指向的链表的操作方式
*			OILIST_GET_HEADNODE		从内存中获取链表，头指针保存在pOI_HeadNode
			OILIST_CHANGE_HEADNODE	将pOI_HeadNode指向的链表覆盖保存，同时删除内存中以前链表
*	返回：
*		IDDATA_NUM		返回链表节点数即订单总数
*/
IDDATA_NUM DataOrderLinkedListMem(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
{
	static LPORDERINFONODE pOINode_Mem = NULL;
	IDDATA_NUM iListNum = 0;//保存链表数
	LPORDERINFONODE pOINode = NULL;
	LPORDERINFONODE pOINode_next = NULL;
	switch (oil_o)
	{
		//读取链表
	case OILIST_GET_HEADNODE:
		pOINode = *ppOINode_Head = pOINode_Mem;
		while (pOINode != NULL)
		{
			iListNum++;
			pOINode = pOINode->next;
		}
		break;
		//改变链表
	case OILIST_CHANGE_HEADNODE:
		pOINode = pOINode_Mem;

		//保存传进来的链表
		pOINode = pOINode_Mem = *ppOINode_Head;
		//记录数目
		while (pOINode != NULL)
		{
			iListNum++;
			pOINode = pOINode->next;
		}
		break;
	}

	return iListNum;
}

/*
*   函数: DataOrderLinkedListFile(OI_FILE oi_file)
*
*   目标: 将所有订单数据从内存读取到文件或从文件中读取到内存，内存数据由DataOrderLinkedListMem管理
*
*   输入：
*		fl_file		指定操作方式
*			OILIST_TO_FILE			从内存覆盖写入磁盘文件
*			OILIST_FROM_FILE		从磁盘文件中全部读取到内存
*	返回：
*		BOOL		判断程序运行状态
*/
BOOL DataOrderLinkedListFile(OI_FILE oi_file)
{
	TCHAR szBuf[LIMIT_ORDERDATA] = TEXT("\0");
	size_t uLen;
	FILE* fp = NULL;
	errno_t err;
	long iStart, iEnd;
	LPORDERINFONODE pOINode_head = NULL;
	LPORDERINFONODE pOINode = NULL, pOINode_new = NULL;
	size_t iItemLen;
	long test = 0L;
	switch (oi_file)
	{
	case OILIST_TO_FILE:
		DataOrderLinkedListMem(&pOINode, FLLIST_GET_HEADNODE);
		if ((err = fopen_s(&fp, URL_ORDER, TEXT("wb+"))) != 0)
		{
			return FALSE;
		}
		if (fp == NULL) return FALSE;
		while (pOINode != NULL)
		{
			DataOrderFormat(&pOINode->oi, szBuf, OI_TO_TEXT, &uLen);
			fwrite(szBuf, sizeof(TCHAR), lstrlen(szBuf), fp);
			pOINode = pOINode->next;
		}
		fclose(fp);
		break;
	case OILSIT_FROM_FILE:
		if ((err = fopen_s(&fp, URL_ORDER, TEXT("rb"))) != 0)
		{
			//不存在文件
			if ((err = fopen_s(&fp, URL_ORDER, TEXT("wb+"))) != 0)
			{
				ErrorReport(ERR_FPOPEN_FAIL + ERR_IN_DataOrderLinkedListFile);
				return FALSE;
			}
			
		}

		if (fp == NULL) return FALSE;

		while (!feof(fp))
		{
			fread(szBuf, sizeof(TCHAR), 1, fp);
			if (feof(fp)) break;

			if (szBuf[0] == TEXT('{'))
			{
				test = ftell(fp);
				pOINode_new = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
				if (pOINode_new == NULL)
				{
					ErrorReport(ERR_MEM_CREATE_FAIL + ERR_IN_DataOrderLinkedListFile);
					return FALSE;
				}
				if (pOINode == NULL)
				{
					//首次
					pOINode_head = pOINode_new;
				}
				else
				{
					pOINode->next = pOINode_new;
				}
				iStart = ftell(fp) - FCOUNT;
				//READ TO szBuf
				while (szBuf[0] != TEXT('}'))
				{
					fread(szBuf, sizeof(TCHAR), 1, fp);
					test = ftell(fp);
				}
				iEnd = ftell(fp) - FCOUNT;
				fseek(fp, iStart, SEEK_SET);
				test = ftell(fp);
				iItemLen = (iEnd - iStart + FCOUNT) / FCOUNT;
				fread(szBuf, sizeof(TCHAR), iItemLen, fp);
				//文本到结构体
				DataOrderFormat(&(pOINode_new->oi), szBuf, TEXT_TO_OI, &iItemLen);
				//重置szBuf
				ClearStrToZero(szBuf, iItemLen);

				//保存上一个地址
				pOINode = pOINode_new;
			}
		}
		if (pOINode_new != NULL)
			pOINode_new->next = NULL;
		fclose(fp);
		//存储到内存
		DataOrderLinkedListMem(&pOINode_head, OILIST_CHANGE_HEADNODE);

		break;
	}
	return TRUE;
}

/*
*   函数: DataOrderFormat(LPORDERINFO pOI, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
*
*   目标: 将单个订单数据格式化，用于文件保存或读取，TEXT转换成OI结构体或是相反
*
*   输入：
*		pOI		指向ORDERINFO结构体的指针变量
*		szBuf	TEXT
*		d_f		转换方向
*			OI_TO_TEXT		256 存储TCHAR
*			TEXT_TO_OI
*		pLength	TEXT长度，字符个数(编码默认，不一定按CHAR计算)，TEXT传入OI时需指定该长度，OI传入TEXT时自动赋值返回
*	返回：
*		BOOL	判断程序运行状态
*	注释：
*		包含{[],[],...}
*/
BOOL DataOrderFormat(LPORDERINFO pOI, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
{
	/*
	*					 [username20][航班信息][订单号20]
	*/
	UINT i = 0, j = 0, k = 0, l = 0;
	UINT iPosItem = 0;
	UINT iStart = 0, iEnd = 0, iPosInnerItem;
	UINT iItemStart = 0, iItemEnd = 0;
	TCHAR szItem[LIMIT_ORDERDATA] = TEXT("\0");
	TCHAR szInnerItem[32] = TEXT("\0");
	

	switch (d_f)
	{
	case OI_TO_TEXT:
		StringCchPrintf(szItem, LIMIT_FLIGHTDATA,
			TEXT("[%s][%s][%d,%d,%d,%d][%d,%d,%d,%d][%s][%s][%.1lf][%d][%s][%d]"),
			pOI->fldata.szDepart, pOI->fldata.szDest,
			pOI->fldata.dtDepart.month, pOI->fldata.dtDepart.day, pOI->fldata.dtDepart.hr, pOI->fldata.dtDepart.min,
			pOI->fldata.dtDest.month, pOI->fldata.dtDest.day, pOI->fldata.dtDest.hr, pOI->fldata.dtDest.min,
			pOI->fldata.szFlightID, pOI->fldata.szPlaneID, pOI->fldata.dPrice, pOI->fldata.iAmount, pOI->fldata.szType, pOI->fldata.id
		);
		StringCchPrintf(szBuf, LIMIT_ORDERDATA,
			TEXT("{[%s]%s[%s]}"),
			pOI->szUsername,szItem,pOI->szOrderNumber
		);
		*pLength = lstrlen(szBuf);
		break;
	case TEXT_TO_OI:
		i = 0;
		iPosItem = 0;
		while (i < *pLength && szBuf[i] != TEXT('\0'))
		{
			if (szBuf[i] == TEXT('['))
			{
				ClearStrToZero(szItem, 128);
				iPosItem++;
				iItemStart = i + 1;
			}
			if (szBuf[i] == TEXT(']'))
			{
				iItemEnd = i - 1;
				for (j = iItemStart, k = 0; j <= iItemEnd; j++, k++)
				{
					szItem[k] = szBuf[j];
				}
				szItem[k] = TEXT('\0');//只保存[]内
				switch (iPosItem)
				{
				case 1:
					StringCchPrintf(pOI->szUsername, 20, szItem);
					break;
				case 2:
					StringCchPrintf(pOI->fldata.szDepart, LIMIT_SZDEPARTDEST, szItem);
					break;
				case 3:
					StringCchPrintf(pOI->fldata.szDest, LIMIT_SZDEPARTDEST, szItem);
					break;
				case 6:
					StringCchPrintf(pOI->fldata.szFlightID, LIMIT_SZFLIGHTID, szItem);
					break;
				case 7:
					StringCchPrintf(pOI->fldata.szPlaneID, LIMIT_SZFLIGHTID, szItem);
					break;
				case 8:
					pOI->fldata.dPrice = atof(szItem);
					break;
				case 9:
					pOI->fldata.iAmount = atoi(szItem);
					break;
				case 10:
					StringCchPrintf(pOI->fldata.szType, LIMIT_SZFLIGHTID, szItem);
					break;
				case 11:
					pOI->fldata.id = atoi(szItem);
					break;
				case 4:case 5://dtDepart,dtDest
					for (iPosInnerItem = 0, iStart = j = 0; ; j++)
					{
						//InnerItem分隔
						//旧结束位置，新的开始位置
						if (szItem[j] == TEXT(',') || szItem[j] == TEXT('\0'))
						{
							iPosInnerItem++;
							iEnd = j - 1;
							for (l = 0, k = iStart; k <= iEnd; k++, l++)
							{
								szInnerItem[l] = szItem[k];
							}
							szInnerItem[l] = TEXT('\0');
							switch (iPosItem)
							{
								//dtDepart
							case 4:
								switch (iPosInnerItem)
								{
								case 1:
									pOI->fldata.dtDepart.month = atoi(szInnerItem);
									break;
								case 2:
									pOI->fldata.dtDepart.day = atoi(szInnerItem);
									break;
								case 3:
									pOI->fldata.dtDepart.hr = atoi(szInnerItem);
									break;
								case 4:
									pOI->fldata.dtDepart.min = atoi(szInnerItem);
									break;
								}
								
								break;
								//dtDest
							case 5:
								switch (iPosInnerItem)
								{
								case 1:
									pOI->fldata.dtDest.month = atoi(szInnerItem);
									break;
								case 2:
									pOI->fldata.dtDest.day = atoi(szInnerItem);
									break;
								case 3:
									pOI->fldata.dtDest.hr = atoi(szInnerItem);
									break;
								case 4:
									pOI->fldata.dtDest.min = atoi(szInnerItem);
									break;
								}
								break;
							}
							//InnerItem分隔结束
						iStart = j + 1;
						}//if结束
						if (szItem[j] == TEXT('\0')) break;
					}
					break;//dtDepart,dtDest结束

				case 12:
					StringCchPrintf(pOI->szOrderNumber, 20, szItem);
					break;
				}
			}//if TEXT(']')结束
			i++;
		}//while结束
		break;
	}
	return TRUE;
}


/*
*   函数: DataFlightSearchRet(IDDATA iddata[MAX_SEARCH_FLIGHT_ID], FL_SEARCHRET_MEM fl_sr_m)
*
*   目标: 存储或读取航班(搜索结果)数据
*
*   输入：
*		iddata				指定的航班ID数据数组
*		FL_SEARCHRET_MEM	指定读取或是存储
*		
*	返回：
*		IDDATA_NUM	相关航班的数量
*	注释：
*/
IDDATA_NUM DataFlightSearchRet(IDDATA iddata[MAX_SEARCH_FLIGHT_ID], FL_SEARCHRET_MEM fl_sr_m)
{
	static IDDATA iddataStore[MAX_SEARCH_FLIGHT_ID] = { 0 };
	int i;
	switch (fl_sr_m)
	{
	case FL_SR_GET:
	case FL_SR_SET:
		for (i = 0; i < MAX_SEARCH_FLIGHT_ID; i++)
		{
			if (fl_sr_m == FL_SR_GET)
			{
				if ((iddata[i] = iddataStore[i]) == 0) break;
			}
			else if(fl_sr_m == FL_SR_SET)
			{
				if (i == 0) ClearIDDATAARRToZero(iddataStore);
				if ((iddataStore[i] = iddata[i]) == 0) break;
				
			}
		}
		return i;
		break;
	default:
		return -1;
	}

}

/*
*	函数：DataFreeMemory()
*
*	目标：释放程序创建的所有堆内存空间
*
*	输入：无
*
*	返回：BOOL		TRUE正确执行
*/
BOOL DataFreeMemory()
{
	LPFLIGHTDATANODE pFlNode = NULL, pFlNode_next = NULL;
	LPUSERACCOUNTNODE pUaN = NULL, pUaN_next = NULL;
	LPORDERINFONODE pOINode = NULL, pOINode_next = NULL;
	DataFlightLinkedListMem(&pFlNode, FLLIST_GET_HEADNODE);
	//清除航班内存空间
	if (pFlNode != NULL)
	{
		while (pFlNode != NULL)
		{
			pFlNode_next = pFlNode->next;
			free(pFlNode);
			pFlNode = pFlNode_next;
		}
	}
	//用户链表空间
	DataAccount_LinkedListMem(&pUaN, UALIST_GET);
	if (pUaN != NULL)
	{
		while (pUaN != NULL)
		{
			pUaN_next = pUaN->next;
			free(pUaN);
			pUaN = pUaN_next;
		}
	}
	DataAccount_LinkedListMem(&pUaN, UALIST_GET_ADMIN);
	if (pUaN != NULL)
	{
		while (pUaN != NULL)
		{
			pUaN_next = pUaN->next;
			free(pUaN);
			pUaN = pUaN_next;
		}
	}
	DataUserOrder(&pOINode, OILIST_GET_HEADNODE);
	if (pOINode != NULL)
	{
		while (pOINode != NULL)
		{
			pOINode_next = pOINode->next;
			free(pOINode);
			pOINode = pOINode_next;
		}
	}
	return TRUE;
}

/*
	基础算法
*/
/*
*	函数：void ClearStrToZero(TCHAR* szText, size_t iLen)
*
*	目标：将指定字符串的前iLen个字符进行\0处理
*
*	输入：szText	字符串
*			iLen	长度
*
*	返回：无
*/
void ClearStrToZero(TCHAR* szText, size_t iLen)
{
	size_t i = 0;
	while (i < iLen)
	{
		*szText = TEXT('\0');
		szText++;
		i++;
	}
}
/*
*	函数：ClearUAStructToZero(LPUSERACCOUNT pUa)
*
*	目标：将指定的USERACCOUNT数据结构变量进行置空处理
*
*	输入：pUa		指向USERACCOUNT类型的结构体指针变量
*
*	返回：无
*/
void ClearUAStructToZero(LPUSERACCOUNT pUa)
{
	ClearStrToZero(pUa->szPassword, 20);
	ClearStrToZero(pUa->szUsername, 20);
	ClearStrToZero(pUa->szSelfInfo, 128);
	pUa->ut = UT_USER;

}
/*
*	函数：void ClearIDDATAARRToZero(IDDATA iddata[MAX_SEARCH_FLIGHT_ID])
*
*	目标：将指定的IDDATA类型的数组\0置空
*
*	输入：iddata		待处理的IDDATA类型的数组
*
*	返回：无
*/
void ClearIDDATAARRToZero(IDDATA iddata[MAX_SEARCH_FLIGHT_ID])
{
	size_t i;
	for (i = 0; i < MAX_SEARCH_FLIGHT_ID; i++)
	{
		iddata[i] = 0;
	}
}
/*
*	函数：void ClearPointerArrToZero(void* pArr[], size_t num)
*
*	目标：将指定指针数组的前num个元素进行置空NULL
*
*	输入：	pArr	指定的指针数组
*			iLen		长度
*
*	返回：无
*/
void ClearPointerArrToZero(void* pArr[], size_t iLen)
{
	size_t i;
	for (i = 0; i < iLen; i++)
	{
		pArr[i] = NULL;
	}
}
/*
*	函数：void IDDATAFoundCommon(IDDATA dest[], const IDDATA src1[], const IDDATA src2[])
*
*	目标：查找指定的两个IDDATA类型的数组src1,src2的共同元素，并将共同元素保存在dest
*
*	输入：
*			_In_  src1,src2		需要查找的IDDATA类型数组
*			_Out_ dest			共同元素的IDDATA类型数组
*
*	返回：无
*/
void IDDATAFoundCommon(IDDATA dest[], const IDDATA src1[], const IDDATA src2[])
{
	int i, j, k;
	for (i = 0, k = 0; i < MAX_SEARCH_FLIGHT_ID - 1; i++)
	{
		if (src1[i] == 0) break;
		for (j = 0; j < MAX_SEARCH_FLIGHT_ID - 1; j++)
		{
			if (src2[j] == 0) break;
			if (src1[i] == src2[j])
			{
				dest[k] = src1[i];
				k++;
			}
		}
	}
	dest[k] = 0;//置0
}
