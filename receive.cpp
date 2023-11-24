#pragma once
// ARQ_r.cpp : Defines the entry point for the application.

//#include "stdafx.h"
//VC 6.0   add  WSOCK32.LIB in Project -> Settings... ->  Link
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")
//--------------------------------------------------------------
// Global Variables:
HINSTANCE hInst;						// current instance

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//--------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lp, int nC)
{
	MSG msg;
	HWND hWnd;
	hInst = hI; // Store instance handle in our global variable
	WNDCLASS wc;

	memset(&wc, 0, sizeof(WNDCLASS));

	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hI;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = TEXT("W1");
	RegisterClass(&wc);

	hWnd = CreateWindow(TEXT("W1"), TEXT("接收端[8888]测试"),
		WS_DLGFRAME | WS_SYSMENU,
		100, 300, 700, 500,
		NULL, NULL, hI, NULL);

	if (!hWnd)   return FALSE;

	ShowWindow(hWnd, nC);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
//--------------------------------------------------------------
//按钮
HWND CreateButton(char* Titel, int x0, int y0, int w, int h, int ID, HWND hW, HINSTANCE hInst)
{
	return CreateWindowEx(WS_EX_PALETTEWINDOW, TEXT("BUTTON"), Titel,
		WS_VISIBLE | WS_CHILD,
		x0, y0, w, h, hW, (HMENU)ID, hInst, NULL);
}
//--------------------------------------------------------------
//单行编辑框
HWND CreateEdit(char* Titel, int x0, int y0, int w, int h, int ID, HWND hW, HINSTANCE hInst)
{
	return CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", Titel,
		WS_VISIBLE | WS_CHILD |
		ES_LEFT | ES_MULTILINE | WS_HSCROLL,
		x0, y0, w, h, hW,
		(HMENU)ID, hInst, NULL);
}
//--------------------------------------------------------------
//多行文本框
HWND CreateMemo(char* Titel, int x0, int y0, int w, int h, int ID, HWND hW, HINSTANCE hInst)
{
	return CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", Titel,
		WS_VISIBLE | WS_CHILD |
		ES_LEFT | ES_MULTILINE | ES_READONLY |
		WS_HSCROLL | WS_VSCROLL,
		x0, y0, w, h,
		hW, (HMENU)ID, hInst, NULL);
}
//---------------------------------------------------------------------------
//添加控件文本
void Puts(HWND hW, int ID_EDIT, char* str)
{
	char b[888];
	GetDlgItemText(hW, ID_EDIT, b, sizeof(b));
	strcat(b, "\r\n");
	strcat(b, str);
	SetDlgItemText(hW, ID_EDIT, (LPSTR)b);
	SendMessage(GetDlgItem(hW, ID_EDIT), WM_VSCROLL, SB_THUMBPOSITION + 1000 * 0x10000, 0);
}
//---------------------------------------------------------------------------
void wlc(BYTE Fram);
//按钮
#define BUTTON1 501
#define BUTTON2 502
//单行编辑框
#define EDIT1 601
//多行文本框
#define MEMO1 801
#define MEMO2 802
//--------------------------------------------------------------
WSADATA ws;
SOCKET Cs1, Cs2;
struct sockaddr_in Cs1A, Cs2A;
int SendBufLeng;			//报文长度
char Sendi;					//数据帧发送计数
char aa[200];
char bb[200];
char sendback[200];
int d, i;
#define Cs2Port 7777		//远程端口
#define Cs1Port 8888		//本地端口
#define Cs1IP   "127.0.0.1"	//本地网址
#define Cs2IP   "127.0.0.1"	//远程网址
//以上不需改动!
//--------------------------------------------------------------
//你可以在这里定义变量
void wlcSend(BYTE c);	//物理层：发送一个字节
BYTE wlcRev();			//物理层：接收一个字节
bool checkByte(BYTE c);
//--------------------------------------------------------------
//消息处理
LRESULT CALLBACK WndProc(HWND hW, UINT msg, WPARAM wP, LPARAM lP)
{
	switch (msg)
	{
	case WM_DESTROY:
		WSAAsyncSelect(Cs1, hW, 0, 0);
		closesocket(Cs1);
		WSACleanup();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		WSAStartup(0x0101, &::ws);
		Cs1 = socket(AF_INET, SOCK_DGRAM, 0);
		Cs1A.sin_family = AF_INET;
		Cs1A.sin_port = htons(Cs1Port);
		//		Cs1A.sin_addr.s_addr =INADDR_ANY;//在本机设置的网址(可能多个)中，都有效。
		Cs1A.sin_addr.s_addr = inet_addr(Cs1IP);
		d = bind(Cs1, (struct sockaddr*)&Cs1A, sizeof(Cs1A));
		if (d == -1)
		{
			MessageBox(0, TEXT("端口发生冲突"), TEXT("数字信道仿真"), 0);
			PostQuitMessage(0);
		}
		Cs2A.sin_family = AF_INET;
		Cs2A.sin_port = htons(Cs2Port);
		Cs2A.sin_addr.s_addr = inet_addr(Cs2IP);
		WSAAsyncSelect(Cs1, hW, WM_USER + 1, FD_READ);
		Beep(2000, 100);
		CreateEdit(const_cast <char*>("接收端"), 20, 20, 230, 70, EDIT1, hW, hInst);
		CreateButton(const_cast <char*>("Quit"), 250, 20, 50, 60, BUTTON1, hW, hInst);
		CreateButton(const_cast <char*>("Reset"), 350, 20, 50, 60, BUTTON2, hW, hInst);
		CreateWindow("STATIC", "接收端物理层发送的信息",
			WS_VISIBLE | WS_CHILD,
			50, 100, 200, 20, hW, NULL, hInst, NULL);
		CreateMemo(const_cast <char*>(""), 50, 150, 200, 220, MEMO1, hW, hInst);

		//CreateMemo(const_cast <char*>("物理层收到的信息"), 0, 50, 90, 220, MEMO1, hW, hInst);
		CreateWindow("STATIC", "物理层上交网络层的消息",
			WS_VISIBLE | WS_CHILD,
			260, 100, 200, 20, hW, NULL, hInst, NULL);
		CreateMemo(const_cast<char*>(""), 260, 150, 200, 220, MEMO2, hW, hInst);
		//CreateMemo(const_cast <char*>("上交网络层的信息"), 100, 50, 90, 220, MEMO2, hW, hInst);

		break;
		//============================================================================
		//接收消息事件：
		//你可以在这里编写代码：
	case WM_USER + 1:
		switch (LOWORD(lP))
		{
		case FD_READ:
			
			bb[0] = wlcRev();

			
			if (checkByte(bb[0])) {
					sendback[0] = ( ((bb[0] >> 4) & 0x07)<< 4)|0x0F;
					wlcSend(sendback[0]);
				}
				else {
					sendback[0] = (bb[0] >> 4 & 0x07) << 4 | 0x0E;
					wlcSend(sendback[0]);
				}	
			
			
			bb[1] = 0;
			char buf[22];
			wsprintf(buf, "%02XH", (unsigned char)bb[0]);
			Puts(hW, MEMO1, buf);		//显示，物理层收到的信息


			bb[0] = (bb[0] & 0x0F) + 0x30;//去掉高4位内容,低4位转为ASCII码
			Puts(hW, MEMO2, bb);		//显示，上交网络层的信息
			break;
		}
		break;
		//============================================================================
	case WM_COMMAND:
		switch (wP)
		{
		case BUTTON1:				//退出程序
			PostQuitMessage(0);
			break;
		case BUTTON2:				//清除信息框内容
			SetDlgItemText(hW, MEMO1, (LPSTR)"物理层收到的信息");
			SetDlgItemText(hW, MEMO2, (LPSTR)"上交网络层的信息");
			break;
		}
	}
	return DefWindowProc(hW, msg, wP, lP);
}
//--------------------------------------------------------------
void wlcSend(BYTE c)	//物理层：发送一个字节
{
	char a[2];
	a[0] = c; a[1] = 0;
	d = sizeof(Cs2A);
	sendto(Cs1, a, strlen(a), 0, (struct sockaddr*)&Cs2A, d);
}
//--------------------------------------------------------------
BYTE wlcRev()			//物理层：接收一个字节
{
	char b[1];
	d = sizeof(Cs2A);
	d = recvfrom(Cs1, b, 1, 0, (struct sockaddr*)&Cs2A, &d);
	return b[0];
}
bool checkByte(BYTE c) //C为一个字节
{
	int i, sum = 0;
	BYTE x = 0x01;
	for (i = 0; i < 8; i++)
	{
		if (c & x) sum++;
		x = x << 1;
	}
	if (sum % 2) return false;
	return true;
}

//--------------------------------------------------------------

