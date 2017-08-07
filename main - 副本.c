// DEMO2_3.CPP - A complete windows program

// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <stdio.h>     
#include <math.h>
#include "resources.h"

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// GLOBALS ////////////////////////////////////////////////
HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app      = NULL; // globally track hinstance
char buffer[80];                     // general printing buffer
// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT		ps;		// used in WM_PAINT
HDC				hdc;	// handle to a device context
char buffer[80];        // used to print strings

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
 PlaySound(MAKEINTRESOURCE(SOUND_ID_TELEPORT), 
                               hinstance_app, SND_RESOURCE | SND_ASYNC);
		// do initialization stuff here
        // return success
		return(0);
		} break;

    case WM_MOUSEMOVE:
         {
         // get the position of the mouse
         int mouse_x = (int)LOWORD(lparam);
         int mouse_y = (int)HIWORD(lparam);

         // get the button state
         int buttons = (int)wparam;

         // get a graphics context
         hdc = GetDC(hwnd);

         // set the foreground color to green
         SetTextColor(hdc, RGB(0,255,0));
         
         // set the background color to black
         SetBkColor(hdc, RGB(0,0,0));
         
         // set the transparency mode to OPAQUE
         SetBkMode(hdc, OPAQUE);

         // print the ascii code and key state
         sprintf(buffer,"Mouse (X,Y) = (%d,%d)      ",mouse_x,mouse_y);
         TextOut(hdc, 0,0, buffer, strlen(buffer));

         sprintf(buffer,"Right Button = %d  ",((buttons & MK_RBUTTON) ? 1 : 0));
         TextOut(hdc, 0,16, buffer, strlen(buffer));

         sprintf(buffer,"Left Button = %d  ",((buttons & MK_LBUTTON) ? 1 : 0));
         TextOut(hdc, 0,32, buffer, strlen(buffer));


         // release the dc back
         ReleaseDC(hwnd, hdc);

         } break;

