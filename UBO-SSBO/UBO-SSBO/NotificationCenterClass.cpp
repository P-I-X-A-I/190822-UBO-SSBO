#include "stdafx.h"
#include "NotificationCenterClass.h"


NotificationCenterClass::NotificationCenterClass()
{
}


NotificationCenterClass::~NotificationCenterClass()
{
}


void NotificationCenterClass::messageFromGUI(UINT message, int GUI_ID, int EVENT_CODE, LPARAM lParam)
{
	// message from GUI

	int HI = HIWORD(lParam);
	int LO = HIWORD(lParam);

	NMHDR* nmHandler = (NMHDR*)lParam; // for WM_NOTIFY message

	switch (GUI_ID)
	{
	case 1000:
		// if it "button"
		if (EVENT_CODE == BN_CLICKED) // when clicked
		{
			printf("button clicked\n");
		}
		break;
	case 2000:
		// if it "slider"
		if (nmHandler->code == 0xfffffff0)
		{
			printf("released %d\n", EVENT_CODE);
		}
		else if (nmHandler->code == 0xfffffff4)
		{
			// should not use here, 
			printf("moved %d\n", EVENT_CODE);
		}
		break;
	case 7000:
		// if it "combobox"
		if (EVENT_CODE == CBN_DROPDOWN) // when open
		{
			printf("dropdown\n");
		}
		else if (EVENT_CODE == CBN_CLOSEUP) // when close 
		{
			printf("close up\n");
		}
		else if (EVENT_CODE == CBN_SELCHANGE) // when sel change
		{
			printf("select change\n");
		}
		else if (EVENT_CODE == CBN_SETFOCUS) // when get focused
		{
			printf("set focused\n");
		}
		else if (EVENT_CODE == CBN_KILLFOCUS) // when lost focuse
		{
			printf("lost focus\n");
		}
		break;
	case 8000:
		// if it listbox

		break;
	default:
		break;
	}
}


void NotificationCenterClass::keyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// key event from WindowManager
	//printf("key event ");
}


void NotificationCenterClass::mouseEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// mouse event form WindowManager
	//printf("mouse event ");
}



//***************************************

void NotificationCenterClass::receiveOSC(char* address)
{
	if (strcmp(address, "registeredAddress") == 0)
	{
		// do something
	}
	//printf("Notification : %s\n", address);
}
