#include "DxLib.h"
#include "InputManager.h"
#include "GameSceneManager.h"
#include <ctime>

// メインエントリーポイントを担う関数
int WINAPI WinMain(_In_ HINSTANCE _h_Instance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int _nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(1280, 720, 32);
	SetProcessDPIAware();

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 描画先を最初に決める(この関数でカメラ設定などが初期化されるため)
	SetDrawScreen(DX_SCREEN_BACK);

	// --- 3D描画の設定(SetDrawScreenの後) ---
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	// 背景を空色にする(スカイボックスが描けない場合も黒くならない)
	SetBackgroundColor(0, 0, 0);

	// 拡大縮小時に補間をかけて滑らかに描画する
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	srand((unsigned int)time(NULL));

	// 読み込み開始前に Loading 表示を出す
	ClearDrawScreen();
	DrawString(10, 10, "読み込み中......", GetColor(255, 255, 255));
	ScreenFlip();

	InputManager _input;
	GameSceneManager _gameScene;// ← ここで重い読み込みが走る
	
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