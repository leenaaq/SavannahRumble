#include "Team06.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, DedicatedX, "DedicatedX");

#pragma region GeneralLogging

DEFINE_LOG_CATEGORY(LogDX);

TAutoConsoleVariable<bool> CVarDebugGeneral(TEXT("DX.Debug.General"), false, TEXT("일반적인 디버깅 정보를 띄우고자 할 때 사용합니다.\n"), ECVF_Cheat);

#pragma endregion

#pragma region NetLogging

DEFINE_LOG_CATEGORY(LogDXNet);

#pragma endregion