#include "PlayerManager.h"
#include <cstdlib>

/// <summary>
/// プレイヤークラスのコンストラクタ
/// </summary>
PlayerManager::PlayerManager()
{
	Init();// コンストラクタで初期化関数を呼び出す
}

/// <summary>
/// プレイヤーの初期化を行う関数
/// </summary>
void PlayerManager::Init()
{
	// --- プレイヤー情報を初期化 ---
	// 残弾数を最大値に設定
	_currentAmmo = _maxAmmo;
	// カメラの水平角度をリセット
	_cameraAngleY = 0.0f;
	// カメラの「目標の角度」をリセット
	TargetCameraAngleY = 0.0f;
	// リロード状態をリセット
	_reloadState = ReloadState::Wait;
	// モードもリセット
	_currentMode = PlayerMode::Explore;
	// カメラの上下角度をリセット
	_cameraAngleX = 0.0f;
	// プレイヤーのスピードをリセット
	_playerSpeed = 2.0f;
	// 壁に近づける限界の距離を指定
	_playerRadius = 10.0f;
}

/// <summary>
/// 毎フレーム呼び出されるプレイヤーの状態を更新する関数
/// </summary>
/// <param name="input"></param>
void PlayerManager::Update(const InputManager& _input, int mapModelHandle)
{
	// --- テスト ---
	// もしTABキーを押した場合
	if (_input.IsKeyDown(KEY_INPUT_TAB))
	{
		// もしモードが探索モードの場合
		if (_currentMode == PlayerMode::Explore)
		{
			// バトルモードに変更
			_currentMode = PlayerMode::Battle;
		}
		else
		{
			// 探索モードに変更
			_currentMode = PlayerMode::Explore;
		}
	}

	// もしモードが探索モードの場合
	if (_currentMode == PlayerMode::Explore)
	{
		// --- 探索モード ---

		// --- 自由な移動と視点操作を呼び出す ---
		LookExplore();
		MoveExplore(_input, mapModelHandle);
	}
	else
	{
		// --- バトルモード ---

		// 視点操作メソッド呼び出し
		Look(_input);

		// リロードミニゲームの状態に応じた処理
		switch (_reloadState)
		{
		case ReloadState::Wait:

			// もし、弾が満タンでなく、かつRキーが押された場合
			if (_currentAmmo < _maxAmmo && _input.IsKeyDown(KEY_INPUT_R))
			{
				// ---ターゲットキーをランダムに抽選---
				// 0から4のランダムな整数を生成して参照する変数を定義
				int _r = rand() % 5;
				// ランダムに選ばれたキーをターゲットキーとして設定
				_targetKey = _candidateKeys[_r];
				// ランダムに選ばれたキーの文字をターゲットキーの文字として設定
				_targetKeyChar = _candidateChars[_r];

				_reloadState = ReloadState::Play;

				// リロードミニゲーム開始の音を再生
				_audioManager.PlaySE(_audioManager.SE_HandleReloadGameStartSound);
			}

			break;

		case ReloadState::Play:

			// もし正解キーが押された場合
			if (_input.IsKeyDown(_targetKey))
			{
				_reloadState = ReloadState::Finished;
			}

			break;

		case ReloadState::Finished:

			// ミニゲーム完了時に残弾数を最大までリセット
			_currentAmmo = _maxAmmo;

			_reloadState = ReloadState::Wait;

			// リロード成功の音を再生
			_audioManager.PlaySE(_audioManager.SE_HandleReloadSound);

			break;
		}
	}
}

