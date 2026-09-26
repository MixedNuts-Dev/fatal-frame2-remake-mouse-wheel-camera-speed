# Mouse Wheel Camera Speed

**FATAL FRAME II: Crimson Butterfly REMAKE** 用の Mod です。
A mod for FATAL FRAME / PROJECT ZERO II: Crimson Butterfly REMAKE.

Created by MixedNuts

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

ゲームのファイルは一切変更しないため、Steam のファイル整合性チェックに
引っかかることはありません。

## 同梱ファイル

| ファイル | 役割 |
|---|---|
| `version.dll` | ローダー |
| `Mods\wheelspeed\wheelspeed.dll` | 本体 |
| `Mods\wheelspeed\wheelspeed.ini` | 設定ファイル |
| `Mods\wheelspeed\README.md` | このファイル |

このうち **`version.dll` と `Mods` フォルダの 2 つ**をコピーします。
起動すると `Mods\wheelspeed\` の中にログ `wheelspeed.log` が作られます。

### 他の Mod との併用

**Native 120FPS Option とは干渉しません。** あちらは `dinput8.dll`、こちらは
`version.dll` を使うので、両方入れても、どちらか一方だけでも、どの順番で
入れても動作します。`Mods` フォルダは中身が合流するだけです。

ゲームのルートに **別の `version.dll` が既にある場合は、上書きしないでください。**
他のツールが同じ名前を使っている可能性があります。

## 導入方法

1. ゲームを終了します

2. 同梱の `version.dll` と `Mods` フォルダを、ゲームのルートディレクトリ
   （`FatalFrameII.exe` と同じ場所）にそのままコピーします

   ```
   ...\steamapps\common\FatalFrameII\FatalFrameII.exe
   ...\steamapps\common\FatalFrameII\version.dll        ← 追加
   ...\steamapps\common\FatalFrameII\Mods\wheelspeed\   ← 追加
   ```

   ゲームフォルダの開き方：Steam ライブラリでタイトルを右クリック →
   **管理** → **ローカルファイルを閲覧**

3. ゲームを起動し、射影機を構えてホイールを回してみてください

## 削除方法

`version.dll` と `Mods\wheelspeed` フォルダを削除するだけです。
ゲームのファイルは一切変更していないため、完全に元に戻ります。

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

1. `version.dll` がゲームのルート（`FatalFrameII.exe` と同じ場所）にあるか。
   **`Mods` フォルダの中ではありません**
2. `Mods\wheelspeed\` の中に `wheelspeed.dll` があるか。
   フォルダ名・ファイル名を変更していないか
3. `Mods\wheelspeed\` に `wheelspeed.log` が生成されているか。
   生成されていなければ `version.dll` が読み込まれていません

ログに次の行が出ていれば正常に適用されています（ログは英語で出力されます）。

```
[OK] Code located: ...
[OK] Record 0 (id 0x490E617F) zoom: 0.6 -> 6
[OK] Record 0 (id 0x490E617F) focus: 4 -> 40
[OK] Camera parameter table found. Now watching it once per second.
```

これらの行は、ゲームの起動から数秒で出ます。
不具合を報告するときは、GitHub の Issue で `wheelspeed.log` を添付してください。

https://github.com/MixedNuts-Dev/fatal-frame2-remake-mouse-wheel-camera-speed/issues

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

No game files are modified, so this will not trip Steam's file integrity verification.

## What's included

| File | Role |
|---|---|
| `version.dll` | loader |
| `Mods\wheelspeed\wheelspeed.dll` | the mod itself |
| `Mods\wheelspeed\wheelspeed.ini` | configuration |
| `Mods\wheelspeed\README.md` | this file |

You copy two things: **`version.dll` and the `Mods` folder.**
A log, `wheelspeed.log`, is created in `Mods\wheelspeed\` when the game runs.

### Using it with other mods

**It does not interfere with Native 120FPS Option.** That mod uses `dinput8.dll` and
this one uses `version.dll`, so either or both can be installed, in any order. The
`Mods` folders simply merge.

If the game folder **already contains a different `version.dll`, do not overwrite
it.** Another tool may be using the same name.

## Installation

1. Close the game.

2. Copy `version.dll` and the `Mods` folder into the game's root directory
   (the folder containing `FatalFrameII.exe`).

   ```
   ...\steamapps\common\FatalFrameII\FatalFrameII.exe
   ...\steamapps\common\FatalFrameII\version.dll        <- added
   ...\steamapps\common\FatalFrameII\Mods\wheelspeed\   <- added
   ```

   To open the game folder: right-click the title in your Steam library →
   **Manage** → **Browse local files**

3. Launch the game, raise the Camera Obscura and scroll.

## Uninstallation

Delete `version.dll` and the `Mods\wheelspeed` folder. No game files are modified,
so removal restores the original state completely.

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

1. Is `version.dll` in the game's root folder (next to `FatalFrameII.exe`)?
   **It does not go inside the `Mods` folder**
2. Is `wheelspeed.dll` present in `Mods\wheelspeed\`?
   Have the folder or file names been changed?
3. Has `wheelspeed.log` been created in `Mods\wheelspeed\`?
   If not, `version.dll` is not being loaded

If the log contains lines like these, the mod is working:

```
[OK] Code located: ...
[OK] Record 0 (id 0x490E617F) zoom: 0.6 -> 6
[OK] Record 0 (id 0x490E617F) focus: 4 -> 40
[OK] Camera parameter table found. Now watching it once per second.
```

These lines appear within a few seconds of launching the game.
When reporting a problem, please open a GitHub Issue and attach `wheelspeed.log`.

https://github.com/MixedNuts-Dev/fatal-frame2-remake-mouse-wheel-camera-speed/issues

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

https://github.com/MixedNuts-Dev/fatal-frame2-remake-mouse-wheel-camera-speed
