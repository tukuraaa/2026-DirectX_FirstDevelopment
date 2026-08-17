#pragma once
#include "DxLib.h"
#include "StateManager.h"
#include "InputManager.h"
#include "AudioManager.h"

/// <summary>
/// プレイヤーの状態を管理するクラス
/// </summary>
class PlayerManager
{
	/// <summary>
	/// ローカル参照
	/// </summary>
private:

	// --- 他クラス参照データ ---
	/// <summary>
	/// リロードミニゲームの状態を参照する変数
	/// </summary>
	ReloadState _reloadState;
	/// <summary>
	/// 音響の状態を参照する変数
	/// </summary>
	AudioManager _audioManager;

	// --- 残弾データ ---
	/// <summary>
	/// 残弾数を参照する変数
	/// </summary>
	int _currentAmmo;
	/// <summary>
	/// 残弾数の最大値を参照する定数の変数
	/// </summary>
	const int _maxAmmo = 9;

	// --- キーデータ ---
	/// <summary>
	/// ターゲットのキーを参照する変数
	/// </summary>
	int _targetKey;
	/// <summary>
	/// キーの候補を参照する配列の変数
	/// </summary>
	int _candidateKeys[5] =
	{
		KEY_INPUT_A,
		KEY_INPUT_B,
		KEY_INPUT_C,
		KEY_INPUT_X,
		KEY_INPUT_Y
	};

	// --- 画像データ ---
	/// <summary>
	/// 照準画像を読み込み参照する変数
	/// </summary>
	int _aimSpriteHandle = LoadGraph("Sprites/Icons/AimSilhouetteSprite.png");

	// --- キーデータ ---
	/// <summary>
	/// ターゲットのキーの文字を参照する変数
	/// </summary>
	char _targetKeyChar;
	/// <summary>
	/// ターゲットのキーの文字の候補を参照する配列の変数
	/// </summary>
	char _candidateChars[5] =
	{
		'A',
		'B',
		'C',
		'X',
		'Y'
	};

	// --- カメラデータ ---
	/// <summary>
	/// カメラの水平角度を参照する変数
	/// </summary>
	float _cameraAngleY = 0.0f;
	/// <summary>
	/// カメラ水平角度の回転速度を参照する変数
	/// </summary>
	float _moveCameraAngleSpeedValue = 5.0f;
	/// <summary>
	/// カメラのY座標を参照する変数
	/// </summary>
	float _cameraPositionY_Value = 200.0f;
	/// <summary>
	/// カメラ水平角度の最大値を参照する変数
	/// </summary>
	float _moveCameraAngleMaxValue = 180.0f;

	/// <summary>
	/// グローバル参照
	/// </summary>
public:

	// --- 位置データ ---
	/// <summary>
	/// カメラの位置を取得して参照する変数
	/// </summary>
	VECTOR CamPos = GetCameraPosition();

	// --- カメラデータ ---
	/// <summary>
	/// カメラの「目標の角度」を参照する変数
	/// </summary>
	float TargetCameraAngleY = 0.0f;
	/// <summary>
	/// カメラ角度の動かす量をを参照する変数
	/// </summary>
	float MoveCameraAngle = 90.0f;
	/// <summary>
	/// カメラのX座標を参照する変数
	/// </summary>
	float CameraPositionX_Value = 30.0f;
	/// <summary>
	/// カメラのZ座標を参照する変数
	/// </summary>
	float CameraPositionZ_Value = -130.0f;

	// --- コンストラクタ ---
	/// <summary>
	/// プレイヤークラスのコンストラクタ
	/// </summary>
	PlayerManager();

	// --- 銃系管理メソッド ---
	/// <summary>
	/// リロードミニゲームの状態を取得する関数
	/// </summary>
	/// <returns></returns>
	ReloadState GetReloadState() const
	{
		// リロードミニゲームの状態を返す
		return _reloadState;
	}

	// --- 初期化管理メソッド ---
	/// <summary>
	/// プレイヤーの初期化を行う関数
	/// </summary>
	void Init();

	// --- 毎フレーム管理メソッド ---
	/// <summary>
	/// 毎フレーム呼び出されるプレイヤーの状態を更新する関数
	/// </summary>
	/// <param name="input"></param>
	void Update(const InputManager& _input);

	// ---プレイヤー操作管理メソッド---
	/// <summary>
	/// プレイヤーの視点操作を行う関数
	/// </summary>
	void Look(const InputManager& _input);

	// --- 描画管理メソッド ---
	/// <summary>
	/// 描画を行う関数
	/// </summary>
	void Draw() const;

	// --- カメラ適応メソッド ---
	/// <summary>
	/// カメラを適応する関数
	/// </summary>
	void CameraAdapt();

	// --- 銃系管理メソッド ---
	/// <summary>
	/// 発砲を試みる関数
	/// </summary>
	/// <returns></returns>
	bool TryShoot();
	/// <summary>
	/// 残弾数をリロードする関数
	/// </summary>
	/// <returns></returns>
	bool IsOutOfAmmo() const
	{
		// 残弾数が0かどうかを返す
		return _currentAmmo == 0;
	}
};