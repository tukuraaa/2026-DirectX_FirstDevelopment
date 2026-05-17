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
	Clear,
	GameOver
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
/// 前のフレームの左キーの状態を参照する変数
/// </summary>
int _prevLeftKey = 0;
/// <summary>
/// 前のフレームの右キーの状態を参照する変数
/// </summary>
int _prevRightKey = 0;
/// <summary>
/// ボタン入力の成功数を参照する変数
/// </summary>
int _inputNumber = 1;
/// <summary>
/// 割る数を参照する変数
/// </summary>
int _divisorValue = 2;
/// <summary>
/// 引く数を参照する変数
/// </summary>
int _subtractValue = 1;
/// <summary>
/// 方向の数を参照する変数
/// </summary>
int _directionNumber = 4;
/// <summary>
/// 敵のアニメーションの紐付け関係のインデックスを参照する変数
/// </summary>
int _enemyAnimAttachAbout_Index = -1;
/// <summary>
/// 前のフレームのエンターキーの状態を参照する変数
/// </summary>
int _prevReturnKey = 0;

/// <summary>
/// カメラの水平角度を参照する変数
/// </summary>
float _cameraAngleY = 0.0f;
/// <summary>
/// カメラ角度の動かす量をを参照する変数
/// </summary>
float _moveCameraAngle = 90.0f;
/// <summary>
/// カメラ水平角度の最大値を参照する変数
/// </summary>
float _moveCameraAngleMaxValue = 180.0f;
/// <summary>
/// カメラのX座標を参照する変数
/// </summary>
float _cameraPositionX_Value = 30.0f;
/// <summary>
/// カメラのY座標を参照する変数
/// </summary>
float _cameraPositionY_Value = 200.0f;
/// <summary>
/// カメラのZ座標を参照する変数
/// </summary>
float _cameraPositionZ_Value = -130.0f;
/// <summary>
/// カメラ水平角度の回転速度を参照する変数
/// </summary>
float _moveCameraAngleSpeedValue = 5.0f;
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
/// 円周率を参照する変数
/// </summary>
float _piValue = 3.14f;
/// <summary>
/// スカイボックスのZ軸回転の倍率を参照する変数
/// </summary>
float _skyBoxAngleZ_MagnificationValue = 1.0f;
/// <summary>
/// 敵の回転角度の最大値を参照する変数
/// </summary>
float _enemyRotationRagianValueMax = 180.0f;
/// <summary>
/// 敵モデルの大きさを参照する変数
/// </summary>
float _enemyModel_ScaleValue = 0.9f;

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
	// もしライブラリの初期化が失敗した場合
	if (DxLib_Init() == _errorNumber)
	{
		return _errorNumber;// エラー番号を返して終了
	}

	// 3D描画用のZバッファを有効化
	SetUseZBuffer3D(TRUE);// 3D描画でZバッファを使用するように設定
	SetWriteZBuffer3D(TRUE);// 3D描画でZバッファに書き込むように設定

	SetCameraNearFar(_cameraTransformForegroundValue, _cameraTransformBackValue);// カメラの描画限界を広げる

	SetDrawScreen(DX_SCREEN_BACK);// 裏画面を描画先に設定

	// ステートの初期化
	SceneState _currentScene = SceneState::Title;// シーンの状態を参照する変数を定義
	ReloadState _reloadState = ReloadState::Wait;// リロードミニゲームの状態を参照する変数を定義

	int _enemyModel = MV1LoadModel("Models/EnemyModel/Enemy.mv1");// 敵モデルを読み込み参照する変数を定義
	MV1SetScale(_enemyModel, VGet(_enemyModel_ScaleValue, _enemyModel_ScaleValue, _enemyModel_ScaleValue));// モデルの大きさを調整

	int _enemyAnimAttach_Index = MV1AttachAnim(_enemyModel, 0, _enemyAnimAttachAbout_Index, FALSE);// モデルに含まれているアニメーションを紐付け参照する変数を定義

	float _enemyAnimTime = 0.0f;// アニメーションの現在の再生時間を管理する変数を定義

	float _enemyAnimTotal_Time = MV1GetAttachAnimTotalTime(_enemyModel, _enemyAnimAttach_Index);// アニメーションの総時間（全体の長さ）を取得して保存する変数を定義

	bool _isEnemyAlive[4] = { false, false, false, false };// 4方向（0:正面, 1:右, 2:後ろ, 3:左）に敵がいるかどうかを判定する配列の変数を定義

	// 敵の出現管理の変数の初期化
	int _enemySpawnTimer = 0;// 敵が出現するまでの時間を計るタイマーを参照する変数を定義
	int _enemySpawn_Interval = 300;// 敵が出現する間隔を参照する変数を定義

	float _enemyDistanceX = 100.0f; // 敵が出現する距離を参照する変数を定義
	float _enemyDistanceY = 140.0f; // 敵が出現する高さを参照する変数を定義
	float _enemyDistanceZ = 100.0f; // 敵が出現する距離を参照する変数を定義

	// 敵の位置を管理する配列の変数を定義（4方向：0:正面, 1:右, 2:後ろ, 3:左）
	VECTOR _enemyPositions[4] =
	{
		VGet(0.0f + _cameraPositionX_Value, _enemyDistanceY, _enemyDistanceZ + _cameraPositionZ_Value),// 0:正面 (Zプラス方向)
		VGet(_enemyDistanceX + _cameraPositionX_Value, _enemyDistanceY, 0.0f + _cameraPositionZ_Value),// 1:右 (Xプラス方向)
		VGet(0.0f + _cameraPositionX_Value,  _enemyDistanceY, -_enemyDistanceZ + _cameraPositionZ_Value),// 2:後ろ (Zマイナス方向)
		VGet(-_enemyDistanceX + _cameraPositionX_Value, _enemyDistanceY, 0.0f + _cameraPositionZ_Value)// 3:左 (Xマイナス方向)
	};

	// 敵が常にプレイヤーを向くためのY軸回転角度を管理する配列の変数を定義（4方向：0:正面, 1:右, 2:後ろ, 3:左）
	float _enemyRotations[4] =
	{
		0.0f,// 0:正面の敵は、回転なし（0度）で手前（プレイヤー側）を向く
		(_enemyRotationRagianValueMax / _divisorValue) * _piValue / _enemyRotationRagianValueMax,// 1:右の敵は、90度回転して左を向く
		_enemyRotationRagianValueMax * _piValue / _enemyRotationRagianValueMax,// 2:後ろの敵は、180度回転して奥を向く
		(-_enemyRotationRagianValueMax / _divisorValue) * _piValue / _enemyRotationRagianValueMax// 3:左の敵は、-90度回転して右を向く
	};

	int _gameTimer = 3600;// 制限時間を参照する変数を定義
	int _gameTimerMax = 3600;// 制限時間の最大値を参照する変数を定義

	// 変数の初期化
	int _targetKey = 0;// 押すべきキーコードを管理する変数を定義
	char _targetKeyChar = ' ';// 押すべきキーの文字を管理する変数を定義
	int _candidateKeys[] = { KEY_INPUT_A, KEY_INPUT_B, KEY_INPUT_C, KEY_INPUT_X, KEY_INPUT_Y };// 候補となるキーコードを参照する変数の配列を定義
	char _candidateChars[] = { 'A', 'B', 'C', 'X', 'Y' };// 候補となるキーの文字を参照する変数の配列を定義

	srand((unsigned int)time(NULL));// 乱数の種を現在の時刻で初期化

	int _houseModel = MV1LoadModel("Models/HouseModel/House.mv1");// 廃屋モデルを読み込みモデルハンドルを参照する変数を定義

	MV1SetScale(_houseModel, VGet(_houseModel_ScaleValue, _houseModel_ScaleValue, _houseModel_ScaleValue));// モデルの大きさを調整

	int _skyboxModel = MV1LoadModel("Models/SkyboxModel/Skybox.mv1");// スカイボックス用の球体モデルをファイルから読み込む

	MV1SetScale(_skyboxModel, VGet(_skyBoxScale, _skyBoxScale, _skyBoxScale));// モデルをマイナス100倍にして「超巨大化」＆「裏返し」にする
	MV1SetRotationXYZ(_skyboxModel, VGet(0.0f, 0.0f, _piValue * _skyBoxAngleZ_MagnificationValue));// スカイボックスの向きを調整

	// BGMの読み込み
	int _bgmHandleTitle = LoadSoundMem("Sounds/BGM/TitleSound.mp3");// タイトルBGMを読み込み参照する変数を定義
	int _bgmHandleMainStage = LoadSoundMem("Sounds/BGM/MainStageSound.mp3");// メインステージBGMを読み込み参照する変数を定義
	int _bgmHandleClear = LoadSoundMem("Sounds/BGM/ClearSound.mp3");// クリアシーンBGMを読み込み参照する変数を定義
	int _bgmHandleGameOver = LoadSoundMem("Sounds/BGM/GameOverSound.mp3");// ゲームオーバーシーンBGMを読み込み参照する変数を定義

	int _screamSoundHandle = LoadSoundMem("Sounds/SE/MonsterScream.mp3");// 唸り声の効果音を読み込み参照する変数を定義

	int _screamVolume = 200;// 唸り声のボリュームを参照する変数を定義
	ChangeVolumeSoundMem(_screamVolume, _screamSoundHandle);// 唸り声をハッキリ聞こえさせるために少し大きめ（200）に設定

	// メインループ（メッセージ処理とESCキーが押されるまで続く）
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		// 現在のキー入力状態を取得
		int _currentLeft = CheckHitKey(KEY_INPUT_LEFT);// 左キーの状態を参照する変数を定義
		int _currentRight = CheckHitKey(KEY_INPUT_RIGHT);// 右キーの状態を参照する変数を定義
		int _currentReturn = CheckHitKey(KEY_INPUT_RETURN);// 現在のエンターキーの状態を参照する変数を定義

		VECTOR _camPos = GetCameraPosition();// カメラの位置を取得

		// 残り時間の画面表示に必要な変数の定義
		int _surviveTime = _gameTimer / 60;// 60で割ることで「秒」に変換する変数を定義
		int _timeFormatTransformX_Value = 10;// 時間表示の位置Xを参照する変数を定義
		int _timeFormatTransformY_Value = 30;// 時間表示の位置Yを参照する変数を定義

		// シーンごとの処理
		switch (_currentScene)
		{
		case SceneState::Title:
			StopSoundMem(_bgmHandleClear);// クリアシーンBGMを停止する
			StopSoundMem(_bgmHandleGameOver);// ゲームオーバーシーンBGMを停止する

			// もしタイトルBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleTitle) == 0)
			{
				PlaySoundMem(_bgmHandleTitle, DX_PLAYTYPE_LOOP, TRUE);// タイトルBGMをループ再生する
			}

			DrawString(_titleTextPositionValue, _titleTextPositionValue, "--- PESDISASTER ---", GetColor(_colorMaxValue, 0, 0));
			DrawString(_startButtonNaviTextPositionX_Value, _startButtonNaviTextPositionY_Value, "Press ENTER to Start", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしエンターキーが押された上で、前のフレームではエンターキーが押されていなかった場合
			if (_currentReturn == _inputNumber && _prevReturnKey == 0)
			{
				_currentScene = SceneState::MainStage;
				_reloadState = ReloadState::Wait;

				// 4方向の敵の出現状態をリセットするループ
				for (int i = 0; i < _directionNumber; i++)
				{
					_isEnemyAlive[i] = false;// 敵の出現状態をリセット
				}

				_enemySpawnTimer = 0;// タイマーをリセット
			}

			break;

		case SceneState::MainStage:
			StopSoundMem(_bgmHandleTitle);// タイトルBGMを停止する

			// もしメインステージBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleMainStage) == 0)
			{
				PlaySoundMem(_bgmHandleMainStage, DX_PLAYTYPE_LOOP, TRUE);// メインステージBGMをループ再生する
			}

			MV1SetPosition(_skyboxModel, _camPos);// カメラに追従させる

			// 描画前の環境設定
			SetWriteZBuffer3D(FALSE);// Zバッファ書き込みをOFF
			SetUseLighting(FALSE);// 空間の光をOFF

			MV1DrawModel(_skyboxModel);// 描画

			// 描画後の環境復元
			SetUseLighting(TRUE);// 空間の光をONに戻す
			SetWriteZBuffer3D(TRUE);// Zバッファ書き込みをONに戻す

			// 座標(0, 0, 0)に家を描画
			MV1SetPosition(_houseModel, VGet(0.0f, 0.0f, 0.0f));// モデルの位置を設定
			MV1DrawModel(_houseModel);// モデルを描画

			DrawString(_reloadButtonNaviTextPositionValue, _reloadButtonNaviTextPositionValue, "Press R to Reload", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			_enemySpawnTimer++;// 毎フレームタイマーを増やす

			// もしタイマーが敵の出現間隔以上になった場合
			if (_enemySpawnTimer >= _enemySpawn_Interval)
			{
				// まだ敵がいない「空き場所」を探す
				int _emptySpots[4];// 空き場所のインデックスを保存する配列の変数を定義
				int _emptyCount = 0;// 空き場所の数を数える変数を定義

				// 4方向をチェックするループ
				for (int i = 0; i < _directionNumber; i++)
				{
					// もしその場所に敵がいない場合
					if (_isEnemyAlive[i] == false)
					{
						_emptySpots[_emptyCount] = i;// 空き場所のインデックスを保存
						_emptyCount++;// 空き場所の数を増やす
					}
				}

				// もし空き場所がある場合
				if (_emptyCount > 0)
				{
					int _spawn_Index = GetRand(_emptyCount - _subtractValue);// 0 ～ 空き数-1 の乱数を取得し参照する変数を定義
					_isEnemyAlive[_emptySpots[_spawn_Index]] = true;// 選ばれた場所の敵を出現状態にする

					int _randomFrequency = 21000 + (rand() % 18001);// 21000から39000の乱数を生成して参照する変数を定義
					SetFrequencySoundMem(_randomFrequency, _screamSoundHandle);// 唸り声の周波数を設定する

					PlaySoundMem(_screamSoundHandle, DX_PLAYTYPE_BACK, TRUE);// 効果音を再生する
				}

				_enemySpawnTimer = 0;// タイマーをリセットして次の出現に備える
			}

			_enemyAnimTime += 0.5f;// 毎フレーム再生時間を進める

			// もしアニメーションが最後まで再生された場合
			if (_enemyAnimTime >= _enemyAnimTotal_Time)
			{
				_enemyAnimTime -= _enemyAnimTotal_Time;// 再生時間をリセットしてループさせる
			}

			MV1SetAttachAnimTime(_enemyModel, _enemyAnimAttach_Index, _enemyAnimTime);// 現在の再生時間をモデルに反映させる

			// 4方向の敵のループ
			for (int i = 0; i < _directionNumber; i++)
			{
				// もし敵が出現している場合
				if (_isEnemyAlive[i] == true)
				{
					MV1SetPosition(_enemyModel, _enemyPositions[i]);// 位置を設定
					MV1SetRotationXYZ(_enemyModel, VGet(0.0f, _enemyRotations[i], 0.0f));// プレイヤーの方を向かせる
					MV1DrawModel(_enemyModel);// 描画
				}
			}

			// もし0〜3（正面・右・後ろ・左）すべてがtrueの場合
			if (_isEnemyAlive[0] && _isEnemyAlive[((_directionNumber - _subtractValue) - _subtractValue) - _subtractValue] && _isEnemyAlive[(_directionNumber - _subtractValue) - _subtractValue] && _isEnemyAlive[_directionNumber - _subtractValue])
			{
				_currentScene = SceneState::GameOver;
			}

			// もし左キーを押した上で、前のフレームでは左キーが押されていなかった場合
			if (_currentLeft == _inputNumber && _prevLeftKey == 0)
			{
				_targetCameraAngleY -= _moveCameraAngle;// 「目標角度」をマイナス90度する
			}

			// もし右キーを押した上で、前のフレームでは右キーが押されていなかった場合
			if (_currentRight == _inputNumber && _prevRightKey == 0)
			{
				_targetCameraAngleY += _moveCameraAngle;// 「目標角度」をプラス90度する
			}

			// もし「目標角度」が現在のカメラ角度より小さい場合
			if (_cameraAngleY < _targetCameraAngleY)
			{
				_cameraAngleY += _moveCameraAngleSpeedValue;// カメラ角度を「目標角度」に近づけるようにプラスする

				// もしカメラ角度が「目標角度」を超えてしまった場合
				if (_cameraAngleY > _targetCameraAngleY)
				{
					_cameraAngleY = _targetCameraAngleY;// カメラ角度を「目標角度」に合わせる
				}
			}
			// もし「目標角度」が現在のカメラ角度より大きい場合
			else if (_cameraAngleY > _targetCameraAngleY)
			{
				_cameraAngleY -= _moveCameraAngleSpeedValue;// カメラ角度を「目標角度」に近づけるようにマイナスする

				// もしカメラ角度が「目標角度」を超えてしまった場合
				if (_cameraAngleY < _targetCameraAngleY)
				{
					_cameraAngleY = _targetCameraAngleY;// カメラ角度を「目標角度」に合わせる
				}
			}

			// もしタイマーが0より大きい場合
			if (_gameTimer > 0)
			{
				_gameTimer--;// 毎フレーム1ずつ減らす
			}
			else
			{
				_currentScene = SceneState::Clear;// タイマーが0になったらクリアシーンへ移行
			}

			DrawFormatString(_timeFormatTransformX_Value, _timeFormatTransformY_Value, GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue), "SURVIVE TIME: %d", _surviveTime);// 60で割ることで「秒」に変換して画面に表示します

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
				_reloadState = ReloadState::Wait;
				break;
			}

			break;

		case SceneState::Clear:
			StopSoundMem(_bgmHandleMainStage);// メインステージBGMを停止する

			// もしクリアシーンBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleClear) == 0)
			{
				PlaySoundMem(_bgmHandleClear, DX_PLAYTYPE_LOOP, TRUE);// クリアシーンBGMをループ再生する
			}

			// メインステージの情報をリセット
			_gameTimer = _gameTimerMax;// タイマーをリセットしておく
			_cameraAngleY = 0.0f;// カメラの角度を正面にリセット
			_targetCameraAngleY = 0.0f;// 「目標の角度」も正面にリセット

			DrawString(_sceneNamePositionX_Value, _sceneNamePositionY_Value, "*** CLEAR ***", GetColor(0, _colorMaxValue, 0));
			DrawString(_enterButtonNaviTextPositionX_Value, _enterButtonNaviTextPositionY_Value, "Press ENTER to Title", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしエンターキーが押された上で、前のフレームではエンターキーが押されていなかった場合
			if (_currentReturn == _inputNumber && _prevReturnKey == 0)
			{
				_currentScene = SceneState::Title;
			}

			break;

		case SceneState::GameOver:
			StopSoundMem(_bgmHandleMainStage);// メインステージBGMを停止する

			// もしゲームオーバーシーンBGMが再生されていない場合
			if (CheckSoundMem(_bgmHandleGameOver) == 0)
			{
				PlaySoundMem(_bgmHandleGameOver, DX_PLAYTYPE_LOOP, TRUE);// ゲームオーバーシーンBGMをループ再生する
			}

			// メインステージの情報をリセット
			_gameTimer = _gameTimerMax;// タイマーをリセットしておく
			_cameraAngleY = 0.0f;// カメラの角度を正面にリセット
			_targetCameraAngleY = 0.0f;// 「目標の角度」も正面にリセット

			DrawString(_sceneNamePositionX_Value, _sceneNamePositionY_Value, "*** GAME OVER ***", GetColor(_colorMaxValue, 0, 0));
			DrawString(_enterButtonNaviTextPositionX_Value, _enterButtonNaviTextPositionY_Value, "Press ENTER to Title", GetColor(_colorMaxValue, _colorMaxValue, _colorMaxValue));

			// もしエンターキーが押された上で、前のフレームではエンターキーが押されていなかった場合
			if (_currentReturn == _inputNumber && _prevReturnKey == 0)
			{
				_currentScene = SceneState::Title;
			}

			break;
		}

		// カメラ設定
		float _radianY = _cameraAngleY * DX_PI_F / _moveCameraAngleMaxValue;// 水平角度をラジアンに変換した値を参照する変数を定義
		SetCameraPositionAndAngle(VGet(_cameraPositionX_Value, _cameraPositionY_Value, _cameraPositionZ_Value), 0.0f, _radianY, 0.0f);// カメラの位置と角度を設定

		// 入力状態を保存（次のフレームで比較するため）
		_prevLeftKey = _currentLeft;// 左キーの状態を保存
		_prevRightKey = _currentRight;// 右キーの状態を保存
		_prevReturnKey = _currentReturn;// エンターキーの状態を保存

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}