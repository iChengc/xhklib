#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <X11/Xlib.h>

#include "xhklib.h"
#include "unix_hot_keys.h"

int registerUnixHotKeys(void(*callbck)(xhkEvent, void *, void *, void *))
{  
    xhkConfig *hkconfig;
    hkconfig = xhkInit(NULL);

    xhkBindKey(hkconfig, 0, /*XK_Shift_L*/XK_Up, 0, xhkKeyPress, callbck, 0, 0, 0);

    xhkBindKey(hkconfig, 0, XK_Left, 0, xhkKeyPress, callbck, 0, 0, 0);

    xhkBindKey(hkconfig, 0, XK_Right, 0, xhkKeyPress, callbck, 0, 0, 0);

    xhkBindKey(hkconfig, 0, XK_S, 0, xhkKeyPress, callbck, 0, 0, 0);

    while (1) {
        xhkPollKeys(hkconfig, 1);
    }

    xhkClose(hkconfig);
}

static void sendKey (KeySym keysym, KeySym modsym) {
	Display * disp = XOpenDisplay(NULL); 
 	KeyCode keycode = 0, modcode = 0;
 	keycode = XKeysymToKeycode (disp, keysym);
 	if (keycode == 0) return; //init value

 	/* Generate modkey press */
 	if (modsym != 0) {
  		modcode = XKeysymToKeycode(disp, modsym);
  		XTestFakeKeyEvent (disp, modcode, True, 0);
 		XFlush(disp);
 	}
 	/* Generate regular key press and release */
 	XTestFakeKeyEvent (disp, keycode, True, 0);
 	usleep(500000);
    XFlush(disp);
	XTestFakeKeyEvent (disp, keycode, False, 0); 
 
 	/* Generate modkey release */
 	if (modsym != 0) {
 	 	XTestFakeKeyEvent (disp, modcode, False, 0);
 	}
 	XFlush(disp);
 	XCloseDisplay(disp);
}

void sendPasteKeyEvent() {
	sendKey(XK_V, XK_Control_L);
}