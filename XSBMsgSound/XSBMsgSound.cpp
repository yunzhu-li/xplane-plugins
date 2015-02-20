
#include "stdafx.h"

#include "XPLMPlugin.h"
#include "XSBComDefs.h"
#include "XPLMDataAccess.h"
#include "XPLMMenus.h"
#include "XPLMUtilities.h"
#include <stdio.h>
#include <Mmsystem.h>

PLUGIN_API int 	XPluginStart(char * outName, char * outSig, char * outDesc);
PLUGIN_API void	XPluginStop(void);
PLUGIN_API int XPluginEnable(void);
PLUGIN_API void XPluginDisable(void);
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, long inMsg, void * inParam);

XPLMPluginID	gPlugin;
XPLMMenuID		gMenu;

typedef	void (* SimpleFunc)();

void	MenuHandler(void *, void *);

void DoSubscribe()
{
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_SUBSCRIBE, (void *) (XSB_NETWORK | XSB_AUDIO | XSB_WEATHER | XSB_TEXT));

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgSound: Started.", 0, 23);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);
}

void DoUnsubscribe()
{
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_UNSUBSCRIBE, (void *) (XSB_NETWORK | XSB_AUDIO | XSB_WEATHER | XSB_TEXT));

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgSound: Stopped.", 0, 23);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);
}

void DoSetCallsign()
{
	XPLMSetDatab(XPLMFindDataRef(XSB_CON_CALLSIGN), (void *) "AAL001", 0, 7);
}

void DoAbout()
{
	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "--", 0, 3);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgSound 0.1.1 [FatLYZ].", 0, 29);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "Visit http://fatlyz.com", 0, 24);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "--", 0, 3);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);
}	

static void PlaySound()
{
	PlaySound(TEXT("XSBMsgSound.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc)
{
    strcpy(outName, "XSBMsgSound [0.1.1]");
    strcpy(outSig, "fatlyz.xsb_msg_sound");
	strcpy(outDesc, "A plugin that plays sound when receiving XSquawkBox messages. [Credit: FatLYZ]");

	gMenu = XPLMCreateMenu("XSBMsgSound", 
					XPLMFindPluginsMenu(), 
					XPLMAppendMenuItem(XPLMFindPluginsMenu(), "XSBMsgSound", NULL, 1),
					MenuHandler,
					NULL);

	XPLMAppendMenuItem(gMenu, "Start", DoSubscribe, 1);
	XPLMAppendMenuItem(gMenu, "Stop", DoUnsubscribe, 1);
	XPLMAppendMenuSeparator(gMenu);
	XPLMAppendMenuItem(gMenu, "About", DoAbout, 1);
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	//DoUnsubscribe();
}

PLUGIN_API int XPluginEnable(void)
{
	gPlugin = XPLMFindPluginBySignature("vatsim.protodev.clients.xsquawkbox");
	return 1;
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, long inMsg, void * inParam)
{

	int		frqs[10];
	int		count;
	char	buf_msg[500];
	char	buf_callsign[100];

	switch(inMsg) {
		case XSB_MSG_TEXT:

			// Get callsign
			buf_callsign[XPLMGetDatab(XPLMFindDataRef(XSB_CON_CALLSIGN), buf_callsign, 0, sizeof(buf_callsign) - 1)] = 0;

			// Get message
			buf_msg[XPLMGetDatab(XPLMFindDataRef(XSB_TEXT_MESSAGE), buf_msg, 0, sizeof(buf_msg) - 1)] = 0;
			// Get count
			count = XPLMGetDatavi(XPLMFindDataRef(XSB_TEXT_FREQS), frqs, 0, sizeof(frqs) / sizeof(int));
			// Play sound
			if (!count) {
				PlaySound();
			} else {
				// Convert to lowercase
				int len = (int) strlen(buf_msg); if (len > 400) len = 400;
				for (int i = 0; i < len; i++) buf_msg[i] = tolower(buf_msg[i]);
				// Callsign to lowercase
				len = (int) strlen(buf_callsign); if (len > 100) len = 100;
				for (int i = 0; i < len; i++) buf_callsign[i] = tolower(buf_callsign[i]);
				
				if (strstr(buf_msg, buf_callsign))
					PlaySound();
			}

			break;
	}
}

void MenuHandler(void *, void * func)
{
	SimpleFunc sfunc = (SimpleFunc)(func);
	sfunc();
}	
