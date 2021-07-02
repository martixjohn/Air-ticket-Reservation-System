/*
	data.c
	���ݴ���
*/
#include "header.h"
extern HWND hwndLogIn;
/*
	��������ǰ��ʹ����Ч��麯����������Ч�ԺϷ���
*/
/*-----------USER ACCOUNT���-------------*/
/*
*   ����: DataUAValid(USERACCOUNT ua)
*
*   Ŀ��: ���ָ���û��˻��������û���������Ϸ���
*
*   ���룺
*		ua		�û��˻�
*	���أ�
*		BOOL	�жϺϷ���TRUE or FALSE
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
*   ����: DataGetValidUA_UOPStd(TCHAR szInfo[128])
*
*   Ŀ��: ��úϷ����û��˺�������ʾ��Ϣ���������ָ��szBuf
*
*   ���룺
*		szInfo	��ʾ��Ϣ��Ŵ�
*	���أ�
*		BOOL	�ж��Ƿ���ȷִ��
*
*/
BOOL DataGetValidUA_UOPStd(TCHAR szInfo[128])
{
	StringCchPrintf(szInfo, 128, 
		TEXT("�Ϸ����˺����룺                      \n������ĸ��������ɣ�����%d��%dλ����\n"), 5, LIMIT_USERNAME);
	return TRUE;
}

/*
*   ����: DataGetValidUA_SelfInfoStd(TCHAR szInfo[128])
*
*   Ŀ��: ��úϷ����û�������Ϣ����ʾ��Ϣ���������ָ��szBuf
*
*   ���룺
*		szInfo	��ʾ��Ϣ��Ŵ�
*	���أ�
*		BOOL	�ж��Ƿ���������
*
*/
BOOL DataGetValidUA_SelfInfoStd(TCHAR szInfo[128])
{
	StringCchPrintf(szInfo, 128, 
		TEXT("�Ϸ��ĸ�����Ϣ��                      \n��Χ��18�ַ�����\n���ú���: \",\"\"\\\"\n���֤��18λ���ֹ���\n"));
	return TRUE;
}

