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

	_reloadState = ReloadState::Wait;
}

/// <summary>
/// 毎フレーム呼び出されるプレイヤーの状態を更新する関数
/// </summary>
/// <param name="input"></param>
void PlayerManager::Update(const InputManager& _input)
{
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