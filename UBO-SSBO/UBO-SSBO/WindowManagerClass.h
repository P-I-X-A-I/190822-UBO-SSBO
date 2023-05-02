#pragma once

#include "NotificationCenterClass.h"

#define MAX_WINDOW_NUM 64

LRESULT CALLBACK winCallBack(HWND, UINT, WPARAM, LPARAM);


class WindowManagerClass
{
public:
	WindowManagerClass();
	~WindowManagerClass();

	// VARIABLES
	int num_of_window;
	HWND windowHandler[MAX_WINDOW_NUM];
	int current_x[MAX_WINDOW_NUM];
	int current_y[MAX_WINDOW_NUM];
	int current_width[MAX_WINDOW_NUM];
	int current_height[MAX_WINDOW_NUM];
	HWND window_level[MAX_WINDOW_NUM];


	NotificationCenterClass* notification_center;


	// METHOD
	HWND createNewWindow(LPCWSTR className, LPCWSTR title, int mode);

	HWND create_nomenu_window(LPCWSTR className, LPCWSTR title);
	HWND create_borderless_window(LPCWSTR className);
	HWND create_panel_window(LPCWSTR className, LPCWSTR title);

	void set_window_size(HWND wnd, int width, int height);
	void set_window_position(HWND wnd, int x, int y);
	void set_window_level(HWND wnd, int level);

	void close_window(HWND wnd);
	void show_window(HWND wnd);
	void minimize_window(HWND wnd);
	void maximize_window(HWND wnd);
	void hide_window(HWND wnd);

	void set_notification_center(NotificationCenterClass* nc);

private:
	void initWindowParam(int index);
};

