#include "stdafx.h"
#include "WindowManagerClass.h"

WindowManagerClass* thisManager_obj;
NotificationCenterClass* nc_obj = NULL;

WindowManagerClass::WindowManagerClass()
{
	printf("WindowManagerClass init\n");

	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		windowHandler[i] = NULL;

		this->initWindowParam(i);
	}
	num_of_window = 0;

	thisManager_obj = this;

	notification_center = NULL;
}


WindowManagerClass::~WindowManagerClass()
{
}


void WindowManagerClass::set_notification_center(NotificationCenterClass* nc)
{
	notification_center = nc;
	nc_obj = nc;
}

void WindowManagerClass::initWindowParam( int index )
{
	current_x[index] = 100;
	current_y[index] = 100;
	current_width[index] = 300;
	current_height[index] = 300;
	window_level[index] = HWND_TOP;
}

HWND WindowManagerClass::createNewWindow(LPCWSTR className, LPCWSTR title, int mode)
{
		// flag & blush
	long FLAG;
	HBRUSH bgBrush;

	switch (mode)
	{
	case 0: // no menu window
		FLAG = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
		bgBrush = CreateSolidBrush(RGB(255, 255, 255));
		break;
	case 1: // borderless window
		FLAG = WS_SYSMENU | WS_POPUP;
		bgBrush = CreateSolidBrush(RGB(255, 255, 255));
		break;
	case 2: // panel window
		FLAG = WS_OVERLAPPED;
		bgBrush = CreateSolidBrush(RGB(210, 210, 210));
		break;
	default:
		FLAG = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
		bgBrush = CreateSolidBrush(RGB(255, 255, 255));
		break;
	}

	// register class
	WNDCLASSEXW wcex;
	HINSTANCE appInstance = GetModuleHandle(NULL); // application handler

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // redraw flag
	wcex.lpfnWndProc = winCallBack;	//callback to catch event
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = appInstance; // application handler
	wcex.hIcon = LoadIcon(appInstance, MAKEINTRESOURCE(107)); // 107 means default, IDI_APPNAME defined in Resource.h
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH)bgBrush;
	//wcex.lpszMenuName = MAKEINTRESOURCEW(109); // 109 means default, IDC_APPNAME defined in Resource.h
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108)); // 108 means small icon defined in Resource.h

	RegisterClassExW(&wcex);


	// create window
	HWND returnWindow = CreateWindowW(
		className,
		title,
		FLAG,
		current_x[num_of_window],
		current_y[num_of_window],
		current_width[num_of_window],
		current_height[num_of_window],
		nullptr, // parent window
		nullptr, // menu
		appInstance,
		nullptr // lParam
	);

	// set handler to variable
	windowHandler[num_of_window] = returnWindow;
	// add num of window
	num_of_window++;
	if (num_of_window >= MAX_WINDOW_NUM)
	{
		num_of_window = 0;
	}

	
	// show window
	ShowWindow(returnWindow, SW_SHOW);
	UpdateWindow(returnWindow);

	return returnWindow;

}


HWND WindowManagerClass::create_nomenu_window(LPCWSTR className, LPCWSTR title)
{
	if (windowHandler[num_of_window] != NULL)
	{
		DestroyWindow(windowHandler[num_of_window]);
		this->initWindowParam(num_of_window);
	}

	return createNewWindow(className, title, 0);
}

HWND WindowManagerClass::create_borderless_window(LPCWSTR className)
{
	if (windowHandler[num_of_window] != NULL)
	{
		DestroyWindow( windowHandler[num_of_window] );
		this->initWindowParam(num_of_window);
	}

	return createNewWindow(className, NULL, 1);
}


HWND WindowManagerClass::create_panel_window(LPCWSTR className, LPCWSTR title)
{
	if (windowHandler[num_of_window] != NULL)
	{
		DestroyWindow(windowHandler[num_of_window]);
		this->initWindowParam(num_of_window);
	}

	return createNewWindow(className, title, 2);
}



//***********************************************************************


void WindowManagerClass::set_window_size(HWND wnd, int width, int height)
{
	int INDEX = 10000;

	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			INDEX = i;
			break;
		}
	}

	if(INDEX == 10000)
	{
		printf("the window is not registered in WindowManager. return\n");
		return;
	}

	HWND targetWnd = windowHandler[INDEX];
	current_width[INDEX] = width;
	current_height[INDEX] = height;

	SetWindowPos(
		targetWnd,
		window_level[INDEX],
		current_x[INDEX],
		current_y[INDEX],
		current_width[INDEX],
		current_height[INDEX],
		SWP_SHOWWINDOW
	);
}


