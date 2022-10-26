// kkkkkkkk.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <curl/curl.h>  
#include <stdio.h>
#include <string.h>
#include <tlhelp32.h>
#include "SDLAPI.h"
#include <cmath>

using namespace std;
HINSTANCE g_hInstance = 0;//全局窗口句柄变量


//画图函数
void Onpen(HWND hWnd)
{

	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);

	// 创建画笔
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
	// 应用到DC中
	HGDIOBJ nOldPen = SelectObject(hdc, hPen);

	// 绘图
	// 绘制点(圆心)
	//SetPixel(hdc,350,250,RGB(255,0,0));

	HGDIOBJ hBrush = GetStockObject(NULL_BRUSH);
	HGDIOBJ nOldBrush = SelectObject(hdc, hBrush);
	// 绘制矩形
	Rectangle(hdc, 0, 0, 200, 800);




	SelectObject(hdc, nOldPen);
	DeleteObject(hPen);
	//DeleteObject(hPen2);

	EndPaint(hWnd, &ps);

}

/*
 2 窗口处理函数（用户自定义 处理消息）
*/


LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID,
	WPARAM wParam, LPARAM lParam)
{
	switch (msgID)
	{
	case WM_PAINT:
		Onpen(hWnd);
		break;
	case WM_DESTROY:
		//使得GetMessage返回0
		PostQuitMessage(0);
		break;
	}
	//给各种消息做默认处理
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}


/*
  3 注册窗口类
*/

void Register(LPCTSTR lpClassName, WNDPROC wndPrco)
{
	//注册窗口的结构体
	WNDCLASSEX wce = { 0 };
	//cbSize WNDCLASSEX的大小
	wce.cbSize = sizeof(wce);
	/*
	    windows程序为每一个窗口设计类管理一个WNDCLASS结构.
	    在应用程序注册一个窗口类的时候，可以让windows分配一定字节空间的内存，
	    这部分内存成为类的附件内存，有属于这个窗口类的所有窗口共享，
	    类附件内存信息用于存储窗口类的附加信息。windows系统将这部分内存初始化为0，
	    因此我们经常设置此参数为0.
	*/
	wce.cbClsExtra = 0;
	/*
	    windows程序为每一个窗口管理一个内部数据结构，在注册窗口类的时候，
	    系统可以为每一个窗口分配一定的字节数的附加内存空间，称为窗口附件内存。
	    应用程序可使用这部分内存存储窗口特有的数据，
	    windows系统把这部分内存初始化为0.
	*/
	wce.cbWndExtra = 0;
	//主窗口的背景颜色的设置
	wce.hbrBackground = CreateSolidBrush(RGB(220, 200, 220));
	//光标句柄
	wce.hCursor = NULL;
	/*
	    hIconSm是窗口标题栏的图标，hIcon是应用程序的图标
	*/
	//图标的句柄
	wce.hIcon = NULL;
	wce.hIconSm = NULL;
	//全局变量句柄的赋值
	wce.hInstance = g_hInstance;
	//消息处理的过程
	wce.lpfnWndProc = WndProc;
	//窗口类的名称
	wce.lpszClassName = lpClassName;
	//窗口菜单资源名称
	wce.lpszMenuName = NULL;
	//窗口的风格
	wce.style = CS_HREDRAW | CS_VREDRAW;
	//该函数为随后在调用Createwindow函数和CreatewindowEx函数中使用的窗口注册一个窗口类。
	RegisterClassEx(&wce);
}


/*
4 创建窗口
*/
HWND CreateMain(LPCTSTR lpClassName, LPCTSTR lpWndName)
{
	//创建窗口函数
	HWND hWnd = CreateWindowEx(0, lpClassName, lpWndName, WS_OVERLAPPEDWINDOW,
		100, 100, 700, 500, NULL, NULL, g_hInstance, NULL);
	return hWnd;
}

/*
5 显示窗口
*/
void Display(HWND hWnd)
{
	//显示窗口
	ShowWindow(hWnd, SW_SHOW);
	

	ISDRmcInstance * pInstance;
	bool ret = false;
	SDWLResult res;
	ISDRmUser *puser = NULL;
	string secr = "{6829b159-b9bb-42fc-af19-4a6af3c9fcf6}";
	ISDRmTenant *pTenant = NULL;
	const std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> font_color
		= std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>(255, 255, 255, 0);

	HWND hTemp = hWnd;
	if (hTemp == NULL) {
		cout << "HWND is null" << endl;
	}
	RECT rect;
	::GetWindowRect(hTemp, &rect);
	int w = rect.right - rect.left, h = rect.bottom - rect.top;
	cout << "宽:" << w << " " << "高:" << h << endl;
	const std::wstring font_name = L"Arial";
	wstring overlay_text = L"!~@#$%^&*()_+=-{}[]:'<>,\"`.?\t/   1234567890\n http://bugs.cn.nextlabs.com/show_bug.cgi?id=57732 \n ABCD efgh 宽 高 繁体字";
	int font_size = 15;  // recomment [10,100]
	int font_rotation = 0;
	int font_sytle = 2;   // 0:regular, 1:bold, 2 italic 3 bold_italic
	int text_alignment = 0;   //  0 left, 1 centre, 2 right 
	const std::tuple<int, int, int, int> display_offset = { 0,1,0,0 };
	res = RPMGetCurrentLoggedInUser(secr, pInstance, pTenant, puser);
	res = pInstance->RPMSetViewOverlay(hTemp, overlay_text, font_color, font_name, font_size, font_rotation, font_sytle, text_alignment, display_offset);
	cout << "RPMSetViewOverlay return code: " << res.GetCode() << " msg: " << res.GetMsg() << endl;

	//更新操作
	UpdateWindow(hWnd);

}


/*
6 消息循环
*/
void Message()
{

	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))
	{
		TranslateMessage(&nMsg);
		DispatchMessage(&nMsg);
	}
}


/*
1 WinMain函数
*/


int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns,
	LPSTR lpCmdLine, int nCmdShow)
{
	wstring a = L"Main";
	LPCTSTR b = a.c_str();
	wstring w = L"window";
	//当前句柄的一个实例
	g_hInstance = hIns;
	//注册窗口类
	Register(b, WndProc);
	//创建窗口
	HWND hWnd = CreateMain(b, w.c_str());
	//显示窗口
	Display(hWnd);
	//消息循环
	Message();

}
