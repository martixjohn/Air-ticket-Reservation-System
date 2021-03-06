/*
	login.h
	登录界面相关头文件
*/
#include "header.h"
//子窗口ID
#define LOG_INPUTBOX				0x00001000
#define LOG_INPUTBOX_TITLE			0x00001100
#define LOG_INPUTBOX_USERNAME		0x00001200
#define LOG_INPUTBOX_PASSWORD		0x00001300
#define LOG_INPUTBOX_INPUTTITLE		0x00001201
#define LOG_INPUTBOX_INPUTCONTENT	0x00001202
#define LOG_INPUTBOX_WARNING		0x00001205
#define LOG_INPUTBOX_REGISTER		0x00001400
#define LOG_INPUTBOX_SIGNIN			0x00001500
#define REGISTER_INPUT_NAME			0x00002100
#define REGISTER_INPUT_IDCARD		0x00002200
#define REGISTER_INPUT_NICKNAME		0x00002300
#define REGISTER_INPUT_BUTTON		0x00002400
//消息
#define WM_U_SIGNIN					WM_USER+1
#define WM_U_REGISTER				WM_USER+2

/*TIMER ID*/
#define TID_DESWARNNING 1
#define TID_MAINBK      2
#define TID_MAINBK_2    3
#define TID_MAINNOTE    4