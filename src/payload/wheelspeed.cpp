// FATAL FRAME II: Crimson Butterfly REMAKE — Mouse Wheel Camera Speed
// Created by MixedNuts - https://github.com/MixedNuts-Dev/fatal-frame2-remake-mouse-wheel-camera-speed
// Licensed under the MIT License. See LICENSE for details.
//
// 射影機のズームとピントは、入力 1 回ごとに決まった量（ズーム 0.6 度 / ピント 4.0）
// だけ動く。パッドはボタンを押している間毎フレーム入力が続くので十分速いが、
// マウスホイールは 1 ノッチが 1 回分にしかならず、端から端まで数百ノッチかかる。
//
// この量はカメラのパラメータ表（ヒープ上、レコード 3 件）にあり、毎フレーム
// カメラ構造体へ丸ごとコピーされて使われる。構造体を書き換えても次のフレームで
// 戻るので、コピー元の表に倍率を掛ける。パッドも同じ値を使うため一緒に速くなるが、
// この Mod はマウス操作の人向けなので割り切っている。
//
// 表の位置とレコード内のオフセットは、ビルド差に備えてコードから実行時に割り出す。
// ゲームのファイルは一切変更しない。
//
// ログは英語で書く（Native 120FPS Option の 1.0.1 で、日本語のログでは利用者が
// 自分で状況を判断できないと分かったため）。コメントは日本語のままにする。

#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <map>

