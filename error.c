/*
	error.c
	����������ʱ�ĺ���
*/
#include "header.h"
VOID ErrorReport(UINT code)
{
	TCHAR szText[200] = { '\0' };
	StringCchPrintf(szText, 200, TEXT("�����������󣬴�����Ϊ 0x%08x, ����ϵ������"), (UINT)code);
	if (MessageBox(NULL, szText, TEXT("������������"), MB_ICONERROR | MB_OK) == IDOK)
		PostQuitMessage(0);
}