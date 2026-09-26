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
#include <cstdint>
#include <string>

namespace {

HMODULE      g_real = nullptr;
std::wstring g_modDir;

void LoadRealVersion()
{
    if (g_real) return;
    wchar_t path[MAX_PATH]{};
    GetSystemDirectoryW(path, MAX_PATH);
    wcscat_s(path, L"\\version.dll");
    g_real = LoadLibraryW(path);
}

FARPROC RealProc(const char* name)
{
    LoadRealVersion();
    return g_real ? GetProcAddress(g_real, name) : nullptr;
}

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
// 浮動小数点の引数も無い。x64 の呼び出し規約では、引数をそのまま 8 個
// 受け渡せば元の関数と同じに振る舞うので、型ごとに書き分けずに済ませる。
// エクスポート名は .def で本来の名前に付け替える（windows.h の宣言と衝突しないように）。

using Fwd8 = uintptr_t(WINAPI*)(uintptr_t, uintptr_t, uintptr_t, uintptr_t,
                                uintptr_t, uintptr_t, uintptr_t, uintptr_t);

#define FORWARD(name)                                                              \
    extern "C" uintptr_t WINAPI Proxy_##name(uintptr_t a, uintptr_t b, uintptr_t c, \
                                             uintptr_t d, uintptr_t e, uintptr_t f, \
                                             uintptr_t g, uintptr_t h)              \
    {                                                                              \
        static Fwd8 fn = reinterpret_cast<Fwd8>(RealProc(#name));                  \
        return fn ? fn(a, b, c, d, e, f, g, h) : 0;                                \
    }

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

        wchar_t exe[MAX_PATH]{};
        GetModuleFileNameW(nullptr, exe, MAX_PATH);
        std::wstring dir(exe);
        dir.resize(dir.find_last_of(L'\\') + 1);
        g_modDir = dir + L"Mods\\wheelspeed\\";

        LoadRealVersion();

        // ローダロックを避けるため、Mod 本体は別スレッドで読み込む
        if (HANDLE t = CreateThread(nullptr, 0, LoadMod, nullptr, 0, nullptr))
            CloseHandle(t);
    }
    return TRUE;
}