namespace {

constexpr char kVersion[] = "1.0.0";

std::wstring g_modDir;
bool         g_log        = true;
bool         g_enabled    = true;
float        g_multiplier = 10.0f;

// ---- ログ ---------------------------------------------------------------

// ログは UTF-8 で書くので、ワイド文字列は明示的に変換する
std::string Utf8(const wchar_t* w)
{
    if (!w || !*w) return std::string();
    const int n = WideCharToMultiByte(CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr);
    if (n <= 1) return std::string();
    std::string s(static_cast<size_t>(n - 1), '\0');
    WideCharToMultiByte(CP_UTF8, 0, w, -1, &s[0], n, nullptr, nullptr);
    return s;
}

void Log(const char* fmt, ...)
{
    if (!g_log) return;
    const std::wstring path = g_modDir + L"wheelspeed.log";
    const bool isNew = (GetFileAttributesW(path.c_str()) == INVALID_FILE_ATTRIBUTES);
    FILE* f = nullptr;
    if (_wfopen_s(&f, path.c_str(), L"a") != 0 || !f) return;
    if (isNew) fwrite("\xEF\xBB\xBF", 1, 3, f);   // UTF-8 BOM（メモ帳で文字化けしないように）

    SYSTEMTIME st{};
    GetLocalTime(&st);
    fprintf(f, "[%02d:%02d:%02d] ", st.wHour, st.wMinute, st.wSecond);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fputc('\n', f);
    fclose(f);
}

// ---- 環境情報 -----------------------------------------------------------
//
// 不具合報告のログだけで、報告者のゲームのビルドがこちらの検証環境と
// 同じかどうかを判断できるようにする。

std::string ReadTextFile(const wchar_t* path, size_t maxBytes)
{
    HANDLE h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                           nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return std::string();
    LARGE_INTEGER sz{};
    if (!GetFileSizeEx(h, &sz) || sz.QuadPart <= 0)
    {
        CloseHandle(h);
        return std::string();
    }
    const size_t n = (static_cast<unsigned long long>(sz.QuadPart) < maxBytes)
                     ? static_cast<size_t>(sz.QuadPart) : maxBytes;
    std::string s(n, '\0');
    DWORD got = 0;
    const BOOL ok = ReadFile(h, &s[0], static_cast<DWORD>(n), &got, nullptr);
    CloseHandle(h);
    if (!ok) return std::string();
    s.resize(got);
    return s;
}

// Steam の acf は `"key"  "value"` 形式
std::string AcfValue(const std::string& s, const char* key)
{
    const std::string k = std::string("\"") + key + "\"";
    size_t at = s.find(k);
    if (at == std::string::npos) return std::string();
    at = s.find('"', at + k.size());
    if (at == std::string::npos) return std::string();
    const size_t end = s.find('"', at + 1);
    if (end == std::string::npos) return std::string();
    return s.substr(at + 1, end - at - 1);
}

// buildid はゲームのビルドを一意に示すので、バージョン違いの判定に一番効く。
//   steamapps\common\FatalFrameII\FatalFrameII.exe
//   steamapps\appmanifest_3920610.acf
void LogEnvironment()
{
    wchar_t exe[MAX_PATH]{};
    GetModuleFileNameW(nullptr, exe, MAX_PATH);
    Log("exe: %s", Utf8(exe).c_str());

    WIN32_FILE_ATTRIBUTE_DATA fad{};
    if (GetFileAttributesExW(exe, GetFileExInfoStandard, &fad))
        Log("exe size: %llu bytes",
            (static_cast<unsigned long long>(fad.nFileSizeHigh) << 32) | fad.nFileSizeLow);

    std::wstring dir(exe);
    for (int up = 0; up < 3; ++up)   // exe 名 / FatalFrameII / common
    {
        const size_t slash = dir.find_last_of(L'\\');
        if (slash == std::wstring::npos) return;
        dir.resize(slash);
    }
    const std::wstring acf = dir + L"\\appmanifest_3920610.acf";
    const std::string s = ReadTextFile(acf.c_str(), 64 * 1024);
    const std::string build = AcfValue(s, "buildid");
    Log("Steam build: %s", build.empty() ? "? (manifest not found)" : build.c_str());
}

// ---- コードの探索 -------------------------------------------------------

bool GetTextSection(uint8_t*& base, size_t& size)
{
    auto mod = reinterpret_cast<uint8_t*>(GetModuleHandleW(nullptr));
    if (!mod) return false;
    auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(mod);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    auto nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(mod + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return false;

    auto sec = IMAGE_FIRST_SECTION(nt);
    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++sec)
    {
        if (memcmp(sec->Name, ".text", 5) == 0)
        {
            base = mod + sec->VirtualAddress;
            size = sec->Misc.VirtualSize;
            return true;
        }
    }
    return false;
}

unsigned long long Rva(const void* p)
{
    return static_cast<unsigned long long>(
        static_cast<const uint8_t*>(p) - reinterpret_cast<uint8_t*>(GetModuleHandleW(nullptr)));
}

inline int32_t ReadI32(const uint8_t* p)
{
    int32_t v = 0;
    memcpy(&v, p, sizeof(v));
    return v;
}

// パターン（mask 0 = ワイルドカード）の出現を数え、最初の位置を返す
struct Pattern {
    const uint8_t* bytes;
    const uint8_t* mask;
    size_t         len;
};

bool Match(const uint8_t* p, const Pattern& pat)
{
    for (size_t i = 0; i < pat.len; ++i)
        if (pat.mask[i] && p[i] != pat.bytes[i]) return false;
    return true;
}

int FindAll(const uint8_t* base, size_t size, const Pattern& pat, const uint8_t*& first)
{
    int n = 0;
    first = nullptr;
    const uint8_t head = pat.bytes[0];   // 先頭バイトは必ず固定にしておく
    for (size_t i = 0; i + pat.len <= size; ++i)
    {
        if (base[i] != head || !Match(base + i, pat)) continue;
        if (n++ == 0) first = base + i;
    }
    return n;
}

// 構造体へのコピー箇所:
//   call  <表からレコードを引く関数>
//   mov   rdx,rax
//   test  rax,rax
//   je    short ...
//   lea   rcx,[r14+0x118]      <- コピー先 = 構造体 +0x118
constexpr uint8_t kCopyBytes[] = {
    0xE8, 0, 0, 0, 0, 0x48, 0x8B, 0xD0, 0x48, 0x85, 0xC0, 0x74, 0,
    0x49, 0x8D, 0x8E, 0, 0, 0, 0,
};
constexpr uint8_t kCopyMask[] = {
    1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 0, 0, 0, 0,
};

// レコードを引く関数の冒頭付近:
//   mov   rsi,[rip+X]          <- 表を持つオブジェクトへのポインタ
//   mov   r14d,ecx
//   test  rsi,rsi
//   je    ...
//   mov   rax,[rsi+0x110]      <- 表
constexpr uint8_t kGetterBytes[] = {
    0x48, 0x8B, 0x35, 0, 0, 0, 0, 0x44, 0x8B, 0xF1, 0x48, 0x85, 0xF6,
    0x0F, 0x84, 0, 0, 0, 0, 0x48, 0x8B, 0x86, 0, 0, 0, 0,
};
constexpr uint8_t kGetterMask[] = {
    1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
};

// ホイール（とパッド）によるズームの更新:
//   movss xmm0,[rbx+0x128]     <- 1 回あたりのズーム量（度）
//   mulss xmm0,[rip+X]         <- 度 → ラジアン
constexpr uint8_t kZoomBytes[] = {
    0xF3, 0x0F, 0x10, 0x83, 0, 0, 0, 0, 0xF3, 0x0F, 0x59, 0x05,
};
constexpr uint8_t kZoomMask[] = {
    1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
};

// ズームの直後にあるピントの更新:
//   mulss xmm8,[rbx+0x150]     <- 1 回あたりのピント量
constexpr uint8_t kFocusBytes[] = { 0xF3, 0x44, 0x0F, 0x59, 0x83, 0, 0, 0, 0 };
constexpr uint8_t kFocusMask[]  = { 1, 1, 1, 1, 1, 0, 0, 0, 0 };
constexpr size_t  kFocusWindow  = 0x200;   // 実測ではズームから 0xDE 後ろ

// ズームの手前にある入力判定: mov edx,0x1D（ズーム操作の入力アクション番号）
//
// ズームの命令列はオフセットを伏せると .text 中に 35 箇所あるので、
// 「手前に入力判定があり、後ろにピントの更新がある」ものだけを採る。
constexpr uint8_t kZoomAction[] = { 0xBA, 0x1D, 0x00, 0x00, 0x00 };
constexpr size_t  kActionWindow = 0x80;    // 実測ではズームから 0x5F 手前

const uint8_t* FindFocusAfter(const uint8_t* zoom, const uint8_t* end)
{
    for (size_t i = sizeof(kZoomBytes); i < kFocusWindow && zoom + i + sizeof(kFocusBytes) <= end; ++i)
        if (Match(zoom + i, {kFocusBytes, kFocusMask, sizeof(kFocusBytes)})) return zoom + i;
    return nullptr;
}

bool HasActionBefore(const uint8_t* zoom, const uint8_t* begin)
{
    for (size_t i = 1; i <= kActionWindow && zoom - i >= begin; ++i)
        if (memcmp(zoom - i, kZoomAction, sizeof(kZoomAction)) == 0) return true;
    return false;
}

// 探索の結果
struct Layout {
    uint8_t** owner      = nullptr;   // 表を持つオブジェクトへのポインタの格納先
    uint32_t  tableOff   = 0;         // オブジェクト内の表ポインタの位置（0x110）
    uint32_t  zoomOff    = 0;         // レコード内のズーム量の位置（0x10）
    uint32_t  focusOff   = 0;         // レコード内のピント量の位置（0x38）
};

enum class FindResult { Ok, NoText, NotYet, Mismatch };

// 何が見つからなかったかをログに残す（ゲーム更新時の追随用）
FindResult FindLayout(Layout& out, std::string& why)
{
    uint8_t* text = nullptr;
    size_t   size = 0;
    if (!GetTextSection(text, size)) { why = "no .text"; return FindResult::NoText; }

    const uint8_t* copy = nullptr;
    const int nCopy = FindAll(text, size, {kCopyBytes, kCopyMask, sizeof(kCopyBytes)}, copy);
    if (nCopy == 0) { why = "copy site not found"; return FindResult::NotYet; }
    if (nCopy > 1)  { why = "copy site ambiguous (" + std::to_string(nCopy) + ")"; return FindResult::Mismatch; }

    // call の飛び先 = レコードを引く関数。冒頭付近にシグネチャがあるはず
    const uint8_t* getter = copy + 5 + ReadI32(copy + 1);
    if (getter < text || getter + 0x40 > text + size) { why = "getter out of range"; return FindResult::Mismatch; }
    const uint8_t* g = nullptr;
    for (size_t i = 0; i < 0x20; ++i)
    {
        if (Match(getter + i, {kGetterBytes, kGetterMask, sizeof(kGetterBytes)}))
        {
            g = getter + i;
            break;
        }
    }
    if (!g) { why = "getter signature mismatch"; return FindResult::Mismatch; }

    const uint32_t copyBase = static_cast<uint32_t>(ReadI32(copy + 16));
    out.owner    = reinterpret_cast<uint8_t**>(const_cast<uint8_t*>(g + 7 + ReadI32(g + 3)));
    out.tableOff = static_cast<uint32_t>(ReadI32(g + 22));

    const uint8_t* zoom = nullptr;
    const uint8_t* focus = nullptr;
    int nZoom = 0;
    const Pattern zp{kZoomBytes, kZoomMask, sizeof(kZoomBytes)};
    for (size_t i = 0; i + zp.len <= size; ++i)
    {
        if (text[i] != kZoomBytes[0] || !Match(text + i, zp)) continue;
        const uint8_t* f = FindFocusAfter(text + i, text + size);
        if (!f || !HasActionBefore(text + i, text)) continue;
        if (nZoom++ == 0)
        {
            zoom  = text + i;
            focus = f;
        }
    }
    if (nZoom != 1) { why = "zoom/focus site count " + std::to_string(nZoom); return FindResult::Mismatch; }

    const uint32_t zoomField  = static_cast<uint32_t>(ReadI32(zoom + 4));
    const uint32_t focusField = static_cast<uint32_t>(ReadI32(focus + 5));
    if (zoomField <= copyBase || focusField <= copyBase)
    {
        why = "field offsets are outside the copied record";
        return FindResult::Mismatch;
    }
    out.zoomOff  = zoomField - copyBase;
    out.focusOff = focusField - copyBase;

    Log("[OK] Code located: copy RVA 0x%llX, getter RVA 0x%llX, zoom RVA 0x%llX,"
        " focus RVA 0x%llX", Rva(copy), Rva(g), Rva(zoom), Rva(focus));
    Log("[OK] Table owner at RVA 0x%llX (+0x%X), record fields: zoom +0x%X, focus +0x%X",
        Rva(out.owner), out.tableOff, out.zoomOff, out.focusOff);
    return FindResult::Ok;
}

// ---- 表への倍率適用 -----------------------------------------------------
//
// 表のヘッダ: +0x08 件数 / +0x0C レコード長 / +0x20 からレコードが並ぶ。
// 表はヒープ上にあり、ゲームが読み直すと場所や値が戻る可能性があるので、
// 1 秒おきに見て「自分が最後に書いた値」と違っていれば掛け直す。
// 掛けるのは元の値に対してだけなので、倍率が二重に掛かることはない。

struct Table {
    uint8_t* base   = nullptr;
    int32_t  count  = 0;
    int32_t  stride = 0;
};

bool ReadTable(const Layout& L, Table& t)
{
    __try
    {
        uint8_t* owner = *L.owner;
        if (!owner) return false;
        uint8_t* tbl = *reinterpret_cast<uint8_t**>(owner + L.tableOff);
        if (!tbl) return false;
        t.base   = tbl;
        t.count  = *reinterpret_cast<int32_t*>(tbl + 0x08);
        t.stride = *reinterpret_cast<int32_t*>(tbl + 0x0C);
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

bool ReadFloat(const uint8_t* p, float& v)
{
    __try { memcpy(&v, p, 4); return true; }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

bool WriteFloat(uint8_t* p, float v)
{
    __try { memcpy(p, &v, 4); return true; }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

// 元の値として妥当か（ゴミを掛けないため）。実測はズーム 0.6 / ピント 4.0
bool Plausible(float v)
{
    return std::isfinite(v) && v > 0.0f && v < 1000.0f;
}

std::map<uint8_t*, float> g_written;   // アドレス → 自分が最後に書いた値

// 戻り値: 今回書き換えたフィールド数
int ApplyOnce(const Layout& L, bool& tableSeen)
{
    Table t;
    if (!ReadTable(L, t)) return 0;
    if (t.count <= 0 || t.count > 64 || t.stride <= 0 || t.stride > 0x1000 ||
        static_cast<uint32_t>(t.stride) < L.focusOff + 4 ||
        static_cast<uint32_t>(t.stride) < L.zoomOff + 4)
        return 0;
    tableSeen = true;

    int changed = 0;
    for (int i = 0; i < t.count; ++i)
    {
        uint8_t* rec = t.base + 0x20 + static_cast<size_t>(i) * t.stride;
        const uint32_t offs[2]  = { L.zoomOff, L.focusOff };
        const char*    names[2] = { "zoom", "focus" };
        for (int k = 0; k < 2; ++k)
        {
            uint8_t* p = rec + offs[k];
            float v = 0.0f;
            if (!ReadFloat(p, v)) continue;

            auto it = g_written.find(p);
            if (it != g_written.end() && it->second == v) continue;   // 自分が書いたまま
            if (!Plausible(v))
            {
                Log("[??] Record %d %s value %g looks wrong; left alone", i, names[k], v);
                g_written[p] = v;   // 同じ値で毎秒ログが出ないように
                continue;
            }
            const float nv = v * g_multiplier;
            if (!WriteFloat(p, nv)) continue;
            g_written[p] = nv;
            ++changed;
            uint32_t id = 0;
            memcpy(&id, rec, 4);
            Log("[OK] Record %d (id 0x%08X) %s: %g -> %g", i, id, names[k], v, nv);
        }
    }
    return changed;
}

// ---- 設定読み込み -------------------------------------------------------

void LoadConfig(HMODULE self)
{
    wchar_t path[MAX_PATH]{};
    GetModuleFileNameW(self, path, MAX_PATH);
    std::wstring dir(path);
    dir.resize(dir.find_last_of(L'\\') + 1);
    g_modDir = dir;

    const std::wstring ini = dir + L"wheelspeed.ini";
    g_enabled = GetPrivateProfileIntW(L"General", L"Enabled", 1, ini.c_str()) != 0;
    g_log     = GetPrivateProfileIntW(L"General", L"Log", 1, ini.c_str()) != 0;

    // 小数も書けるように文字列で読む
    wchar_t buf[64]{};
    GetPrivateProfileStringW(L"Speed", L"Multiplier", L"10", buf, 64, ini.c_str());
    const float m = wcstof(buf, nullptr);
    g_multiplier = (std::isfinite(m) && m >= 0.1f && m <= 100.0f) ? m : 10.0f;
}

DWORD WINAPI Worker(LPVOID param)
{
    LoadConfig(static_cast<HMODULE>(param));
    Log("=== Mouse Wheel Camera Speed %s / Created by MixedNuts ===", kVersion);
    if (!g_enabled)
    {
        Log("[--] Enabled=0, doing nothing");
        return 0;
    }
    LogEnvironment();
    Log("Multiplier: %g", g_multiplier);
    if (g_multiplier == 1.0f)
    {
        Log("[--] Multiplier is 1, nothing to do");
        return 0;
    }

    // Steam DRM がコードを復号するまで待つ。探索は .text を 1 周するだけで軽いが、
    // 有限回で打ち切る
    Layout L;
    std::string why, lastWhy;
    FindResult r = FindResult::NotYet;
    for (int i = 0; i < 60; ++i)
    {
        r = FindLayout(L, why);
        if (r == FindResult::Ok || r == FindResult::NoText) break;
        if (r == FindResult::Mismatch && why == lastWhy) break;   // 2 回続けて同じ不一致
        lastWhy = why;
        Sleep(1000);
    }
    if (r != FindResult::Ok)
    {
        Log("[NG] Could not locate the camera code (%s). A game update has most likely"
            " changed it. The mod is doing nothing. Please report this log.", why.c_str());
        return 0;
    }

    // 表はゲームの進行に合わせて作られるので、見つかるまで待ち、
    // 見つかった後も読み直しに備えて見張り続ける。触るのは数十バイトだけ。
    bool announced = false;
    for (;;)
    {
        bool seen = false;
        ApplyOnce(L, seen);
        if (seen && !announced)
        {
            Log("[OK] Camera parameter table found. Now watching it once per second.");
            announced = true;
        }
        Sleep(1000);
    }
}

} // namespace

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        if (HANDLE t = CreateThread(nullptr, 0, Worker, hModule, 0, nullptr))
            CloseHandle(t);
    }
    return TRUE;
}
