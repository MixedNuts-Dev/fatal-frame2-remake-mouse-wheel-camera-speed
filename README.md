# MouseWheelCameraSpeed

**FATAL FRAME II: Crimson Butterfly REMAKE**（零 〜紅い蝶〜 REMAKE / Steam AppID 3920610）用の Mod です。
A mod for FATAL FRAME / PROJECT ZERO II: Crimson Butterfly REMAKE (Steam, AppID 3920610).

射影機のズームとピント調整を、**マウスホイールで快適に操作できる速さ**にします。
既定値はゲーム標準の 10 倍で、設定ファイルで倍率を変えられます。

Makes the Camera Obscura's zoom and focus **usable with a mouse wheel**. The default is
ten times the game's speed, and the multiplier can be changed in the config file.

ゲーム本来の処理では、ズームとピントは入力 1 回につき決まった量だけ動きます。
パッドはボタンを押している間ずっと入力が続くので十分な速さになりますが、
マウスホイールは 1 ノッチが入力 1 回分にしかならず、端から端まで動かすのに
ホイールを延々と回し続けることになります。ゲーム内にはこれを変える設定がありません。

The game moves zoom and focus by a fixed amount per input. On a gamepad the input
repeats every frame while the button is held, so it feels fine, but a mouse wheel notch
counts as a single input, so going from one end to the other takes an enormous amount
of scrolling. The game has no setting for this.

> **2.0.0 から [MixedNutsModLoader](https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader)
> （1.0.0 以降）が必要です。** ローダーは別途導入してください。1.x は `version.dll` で
> 単独で動作していましたが、2.0.0 はローダーのプラグインになり、`version.dll` は同梱していません。
>
> **2.0.0 requires [MixedNutsModLoader](https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader)
> (1.0.0 or later)**, installed separately. 1.x ran on its own via `version.dll`; 2.0.0 is a
> loader plugin and no longer ships `version.dll`.

## 導入 / Installation

**ビルドは不要です。**