void WindowManagerClass::set_window_position(HWND wnd, int x, int y)
{
	int INDEX = 10000;

	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			INDEX = i;
			break;
		}
	}

	if (INDEX == 10000)
	{
		printf("the window is not registered in WindowManager. return\n");
		return;
	}

	HWND targetWnd = windowHandler[INDEX];
	current_x[INDEX] = x;
	current_y[INDEX] = y;

	SetWindowPos(
		targetWnd,
		window_level[INDEX],
		current_x[INDEX],
		current_y[INDEX],
		current_width[INDEX],
		current_height[INDEX],
		SWP_SHOWWINDOW
	);
}


void WindowManagerClass::set_window_level(HWND wnd, int level)
{
	int INDEX = 10000;

	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			INDEX = i;
			break;
		}
	}

	if (INDEX == 10000)
	{
		printf("the window is not registered in WindowManager. return\n");
		return;
	}

	switch (level)
	{
	case 0:
		window_level[INDEX] = HWND_BOTTOM;
		break;
	case 1:
		window_level[INDEX] = HWND_TOP;
		break;
	case 2:
		window_level[INDEX] = HWND_TOPMOST;
		break;
	default:
		window_level[INDEX] = HWND_TOP;
		break;
	}

	HWND targetWnd = windowHandler[INDEX];

	SetWindowPos(
		targetWnd,
		window_level[INDEX],
		current_x[INDEX],
		current_y[INDEX],
		current_width[INDEX],
		current_height[INDEX],
		SWP_SHOWWINDOW
	);
}


//******************************************************************


void WindowManagerClass::close_window(HWND wnd)
{

	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			DestroyWindow(windowHandler[i]);
			windowHandler[i] = NULL;
			this->initWindowParam(i);

			return;
		}
	}

	printf("the window is not registerd in WindowManager. return\n");
	return;
}


void WindowManagerClass::show_window(HWND wnd)
{
	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			ShowWindow(windowHandler[i], SW_SHOWNORMAL);
			return;
		}
	}

	printf("the window is not registerd in WindowManager. return\n");
	return;
}


void WindowManagerClass::minimize_window(HWND wnd)
{
	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			ShowWindow(windowHandler[i], SW_MINIMIZE);
			return;
		}
	}

	printf("the window is not registerd in WindowManager. return\n");
	return;
}


void WindowManagerClass::maximize_window(HWND wnd)
{
	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			ShowWindow(windowHandler[i], SW_MAXIMIZE);
			return;
		}
	}

	printf("the window is not registerd in WindowManager. return\n");
	return;
}



void WindowManagerClass::hide_window(HWND wnd)
{
	for (int i = 0; i < MAX_WINDOW_NUM; i++)
	{
		if (wnd == windowHandler[i])
		{
			ShowWindow(windowHandler[i], SW_HIDE);
			return;
		}
	}

	printf("the window is not registerd in WindowManager. return\n");
	return;
}
//***********************************************************




LRESULT CALLBACK winCallBack(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst = GetModuleHandle(NULL); // application handler;

	// for window creation
	CREATESTRUCT* createStruct = (CREATESTRUCT*)lParam;

	// for window size restriction
	MINMAXINFO* minmaxStruct = (MINMAXINFO*)lParam;
	RECT winRect;


	// messega from children GUI
	int GUI_ID, EVENT_CODE;
	GUI_ID = LOWORD(wParam);
	EVENT_CODE = HIWORD(wParam);

	if (nc_obj != NULL)
	{
		nc_obj->messageFromGUI(message, GUI_ID, EVENT_CODE, lParam);
	}


	switch (message)
	{
	case WM_CREATE:

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
			//case IDM_ABOUT: // IDM_ABOUT, defined in Resource.h
			//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			//	break;
			//case IDM_EXIT:
			//	DestroyWindow(hWnd);
			//	break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: HDC を使用する描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO: // limit window size
			//GetWindowRect(hWnd, &winRect );
			//minmaxStruct->ptMaxPosition.x = winRect.left;
			//minmaxStruct->ptMaxPosition.y = winRect.top;
			//minmaxStruct->ptMinTrackSize.x = winRect.right - winRect.left;
			//minmaxStruct->ptMinTrackSize.y = winRect.bottom - winRect.top;
			//minmaxStruct->ptMaxTrackSize.x = winRect.right - winRect.left;
			//minmaxStruct->ptMaxTrackSize.y = winRect.bottom - winRect.top;
		break;

	case WM_KEYDOWN:
		if (nc_obj != NULL)
		{
			nc_obj->keyEvent(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYUP:
		if (nc_obj != NULL)
		{
			nc_obj->keyEvent(hWnd, message, wParam, lParam);
		}
		break;

	case WM_MOUSEMOVE:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONUP:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;
	case WM_RBUTTONUP:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDBLCLK:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;
	case WM_RBUTTONDBLCLK:
		if (nc_obj != NULL)
		{
			nc_obj->mouseEvent(hWnd, message, wParam, lParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

