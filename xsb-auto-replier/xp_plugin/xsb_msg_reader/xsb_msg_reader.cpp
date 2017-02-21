
#include "stdafx.h"

#include "XPLMPlugin.h"
#include "XSBComDefs.h"
#include "XPLMDataAccess.h"
#include "XPLMMenus.h"
#include "XPLMUtilities.h"
#include <stdio.h>

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

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgReader: Started.", 0, 23);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);
}

void DoUnsubscribe()
{
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_UNSUBSCRIBE, (void *) (XSB_NETWORK | XSB_AUDIO | XSB_WEATHER | XSB_TEXT));

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgReader: Stopped.", 0, 23);
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

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgReader 0.1.0 [FatLYZ].", 0, 29);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "Visit http://fatlyz.com", 0, 24);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);

	XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "--", 0, 3);
	if (gPlugin != XPLM_NO_PLUGIN_ID)
		XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);
}	

static void WriteMsgFile(char *msg)
{
    FILE *f;
    char buf[512];
	
	// Open file
    XPLMGetSystemPath(buf);
    strcat(buf, "flxsbar_pm.txt");
    f = fopen(buf, "w");

	// Write if successfully opened
    if (! f) return;
    fprintf(f, "%s", msg);

    fclose(f);
}

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc)
{
    strcpy(outName, "XSBMsgReader [0.1.0]");
    strcpy(outSig, "fatlyz.xsb_pm_reader");
	strcpy(outDesc, "A plugin that reads private messages (Don't use this solely). [Credit: FatLYZ]");

	gMenu = XPLMCreateMenu("XSBMsgReader", 
					XPLMFindPluginsMenu(), 
					XPLMAppendMenuItem(XPLMFindPluginsMenu(), "XSBMsgReader", NULL, 1),
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
	int		freqCount;
	char	buf[256];

	switch(inMsg) {
		case XSB_MSG_TEXT:
			buf[XPLMGetDatab(XPLMFindDataRef(XSB_TEXT_FROM), buf, 0, sizeof(buf) - 1)] = 0;
			freqCount = XPLMGetDatavi(XPLMFindDataRef(XSB_TEXT_FREQS), frqs, 0, sizeof(frqs) / sizeof(int));
		
			if (!freqCount) {
				WriteMsgFile(buf);

				XPLMSetDatab(XPLMFindDataRef(XSB_INPUT_USER_MSG), (void *) "XSBMsgReader: Got private message.", 0, 35);
				if (gPlugin != XPLM_NO_PLUGIN_ID)
					XPLMSendMessageToPlugin(gPlugin, XSB_CMD_USER_MSG, NULL);
			}
			break;
	}
}

void	MenuHandler(void *, void * func)
{
	SimpleFunc sfunc = (SimpleFunc)(func);
	sfunc();
}	