/*
*   ����: DataValidUNorPW(TCHAR* szInfo)
*
*   Ŀ��: ���ָ���û��˻������е��û�������ĳһ��ĺϷ���
*
*   ���룺
*		szInfo	szUsername or szPassword
*	���أ�
*		BOOL	�жϺϷ���TRUE or FALSE
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
*   ����: DataValidCharcterSelfInfo(TCHAR* szInfo, UA_SETINFO ua_setinfo)
*
*   Ŀ��: ���ָ���û��˻������е�SelfInfoĳһ��ĺϷ���
*
*   ���룺
*		szInfo			�û�����
*		ua_setinfo		ָ��szInfo����
*	���أ�
*		BOOL	�жϺϷ���TRUE or FALSE
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
*   ����: DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o)
*
*   Ŀ��: ���ڴ��ж�ȡ������û��˻�(�������˻�)����Ϊ����ǰ�˻����ú�������ǰ�˻���Ϣ
*
*   ���룺
*		pUa		ָ��UA���û��˻���Ϣ����ָ�����
*		ua_o	������ʽ����/ȡ
*	���أ�
*		BOOL	�жϳ�������״̬
*/
BOOL DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o)
{
	//�����ڸú�����
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
*   ����: DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o)
*
*   Ŀ��: ���ڴ��ж�ȡ�򱣴��û��˻�(�������˻�)�����ú����洢ͷָ��
*
*   ���룺
*		pUaN	��������
*		uan_o	������ʽ����/ȡ UALIST_GET or _CHANGE		(����Ա��׺_ADMIN)
*	���أ�
*		BOOL	�жϳ�������״̬
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
*   ����: DataAccount_LinkedListFile(UALIST_FILE ual_f)
*
*   Ŀ��: ���û�����������ڴ��ж�ȡ���ļ�����ļ���ȡ���ڴ��е�����
*
*
*   ���룺
*		ual_f	��ȡ��ʽ/����
*				��ͨ�û�UALLIST_TO_FILE FILE_TO_UALLIST
*				������Ա _ADMIN
*	���أ�
*		BOOL	�жϳ�������״̬
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
		//�ļ�������
		if ((err = fopen_s(&fp, URL_UA, TEXT("rb"))) != 0)
		{
			//�����ļ�
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
			//��ֹԽ��
			if (feof(fp)) {
				fclose(fp);
				break;
			}

			//��ȡ��һ���û�����
			if (szBuf[0] == TEXT('{'))
			{
				//��¼{�ļ�ָ��λ��,ע����ַ�2
				userStart = ftell(fp) - FCOUNT;
				//�����ڴ�ռ�
				if ((pUaN = malloc(sizeof(USERACCOUNTNODE))) == NULL)
				{
					ErrorReport(ERR_MEM_CREATE_FAIL | ERR_IN_DataAccount_LinkedListFile);
					return FALSE;
				}
				if (pUaN_head == NULL)
				{
					pUaN_head = pUaNOld = pUaN;
					
				}
				pUaN->ua.ut = UT_USER;//�û���������
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//�����ַ�������
				fseek(fp, userStart, SEEK_SET);
				//��ȡ������
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//��ʽ�������浽ua
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
		//����ͷָ��
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
		//�ļ�������
		if ((err = fopen_s(&fp, URL_ADMIN, TEXT("rb"))) != 0)
		{
			//�����ļ�
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
			//��ֹԽ��
			if (feof(fp)) {
				fclose(fp);
				break;
			}

			//��ȡ��һ���û�����
			if (szBuf[0] == TEXT('{'))
			{
				//��¼{�ļ�ָ��λ��,ע����ַ�2
				userStart = ftell(fp) - FCOUNT;
				//�����ڴ�ռ�
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
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//�����ַ�������
				fseek(fp, userStart, SEEK_SET);
				//��ȡ������
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//��ʽ�������浽ua
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
		//����ͷָ��
		DataAccount_LinkedListMem(&pUaN_head, UALIST_CHANGE_ADMIN);
		break;
	}
	return TRUE;
}

/*
*   ����: DataUAFile(UA_FILE u_f)
*
*   Ŀ��: ��ͨ�û�ʹ�ã�������ͨ�û����ݿ��ļ�����ָ����UA�Ͳ�����ʽ��ע��û�в�����ͬ���ܣ�����������ʵ�ֲ���
*			
*   ���룺
*		u_f		˵��������ʽ
*			UA_ADDTO_FILE					��ָ��������ͨ�˻���ӵ��ļ� (�ļ�׷����ʽ)
*			UA_UPDATE_FILE					��Ϣ���и���
* 
*		ua		���������û��˻�
*	���أ�
*		BOOL	�жϳ�������״̬
*/
BOOL DataUAFile(UA_FILE u_f, LPUSERACCOUNT pUa)
{
	FILE* fp = NULL;
	errno_t err;
	TCHAR szBuf[256] = {TEXT('\0')};
	size_t uLen = 0;
	LPUSERACCOUNTNODE pUaN = NULL, pUaN_head = NULL;

	//���ڴ����û���Ϣ,�浽��ʱ����ua
	//DataUserAccount(&ua, UA_GET);
	switch (u_f)
	{
	case UA_ADDTO_FILE:
		//ֱ��д�ļ��������پ�������
		//���ļ�,���򴴽�
		if ((err = fopen_s(&fp, URL_UA, TEXT("ab+"))) != 0)
		{
			ErrorReport(ERR_FPOPEN_FAIL | ERR_IN_DataUAFile);
			return FALSE;
		}
		if (fp == NULL) return FALSE;
		//�����ݸ�ʽ����szBuf�Ա��ļ�����
		if (!DataAccountFormat(pUa, szBuf, UA_TO_TEXT, &uLen))
		{
			//��ʱ˵�������쳣
			fclose(fp);
			return FALSE;
		}
		fwrite(szBuf, uLen, FCOUNT, fp);
		fclose(fp);
		return TRUE;
		break;

	case UA_UPDATE_FILE:
		//���ļ�,�������
		err = fopen_s(&fp, URL_UA, TEXT("ab+"));
		if (err != 0) {
			return FALSE;
			break;
		}
		if (fp == NULL) return FALSE;
		fclose(fp);

		//�ļ������ڴ�������
		DataAccount_LinkedListFile(FILE_TO_UALIST);
		DataAccount_LinkedListMem(&pUaN_head, UALIST_GET);
		pUaN = pUaN_head;
		while (pUaN != NULL)
		{
			//�����������ݽ��жԱ�
			if (lstrcmp(pUa->szUsername, pUaN->ua.szUsername) == 0)
			{
				//��ͬ��ֱ�Ӹ���
				pUaN->ua = *pUa;
				break;
			}
			pUaN = pUaN->next;
		}
		//�ļ�����
		DataAccount_LinkedListFile(UALIST_TO_FILE);
		//���µ�ǰ�û���Ϣ
		if (pUaN != NULL)
		{
			DataUserAccount(&(pUaN->ua), UA_CHANGE);
		}
		
		break;
	}
	return TRUE;
}


/*
*   ����: DataGetUserType()
*
*   Ŀ��: ��÷��ص�ǰ�û�������
*

*	���أ�
*		USERTYPE
*/
USERTYPE DataGetUserType()
{
	USERACCOUNT ua;
	DataUserAccount(&ua, UA_GET);
	return ua.ut;
}

//�û��˻����������ȡ�������û���Ŀ�����󷵻�-1
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

//��ȡָ���û��Ķ�������,����ǰ�뿪���û������ڴ�
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
*   ����: DataSearchAccount_File(USERACCOUNT uaTrgt)
*
*   Ŀ��: �����ݿ��ļ��в����Ƿ���Ŀ���û������Ա�����ݣ�ֻ��ѯ�û����������Ƿ���ڻ�ƥ��
*
*   ���룺
*		fp					ָ�����ļ�ָ��
*		uaTrgt				Ŀ���û�
*	���أ�
*		UA_SEARCHRET����	�������
*			
*/
UA_SEARCHRET DataAccountSearch_File(USERACCOUNT uaTrgt)
{
	FILE* fp = NULL;
	errno_t err;//�������
	TCHAR szBuf[2] = { TEXT('\0') };
	TCHAR szUserData[256] = { TEXT('\0') };
	TCHAR szUsername[20] = { TEXT('\0') };
	TCHAR szPassword[20] = { TEXT('\0') };
	long userStart = 0L, userEnd = 0L;
	size_t userLen = 0;
	int i = 0, j = 0;
	USERACCOUNT ua;

	//�ֱ����û��͹���Ա�����м���
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
			//��ֹԽ��
			if (feof(fp)) {
				fclose(fp);
				break;
			}
			//��ȡ��һ���û�����
			if (szBuf[0] == TEXT('{'))
			{
				//��¼{�ļ�ָ��λ��,ע����ַ�2
				userStart = ftell(fp) - FCOUNT;
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//�����ַ�������
				fseek(fp, userStart, SEEK_SET);
				//��ȡ������
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//��ʽ�������浽ua
				DataAccountFormat(&ua, szUserData, TEXT_TO_UA, &userLen);
				//ID��ͬ
				if (lstrcmp(ua.szUsername, uaTrgt.szUsername) == 0)
				{
					//������ͬ
					if (lstrcmp(ua.szPassword, uaTrgt.szPassword) == 0)
					{
						fclose(fp);
						switch (j)
						{
						case 0:
							return UA_FOUND;//��ͨ�û�
						case 1:
							return UA_FOUND_ADMIN;//����Ա
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
*   ����: DataAccountSearchFileToMem(USERACCOUNT uaTrgt)
*
*   Ŀ��: �����ݿ��ļ��в����Ƿ���Ŀ���û������Ա�����ݣ�����ѯ�������Զ������ڴ�
*			�ڴ�������DataUserAccount����(��ǰ)
*
*   ���룺
*		fp					ָ�����ļ�ָ��
*		uaTrgt				Ŀ���û�
*	���أ�
*		UA_SEARCHRET����	�������
*
*/
UA_SEARCHRET DataAccountSearchFileToMem(USERACCOUNT uaTrgt)
{
	FILE* fp = NULL;
	errno_t err;//�������
	TCHAR szBuf[2] = TEXT("\0");
	TCHAR szUserData[256] = TEXT("\0");
	TCHAR szUsername[20] = TEXT("\0");
	TCHAR szPassword[20] = TEXT("\0");
	long userStart = 0L, userEnd = 0L;
	size_t userLen = 0;
	int i = 0, j = 0;
	USERACCOUNT ua = {0};

	//�ֱ����û��͹���Ա�����м���
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
			//��ֹԽ��
			if (feof(fp)) {
				fclose(fp);
				break;
			}
			//��ȡ��һ���û�����
			if (szBuf[0] == TEXT('{'))
			{
				//��¼{�ļ�ָ��λ��,ע����ַ�2
				userStart = ftell(fp) - FCOUNT;
			}
			if (szBuf[0] == TEXT('}'))
			{
				userEnd = ftell(fp) - FCOUNT;
				userLen = (userEnd - userStart + FCOUNT) / FCOUNT;//�����ַ�������
				fseek(fp, userStart, SEEK_SET);
				//��ȡ������
				fread(szUserData, sizeof(TCHAR), userLen, fp);
				fseek(fp, userEnd + FCOUNT, SEEK_SET);
				//��ʽ�������浽ua
				DataAccountFormat(&ua, szUserData, TEXT_TO_UA, &userLen);
				//ID��ͬ
				if (lstrcmp(ua.szUsername, uaTrgt.szUsername) == 0)
				{
					//������ͬ
					if (lstrcmp(ua.szPassword, uaTrgt.szPassword) == 0)
					{
						fclose(fp);
						switch (j)
						{
						case 0:
							ua.ut = UT_USER;
							DataUserAccount(&ua, UA_CHANGE);
							return UA_FOUND;//��ͨ�û�
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
							//ע�⣬����Աֻ��nickname
							return UA_FOUND_ADMIN;//����Ա
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
*   ����: DataFormatAccount(LPUSERACCOUNT pUa, TCHAR* szBuf, DATA_FORMAT d_f, size_t *pLength)
*
*   Ŀ��: ���������ݸ�ʽ���������ļ�������ȡ��TEXTת����UA�ṹ������෴
*
*   ���룺
*		pUa		ָ��USERACCOUNT�ṹ���ָ�����
*		szBuf	TEXT
*		d_f		ת������
*		pLength	TEXT����(����Ĭ�ϣ���һ����CHAR����)��TEXT����UAʱ��ָ���ó��ȣ�UA����TEXTʱ�Զ���ֵ����
*	���أ�
*		BOOL	�жϳ�������״̬
*/

BOOL DataAccountFormat(LPUSERACCOUNT pUa, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
{
	/*
		�ļ���������֯��ʽ��
					iDataPos=1       2     ...
		UA TEXT:	{[USERNAME],[PASSWORD],[NICKNAME,NAME,IDCARD]},...
					ADMIN:					IDCARDΪ0:ADMIN 1:SUPERADMIN
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
		*pLength = lstrlen(szBuf);//���س���
		return TRUE;
		break;
	case TEXT_TO_UA:
		//�����
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
				if (iStart > iEnd) return FALSE;//˵��Ϊ��
				if (iDataPos == 1)
					StringCchCopy(pUa->szUsername, iEnd - iStart + 2, szTemp);//ע��\0��������
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
*   ����: DataAccount_GetSelfInfoFromUA(TCHAR* szBuf, UA_GETINFO ua_getinfo, LPUSERACCOUNT pUa)
*
*   Ŀ��: ��ָ���û��˻�(��ͨ�û�)�ṹ���ȡinfo
*
*   ���룺
*		_In_  pUa			ָ��USERACCOUNT�ṹ���ָ�����
*		_Out_ szDest			�����Ŀ��
*		_In_  ua_getinfo	ָ�������Ŀ��
*				birth��ʽ 2000-12-11
*				sex��ʽ   ��/Ů
*		
*	���أ�
*		BOOL	�жϽ�ȡ�������Ƿ����
*/
BOOL DataAccount_GetSelfInfoFromUA(TCHAR* szDest, UA_GETINFO ua_getinfo, LPUSERACCOUNT pUa)
{
	/*
		nickname name ID
	*/

	TCHAR szInfo[128] = TEXT("\0");//���������Ϣ
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
			pos++;//��ǰ,λ��
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
	//�����ڸ���

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
			szDest[0] = TEXT('Ů');
		else szDest[0] = TEXT('��');
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
*   ����: DataAccount_SetSelfInfoFromUA(TCHAR* szBuf, UA_SETINFO ua_setinfo, LPUSERACCOUNT pUa)
*
*   Ŀ��: ��ָ��������Ϣ���ͣ���szBuf�е������޸ĵ�UA
*
*   ���룺
*		_Out_  pUa			ָ��USERACCOUNT�ṹ���ָ�����
*		_In_ szBuf			�޸�����
*		_In_  ua_setinfo	ָ���޸ĵ�����

*
*	���أ�
*		BOOL	�жϳ�������״̬
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

/*-----------�����������-------------*/
/*
{[������][Ŀ�ĵ�][6,21,19,30][6,22,10,0][����ID][�ɻ����][�۸�][Ʊ��][����˵���羭�ò�]}
	256�ַ��ռ�
*/

/*
*   ����: DataFlightLinkedListMem(LPFLIGHTDATANODE pFlNode_Head, FLLIST_OPERATE fl_o)
*
*   Ŀ��: ���ڴ��б���Ͷ�ȡ�ɻ���̬�������ݣ����ô˺������е�ǰ�������ݵĻ�ȡ���޸�
*
*   ���룺
*		pFlNode_Head		ָ�������ͷָ���ָ�룬�׽ڵ�
*		fll_o				˵��ָ�������Ĳ�����ʽ
*			FLLIST_GET_HEADNODE		���ڴ��л�ȡ����ͷָ�뱣����pFl_HeadNode
			FLLIST_CHANGE_HEADNODE	��pFl_HeadNodeָ��������Ǳ��棬ͬʱɾ���ڴ�����ǰ����
*	���أ�
*		size_t		��������ڵ�����������������
*/
IDDATA_NUM DataFlightLinkedListMem(LPFLIGHTDATANODE* ppFlNode_Head, FLLIST_OPERATE fll_o)
{
	static LPFLIGHTDATANODE pFlNode_Mem = NULL;
	IDDATA_NUM iListNum = 0;//����������
	LPFLIGHTDATANODE pFlNode = NULL;
	LPFLIGHTDATANODE pFlNode_next = NULL;
	switch (fll_o)
	{
		//��ȡ����
	case FLLIST_GET_HEADNODE:
		pFlNode = *ppFlNode_Head = pFlNode_Mem;
		while (pFlNode != NULL)
		{
			iListNum++;
			pFlNode = pFlNode->next;
		}
		break;
		//�ı�����
	case FLLIST_CHANGE_HEADNODE:
		pFlNode = pFlNode_Mem;
		//���洫����������
		pFlNode = pFlNode_Mem = *ppFlNode_Head;
		//��¼��Ŀ
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
*   ����: DataFlightLinkedListFile(FL_FILE fl_file)
*
*   Ŀ��: ���������ݴ��ڴ��ȡ���ļ�����ļ��ж�ȡ���ڴ棬�ڴ�������DataFlightLinkedListMem����
*
*   ���룺
*		fl_file		ָ��������ʽ
*			FLLIST_TO_FILE			���ڴ渲��д������ļ�
*			FLLIST_FROM_FILE		�Ӵ����ļ���ȫ����ȡ���ڴ�
*	���أ�
*		BOOL		�жϳ�������״̬
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
					//�״�
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
				//�ı����ṹ��
				DataFlightFormat(&(pFlNode_new->fl), szBuf, TEXT_TO_FL, &iItemLen);
				//����szBuf
				ClearStrToZero(szBuf, iItemLen);

				//������һ����ַ
				pFlNode = pFlNode_new;
			}
		}
		if (pFlNode_new != NULL)
			pFlNode_new->next = NULL;
		fclose(fp);
		//�洢���ڴ�
		DataFlightLinkedListMem(&pFlNode_head, FLLIST_CHANGE_HEADNODE);
		
		break;
	}
	return TRUE;
}

/*
*   ����: DataGetValidFlightInfoStd(TCHAR szInfo[128])
*
*   Ŀ��: ��úϷ��ĺ�����Ϣ����ʾ��Ϣ���������ָ��szBuf
*
*   ���룺
*		szInfo	��ʾ��Ϣ��Ŵ�
*	���أ�
*		BOOL	�ж��Ƿ���ȷִ��
*
*/
BOOL DataGetValidFlightInfoStd(TCHAR szInfo[128])
{
	StringCchPrintf(szInfo, 128, 
		TEXT("�Ϸ��ĺ�����Ϣ��                 \n1. ������Ŀ�ĵء����չ�˾�������Ų����пո񣬲�����15�ַ�\n2. ʱ�䡢Ʊ��Ϊ����\n3. Ʊ��ֻ����1λС����������5λ����\n4. ���ݺ���"));
	return TRUE;
}

/*
*   ����: DataValidCharacterFlightIDOrDepartOrDest(TCHAR* szInfo)
*
*   Ŀ��: ���ָ�����������е�szFlightID,szDepart,szDest�Ϸ���
*   ���룺
*		szBuf	ָ����szBuf
*	���أ�
*		BOOL	�жϺϷ���TRUE or FALSE
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
*   ����: DataFlightFormat(LPFLIGHTDATA pFl, TCHAR* szBuf, DATA_FORMAT d_f, size_t *pLength)
*
*   Ŀ��: ���������ݸ�ʽ���������ļ�������ȡ��TEXTת����FL�ṹ������෴
*
*   ���룺
*		pFl		ָ��FLIGHTDATA�ṹ���ָ�����
*		szBuf	TEXT
*		d_f		ת������
*			FL_TO_TEXT		256 �洢TCHAR
*			TEXT_TO_FL
*		pLength	TEXT���ȣ��ַ�����(����Ĭ�ϣ���һ����CHAR����)��TEXT����FLʱ��ָ���ó��ȣ�FL����TEXTʱ�Զ���ֵ����
*	���أ�
*		BOOL	�жϳ�������״̬
*	ע�ͣ�
*		����{[],[],...}
*/
BOOL DataFlightFormat(LPFLIGHTDATA pFl, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
{
	/*
	*					  1 2  3  4   1  2  3 4
		{[������][Ŀ�ĵ�][6,21,19,30][6,22,10,0][����ID][�ɻ����][�۸�][Ʊ��][����][ΨһID]}
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
				szItem[k] = TEXT('\0');//ֻ����[]��
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
						//InnerItem�ָ�
						//�ɽ���λ�ã��µĿ�ʼλ��
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
							//InnerItem�ָ�����
						iStart = j + 1;
						}//if����
						if (szItem[j] == TEXT('\0')) break;
					}
					break;//dtDepart,dtDest����
					
				}
			}//if TEXT(']')����
			i++;
		}//while����
		break;
	}
	return TRUE;
}

//IDDATA����Ϊ0��ʾû��
/*
*	������DataFlightSearch_ReturnIDDATA(FL_SEARCHDATA fl_sd, IDDATA pIdData[MAX_SEARCH_FLIGHT_ID])
*	
*	Ŀ�꣺��ָ���Ĳ�ѯ�����������������ݲ�����
*	
*	���룺
*		_In_  fl_sd		��ѯ������Դ���������ݺ����ݲ�ѯ��־
*		_Out_ IDDATA	���ز�ѯ�ĺ���ID
* 
*	���أ�FL_SEARCHRET	���������
*					FL_NOTFOUND, FL_FOUND, FL_ERRFOUND
*/
FL_SEARCHRET DataFlightSearch_ReturnIDDATA(FL_SEARCHDATA fl_sd, IDDATA IdData[MAX_SEARCH_FLIGHT_ID])
{
	LPFLIGHTDATANODE pFlNode = NULL, pFlNode_head = NULL;
	//����������ID
	IDDATA idSearchRet[MAX_SEARCH_FLIGHT_ID] = { 0 };
	//�����ʱ�������ID
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
	//�����ÿ�
	ClearIDDATAARRToZero(IdData);
	//��ȡfl_sd����
	DataFlight_GetCityName(szCityTrgt_Depart, fl_sd.szDepart);
	DataFlight_GetCityName(szCityTrgt_Dest, fl_sd.szDest);
	//���ڴ��л�ȡ����ͷָ��
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);
	pFlNode = pFlNode_head;
	if (pFlNode == NULL)
	{
		return FL_ERRFOUND;
	}
	//��ѯDepartAndDest(��ѡ),��һ�β�ѯ���ո��ѯ
	iIdCount = 0;
	while (pFlNode != NULL)
	{
		
		DataFlight_GetCityName(szCity_Depart, pFlNode->fl.szDepart);
		DataFlight_GetCityName(szCity_Dest, pFlNode->fl.szDest);

		if ((lstrcmp(szCityTrgt_Depart, szCity_Depart) == 0) &&
			(lstrcmp(szCityTrgt_Dest, szCity_Dest) == 0))
		{
			//����ID
			idSearchRetBuf[0][iIdCount] = pFlNode->fl.id;
			iIdCount++;//��¼��Ŀ
			fFound = TRUE;
		}
		pFlNode = pFlNode->next;
	}
	if (!fFound) return FL_NOTFOUND;

	//��ѯ����������һ�β�ѯ���ݺϲ�
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
				//����ID
				idSearchRetTemp1[iIdCount] = pFlNode->fl.id;
				iIdCount++;
				fFound = TRUE;
			}
			pFlNode = pFlNode->next;
		}
		//�ϲ���buf
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
				//����ID
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
				//����ID
				idSearchRetTemp3[iIdCount] = pFlNode->fl.id;
				iIdCount++;
				fFound = TRUE;
			}
			pFlNode = pFlNode->next;
		}
		IDDATAFoundCommon(idSearchRetBuf[3], idSearchRetTemp3, idSearchRetBuf[0]);
	}
	if (!fFound) return FL_NOTFOUND;
	
	//�����ҵ�����������ϲ�
	for (i = 0,iNumNeeded = 0 ; i < 4 ; i++)
	{
		if (fSearchNeeded[i] == TRUE)
		{
			iNumNeeded++;				//��¼��Ҫ�ϲ�����������
			iSearchNeeded[iNumNeeded-1] = i;//��¼��Ҫ�ϲ��������±�
		}
	}
	//ֻ����Depart Dest�������������һ��(�Ѿ��ϲ�)
	if (iNumNeeded == 0 || iNumNeeded == 1)
	{
		for (i = 0; i < MAX_SEARCH_FLIGHT_ID; i++)
		{
			if (idSearchRetBuf[iSearchNeeded[0]][i] == 0) break;
			idSearchRet[i] = idSearchRetBuf[iSearchNeeded[0]][i];
		}
	}
	//����������������
	else if (iNumNeeded == 2)
	{
		IDDATAFoundCommon(idSearchRet, idSearchRetBuf[iSearchNeeded[0]], idSearchRetBuf[iSearchNeeded[1]]);
	}
	//ȫ������
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
*	������DataFlight_GetPNode(IDDATA id)
*
*	Ŀ�꣺����ָ��������ڵ�ID�Զ�Ӧ��ָ��
*
*	���룺
*		id					�ڵ�id
*
*	���أ�LPFLIGHTDATANODE	�ڵ�ָ��
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
*	������DataFlight_GetCityName(TCHAR* szDst, const TCHAR* szSrc)
*
*	Ŀ�꣺��Դ�ַ�����ȡ��������
*
*	���룺
*		szDst		Ŀ���ַ���
*		szSrc		Դ�ַ���
*
*	���أ�BOOL		�ж�����״̬
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
*	������DataFlight_GetCityName(TCHAR* szDst, const TCHAR* szSrc)
*
*	Ŀ�꣺��Դ�ַ���(szDepart/szDest)��ȡ��������
*
*	���룺
*		szDst		Ŀ���ַ���
*		szSrc		Դ�ַ���
*
*	���أ�BOOL		�ж�����״̬
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
*	������DataFlightIDDATA_Store(IDDATA iddataSrc, size_t iLen)
*
*	Ŀ�꣺��ʱ�洢����IDDATA���飬�Ա�����������ݲ���������ʾ��
*
*	���룺
*			_In_	IDDATA����
*			size_t	���鳤�ȣ����ô���MAX_SEARCH_FLIGHT_ID
*
*	���أ�BOOL		�ж�����״̬
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
*	������DataValidDateText(const TCHAR* szMonth, const TCHAR* szDay)
*
*	Ŀ�꣺�ж�ʱ���ı��Ƿ�Ϸ�
*
*	���룺
*			ʱ���ı�
*
*	���أ�BOOL		TRUE�Ϸ���FALSE���Ϸ�
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
*	������DataFlight_CreateNULLFlight()
*
*	Ŀ�꣺�����µĺ���ڵ㣨�����Ľڵ�����Ϊ�գ�����䣩
*
*	���룺��
*
*	���أ�LPFLIGHTDATANODE		�½ڵ��ָ��
*/

LPFLIGHTDATANODE DataFlight_CreateNULLFlight()
{
	LPFLIGHTDATANODE pFlNode_head = NULL;
	LPFLIGHTDATANODE pFlNode_new = NULL;
	IDDATA idNew = 0;
	//���ڴ��л�ȡͷָ��
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);

	//����
	pFlNode_new = (LPFLIGHTDATANODE)malloc(sizeof(FLIGHTDATANODE));
	if (pFlNode_new == NULL)
	{
		ErrorReport(ERR_IN_DataFlight_CreateNULLFlight | ERR_MEM_CREATE_FAIL);
		return NULL;
	}
	//û�пɹ�����ĺ���ID�����ﵽ����
	if ((idNew = DataFlight_GetAvailableID()) == 0)
	{
		return NULL;
	}
	//��ʼ��
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
	//���ӵ�����
	pFlNode_head->next = pFlNode_new;
	return pFlNode_new;
}

/*
*	������DataFlight_GetAvailableID()
*
*	Ŀ�꣺��ȡ���õ�ID�Ա���亽��ڵ�,���򷵻�0
*
*	���룺
*			��
*
*	���أ�IDDATA	���õ�ID������Ϊ0
*/
IDDATA DataFlight_GetAvailableID()
{
	LPFLIGHTDATANODE pFlNode_head = NULL, pFlNode = NULL;
	IDDATA iddata[MAX_SEARCH_FLIGHT_ID] = { 0 };
	IDDATA_NUM i = 0;
	IDDATA_NUM ret = 0;
	IDDATA_NUM sameNum = 0;
	//���ڴ��л�ȡͷָ��
	DataFlightLinkedListMem(&pFlNode_head, FLLIST_GET_HEADNODE);
	pFlNode = pFlNode_head;
	while (pFlNode != NULL)
	{
		//��ȡ���е��ѷ���ID
		iddata[i] = pFlNode->fl.id;
		pFlNode = pFlNode->next;
		i++;
	}
	//����Ƿ��и�ID
	for (ret = IDDATA_BEGIN; ret <= IDDATA_END; ret++)
	{
		for (i = 0, sameNum = 0; i < MAX_SEARCH_FLIGHT_ID && iddata[i] != 0; i++)
		{
			//��ͬ������
			if (ret == iddata[i])
			{
				sameNum++;
			}
		}
		//û����ͬ�ģ�����ѭ��
		if (sameNum == 0) break;
	}
	
	if (sameNum != 0) ret = 0;//ʼ������ͬ��
	return ret;
}
/*
	��������
	{[USERNAME][ID][ORDERNUM]}
*/

/*
*   ����: DataOrder(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
*
*   Ŀ��: ���ڴ��д�ȡ��ǰ�û��Ķ������ݣ�������ʹ��ǰ��ǵ��ͷŲ���Ҫ�Ķ��ڴ�
*
*   ���룺
*		pOINode_Head		ָ�������ͷָ���ָ�룬�׽ڵ�
*		oil_o				˵��ָ�������Ĳ�����ʽ
*			OILIST_GET_HEADNODE		���ڴ��л�ȡ����ͷָ�뱣����pOI_HeadNode
*			OILIST_CHANGE_HEADNODE	��pOI_HeadNodeָ��������Ǳ��棬ͬʱɾ���ڴ�����ǰ����
*	���أ�
*		size_t		��������ڵ�������������
*/
IDDATA_NUM DataUserOrder(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
{
	static LPORDERINFONODE pOINode_Mem = NULL;
	IDDATA_NUM iListNum = 0;//����������
	LPORDERINFONODE pOINode = NULL;
	LPORDERINFONODE pOINode_next = NULL;
	switch (oil_o)
	{
		//��ȡ����
	case OILIST_GET_HEADNODE:
		pOINode = *ppOINode_Head = pOINode_Mem;
		while (pOINode != NULL)
		{
			iListNum++;
			pOINode = pOINode->next;
		}
		break;
		//�ı�����
	case OILIST_CHANGE_HEADNODE:
		pOINode = pOINode_Mem;
		//���洫����������
		pOINode = pOINode_Mem = *ppOINode_Head;
		//��¼��Ŀ
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
*	������DataOrder_CreateSingleOrder(IDDATA idData)
*	
*	Ŀ�꣺Ϊ��ǰ���û������µ��û������������������ж�������͸��û���������
* 
*	���룺
*		idData			����ڵ�ID
* 
*	���أ�
*		BOOL			�ɹ�����TRUE
* 
*/
BOOL DataOrder_CreateSingleOrder(IDDATA idData)
{
	USERACCOUNT ua;
	ORDERINFO oi_new;//��ǰ��ӵ�
	LPORDERINFONODE pOINode_New;
	time_t timeNow;
	struct tm tm_tm;
	int year, month, day, hr, min, sec;
	LPORDERINFONODE pOINode_All;		//���ж�������
	LPORDERINFONODE pOINode_UserAll;	//��ǰ�û������ж�������
	LPORDERINFONODE pOINode;			//��ʱָ��

	LPFLIGHTDATANODE pFlNode;			//�����ĺ���
	//��ȡ��ǰ�û�����
	DataUserAccount(&ua, UA_GET);

	//�������ݲ���
	pFlNode = DataFlight_GetPNode(idData);
	pFlNode->fl.iAmount--;                   //�ýڵ��Ʊ����������
	//�������ݲ���
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
	/*ע���ļ�����ʱ�����������ж�����������ı��漴��*/

	//��ȡ���ж���ͷ�ڵ�
	DataOrderLinkedListMem(&pOINode_All, OILIST_GET_HEADNODE);

	//����ڴ�û������
	if (pOINode_All == NULL)
	{
		//�����µĽڵ�
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;//�ÿ�
		pOINode_All = pOINode_New;
		DataOrderLinkedListMem(&pOINode_All, OILIST_CHANGE_HEADNODE);
	}
	else
	{
		//�ڴ����������ת��β�ڵ�
		pOINode = pOINode_All;
		while (pOINode->next != NULL)
		{
			pOINode = pOINode->next;
		}
		//�����µĽڵ�
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;
		//�ڵ����ӵ���������
		pOINode->next = pOINode_New;
	}


	//��ȡ���и��û�����ͷ�ڵ�
	DataUserOrder(&pOINode_UserAll, OILIST_GET_HEADNODE);
	if (pOINode_UserAll == NULL)
	{
		//�����µĽڵ�
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
		//�����µĽڵ�
		pOINode_New = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
		if (pOINode_New == NULL)
		{
			ErrorReport(ERR_IN_DataOrder_CreateSingleOrder | ERR_MEM_CREATE_FAIL);
			return FALSE;
		}
		pOINode_New->oi = oi_new;
		pOINode_New->next = NULL;
		//�ڵ����ӵ��û����ж�������
		pOINode->next = pOINode_New;
	}


	return TRUE;
}

/*
*   ����: DataOrder_GetUserOrder()
*
*   Ŀ��: ��ȡ�û������ж������ݣ����浽�û�����DataUserOrder�����ڳ�ʼ��
*
*   ���룺
*		pOINode_Head		ָ�������ͷָ���ָ�룬�׽ڵ�
*		oil_o				˵��ָ�������Ĳ�����ʽ
*			OILIST_GET_HEADNODE		���ڴ��л�ȡ����ͷָ�뱣����pOI_HeadNode
*			OILIST_CHANGE_HEADNODE	��pOI_HeadNodeָ��������Ǳ��棬ͬʱɾ���ڴ�����ǰ����
*	���أ�
*		BOOL		�ж��Ƿ�ִ�гɹ�
*/
BOOL DataOrder_GetUserOrderToMem()
{
	LPORDERINFONODE pOINode_User = NULL, pOINode_User_Old = NULL;
	LPORDERINFONODE pOINode_User_Head = NULL;
	LPORDERINFONODE pOINode_All = NULL;
	USERACCOUNT ua;
	//��ȡ�û�����,��Ҫ��ȡ�û���
	DataUserAccount(&ua, UA_GET);
	//�����ж����л�ȡ���û�����
	//��ȡ���ж���
	DataOrderLinkedListMem(&pOINode_All, OILIST_GET_HEADNODE);
	if (pOINode_All == NULL)
	{
		//�����ڣ�Ĭ���û�����ΪNULL
		return FALSE;
	}
	else
	{
		while (pOINode_All != NULL)
		{
			//�鵽�뵱ǰ�û�����ͬ
			if (lstrcmp(ua.szUsername, pOINode_All->oi.szUsername) == 0)
			{
				//���ڴ濪��
				pOINode_User = (LPORDERINFONODE)malloc(sizeof(ORDERINFONODE));
				//����ʧ��
				if (pOINode_User == NULL)
				{
					ErrorReport(ERR_IN_DataOrder_GetUserOrderToMem | ERR_MEM_CREATE_FAIL);
					return FALSE;
				}
				//���ٳɹ�

				//��һ�β鵽
				if (pOINode_User_Head == NULL)
				{
					pOINode_User_Head = pOINode_User_Old = pOINode_User;
				}
				else
				{	

					pOINode_User_Old->next = pOINode_User;
					pOINode_User->next = NULL;
					//����ýڵ�
					pOINode_User_Old = pOINode_User;
				}
				pOINode_User->oi = pOINode_All->oi;
			}
			//������һ���ڵ㣨���ж�����
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
*   ����: DataOrderLinkedListMem(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
*
*   Ŀ��: ���ڴ��б���Ͷ�ȡ���ж�����̬�������ݣ����ô˺������е�ǰ�������ݵĻ�ȡ���޸ģ�ʹ��ǰ���β�ڵ�next�ÿ�
*
*   ���룺
*		pOINode_Head		ָ�������ͷָ���ָ�룬�׽ڵ�
*		oil_o				˵��ָ�������Ĳ�����ʽ
*			OILIST_GET_HEADNODE		���ڴ��л�ȡ����ͷָ�뱣����pOI_HeadNode
			OILIST_CHANGE_HEADNODE	��pOI_HeadNodeָ��������Ǳ��棬ͬʱɾ���ڴ�����ǰ����
*	���أ�
*		IDDATA_NUM		��������ڵ�������������
*/
IDDATA_NUM DataOrderLinkedListMem(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o)
{
	static LPORDERINFONODE pOINode_Mem = NULL;
	IDDATA_NUM iListNum = 0;//����������
	LPORDERINFONODE pOINode = NULL;
	LPORDERINFONODE pOINode_next = NULL;
	switch (oil_o)
	{
		//��ȡ����
	case OILIST_GET_HEADNODE:
		pOINode = *ppOINode_Head = pOINode_Mem;
		while (pOINode != NULL)
		{
			iListNum++;
			pOINode = pOINode->next;
		}
		break;
		//�ı�����
	case OILIST_CHANGE_HEADNODE:
		pOINode = pOINode_Mem;

		//���洫����������
		pOINode = pOINode_Mem = *ppOINode_Head;
		//��¼��Ŀ
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
*   ����: DataOrderLinkedListFile(OI_FILE oi_file)
*
*   Ŀ��: �����ж������ݴ��ڴ��ȡ���ļ�����ļ��ж�ȡ���ڴ棬�ڴ�������DataOrderLinkedListMem����
*
*   ���룺
*		fl_file		ָ��������ʽ
*			OILIST_TO_FILE			���ڴ渲��д������ļ�
*			OILIST_FROM_FILE		�Ӵ����ļ���ȫ����ȡ���ڴ�
*	���أ�
*		BOOL		�жϳ�������״̬
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
			//�������ļ�
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
					//�״�
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
				//�ı����ṹ��
				DataOrderFormat(&(pOINode_new->oi), szBuf, TEXT_TO_OI, &iItemLen);
				//����szBuf
				ClearStrToZero(szBuf, iItemLen);

				//������һ����ַ
				pOINode = pOINode_new;
			}
		}
		if (pOINode_new != NULL)
			pOINode_new->next = NULL;
		fclose(fp);
		//�洢���ڴ�
		DataOrderLinkedListMem(&pOINode_head, OILIST_CHANGE_HEADNODE);

		break;
	}
	return TRUE;
}

/*
*   ����: DataOrderFormat(LPORDERINFO pOI, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
*
*   Ŀ��: �������������ݸ�ʽ���������ļ�������ȡ��TEXTת����OI�ṹ������෴
*
*   ���룺
*		pOI		ָ��ORDERINFO�ṹ���ָ�����
*		szBuf	TEXT
*		d_f		ת������
*			OI_TO_TEXT		256 �洢TCHAR
*			TEXT_TO_OI
*		pLength	TEXT���ȣ��ַ�����(����Ĭ�ϣ���һ����CHAR����)��TEXT����OIʱ��ָ���ó��ȣ�OI����TEXTʱ�Զ���ֵ����
*	���أ�
*		BOOL	�жϳ�������״̬
*	ע�ͣ�
*		����{[],[],...}
*/
BOOL DataOrderFormat(LPORDERINFO pOI, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength)
{
	/*
	*					 [username20][������Ϣ][������20]
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
				szItem[k] = TEXT('\0');//ֻ����[]��
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
						//InnerItem�ָ�
						//�ɽ���λ�ã��µĿ�ʼλ��
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
							//InnerItem�ָ�����
						iStart = j + 1;
						}//if����
						if (szItem[j] == TEXT('\0')) break;
					}
					break;//dtDepart,dtDest����

				case 12:
					StringCchPrintf(pOI->szOrderNumber, 20, szItem);
					break;
				}
			}//if TEXT(']')����
			i++;
		}//while����
		break;
	}
	return TRUE;
}


/*
*   ����: DataFlightSearchRet(IDDATA iddata[MAX_SEARCH_FLIGHT_ID], FL_SEARCHRET_MEM fl_sr_m)
*
*   Ŀ��: �洢���ȡ����(�������)����
*
*   ���룺
*		iddata				ָ���ĺ���ID��������
*		FL_SEARCHRET_MEM	ָ����ȡ���Ǵ洢
*		
*	���أ�
*		IDDATA_NUM	��غ��������
*	ע�ͣ�
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
*	������DataFreeMemory()
*
*	Ŀ�꣺�ͷų��򴴽������ж��ڴ�ռ�
*
*	���룺��
*
*	���أ�BOOL		TRUE��ȷִ��
*/
BOOL DataFreeMemory()
{
	LPFLIGHTDATANODE pFlNode = NULL, pFlNode_next = NULL;
	LPUSERACCOUNTNODE pUaN = NULL, pUaN_next = NULL;
	LPORDERINFONODE pOINode = NULL, pOINode_next = NULL;
	DataFlightLinkedListMem(&pFlNode, FLLIST_GET_HEADNODE);
	//��������ڴ�ռ�
	if (pFlNode != NULL)
	{
		while (pFlNode != NULL)
		{
			pFlNode_next = pFlNode->next;
			free(pFlNode);
			pFlNode = pFlNode_next;
		}
	}
	//�û�����ռ�
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
	�����㷨
*/
/*
*	������void ClearStrToZero(TCHAR* szText, size_t iLen)
*
*	Ŀ�꣺��ָ���ַ�����ǰiLen���ַ�����\0����
*
*	���룺szText	�ַ���
*			iLen	����
*
*	���أ���
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
*	������ClearUAStructToZero(LPUSERACCOUNT pUa)
*
*	Ŀ�꣺��ָ����USERACCOUNT���ݽṹ���������ÿմ���
*
*	���룺pUa		ָ��USERACCOUNT���͵Ľṹ��ָ�����
*
*	���أ���
*/
void ClearUAStructToZero(LPUSERACCOUNT pUa)
{
	ClearStrToZero(pUa->szPassword, 20);
	ClearStrToZero(pUa->szUsername, 20);
	ClearStrToZero(pUa->szSelfInfo, 128);
	pUa->ut = UT_USER;

}
/*
*	������void ClearIDDATAARRToZero(IDDATA iddata[MAX_SEARCH_FLIGHT_ID])
*
*	Ŀ�꣺��ָ����IDDATA���͵�����\0�ÿ�
*
*	���룺iddata		�������IDDATA���͵�����
*
*	���أ���
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
*	������void ClearPointerArrToZero(void* pArr[], size_t num)
*
*	Ŀ�꣺��ָ��ָ�������ǰnum��Ԫ�ؽ����ÿ�NULL
*
*	���룺	pArr	ָ����ָ������
*			iLen		����
*
*	���أ���
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
*	������void IDDATAFoundCommon(IDDATA dest[], const IDDATA src1[], const IDDATA src2[])
*
*	Ŀ�꣺����ָ��������IDDATA���͵�����src1,src2�Ĺ�ͬԪ�أ�������ͬԪ�ر�����dest
*
*	���룺
*			_In_  src1,src2		��Ҫ���ҵ�IDDATA��������
*			_Out_ dest			��ͬԪ�ص�IDDATA��������
*
*	���أ���
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
	dest[k] = 0;//��0
}
