#include "DxLib.h"
#include <cstdlib>
#include <ctime>

// ゲームの状態
enum class GameState { Wait, Play, Clear };

// プログラムの入り口（WinMain）
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// ウィンドウモードで起動する（これがないと全画面になります）
	ChangeWindowMode(TRUE);

	// DXライブラリの初期化（ウィンドウの作成やDirectX11の準備を全部やってくれます）
	if (DxLib_Init() == -1) { return -1; }

	// 描画先を裏画面にする（画面のチラつき防止）
	SetDrawScreen(DX_SCREEN_BACK);

	// --- ゲームの準備 ---
	GameState state = GameState::Wait;
	int targetKey = 0;       // DxLibのキーコード
	char targetKeyChar = ' '; // 画面表示用の文字

	// 候補のキー（DXライブラリ専用のキーコードを使います）
	int candidateKeys[] = { KEY_INPUT_A, KEY_INPUT_B, KEY_INPUT_C, KEY_INPUT_X, KEY_INPUT_Y };
	char candidateChars[] = { 'A', 'B', 'C', 'X', 'Y' };
	int candidateCount = 5;

	srand((unsigned)time(NULL));

	// --- メインループ ---
	// ProcessMessage: Windowsの裏側の処理、CheckHitKey: ESCで終了
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		ClearDrawScreen(); // 画面を真っ黒にクリア

		// 【1. ロジック（状態の更新）】
		switch (state) {
		case GameState::Wait:
			// Rキーが押されたか
			if (CheckHitKey(KEY_INPUT_R)) {
				int r = rand() % candidateCount;
				targetKey = candidateKeys[r];
				targetKeyChar = candidateChars[r];
				state = GameState::Play;
			}
			break;

		case GameState::Play:
			// 指定されたキーが押されたか
			if (CheckHitKey(targetKey)) {
				state = GameState::Clear;
			}
			break;

		case GameState::Clear:
			// スペースキーでリトライ
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				state = GameState::Wait;
			}
			break;
		}

		// 【2. 描画処理】
		// 常に表示（文字を出すのが信じられないくらい簡単です！）
		DrawString(10, 10, "Rボタンを押してください (ESCで終了)", GetColor(255, 255, 255));

		switch (state) {
		case GameState::Wait:
			break;

		case GameState::Play:
			// 変数を使って文字を描画する
			DrawFormatString(300, 240, GetColor(255, 255, 0), "指定キー: %c を押せ！", targetKeyChar);
			break;

		case GameState::Clear:
			DrawString(250, 240, "ゲームクリア！ (Spaceでリトライ)", GetColor(0, 255, 255));
			break;
		}

		ScreenFlip(); // 裏画面を表画面に反映
	}

	// DXライブラリの終了処理
	DxLib_End();
	return 0;
}