1. 先に MixedNutsModLoader を導入します（ローダーの
   [Releases](https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader/releases) か [Nexus Mods](https://www.nexusmods.com/fatalframe2crimsonbutterflyremake/mods/26) から
   ダウンロードし、ローダーの README に従ってください）
2. この Mod の [Releases](../../releases) から配布物をダウンロードし、中身の `MixedNuts`
   フォルダを、ゲームのルート（`FatalFrameII.exe` と同じ場所）にそのままコピーします。
   ローダーの `MixedNuts` フォルダに中身が合流します

**No build required.**

1. Install MixedNutsModLoader first (download it from the loader's
   [Releases](https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader/releases) or [Nexus Mods](https://www.nexusmods.com/fatalframe2crimsonbutterflyremake/mods/26) and
   follow its README).
2. Download this mod from [Releases](../../releases) and copy its `MixedNuts` folder into
   the game's root directory (the folder containing `FatalFrameII.exe`). It merges into
   the loader's `MixedNuts` folder.

```
FatalFrameII/
  FatalFrameII.exe
  dinput8.dll                                <- MixedNutsModLoader
  MixedNuts/MixedNutsLoader.dll              <- MixedNutsModLoader
  MixedNuts/Mods/wheelspeed/wheelspeed.dll   <- この Mod / this mod
  MixedNuts/Mods/wheelspeed/wheelspeed.ini
  MixedNuts/Mods/wheelspeed/README.md
  MixedNuts/Mods/wheelspeed/wheelspeed.log   <- 起動時に生成 / generated at launch
```

ゲームのファイルは一切変更しません。メモリ上の値を書き換えるだけです。
No game files are modified; the mod only changes values in memory.

削除は `MixedNuts\Mods\wheelspeed\` を消すだけです。ローダーは他の Mod のために
残しておけます。すべて取り除く場合は、ローダー（`dinput8.dll` と `MixedNuts` フォルダ）も
削除してください。一時的に無効化するには、`wheelspeed.ini` の `Enabled` を `0` にします。

To uninstall, delete `MixedNuts\Mods\wheelspeed\`. The loader can stay for other mods;
to remove everything, also delete the loader (`dinput8.dll` and the `MixedNuts` folder).
To disable temporarily, set `Enabled=0` in `wheelspeed.ini`.

### 1.x からの更新 / Upgrading from 1.x

導入の前に、ゲームのルートから古い `version.dll` と古い `Mods\wheelspeed\` フォルダを
削除してください。1.x の `version.dll` が残っていると、ローダーは新しい Mod を読み込まず、
`MixedNuts\loader.log` に `[!!]` で始まるメッセージを書き出します。

Native120FPSOption（`dinput8.dll` + `Mods\native120fps\`）や TwinSwap
（`xinput1_4.dll` + `Mods\twinswap\`）の 1.x も入れている場合は、まとめて更新してください。
詳しくはローダーの README を参照してください。

Before installing, delete the old `version.dll` and the old `Mods\wheelspeed\` folder
from the game root. If the 1.x `version.dll` is still there, the loader does not load
the new mod and writes a message starting with `[!!]` to `MixedNuts\loader.log`.

If you also have the 1.x versions of Native120FPSOption (`dinput8.dll` +
`Mods\native120fps\`) or TwinSwap (`xinput1_4.dll` + `Mods\twinswap\`), update them all
at once. See the loader's README for details.

## 他の MixedNuts の Mod との併用 / Using it with other MixedNuts mods

**干渉しません。** Native120FPSOption、MouseWheelCameraSpeed、TwinSwap の 2.0.0 は
すべて同じローダーの上で動くため、DLL を 1 つ共有し、競合することはありません。

**They do not conflict.** Native120FPSOption, MouseWheelCameraSpeed and TwinSwap
2.0.0 all run on the same loader, so they share one DLL.

別の Mod が既に `dinput8.dll` を使っている場合は、上書きしないでください。ローダーの
`dinput8.dll` は `version.dll` または `xinput1_4.dll` に名前を変えて使えます（ローダーの
README を参照）。

If another mod already uses `dinput8.dll`, do not overwrite it. The loader's
`dinput8.dll` can be renamed to `version.dll` or `xinput1_4.dll` (see the loader's README).

## 設定 / Configuration

`MixedNuts\Mods\wheelspeed\wheelspeed.ini`

| 項目 / Key | 意味 / Meaning |
|---|---|
| `Enabled` | `1` = 有効 on / `0` = 無効 off |
| `Log` | `1` = ログを出力 write a log / `0` = 出力しない no log |
| `Multiplier` | ズームとピントの倍率。既定 `10`、小数可（`0.1`～`100`） / Zoom and focus multiplier. Default `10`, decimals allowed (`0.1`-`100`) |

**パッドも同じ倍率で速くなります。** ゲームがパッドとマウスで同じ値を使っているためです。
**The gamepad gets faster by the same factor**, because the game uses the same values
for both.

## ビルド / Build

Visual Studio 2022 の C++ ツールセットが必要です。
Requires the Visual Studio 2022 C++ toolset.

共通コード（[mod-loader](https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader)）を
submodule で取り込んでいるので、`--recursive` 付きで clone してください。
The shared code ([mod-loader](https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader))
is a git submodule, so clone with `--recursive`.

```
git clone --recursive https://github.com/MixedNutsJP/fatal-frame2-remake-mouse-wheel-camera-speed.git
build.bat
```

clone 済みなら / If already cloned: `git submodule update --init`

`dist\MixedNuts\Mods\wheelspeed\` に配布用の一式が出力されます。
The distributable set is written to `dist\MixedNuts\Mods\wheelspeed\`.

submodule の代わりに手元の mod-loader を使う場合は、環境変数 `MIXEDNUTS_LOADER` に
その場所を指定してから `build.bat` を実行してください。
To build against a local checkout of mod-loader instead of the submodule, set the
environment variable `MIXEDNUTS_LOADER` to its path before running `build.bat`.

## 仕組み / How it works

射影機のズームとピントの「入力 1 回あたりの量」（標準ではズーム 0.6 度、ピント 4.0）は、
カメラのパラメータ表（ヒープ上）にあります。ゲームは毎フレームこの表のレコードを
カメラの構造体へ丸ごとコピーして使うので、構造体側を書き換えても次のフレームで
戻ります。そこで**コピー元の表**に倍率を掛けています。

The per-input zoom and focus amounts (0.6 degrees and 4.0 by default) live in a camera
parameter table on the heap. The game copies the active record into the camera struct
every frame, so writing to the struct is undone on the next frame. The mod therefore
multiplies the values in **the source table**.

表の位置とレコード内のオフセットは、アドレス直指定ではなく実行時にコードから割り出します。

The table location and the field offsets are derived from the game's code at runtime
rather than hardcoded:

1. 表からレコードを取り出してカメラ構造体（`+0x118`）へコピーする箇所を探し、
   呼び出し先の関数から表の持ち主を指すグローバル変数と表の位置（`+0x110`）を得る
   Find the site that copies a record into the camera struct (`+0x118`); the called
   function yields the global that owns the table and the table offset (`+0x110`).
2. ズームとピントの更新処理から、構造体内のフィールド位置（`+0x128` / `+0x150`）を得て、
   コピー先の基準を引いてレコード内の位置（`+0x10` / `+0x38`）にする
   From the zoom and focus update code, read the struct field offsets (`+0x128` /
   `+0x150`) and subtract the copy base to get the record offsets (`+0x10` / `+0x38`).
   ズームの命令列はそれだけでは一意にならないため、手前にズーム入力の判定
   （`mov edx,0x1D`）があり、後ろにピントの更新があるものだけを採用する
   The zoom instruction pattern alone is not unique, so only a match preceded by the
   zoom input check (`mov edx,0x1D`) and followed by the focus update is accepted.

どれかが見つからない、または複数見つかった場合は何も書き換えません。

If anything is missing or ambiguous, nothing is written.

表は起動後に作られ、ゲームが読み直す可能性もあるので、1 秒おきに値を確認し、
「自分が最後に書いた値」と違っていれば掛け直します。元の値にしか掛けないので、
倍率が二重に掛かることはありません。

The table is created after launch and may be reloaded, so the mod checks it once per
second and reapplies the multiplier whenever a value differs from what it last wrote.
It only ever multiplies original values, so the multiplier is never applied twice.

実行ファイルは Steam DRM により `.text` が暗号化されているため、コードの探索は
起動後に復号されたメモリに対して行います。ゲームのファイルは一切変更しません。

The executable's `.text` is encrypted by Steam DRM, so the code is inspected in memory
after it has been decrypted. No game files are modified.

## 免責事項 / Disclaimer

**この Mod は無保証で提供されます。使用によって生じたいかなる損害についても、
作者は一切の責任を負いません。** セーブデータの破損・消失、ゲームの動作不良、
その他の不具合を含みます。自己責任でご使用ください。

**This mod is provided as-is, without any warranty. The author accepts no liability
for any damage arising from its use,** including but not limited to corruption or loss
of save data, game malfunction, or any other problem. Use it at your own risk.

**導入前に、必ずセーブデータのバックアップを取ってください。**
**Always back up your save data before installing.**

```
%LOCALAPPDATA%\KoeiTecmo\FatalFrameII\Savedata\
```

## 不具合の報告 / Reporting issues

うまく動かないときは、次を確認してください。

1. ローダーが導入されているか（ゲームのルートに `dinput8.dll`、
   `MixedNuts\MixedNutsLoader.dll` があるか）
2. `MixedNuts\loader.log` があり、`[OK] wheelspeed: loaded` の行が出ているか。
   `loader.log` がなければローダーが読み込まれていません。行がなければ、フォルダ名・
   ファイル名が `MixedNuts\Mods\wheelspeed\wheelspeed.dll` になっているかを確認し、
   `[!!]` や `[NG]` の行を探してください
3. `MixedNuts\Mods\wheelspeed\wheelspeed.log` が生成されているか

If it does not work, check the following:

1. Is the loader installed (`dinput8.dll` in the game root, and
   `MixedNuts\MixedNutsLoader.dll` present)?
2. Does `MixedNuts\loader.log` exist and contain the `[OK] wheelspeed: loaded` line?
   If `loader.log` is missing, the loader is not being loaded. If the line is missing,
   check that the path is exactly `MixedNuts\Mods\wheelspeed\wheelspeed.dll`, and look
   for lines starting with `[!!]` or `[NG]`.
3. Has `MixedNuts\Mods\wheelspeed\wheelspeed.log` been created?

不具合を見つけた場合は、GitHub の Issue でご報告ください。その際、**必ず
`MixedNuts\Mods\wheelspeed\wheelspeed.log` と `MixedNuts\loader.log` の両方を
添付してください。** `wheelspeed.log` には、ゲームのバージョン、Steam のビルド、
ディスプレイ、GPU、OS、ロケールなどの環境情報も記録されます。

If you run into a problem, please open a GitHub Issue. **Be sure to attach both
`MixedNuts\Mods\wheelspeed\wheelspeed.log` and `MixedNuts\loader.log`.**
`wheelspeed.log` also records environment information such as the game version, Steam
build, display, GPU, OS and locale.

---

## クレジット / Credits

**Created by MixedNuts**

## ライセンス / License

MIT License — 詳細は [LICENSE](LICENSE) を参照してください。
See [LICENSE](LICENSE) for details.

再配布・改変は自由ですが、**著作権表示とライセンス文を必ず残してください。**
MIT ライセンスの条件です。

You are free to redistribute and modify this, but **the copyright notice and the
license text must be retained** — that is a condition of the MIT License.