case WM_COMMAND:
         {
          switch(LOWORD(wparam))
                {
                // handle the FILE menu
                case MENU_FILE_ID_EXIT:
                     {
                     // terminate window
                     PostQuitMessage(0);
                     } break;

                // handle the HELP menu
                case MENU_HELP_ABOUT:                 
                     {
                     //  pop up a message box
                     MessageBox(hwnd, "Menu Sound Demo", 
                               "About Sound Menu",
                                MB_OK | MB_ICONEXCLAMATION);
                     } break;

                // handle each of sounds
                case MENU_PLAY_ID_ENERGIZE:
                     {
                     // play the sound
                     PlaySound(MAKEINTRESOURCE(SOUND_ID_ENERGIZE), 
                               hinstance_app, SND_RESOURCE | SND_ASYNC);
                     } break;

                case MENU_PLAY_ID_BEAM:
                     {
                     // play the sound
                     PlaySound(MAKEINTRESOURCE(SOUND_ID_BEAM), 
                               hinstance_app, SND_RESOURCE | SND_ASYNC);
                     } break;

                case MENU_PLAY_ID_TELEPORT:
                     {
                     // play the sound
                     PlaySound(MAKEINTRESOURCE(SOUND_ID_TELEPORT), 
                               hinstance_app, SND_RESOURCE | SND_ASYNC);
                     } break;

                case MENU_PLAY_ID_WARP:
                     {
                     // play the sound
                     PlaySound(MAKEINTRESOURCE(SOUND_ID_WARP), 
                              hinstance_app, SND_RESOURCE | SND_ASYNC);
                     } break;

                default: break;

             } // end switch wparam

          } break; // end WM_COMMAND

        case WM_CTLCOLORBTN: 
{ 
        return (LRESULT)CreateSolidBrush(RGB(0, 255, 0)); 
} break;
 
	case WM_PAINT: 
		{
		// simply validate the window 
   	    hdc = BeginPaint(hwnd,&ps);	 
        
        // end painting
        EndPaint(hwnd,&ps);

        // return success
		return(0);
   		} break;

	case WM_DESTROY: 
		{

		// kill the application, this sends a WM_QUIT message 
		PostQuitMessage(0);

        // return success
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASSEX winclass; // this will hold the class we create
HWND	   hwnd;	 // generic window handle
MSG		   msg;		 // generic message
HDC        hdc; 
// first fill in the window class stucture
winclass.cbSize         = sizeof(WNDCLASSEX);
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= "SoundMenu";
winclass.lpszClassName	= WINDOW_CLASS_NAME;
winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

// save hinstance in global
hinstance_app = hinstance;

// register the window class
if (!RegisterClassEx(&winclass))
	return(0);

// create the window
if (!(hwnd = CreateWindowEx(WS_EX_APPWINDOW,                  // extended style
                            WINDOW_CLASS_NAME,     // class
						    "Mouse Tracking Demo", // title
						    WS_POPUP | WS_VISIBLE,
					 	    0,0,	  // initial x,y
						    GetSystemMetrics(SM_CXSCREEN),
                                                    GetSystemMetrics(SM_CYSCREEN),  // initial width, height
						    NULL,	  // handle to parent 
						    NULL,	  // handle to menu
						    hinstance,// instance of this application
						    NULL)))	// extra creation parms
return(0);

// save main window handle
main_window_handle = hwnd;
#define BUTTON_BASE_ID   100
#define NUM_BUTTONS      9

char *button_names[NUM_BUTTONS] = 
     {
     "PUSHBUTTON", 	
     "RADIOBUTTON", 	
     "CHECKBOX", 	
     "3STATE",  	
     "AUTO3STATE", 	
     "AUTOCHECKBOX", 	
     "AUTORADIOBUTTON",	
     "OWNERDRAW",
     "BS_ICON"};

long button_types[NUM_BUTTONS] = 
     {
     BS_PUSHBUTTON, 	
     BS_RADIOBUTTON, 	
     BS_CHECKBOX, 	
     BS_3STATE,  	
     BS_AUTO3STATE,
     BS_AUTOCHECKBOX, 	
     BS_AUTORADIOBUTTON,
     BS_OWNERDRAW,
     BS_ICON
     }; 
for (int button = 0; button < NUM_BUTTONS-1; button++)
{
CreateWindowEx(WS_EX_APPWINDOW,                  // extended style
               "button",              // class
			    button_names[button], // title
	   	        WS_CHILD | WS_VISIBLE | button_types[button],
				200,10+button*36,	  // initial x,y
				strlen(button_names[button])*16,24,    // initial width, height
			    main_window_handle,	  // handle to parent 
			    (HMENU)(BUTTON_BASE_ID + button),	  // handle to menu
			    hinstance,  // instance of this application
			    NULL);      // extra creation parms
} // end for button
static HWND button0;  
button0 =CreateWindowEx(WS_EX_APPWINDOW,                  // extended style
               "button",              // class
			    button_names[NUM_BUTTONS-1], // title
	   	        WS_CHILD | WS_VISIBLE | button_types[NUM_BUTTONS-1],
				200,10+NUM_BUTTONS*36,	  // initial x,y
				strlen(button_names[NUM_BUTTONS-1])*16,24,    // initial width, height
			    main_window_handle,	  // handle to parent 
			    (HMENU)(BUTTON_BASE_ID + NUM_BUTTONS),	  // handle to menu
			    hinstance,  // instance of this application
			    NULL);      // extra creation parms
SendMessage(button0, BM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(NULL, IDI_ERROR)); 
// enter main event loop, but this time we use PeekMessage()
// instead of GetMessage() to retrieve messages
while(TRUE)
	{
    // test if there is a message in queue, if so get it
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	   { 
	   // test if this is a quit
       if (msg.message == WM_QUIT)
           break;
	
	   // translate any accelerator keys
	   TranslateMessage(&msg);

	   // send the message to the window proc
	   DispatchMessage(&msg);
	   } // end if
      hdc = GetDC(hwnd);

       // set the foreground color to green
       SetTextColor(hdc, RGB(0,255,0));
         
       // set the background color to black
       SetBkColor(hdc, RGB(0,0,0));
         
       // set the transparency mode to OPAQUE
       SetBkMode(hdc, OPAQUE);
// print out the state of each arrow key
       sprintf(buffer,"Up Arrow: = %d   ",KEYDOWN(VK_UP));
       TextOut(hdc, 0,48, buffer, strlen(buffer));

       sprintf(buffer,"Down Arrow: = %d   ",KEYDOWN(VK_DOWN));
       TextOut(hdc, 0,64, buffer, strlen(buffer));

       sprintf(buffer,"Right Arrow: = %d   ",KEYDOWN(VK_RIGHT));
       TextOut(hdc, 0,80, buffer, strlen(buffer));

       sprintf(buffer,"Left Arrow: = %d   ",KEYDOWN(VK_LEFT));
       TextOut(hdc, 0,96, buffer, strlen(buffer));
       

       // release the dc back
       ReleaseDC(hwnd, hdc);
    // main game processing goes here
	// Game_Main(); // or whatever your loop is called
	} // end while

// return to Windows like this
return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////