/// <summary>
/// プレイヤーの視点操作を行う関数
/// </summary>
void PlayerManager::Look(const InputManager& _input)
{
	// もし左キーが新しく押された場合
	if (_input.IsKeyDown(KEY_INPUT_LEFT))
	{
		// カメラの「目標の角度」を左に回転させる
		TargetCameraAngleY -= MoveCameraAngle;
	}

	// もし右キーが新しく押された場合
	if (_input.IsKeyDown(KEY_INPUT_RIGHT))
	{
		// カメラの「目標の角度」を右に回転させる
		TargetCameraAngleY += MoveCameraAngle;
	}

	// もしカメラの「目標の角度」と現在の角度が等しくない場合
	if (_cameraAngleY < TargetCameraAngleY)
	{
		// カメラの水平角度を「目標の角度」に近づける
		_cameraAngleY += _moveCameraAngleSpeedValue;

		// もしカメラの水平角度が「目標の角度」を超えてしまった場合
		if (_cameraAngleY > TargetCameraAngleY)
		{
			// カメラの水平角度を「目標の角度」に合わせる
			_cameraAngleY = TargetCameraAngleY;
		}
	}
	// もしカメラの「目標の角度」と現在の角度が等しくない場合
	else if (_cameraAngleY > TargetCameraAngleY)
	{
		// カメラの水平角度を「目標の角度」に近づける
		_cameraAngleY -= _moveCameraAngleSpeedValue;

		// もしカメラの水平角度が「目標の角度」を超えてしまった場合
		if (_cameraAngleY < TargetCameraAngleY)
		{
			// カメラの水平角度を「目標の角度」に合わせる
			_cameraAngleY = TargetCameraAngleY;
		}
	}
}

/// <summary>
/// カメラを適応する関数
/// </summary>
void PlayerManager::CameraAdapt()
{
	// --- カメラの適用 ---
	// カメラの水平角度をラジアンに変換して参照する変数を定義
	float _radianY = _cameraAngleY * DX_PI_F / _moveCameraAngleMaxValue;
	// カメラの位置と角度を設定
	SetCameraPositionAndAngle(VGet(CameraPositionX_Value, _cameraPositionY_Value, CameraPositionZ_Value), 0.0f, _radianY, 0.0f);
}

/// <summary>
/// プレイヤー関係のUIを描画する関数
/// </summary>
void PlayerManager::Draw() const
{
	// カメラの上下角度（_cameraAngleX）も反映させる
	SetCameraPositionAndAngle(CamPos, _cameraAngleX, _cameraAngleY, 0.0f);

	// ---リロードボタンと残弾数をUIで表示---
	DrawString(10, 10, "Press R to Reload", GetColor(255, 255, 255));
	DrawFormatString(10, 50, GetColor(255, 255, 255), "AMMO: %d / %d", _currentAmmo, _maxAmmo);

	// もし弾が尽きていて、かつリロードミニゲームが待機状態の場合
	if (_currentAmmo == 0 && _reloadState == ReloadState::Wait)
	{
		DrawString(10, 70, "OUT OF AMMO! PRESS R!", GetColor(255, 0, 0));
	}

	// もしリロードミニゲームの状態がPlayの場合
	if (_reloadState == ReloadState::Play)
	{
		// ランダムに表示されるボタンの誘導テキストを描画
		DrawFormatString(300, 240, GetColor(255, 255, 0), "Push [ %c ] !", _targetKeyChar);
	}

	// 画面中央にレティクルを描画
	DrawRotaGraph(320, 240, 0.1f, 0.0f, _aimSpriteHandle, TRUE);
}

/// <summary>
/// 射撃を試みる関数
/// </summary>
/// <returns></returns>
bool PlayerManager::TryShoot()
{
	// もし残弾数が0より大きく、かつリロードミニゲームの状態がWaitの場合
	if (_currentAmmo > 0 && _reloadState == ReloadState::Wait)
	{
		// 残弾数を1減らす
		_currentAmmo--;

		// 発砲音再生
		_audioManager.PlaySE(_audioManager.SE_HandleShotSound);

		// 発砲が成功したことを返す
		return true;
	}
	// もしプレイヤーが撃てない状態で、かつリロードミニゲームが待機状態の場合
	else if (IsOutOfAmmo() && GetReloadState() == ReloadState::Wait)
	{
		// 弾切れの音を再生
		_audioManager.PlaySE(_audioManager.SE_HandleNonMagazineSound);

		// 弾切れ音再生が成功したことを返す
		return false;
	}

	// 発砲できない場合はfalseを返す
	return false;
}

