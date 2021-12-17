// 系统托盘Test2.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "系统托盘Test2.h"
#include"shellapi.h"//实现托盘用

#define MAX_LOADSTRING 100
#define WM_NOTIFYICON WM_USER + 0x01//宏定义自定义消息处理，点击托盘图标时的消息

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//函数
int ShowVideoWapper(LPCWSTR lpParameter, LPCWSTR ffplayPath); 
int CloseVideoWapper();
int Do(LPCWSTR lpParameter, LPCWSTR ffplayPath);

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEST2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST2));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEST2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 650, 300, nullptr, nullptr, hInstance, nullptr);

   //定义一个托盘对象
   NOTIFYICONDATA nid = { 0 };//托盘图标

   nid.cbSize = sizeof(NOTIFYICONDATA);//长度
   nid.hWnd = hWnd;//窗口句柄
   nid.uID = IDI_SMALL;//图标的ID,这个在资源文件里面的Icon里面
   nid.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));//加载图标进去
   nid.uCallbackMessage = WM_NOTIFYICON;//这个是点击图标的事件消息，我们在消息处理函数WndProc（）里面写一个case WM_NOTIFYICON:去处理鼠标点击图标事件
   nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;//图标样式
   wcscpy_s(nid.szTip, _T("视频壁纸"));//当鼠标移动到图标的时候显示

   Shell_NotifyIcon(NIM_ADD, &nid); //这个函数的意思就是告诉系统增加一个托盘图标，这时候，就可以看到右下角有图标了

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPCWSTR ffplayPath = L"";
    LPCWSTR lpParameter = L"";
    switch (message)
    {
    case WM_CREATE: 
    {
        CreateWindow(
            TEXT("button"),
            TEXT("关闭"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            250, 50, 200, 100,
            hWnd,
            (HMENU)1,//该按钮的句柄，用于识别
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL);

        CreateWindow(
            TEXT("button"),
            TEXT("启动视频壁纸"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 50, 200, 100,
            hWnd,
            (HMENU)2,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL);

        CreateWindow(
            TEXT("button"),
            TEXT("最小化"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            500, 200, 100, 30,
            hWnd,
            (HMENU)3,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL);

        static HWND hEditUsername = CreateWindow(//地址输入框
            TEXT("edit"), 
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*边框*/ | ES_AUTOHSCROLL /*水平滚动*/,
            10, 10, 450, 30,
            hWnd, 
            (HMENU)4, 
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL);
        static HFONT hFont = CreateFont(//逻辑字体
            -14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑")
        );
        SendMessage(hEditUsername, WM_SETFONT, (WPARAM)hFont, NULL);
    }
    case WM_NOTIFYICON:
        if ((wParam == IDI_SMALL) && (lParam == WM_LBUTTONDOWN)) // 鼠标左键按下时响应
        {
            ShowWindow(hWnd, 10);//当点击鼠标的时候显示窗口
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case 1:
                CloseVideoWapper();
                break;
            case 2:
                //MessageBox(NULL, TEXT("Command Button"), TEXT("Button_Click()"), MB_OK);
                //电脑上ffplay的路径
                ffplayPath = L"D:\\APP\\ffmpeg\\ffmpeg-4.4.1-essentials_build\\bin\\ffplay.exe";
                // 视频路径、1920和1080，要根据实际情况改。建议使用GetSystemMetrics函数获取分辨率属性
                lpParameter = L" D:\\影音\\番\\test1.mp4 -noborder -x 1920 -y 1080 -volume 10 -loop 0 -exitonkeydown";
                ShowVideoWapper(lpParameter, ffplayPath);
                break;
            case 3:
                ShowWindow(hWnd, SW_HIDE);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY://右上角X的消息
        CloseVideoWapper();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
