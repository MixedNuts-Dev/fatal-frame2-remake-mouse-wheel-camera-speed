# MouseWheelCameraSpeed

**FATAL FRAME II: Crimson Butterfly REMAKE** 用の Mod です。
A mod for FATAL FRAME / PROJECT ZERO II: Crimson Butterfly REMAKE.

Created by MixedNuts

**2.0.0 から MixedNutsModLoader（1.0.0 以降）が必要です。** 1.x は `version.dll` で
単独で動作していましたが、2.0.0 はローダーのプラグインです。
**2.0.0 requires MixedNutsModLoader (1.0.0 or later).** 1.x ran on its own via
`version.dll`; 2.0.0 is a loader plugin.

Nexus Mods: https://www.nexusmods.com/fatalframe2crimsonbutterflyremake/mods/26
GitHub: https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader

---

# 日本語

## これは何か

射影機のズームとピント調整を、**マウスホイールで快適に操作できる速さ**にします。
標準の 10 倍が既定値で、設定ファイルで倍率を変えられます。

ゲーム本来の処理では、ズームとピントは「入力 1 回につき決まった量」だけ動きます。
パッドはボタンを押している間ずっと入力が続くので十分な速さになりますが、
マウスホイールは 1 ノッチが入力 1 回分にしかならず、端から端まで動かすのに
ホイールを延々と回し続けることになります。ゲーム内にはこれを変える設定が
ありません。

## 動作環境

- FATAL FRAME II: Crimson Butterfly REMAKE（Steam 版）
- MixedNutsModLoader 1.0.0 以降（別途導入。ローダーの Releases からダウンロードしてください）
  Nexus Mods: https://www.nexusmods.com/fatalframe2crimsonbutterflyremake/mods/26
  GitHub: https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader

ゲームのファイルは一切変更せず、メモリ上の値を書き換えるだけなので、Steam の
ファイル整合性チェックに引っかかることはありません。

## 同梱ファイル

| ファイル | 役割 |
|---|---|
| `MixedNuts\Mods\wheelspeed\wheelspeed.dll` | 本体 |
| `MixedNuts\Mods\wheelspeed\wheelspeed.ini` | 設定ファイル |
| `MixedNuts\Mods\wheelspeed\README.md` | このファイル |
| `MixedNuts\Mods\wheelspeed\LICENSE.txt` | ライセンス |

