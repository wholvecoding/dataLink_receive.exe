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

	hWnd = CreateWindow(TEXT("W1"), TEXT("���ն�[8888]����"),
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
//��ť
HWND CreateButton(char* Titel, int x0, int y0, int w, int h, int ID, HWND hW, HINSTANCE hInst)
{
	return CreateWindowEx(WS_EX_PALETTEWINDOW, TEXT("BUTTON"), Titel,
		WS_VISIBLE | WS_CHILD,
		x0, y0, w, h, hW, (HMENU)ID, hInst, NULL);
}
//--------------------------------------------------------------
//���б༭��
HWND CreateEdit(char* Titel, int x0, int y0, int w, int h, int ID, HWND hW, HINSTANCE hInst)
{
	return CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", Titel,
		WS_VISIBLE | WS_CHILD |
		ES_LEFT | ES_MULTILINE | WS_HSCROLL,
		x0, y0, w, h, hW,
		(HMENU)ID, hInst, NULL);
}
//--------------------------------------------------------------
//�����ı���
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
//��ӿؼ��ı�
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
//��ť
#define BUTTON1 501
#define BUTTON2 502
//���б༭��
#define EDIT1 601
//�����ı���
#define MEMO1 801
#define MEMO2 802
//--------------------------------------------------------------
WSADATA ws;
SOCKET Cs1, Cs2;
struct sockaddr_in Cs1A, Cs2A;
int SendBufLeng;			//���ĳ���
char Sendi;					//����֡���ͼ���
char aa[200];
char bb[200];
char sendback[200];
int d, i;
#define Cs2Port 7777		//Զ�̶˿�
#define Cs1Port 8888		//���ض˿�
#define Cs1IP   "127.0.0.1"	//������ַ
#define Cs2IP   "127.0.0.1"	//Զ����ַ
//���ϲ���Ķ�!
//--------------------------------------------------------------
//����������ﶨ�����
void wlcSend(BYTE c);	//����㣺����һ���ֽ�
BYTE wlcRev();			//����㣺����һ���ֽ�
bool checkByte(BYTE c);
//--------------------------------------------------------------
//��Ϣ����
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
		//		Cs1A.sin_addr.s_addr =INADDR_ANY;//�ڱ������õ���ַ(���ܶ��)�У�����Ч��
		Cs1A.sin_addr.s_addr = inet_addr(Cs1IP);
		d = bind(Cs1, (struct sockaddr*)&Cs1A, sizeof(Cs1A));
		if (d == -1)
		{
			MessageBox(0, TEXT("�˿ڷ�����ͻ"), TEXT("�����ŵ�����"), 0);
			PostQuitMessage(0);
		}
		Cs2A.sin_family = AF_INET;
		Cs2A.sin_port = htons(Cs2Port);
		Cs2A.sin_addr.s_addr = inet_addr(Cs2IP);
		WSAAsyncSelect(Cs1, hW, WM_USER + 1, FD_READ);
		Beep(2000, 100);
		CreateEdit(const_cast <char*>("���ն�"), 20, 20, 230, 70, EDIT1, hW, hInst);
		CreateButton(const_cast <char*>("Quit"), 250, 20, 50, 60, BUTTON1, hW, hInst);
		CreateButton(const_cast <char*>("Reset"), 350, 20, 50, 60, BUTTON2, hW, hInst);
		CreateWindow("STATIC", "���ն�����㷢�͵���Ϣ",
			WS_VISIBLE | WS_CHILD,
			50, 100, 200, 20, hW, NULL, hInst, NULL);
		CreateMemo(const_cast <char*>(""), 50, 150, 200, 220, MEMO1, hW, hInst);

		//CreateMemo(const_cast <char*>("������յ�����Ϣ"), 0, 50, 90, 220, MEMO1, hW, hInst);
		CreateWindow("STATIC", "������Ͻ���������Ϣ",
			WS_VISIBLE | WS_CHILD,
			260, 100, 200, 20, hW, NULL, hInst, NULL);
		CreateMemo(const_cast<char*>(""), 260, 150, 200, 220, MEMO2, hW, hInst);
		//CreateMemo(const_cast <char*>("�Ͻ���������Ϣ"), 100, 50, 90, 220, MEMO2, hW, hInst);

		break;
		//============================================================================
		//������Ϣ�¼���
		//������������д���룺
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
			Puts(hW, MEMO1, buf);		//��ʾ��������յ�����Ϣ


			bb[0] = (bb[0] & 0x0F) + 0x30;//ȥ����4λ����,��4λתΪASCII��
			Puts(hW, MEMO2, bb);		//��ʾ���Ͻ���������Ϣ
			break;
		}
		break;
		//============================================================================
	case WM_COMMAND:
		switch (wP)
		{
		case BUTTON1:				//�˳�����
			PostQuitMessage(0);
			break;
		case BUTTON2:				//�����Ϣ������
			SetDlgItemText(hW, MEMO1, (LPSTR)"������յ�����Ϣ");
			SetDlgItemText(hW, MEMO2, (LPSTR)"�Ͻ���������Ϣ");
			break;
		}
	}
	return DefWindowProc(hW, msg, wP, lP);
}
//--------------------------------------------------------------
void wlcSend(BYTE c)	//����㣺����һ���ֽ�
{
	char a[2];
	a[0] = c; a[1] = 0;
	d = sizeof(Cs2A);
	sendto(Cs1, a, strlen(a), 0, (struct sockaddr*)&Cs2A, d);
}
//--------------------------------------------------------------
BYTE wlcRev()			//����㣺����һ���ֽ�
{
	char b[1];
	d = sizeof(Cs2A);
	d = recvfrom(Cs1, b, 1, 0, (struct sockaddr*)&Cs2A, &d);
	return b[0];
}
bool checkByte(BYTE c) //CΪһ���ֽ�
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

