// FATAL FRAME II: Crimson Butterfly REMAKE — Mouse Wheel Camera Speed
// Mod ローダ（version.dll プロキシ）
// Created by MixedNuts - https://github.com/MixedNuts-Dev/fatal-frame2-remake-mouse-wheel-camera-speed
// Licensed under the MIT License. See LICENSE for details.
//
// ゲームのルートに置くと自動的にロードされ、Mods\wheelspeed\wheelspeed.dll を読み込む。
// version.dll 本来の機能は System32 の実体へ転送するのでゲーム動作に影響しない。
//
// Native 120FPS Option は dinput8.dll を使うので、名前を分けて互いに干渉しない
// ようにしている（どちらか一方だけ入れても、両方入れても、どの順で入れても動く）。

#include <windows.h>
#include <string>

#include <mixednuts/path.hpp>
#include <mixednuts/proxy.hpp>

namespace {

std::wstring g_modDir;

DWORD WINAPI LoadMod(LPVOID)
{
    // Mod 側の DLL が依存物を自分のフォルダから引けるようにする
    SetDllDirectoryW(g_modDir.c_str());
    LoadLibraryW((g_modDir + L"wheelspeed.dll").c_str());
    SetDllDirectoryW(nullptr);
    return 0;
}

} // namespace

// ---- 転送用エクスポート -------------------------------------------------
//
// version.dll の関数はどれも整数・ポインタの引数を 8 個以下しか取らず、
// 浮動小数点の引数も無いので、8 個そのまま受け渡す転送で済ませる（proxy.hpp）。

#define FORWARD(name) MIXEDNUTS_FORWARD(Proxy_##name, #name, 0)

FORWARD(GetFileVersionInfoA)
FORWARD(GetFileVersionInfoByHandle)
FORWARD(GetFileVersionInfoExA)
FORWARD(GetFileVersionInfoExW)
FORWARD(GetFileVersionInfoSizeA)
FORWARD(GetFileVersionInfoSizeExA)
FORWARD(GetFileVersionInfoSizeExW)
FORWARD(GetFileVersionInfoSizeW)
FORWARD(GetFileVersionInfoW)
FORWARD(VerFindFileA)
FORWARD(VerFindFileW)
FORWARD(VerInstallFileA)
FORWARD(VerInstallFileW)
FORWARD(VerLanguageNameA)
FORWARD(VerLanguageNameW)
FORWARD(VerQueryValueA)
FORWARD(VerQueryValueW)

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        g_modDir = mixednuts::GameDir() + L"Mods\\wheelspeed\\";
        mixednuts::proxy::Init(L"version.dll");

        // ローダロックを避けるため、Mod 本体は別スレッドで読み込む
        if (HANDLE t = CreateThread(nullptr, 0, LoadMod, nullptr, 0, nullptr))
            CloseHandle(t);
    }
    return TRUE;
}
