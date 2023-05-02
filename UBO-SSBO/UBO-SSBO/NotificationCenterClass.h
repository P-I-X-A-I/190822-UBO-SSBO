#pragma once
class NotificationCenterClass
{
public:
	NotificationCenterClass();
	~NotificationCenterClass();

	// from WindowManager
	void messageFromGUI(UINT message, int GUI_ID, int EVENT_CODE, LPARAM lParam);
	void keyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void mouseEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// from OSC
	void receiveOSC(char* address);
};

