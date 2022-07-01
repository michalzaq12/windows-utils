#include <node.h>
#include <nan.h>
#include <windows.h>
#pragma comment(lib, "user32.lib")

using namespace v8;

NAN_METHOD(VisualEffects)
{
	if (!info[0]->IsBoolean())
	{
		Nan::ThrowTypeError("First argument must be a boolean.");
		return;
	}
	bool enabled = Nan::To<bool>(info[0]).FromJust();

	// visual effects
	SystemParametersInfo(SPI_SETUIEFFECTS, enabled, NULL, 0);
	SystemParametersInfo(SPI_SETMENUANIMATION, enabled, NULL, 0);
	SystemParametersInfo(SPI_SETSELECTIONFADE, enabled, NULL, 0);
	SystemParametersInfo(SPI_SETTOOLTIPANIMATION, enabled, NULL, 0);
	SystemParametersInfo(SPI_SETCLIENTAREAANIMATION, enabled, NULL, 0);
	SystemParametersInfo(SPI_SETDROPSHADOW, enabled, NULL, 0);
	SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, enabled, NULL, 0);

	// minimize and restore window animation
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ai);
	ai.iMinAnimate = enabled;
	SystemParametersInfo(SPI_SETANIMATION, sizeof(ai), &ai, 0);

	info.GetReturnValue().Set(true);
}

NAN_METHOD(GetInputDesktop) {
	BOOL	bRet = false;
    DWORD	dwLengthNeeded;

    HDESK	hNewDesktop;
    char	strInputDesktop[256];

    hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if (hNewDesktop == NULL) {
		return info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
	}
    
    memset(strInputDesktop, 0, sizeof(strInputDesktop));
    GetUserObjectInformation(hNewDesktop, UOI_NAME, &strInputDesktop, sizeof(strInputDesktop), &dwLengthNeeded);

    CloseDesktop(hNewDesktop);

    info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), strInputDesktop).ToLocalChecked());
}

NAN_METHOD(GetWallpaperPath) {
	char bgPath[MAX_PATH];
	if (SystemParametersInfoA(SPI_GETDESKWALLPAPER, MAX_PATH, bgPath, 0))
	{
		info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), bgPath).ToLocalChecked());
	}
	else {
		info.GetReturnValue().SetUndefined();
	}
}

HCURSOR createBlankCursor()
{
	int cw = GetSystemMetrics(SM_CXCURSOR);
	int ch = GetSystemMetrics(SM_CYCURSOR);
	UINT8* cursorAND = new UINT8[ch * ((cw + 31) >> 2)];
	UINT8* cursorXOR = new UINT8[ch * ((cw + 31) >> 2)];
	memset(cursorAND, 0xff, ch * ((cw + 31) >> 2));
	memset(cursorXOR, 0x00, ch * ((cw + 31) >> 2));
	HCURSOR c = CreateCursor(GetModuleHandle(NULL),
		0, 0, cw, ch, cursorAND, cursorXOR);
	delete[] cursorXOR;
	delete[] cursorAND;
	return c;
}

NAN_METHOD(_ShowCursor) {
	bool show = Nan::To<bool>(info[0]).FromJust();
	if (show) {
		// Reloads the system cursors
		SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	} else {
		// https://github.com/Rick-laboratory/Instruction-Receiver/blob/master/main.cpp#L97
		// Make all built-in system cursors blank 
		// (Can not use SetCursor / ShowCursor methods since they are only linked to the thread window)
		SetSystemCursor(createBlankCursor(), 32512);
		SetSystemCursor(createBlankCursor(), 32650);
		SetSystemCursor(createBlankCursor(), 32515);
		SetSystemCursor(createBlankCursor(), 32649);
		SetSystemCursor(createBlankCursor(), 32651);
		SetSystemCursor(createBlankCursor(), 32513);
		SetSystemCursor(createBlankCursor(), 32648);
		SetSystemCursor(createBlankCursor(), 32646);
		SetSystemCursor(createBlankCursor(), 32643);
		SetSystemCursor(createBlankCursor(), 32645);
		SetSystemCursor(createBlankCursor(), 32642);
		SetSystemCursor(createBlankCursor(), 32644);
		SetSystemCursor(createBlankCursor(), 32516);
		SetSystemCursor(createBlankCursor(), 32514);
		DestroyCursor(createBlankCursor());
	}
}

NAN_METHOD(_SetDPI) {
	int dpi = Nan::To<int>(info[0]).FromJust();
	// dpi = 0 : 100% (default)
	// dpi = 1 : 125% 
	// dpi = 2 : 150% 
	// dpi = 3 : 175% 
	SystemParametersInfo(0x009F, dpi, NULL, 1);

	// It set system DPI (not per device DPI)
	// Per device DPI implementations: 
	// https://github.com/lihas/windows-DPI-scaling-sample
	// https://github.com/imniko/SetDPI/blob/master/SetDpi.cpp
	// ISSUE: Can not map device to id (Electron deviceId = Hash(monitorInfo.szDevice))
}


void Initialize(Local<v8::Object> exports)
{
  Nan::SetMethod(exports, "setVisualEffects", VisualEffects);
  Nan::SetMethod(exports, "getInputDesktop", GetInputDesktop);
  Nan::SetMethod(exports, "getWallpaperPath", GetWallpaperPath);
  Nan::SetMethod(exports, "showCursor", _ShowCursor);
  Nan::SetMethod(exports, "setDPI", _SetDPI);
}

NODE_MODULE(visualEffects, Initialize)