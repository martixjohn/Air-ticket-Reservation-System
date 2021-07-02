/*
	error.c
	处理程序出错时的函数
*/
#include "header.h"
VOID ErrorReport(UINT code)
{
	TCHAR szText[200] = { '\0' };
	StringCchPrintf(szText, 200, TEXT("程序遇到错误，错误码为 0x%08x, 请联系开发者"), (UINT)code);
	if (MessageBox(NULL, szText, TEXT("程序遇到错误"), MB_ICONERROR | MB_OK) == IDOK)
		PostQuitMessage(0);
}