#include "DxLib.h"
#include <cstdlib>
#include <ctime>

/// <summary>
/// ゲームシーンを管理する列挙型のクラス
/// </summary>
enum class SceneState
{
	Title,
	MainStage,
	Clear
};
/// <summary>
/// リロードミニゲームの状態を管理する列挙型のクラス
/// </summary>
enum class ReloadState
{
	Wait,
	Play,
	Finished
};

/// <summary>
/// エラー番号を参照する変数
/// </summary>
int _errorNumber = -1;
/// <summary>
/// 色彩の最大値を参照する変数
/// </summary>
int _colorMaxValue = 255;
/// <summary>
/// タイトルテキストの位置を参照する変数
/// </summary>
int _titleTextPositionValue = 200;
/// <summary>
/// タイトルボタン誘導テキストの位置Xを参照する変数
/// </summary>
int _startButtonNaviTextPositionX_Value = 220;
/// <summary>
/// タイトルボタン誘導テキストの位置Yを参照する変数
/// </summary>
int _startButtonNaviTextPositionY_Value = 280;
/// <summary>
/// リロードボタン誘導テキストの位置を参照する変数
/// </summary>
int _reloadButtonNaviTextPositionValue = 10;
/// <summary>
/// ランダム値を参照する変数
/// </summary>
int _randomValue = 5;
/// <summary>
/// ランダムに表示されるボタンの誘導テキストの位置Xを参照する変数
/// </summary>
int _randomButtonNaviTextPositionX_Value = 300;
/// <summary>
/// ランダムに表示されるボタンの誘導テキストの位置Yを参照する変数
/// </summary>
int _randomButtonNaviTextPositionY_Value = 240;
/// <summary>
/// クリアテキストの位置Xを参照する変数
/// </summary>
int _clearTextPositionX_Value = 220;
/// <summary>
/// クリアテキストの位置Yを参照する変数
/// </summary>
int _clearTextPositionY_Value = 200;
/// <summary>
/// タイトルボタンの誘導テキストの位置Xを参照する変数
/// </summary>
int _titleButtonNaviTextPositionX_Value = 180;
/// <summary>
/// タイトルボタンの誘導テキストの位置Yを参照する変数
/// </summary>
int _titleButtonNaviTextPositionY_Value = 280;

/// <summary>
/// メインエントリーポイントを担う関数
/// </summary>
/// <param name="_h_Instance"></param>
/// <param name="_hPrev_Instance"></param>
/// <param name="_lpstr"></param>
/// <param name="_nCmdShow"></param>
/// <returns></returns>
int WINAPI WinMain(HINSTANCE _h_Instance, HINSTANCE _hPrev_Instance, LPSTR _lpstr, int _nCmdShow)
{
	ChangeWindowMode(TRUE);// ウィンドウモードに変更

	// もしライブラリの初期化が失敗した場合
	if (DxLib_Init() == _errorNumber)
	{
		return _errorNumber;// エラー番号を返して終了
	}

	SetDrawScreen(DX_SCREEN_BACK);// 裏画面を描画先に設定

	// ステートの初期化
	SceneState _currentScene = SceneState::Title;// シーンの状態を参照する変数を定義
	ReloadState _reloadState = ReloadState::Wait;// リロードミニゲームの状態を参照する変数を定義

	// 変数の初期化
	int _targetKey = 0;// 押すべきキーコードを管理する変数を定義
	char _targetKeyChar = ' ';// 押すべきキーの文字を管理する変数を定義
	int _candidateKeys[] = { KEY_INPUT_A, KEY_INPUT_B, KEY_INPUT_C, KEY_INPUT_X, KEY_INPUT_Y };// 候補となるキーコードを参照する変数の配列を定義
	char _candidateChars[] = { 'A', 'B', 'C', 'X', 'Y' };// 候補となるキーの文字を参照する変数の配列を定義

	srand((unsigned int)time(NULL));// 乱数の種を現在の時刻で初期化

	// メインループ（メッセージ処理とESCキーが押されるまで続く）
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		// シーンごとの処理
		switch (_currentScene)
		{
		case SceneState::Title:
			DrawString(_titleTextPositionValue, _titleTextPositionValue, "--- PESDISASTER ---", GetColor(_colorMaxValue, 0, 0));
			DrawString(_startButtonNaviTextPositionX_Value, _startButtonNaviTextPositionY_Value, "Press ENTER to Start", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしエンターキーが押された場合
			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				_currentScene = SceneState::MainStage;
				_reloadState = ReloadState::Wait;
			}

			break;

		case SceneState::MainStage:
			DrawString(_reloadButtonNaviTextPositionValue, _reloadButtonNaviTextPositionValue, "Press R to Reload", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// リロードミニゲームの状態に応じた処理
			switch (_reloadState)
			{
			case ReloadState::Wait:

				// もしRキーが押された場合
				if (CheckHitKey(KEY_INPUT_R))
				{
					int r = rand() % _randomValue;// 0から4の乱数を生成
					_targetKey = _candidateKeys[r];// 押すべきキーコードを設定
					_targetKeyChar = _candidateChars[r];// 押すべきキーの文字を設定
					_reloadState = ReloadState::Play;
				}

				break;

			case ReloadState::Play:
				DrawFormatString(_randomButtonNaviTextPositionX_Value, _randomButtonNaviTextPositionY_Value, GetColor(_colorMaxValue, _colorMaxValue, 0), "Push [ %c ] !", _targetKeyChar);

				// もし押すべきキーが押された場合
				if (CheckHitKey(_targetKey))
				{
					_reloadState = ReloadState::Finished;
				}

				break;

			case ReloadState::Finished:
				_currentScene = SceneState::Clear;
				break;
			}

			break;

		case SceneState::Clear:
			DrawString(_clearTextPositionX_Value, _clearTextPositionY_Value, "*** CLEAR ***", GetColor(0, _colorMaxValue, 0));
			DrawString(_titleButtonNaviTextPositionX_Value, _titleButtonNaviTextPositionY_Value, "Press SPACE to Title", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしスペースキーが押された場合
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				_currentScene = SceneState::Title;
			}

			break;
		}

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}