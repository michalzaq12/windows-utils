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

    // wallpaper
	// https://stackoverflow.com/questions/35716843/how-to-restore-undo-desktop-wallpaper-after-changing
	// https://stackoverflow.com/questions/34710677/systemparametersinfo-sets-wallpaper-completly-black-using-spi-setdeskwallpaper
	// SystemParametersInfo(SPI_SETDESKWALLPAPER, enabled, L"", 0);

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

void Initialize(Local<v8::Object> exports)
{
  Nan::SetMethod(exports, "setVisualEffects", VisualEffects);
}

NODE_MODULE(visualEffects, Initialize)