#include "DxLib.h"
#include "Common.h"
#include <cstdlib>
#include <ctime>

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
/// シーンの名前の位置Xを参照する変数
/// </summary>
int _sceneNamePositionX_Value = 220;
/// <summary>
/// シーンの名前の位置Yを参照する変数
/// </summary>
int _sceneNamePositionY_Value = 200;
/// <summary>
/// エンターキーボタンの誘導テキストの位置Xを参照する変数
/// </summary>
int _enterButtonNaviTextPositionX_Value = 180;
/// <summary>
/// エンターキーボタンの誘導テキストの位置Yを参照する変数
/// </summary>
int _enterButtonNaviTextPositionY_Value = 280;


/// <summary>
/// 前のフレームのエンターキーの状態を参照する変数
/// </summary>
int _prevReturnKey = 0;
/// <summary>
/// 前のフレームのスペースキーの状態を参照する変数
/// </summary>
int _prevSpaceKey = 0;

/// <summary>
/// カメラ角度の動かす量をを参照する変数
/// </summary>
float _moveCameraAngle = 90.0f;
/// <summary>
/// カメラ水平角度の最大値を参照する変数
/// </summary>
float _moveCameraAngleMaxValue = 180.0f;

/// <summary>
/// 廃屋モデルの大きさを参照する変数
/// </summary>
float _houseModel_ScaleValue = 1.0f;
/// <summary>
/// カメラの前景の描画を参照する変数
/// </summary>
float _cameraTransformForegroundValue = 1.0f;
/// <summary>
/// カメラの背景の描画を参照する変数
/// </summary>
float _cameraTransformBackValue = 20000.0f;
/// <summary>
/// カメラの「目標の角度」を参照する変数
/// </summary>
float _targetCameraAngleY = 0.0f;
/// <summary>
/// スカイボックスの大きさを参照する変数
/// </summary>
float _skyBoxScale = -100.0f;

/// <summary>
/// スカイボックスのZ軸回転の倍率を参照する変数
/// </summary>
float _skyBoxAngleZ_MagnificationValue = 1.0f;

/// <summary>
/// 敵モデルの大きさを参照する変数
/// </summary>
float _enemyModel_ScaleValue = 0.9f;

