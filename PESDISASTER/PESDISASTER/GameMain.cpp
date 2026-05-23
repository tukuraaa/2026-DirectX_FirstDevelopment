#include "DxLib.h"
#include "InputManager.h"
#include "GameSceneManager.h"
#include <ctime>

// メインエントリーポイントを担う関数
int WINAPI WinMain(_In_ HINSTANCE _h_Instance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int _nCmdShow)
{
	// もしDXライブラリの初期化でエラーが発生した場合
	if (DxLib_Init() == -1)
	{
		// エラーで返す
		return -1;
	}

	// --- 3D描画の設定 ---
	// Zバッファの使用を有効化
	SetUseZBuffer3D(TRUE);
	// Zバッファへの書き込みを有効化
	SetWriteZBuffer3D(TRUE);
	// カメラの描画限界を広げる
	SetCameraNearFar(1.0f, 20000.0f);
	// 描画先を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	srand((unsigned int)time(NULL));// 乱数の種を初期化

	// --- ゲームの状態管理の変数の初期化 ---
	// 入力管理クラスのインスタンスを生成
	InputManager _input;
	// ゲームシーンの管理クラスのインスタンスを生成
	GameSceneManager _gameScene;

	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		// 入力の更新
		_input.Update();

		// ゲームロジックの更新
		_gameScene.Update(_input);

		// 描画
		_gameScene.Draw();

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}