**`MixedNuts` フォルダ**をコピーします。ローダー本体（`dinput8.dll` と
`MixedNuts\MixedNutsLoader.dll`）は同梱していません。
起動すると `MixedNuts\Mods\wheelspeed\` の中にログ `wheelspeed.log` が作られます。

### 他の Mod との併用

**Native120FPSOption、MouseWheelCameraSpeed、TwinSwap の 2.0.0 は、すべて同じ
ローダーの上で動きます。** DLL を 1 つ共有するので、競合することはありません。
`MixedNuts` フォルダは中身が合流するだけです。

ゲームのルートに **別の Mod の `dinput8.dll` が既にある場合は、上書きしないでください。**
ローダーの `dinput8.dll` は `version.dll` または `xinput1_4.dll` に名前を変えて使えます
（ローダーの README を参照してください）。

## 導入方法

1. ゲームを終了します

2. MixedNutsModLoader を導入します（ローダーの README に従ってください）

3. 同梱の `MixedNuts` フォルダを、ゲームのルートディレクトリ
   （`FatalFrameII.exe` と同じ場所）にそのままコピーします。
   ローダーの `MixedNuts` フォルダに中身が合流します

   ```
   ...\FatalFrameII\FatalFrameII.exe
   ...\FatalFrameII\dinput8.dll                                ← ローダー
   ...\FatalFrameII\MixedNuts\MixedNutsLoader.dll              ← ローダー
   ...\FatalFrameII\MixedNuts\Mods\wheelspeed\wheelspeed.dll   ← この Mod
   ...\FatalFrameII\MixedNuts\Mods\wheelspeed\wheelspeed.ini
   ...\FatalFrameII\MixedNuts\Mods\wheelspeed\wheelspeed.log   ← 起動時に生成
   ```

   ゲームフォルダの開き方：Steam ライブラリでタイトルを右クリック →
   **管理** → **ローカルファイルを閲覧**

4. ゲームを起動し、射影機を構えてホイールを回してみてください

### 1.x からの更新

導入の前に、ゲームのルートから古い `version.dll` と古い `Mods\wheelspeed\` フォルダを
削除してください。1.x の `version.dll` が残っていると、ローダーは新しい Mod を読み込まず、
`MixedNuts\loader.log` に `[!!]` で始まるメッセージを書き出します。

Native120FPSOption（`dinput8.dll` + `Mods\native120fps\`）や TwinSwap
（`xinput1_4.dll` + `Mods\twinswap\`）の 1.x も入れている場合は、まとめて更新してください。
詳しくはローダーの README を参照してください。

## 削除方法

`MixedNuts\Mods\wheelspeed\` フォルダを削除するだけです。
ゲームのファイルは一切変更していないため、完全に元に戻ります。

ローダーは他の Mod のために残しておけます。すべて取り除く場合は、ローダー
（`dinput8.dll` と `MixedNuts` フォルダ）も削除してください。

一時的に無効化したい場合は、`wheelspeed.ini` の `Enabled` を `0` にしてください。

## 設定ファイル

`wheelspeed.ini` で次の項目を変更できます。変更はゲームの再起動後に反映されます。

| 項目 | 意味 |
|---|---|
| `Enabled` | `1` = 有効 / `0` = 無効 |
| `Log` | `1` = ログを出力 / `0` = 出力しない |
| `Multiplier` | ズームとピントの速さの倍率。既定値 `10`。小数も可（`0.1` ～ `100`） |

## 注意事項

- **パッドの操作も同じ倍率で速くなります。** ゲームはパッドとマウスで同じ値を
  使っているためです。主にパッドで遊ぶ場合は、この Mod は向きません
- ゲームのアップデート後に動かなくなることがあります。その場合、Mod は何もせず
  ゲームは素の状態で動きます。ログに理由が記録されます
- ウイルス対策ソフトが誤検知することがあります。ゲームのメモリを書き換える
  仕組みのためで、この Mod はネットワーク通信を一切行わず、ファイルを書き込むのも
  自分のフォルダの中だけです

## 免責事項

**この Mod は無保証で提供されます。使用によって生じたいかなる損害についても、
作者は一切の責任を負いません。** セーブデータの破損・消失、ゲームの動作不良、
その他の不具合を含みます。自己責任でご使用ください。

**導入前に、必ずセーブデータのバックアップを取ってください。**

```
%LOCALAPPDATA%\KoeiTecmo\FatalFrameII\Savedata\
```

## うまく動かないとき

1. ローダーが導入されているか。ゲームのルート（`FatalFrameII.exe` と同じ場所）に
   `dinput8.dll` があり、`MixedNuts\MixedNutsLoader.dll` があるか
2. `MixedNuts\loader.log` があり、次の行が出ているか

   ```
   [OK] wheelspeed: loaded (0 file patches)
   ```

   `loader.log` がなければ、ローダーが読み込まれていません。
   行がなければ、フォルダ名・ファイル名が `MixedNuts\Mods\wheelspeed\wheelspeed.dll`
   になっているかを確認し、`[!!]` や `[NG]` で始まる行を探してください
3. `MixedNuts\Mods\wheelspeed\` に `wheelspeed.log` が生成されているか

`wheelspeed.log` に次の行が出ていれば正常に適用されています（ログは英語で出力されます）。

```
[OK] Code located: ...
[OK] Record 0 (id 0x490E617F) zoom: 0.6 -> 6
[OK] Record 0 (id 0x490E617F) focus: 4 -> 40
[OK] Camera parameter table found. Now watching it once per second.
```

これらの行は、ゲームの起動から数秒で出ます。
ログには、ゲームのバージョン、Steam のビルド、ディスプレイ、GPU、OS、ロケールなどの
環境情報も記録されます。
不具合を報告するときは、GitHub の Issue で `MixedNuts\Mods\wheelspeed\wheelspeed.log` と
`MixedNuts\loader.log` の両方を添付してください。

https://github.com/MixedNutsJP/fatal-frame2-remake-mouse-wheel-camera-speed/issues

## 仕組み

ゲームのファイルは変更しません。

射影機のズームとピントの「入力 1 回あたりの量」は、ゲームがメモリ上に持っている
カメラのパラメータ表に入っています。Mod はこの表の該当する値に倍率を掛けます。
ゲームが表を読み直した場合に備え、1 秒おきに値を確認し、元に戻っていれば
掛け直します（倍率が二重に掛かることはありません）。

表とゲームのコードの位置は、起動時にゲームのコードを調べて割り出します。
想定と違うコードだった場合は何も書き換えません。

---

# English

## What this does

Makes the Camera Obscura's zoom and focus **usable with a mouse wheel**. By default it
is ten times the game's speed, and the multiplier can be changed in the config file.

The game moves zoom and focus by a fixed amount per input. On a gamepad the input
repeats every frame while the button is held, so it feels fine. A mouse wheel notch
counts as a single input, though, so going from one end to the other takes an
enormous amount of scrolling. The game has no setting for this.

## Requirements

- FATAL FRAME II: Crimson Butterfly REMAKE (Steam)
- MixedNutsModLoader 1.0.0 or later (installed separately; download it from the
  loader's Releases)
  Nexus Mods: https://www.nexusmods.com/fatalframe2crimsonbutterflyremake/mods/26
  GitHub: https://github.com/MixedNutsJP/fatal-frame2-remake-mod-loader

No game files are modified; the mod only changes values in memory, so this will not
trip Steam's file integrity verification.

## What's included

| File | Role |
|---|---|
| `MixedNuts\Mods\wheelspeed\wheelspeed.dll` | the mod itself |
| `MixedNuts\Mods\wheelspeed\wheelspeed.ini` | configuration |
| `MixedNuts\Mods\wheelspeed\README.md` | this file |
| `MixedNuts\Mods\wheelspeed\LICENSE.txt` | license |

You copy **the `MixedNuts` folder.** The loader itself (`dinput8.dll` and
`MixedNuts\MixedNutsLoader.dll`) is not included.
A log, `wheelspeed.log`, is created in `MixedNuts\Mods\wheelspeed\` when the game runs.

### Using it with other mods

**Native120FPSOption, MouseWheelCameraSpeed and TwinSwap 2.0.0 all run on the
same loader.** They share one DLL, so they never conflict. The `MixedNuts` folders
simply merge.

If the game folder **already contains another mod's `dinput8.dll`, do not overwrite
it.** The loader's `dinput8.dll` can be renamed to `version.dll` or `xinput1_4.dll`
(see the loader's README).

## Installation

1. Close the game.

2. Install MixedNutsModLoader (follow the loader's README).

3. Copy the `MixedNuts` folder into the game's root directory (the folder containing
   `FatalFrameII.exe`). It merges into the loader's `MixedNuts` folder.

   ```
   ...\FatalFrameII\FatalFrameII.exe
   ...\FatalFrameII\dinput8.dll                                <- loader
   ...\FatalFrameII\MixedNuts\MixedNutsLoader.dll              <- loader
   ...\FatalFrameII\MixedNuts\Mods\wheelspeed\wheelspeed.dll   <- this mod
   ...\FatalFrameII\MixedNuts\Mods\wheelspeed\wheelspeed.ini
   ...\FatalFrameII\MixedNuts\Mods\wheelspeed\wheelspeed.log   <- generated at launch
   ```

   To open the game folder: right-click the title in your Steam library →
   **Manage** → **Browse local files**

4. Launch the game, raise the Camera Obscura and scroll.

### Upgrading from 1.x

Before installing, delete the old `version.dll` and the old `Mods\wheelspeed\` folder
from the game root. If the 1.x `version.dll` is still there, the loader does not load
the new mod and writes a message starting with `[!!]` to `MixedNuts\loader.log`.

If you also have the 1.x versions of Native120FPSOption (`dinput8.dll` +
`Mods\native120fps\`) or TwinSwap (`xinput1_4.dll` + `Mods\twinswap\`), update them all
at once. See the loader's README for details.

## Uninstallation

Delete the `MixedNuts\Mods\wheelspeed\` folder. No game files are modified, so removal
restores the original state completely.

The loader can stay for other mods. To remove everything, also delete the loader
(`dinput8.dll` and the `MixedNuts` folder).

To disable temporarily, set `Enabled` to `0` in `wheelspeed.ini`.

## Configuration

`wheelspeed.ini` exposes the following. Changes take effect after restarting the game.

| Key | Meaning |
|---|---|
| `Enabled` | `1` = on / `0` = off |
| `Log` | `1` = write a log file / `0` = no log |
| `Multiplier` | Speed multiplier for zoom and focus. Default `10`. Decimals allowed (`0.1` - `100`) |

## Notes

- **The gamepad gets faster by the same factor.** The game uses the same values for
  gamepad and mouse. If you mainly play on a gamepad, this mod is not for you
- A game update may break this mod. In that case the mod does nothing and the game
  runs unmodified; the reason is written to the log
- Antivirus software may flag this mod because it writes to the game's memory. It
  performs no network activity, and the only files it writes are inside its own folder

## Disclaimer

**This mod is provided as-is, without any warranty. The author accepts no liability
for any damage arising from its use,** including but not limited to corruption or
loss of save data, game malfunction, or any other problem. Use it at your own risk.

**Always back up your save data before installing.**

```
%LOCALAPPDATA%\KoeiTecmo\FatalFrameII\Savedata\
```

## If it doesn't work

1. Is the loader installed? Is `dinput8.dll` in the game's root folder (next to
   `FatalFrameII.exe`), and is `MixedNuts\MixedNutsLoader.dll` present?
2. Does `MixedNuts\loader.log` exist and contain this line?

   ```
   [OK] wheelspeed: loaded (0 file patches)
   ```

   If `loader.log` is missing, the loader is not being loaded.
   If the line is missing, check that the folder and file names are exactly
   `MixedNuts\Mods\wheelspeed\wheelspeed.dll`, and look for lines starting with `[!!]`
   or `[NG]`
3. Has `wheelspeed.log` been created in `MixedNuts\Mods\wheelspeed\`?

If `wheelspeed.log` contains lines like these, the mod is working:

```
[OK] Code located: ...
[OK] Record 0 (id 0x490E617F) zoom: 0.6 -> 6
[OK] Record 0 (id 0x490E617F) focus: 4 -> 40
[OK] Camera parameter table found. Now watching it once per second.
```

These lines appear within a few seconds of launching the game.
The log also records environment information such as the game version, Steam build,
display, GPU, OS and locale.
When reporting a problem, please open a GitHub Issue and attach both
`MixedNuts\Mods\wheelspeed\wheelspeed.log` and `MixedNuts\loader.log`.

https://github.com/MixedNutsJP/fatal-frame2-remake-mouse-wheel-camera-speed/issues

## How it works

No game files are modified.

The per-input zoom and focus amounts live in a camera parameter table the game keeps
in memory. The mod multiplies those values. In case the game reloads the table, it
checks the values once per second and reapplies the multiplier if they were reset
(it is never applied twice).

The locations of the table and the relevant game code are worked out at startup by
inspecting the game's code. If the code does not look as expected, nothing is changed.

---

## License

MIT License — Copyright (c) 2026 MixedNuts

本ソフトウェアは MIT ライセンスで提供されます。再配布・改変は自由ですが、
著作権表示とライセンス文を必ず残してください。

This software is provided under the MIT License. You are free to redistribute and
modify it, but the copyright notice and the license text must be retained.

https://github.com/MixedNutsJP/fatal-frame2-remake-mouse-wheel-camera-speed