/// <summary>
/// メインエントリーポイントを担う関数
/// </summary>
int WINAPI WinMain(_In_ HINSTANCE _h_Instance, _In_opt_ HINSTANCE _hPrev_Instance, _In_ LPSTR _lpstr, _In_ int _nCmdShow)
{
	// もしライブラリの初期化が失敗した場合
	if (DxLib_Init() == _errorNumber)
	{
		return _errorNumber;// エラー番号を返して終了
	}

	// 3D描画用のZバッファを有効化
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	SetCameraNearFar(_cameraTransformForegroundValue, _cameraTransformBackValue);// カメラの描画限界を広げる

	SetDrawScreen(DX_SCREEN_BACK);// 裏画面を描画先に設定

	// ステートの初期化
	SceneState _currentScene = SceneState::Title;
	ReloadState _reloadState = ReloadState::Wait;


	MV1SetScale(_enemyModel, VGet(_enemyModel_ScaleValue, _enemyModel_ScaleValue, _enemyModel_ScaleValue));// 敵モデルの大きさを設定





	bool _isEnemyAlive[4] = { false, false, false, false };// 4方向（0:正面, 1:右, 2:後ろ, 3:左）を管理する敵の生存状態の配列を参照する変数を定義

	// 敵の出現管理の変数の初期化
	int _enemySpawnTimer = 0;// 敵の出現タイマーを参照する変数を定義








	int _gameTimer = 3600 * 3;// 制限時間を参照する変数を定義
	int _gameTimerMax = 3600 * 3;// 制限時間の最大値を参照する変数を定義

	// 残弾数システムの追加変数
	int _maxAmmo = 9;// マガジンの最大装弾数を参照する変数を定義
	int _currentAmmo = _maxAmmo;// 現在の残弾数を参照する変数を定義

	// 変数の初期化
	int _targetKey = 0;// リロードミニゲームの正解キーを参照する変数を定義
	char _targetKeyChar = ' ';// リロードミニゲームの正解キーの文字を参照する変数を定義
	int _candidateKeys[] = { KEY_INPUT_A, KEY_INPUT_B, KEY_INPUT_C, KEY_INPUT_X, KEY_INPUT_Y };// リロードミニゲームの候補キーの配列を参照する変数を定義
	char _candidateChars[] = { 'A', 'B', 'C', 'X', 'Y' };// リロードミニゲームの候補キーの文字の配列を参照する変数を定義

	srand((unsigned int)time(NULL));// 乱数の種を初期化


	MV1SetScale(_houseModel, VGet(_houseModel_ScaleValue, _houseModel_ScaleValue, _houseModel_ScaleValue));// 廃屋モデルの大きさを設定


	MV1SetScale(_skyboxModel, VGet(_skyBoxScale, _skyBoxScale, _skyBoxScale));// スカイボックスの大きさを設定
	MV1SetRotationXYZ(_skyboxModel, VGet(0.0f, 0.0f, _piValue * _skyBoxAngleZ_MagnificationValue));// スカイボックスの初期回転を設定

	// BGMの読み込み
	int _bgmHandleTitle = LoadSoundMem("Sounds/BGM/TitleSound.mp3");// タイトル画面のBGMを読み込み参照する変数を定義
	int _bgmHandleMainStage = LoadSoundMem("Sounds/BGM/MainStageSound.mp3");// メインステージのBGMを読み込み参照する変数を定義
	int _bgmHandleClear = LoadSoundMem("Sounds/BGM/ClearSound.mp3");// クリア画面のBGMを読み込み参照する変数を定義
	int _bgmHandleGameOver = LoadSoundMem("Sounds/BGM/GameOverSound.mp3");// ゲームオーバー画面のBGMを読み込み参照する変数を定義

	// SEの読み込み
	int _screamSoundHandle = LoadSoundMem("Sounds/SE/MonsterScream.mp3");// 敵出現の唸り声を読み込み参照する変数を定義
	int _shotSoundHandle = LoadSoundMem("Sounds/SE/ShootSound.mp3");// 銃の発砲音を読み込み参照する変数を定義
	int _nonMagazineSoundHandle = LoadSoundMem("Sounds/SE/NonMagazineSound.mp3");// 弾切れの音を読み込み参照する変数を定義

	int _screamVolume = 600;// 敵の唸り声のボリュームを参照する変数を定義
	ChangeVolumeSoundMem(_screamVolume, _screamSoundHandle);// 敵の唸り声のボリュームを設定

	int _aimSpriteHandle = LoadGraph("Sprites/Icons/AimSilhouetteSprite.png");// 照準画像

	// メインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		// 現在のキー入力状態を取得
		int _currentLeft = CheckHitKey(KEY_INPUT_LEFT);// 左キーの状態を取得して参照する変数を定義
		int _currentRight = CheckHitKey(KEY_INPUT_RIGHT);// 右キーの状態を取得して参照する変数を定義
		int _currentReturn = CheckHitKey(KEY_INPUT_RETURN);// エンターキーの状態を取得して参照する変数を定義
		int _currentSpace = CheckHitKey(KEY_INPUT_SPACE);// スペースキーの状態を取得して参照する変数を定義



		// 残り時間の画面表示に必要な変数の定義
		int _surviveTime = _gameTimer / 60;// フレーム数を秒数に変換して参照する変数を定義
		int _timeFormatTransformX_Value = 10;// タイム表示の位置Xを参照する変数を定義
		int _timeFormatTransformY_Value = 30;// タイム表示の位置Yを参照する変数を定義

		// 画面中央にレティクル（照準）を描画するための変数の定義
		int _centerX = 320;// 画面の幅の半分を参照する変数を定義
		int _centerY = 240;// 画面の高さの半分を参照する変数を定義
		float _exRate = 0.1f;// レティクルの拡大率を参照する変数を定義

		// シーンごとの処理
		switch (_currentScene)
		{
		case SceneState::Title:
			StopSoundMem(_bgmHandleClear);
			StopSoundMem(_bgmHandleGameOver);

			// もしタイトル画面のBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleTitle) == 0)
			{
				PlaySoundMem(_bgmHandleTitle, DX_PLAYTYPE_LOOP, TRUE);
			}

			// タイトルテキストとエンターキーの誘導テキストを描画
			DrawString(_titleTextPositionValue, _titleTextPositionValue, "--- PESDISASTER ---", GetColor(_colorMaxValue, 0, 0));
			DrawString(_startButtonNaviTextPositionX_Value, _startButtonNaviTextPositionY_Value, "Press ENTER to Start", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			break;

		case SceneState::MainStage:
			StopSoundMem(_bgmHandleTitle);

			// もしメインステージのBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleMainStage) == 0)
			{
				PlaySoundMem(_bgmHandleMainStage, DX_PLAYTYPE_LOOP, TRUE);
			}



			// UIテキスト表示
			DrawString(_reloadButtonNaviTextPositionValue, _reloadButtonNaviTextPositionValue, "Press R to Reload", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));
			DrawFormatString(_reloadButtonNaviTextPositionValue, _reloadButtonNaviTextPositionValue + 40, GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue), "AMMO: %d / %d", _currentAmmo, _maxAmmo);

			// もし弾が尽きていて、かつリロードミニゲームが待機状態の場合
			if (_currentAmmo == 0 && _reloadState == ReloadState::Wait)
			{
				DrawString(_reloadButtonNaviTextPositionValue, _reloadButtonNaviTextPositionValue + 60, "OUT OF AMMO! PRESS R!", GetColor(_colorMaxValue, 0, 0));
			}

			DrawRotaGraph(_centerX, _centerY, _exRate, 0.0f, _aimSpriteHandle, TRUE);// 画面中央にレティクルを描画

			// もしゲームタイマーが0より大きい場合
			if (_gameTimer > 0)
			{
				_gameTimer--;// ゲームタイマーを減らす
			}
			else
			{
				_currentScene = SceneState::Clear;
			}

			DrawFormatString(_timeFormatTransformX_Value, _timeFormatTransformY_Value, GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue), "SURVIVE TIME: %d", _surviveTime);

			// リロードミニゲームの処理
			switch (_reloadState)
			{
			case ReloadState::Wait:

				// もし、弾が満タンでなく、かつRキーが押された場合
				if (_currentAmmo < _maxAmmo && CheckHitKey(KEY_INPUT_R))
				{
					int r = rand() % _randomValue;// 0から4のランダムな整数を生成して参照する変数を定義
					_targetKey = _candidateKeys[r];// ランダムに選ばれたキーをリロードミニゲームの正解キーに設定
					_targetKeyChar = _candidateChars[r];// ランダムに選ばれたキーの文字を参照する変数に設定
					_reloadState = ReloadState::Play;
				}

				break;

			case ReloadState::Play:
				DrawFormatString(_randomButtonNaviTextPositionX_Value, _randomButtonNaviTextPositionY_Value, GetColor(_colorMaxValue, _colorMaxValue, 0), "Push [ %c ] !", _targetKeyChar);

				// もし正解キーが押された場合
				if (CheckHitKey(_targetKey))
				{
					_reloadState = ReloadState::Finished;
				}

				break;

			case ReloadState::Finished:
				_currentAmmo = _maxAmmo;// ミニゲーム完了時に残弾数を最大までリセット
				_reloadState = ReloadState::Wait;

				break;
			}

			break;

		case SceneState::Clear:
			StopSoundMem(_bgmHandleMainStage);

			// もしクリア画面のBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleClear) == 0)
			{
				PlaySoundMem(_bgmHandleClear, DX_PLAYTYPE_LOOP, TRUE);
			}

			_gameTimer = _gameTimerMax;// ゲームタイマーをリセット
			_cameraAngleY = 0.0f;// カメラの水平角度をリセット
			_targetCameraAngleY = 0.0f;// カメラの「目標の角度」をリセット

			// クリアテキストとエンターキーの誘導テキストを描画
			DrawString(_sceneNamePositionX_Value, _sceneNamePositionY_Value, "*** CLEAR ***", GetColor(0, _colorMaxValue, 0));
			DrawString(_enterButtonNaviTextPositionX_Value, _enterButtonNaviTextPositionY_Value, "Press ENTER to Title", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしエンターキーが新しく押された場合
			if (_currentReturn == _inputNumber && _prevReturnKey == 0)
			{
				_currentScene = SceneState::Title;
			}

			break;

		case SceneState::GameOver:
			StopSoundMem(_bgmHandleMainStage);

			// もしゲームオーバー画面のBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleGameOver) == 0)
			{
				PlaySoundMem(_bgmHandleGameOver, DX_PLAYTYPE_LOOP, TRUE);
			}

			_gameTimer = _gameTimerMax;// ゲームタイマーをリセット
			_cameraAngleY = 0.0f;// カメラの水平角度をリセット
			_targetCameraAngleY = 0.0f;// カメラの「目標の角度」をリセット

			// ゲームオーバーテキストとエンターキーの誘導テキストを描画
			DrawString(_sceneNamePositionX_Value, _sceneNamePositionY_Value, "*** GAME OVER ***", GetColor(_colorMaxValue, 0, 0));
			DrawString(_enterButtonNaviTextPositionX_Value, _enterButtonNaviTextPositionY_Value, "Press ENTER to Title", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしエンターキーが新しく押された場合
			if (_currentReturn == _inputNumber && _prevReturnKey == 0)
			{
				_currentScene = SceneState::Title;
			}

			break;
		}

		// カメラの適用
		float _radianY = _cameraAngleY * DX_PI_F / _moveCameraAngleMaxValue;// カメラの水平角度をラジアンに変換して参照する変数を定義
		SetCameraPositionAndAngle(VGet(_cameraPositionX_Value, _cameraPositionY_Value, _cameraPositionZ_Value), 0.0f, _radianY, 0.0f);// カメラの位置と角度を設定

		// すべてのシーンで共通して毎フレーム入力を保存する
		_prevLeftKey = _currentLeft;// 左キーの履歴保存を追加
		_prevRightKey = _currentRight;// 左キーと右キーの履歴保存を追加
		_prevReturnKey = _currentReturn;// エンターキーの履歴保存を追加
		_prevSpaceKey = _currentSpace;// スペースキーの履歴保存を追加

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}