/// <summary>
/// マウスによる自由視点操作を管理する関数
/// </summary>
void PlayerManager::LookExplore()
{
	// --- マウス関連の情報を定義 ---
	int _width, _height;
	GetDrawScreenSize(&_width, &_height);
	int _centerX = _width / 2;
	int _centerY = _height / 2;

	// --- マウスの座標を定義 ---
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// --- 画面中央からのマウス移動量を計算 ---
	float _deltaX = (float)(mouseX - _centerX) * 0.002f;
	float _deltaY = (float)(mouseY - _centerY) * 0.002f;

	// --- カメラの角度に加算（Y軸は左右、X軸は上下） ---
	_cameraAngleY -= _deltaX;
	_cameraAngleX -= _deltaY;

	// --- 真上や真下を見すぎないように上下の角度を制限 (-80度 〜 80度程度) ---
	// 制限角度を定義
	float _limit = 80.0f * DX_PI_F / 180.0f;
	// もしカメラの回転角度が制限角度（プラス角度）より大きかった場合
	if (_cameraAngleX > _limit)
	{
		// 制限角度で固定する
		_cameraAngleX = _limit;
	}
	// もしカメラの回転角度が制限角度（マイナス角度）より小さかった場合
	if (_cameraAngleX < -_limit)
	{
		// 制限角度で固定する
		_cameraAngleX = -_limit;
	}

	// マウスカーソルを画面中央に強制的に戻す
	SetMousePoint(_centerX, _centerY);
}

/// <summary>
/// WASD移動と壁貫通防止（壁ずり）を管理する関数
/// </summary>
/// <param name="_input"></param>
/// <param name="mapModelHandle"></param>
void PlayerManager::MoveExplore(const InputManager& _input, int _mapModelHandle)
{
	// 動くための座標を定義
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

	// --- WASDキーによる移動方向の決定 ---
	// もしWキーを押している間の場合
	if (_input.IsKeyHold(KEY_INPUT_W))
	{
		// Z軸のプラス方向に動く
		moveVec.z += _playerSpeed;
	}
	// もしSキーを押している間の場合
	if (_input.IsKeyHold(KEY_INPUT_S))
	{
		// Z軸のマイナス方向に動く
		moveVec.z -= _playerSpeed;
	}
	// もしAキーを押している間の場合
	if (_input.IsKeyHold(KEY_INPUT_A))
	{
		// X軸のマイナス方向に動く
		moveVec.x -= _playerSpeed;
	}
	// もしDキーを押している間の場合
		if (_input.IsKeyHold(KEY_INPUT_D))
		{
			// X軸のプラス方向に動く
			moveVec.x += _playerSpeed;
		}

	// --- カメラの向いているY角度に合わせて移動ベクトルを回転させる ---
	MATRIX rotY = MGetRotY(_cameraAngleY);
	VECTOR realMove = VTransform(moveVec, rotY);

	// --- 現在の座標を取得 ---
	VECTOR currentPos = VGet(CameraPositionX_Value, _cameraPositionY_Value, CameraPositionZ_Value);

	// --- X軸の移動と当たり判定（カプセル判定） ---
	VECTOR nextPosX = currentPos;
	nextPosX.x += realMove.x;
	MV1_COLL_RESULT_POLY hitX = MV1CollCheck_Capsule(_mapModelHandle, -1, currentPos, nextPosX, _playerRadius);
	// もしコライダーが当たっていない場合
	if (hitX.HitFlag == FALSE) 
	{
		// 進む
		currentPos.x = nextPosX.x;
	}

	// --- Z軸の移動と当たり判定 ---
	VECTOR nextPosZ = currentPos;
	nextPosZ.z += realMove.z;
	MV1_COLL_RESULT_POLY hitZ = MV1CollCheck_Capsule(_mapModelHandle, -1, currentPos, nextPosZ, _playerRadius);
	// もしコライダーが当たっていない場合
	if (hitZ.HitFlag == FALSE)
	{
		// 進む
		currentPos.z = nextPosZ.z;
	}

	// --- 計算結果をメンバ変数に書き戻す ---
	CameraPositionX_Value = currentPos.x;
	CameraPositionZ_Value = currentPos.z;
	CamPos = VGet(CameraPositionX_Value, _cameraPositionY_Value, CameraPositionZ_Value);
}