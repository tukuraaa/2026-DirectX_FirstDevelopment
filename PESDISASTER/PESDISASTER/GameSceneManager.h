#pragma once
#include "StateManager.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "AudioManager.h"

// --- クラス定義 ---
/// <summary>
/// ゲームシーンを管理するクラス
/// </summary>
class GameSceneManager
{
	/// <summary>
	/// ローカル参照
	/// </summary>
private:

	// --- 他クラス参照データ ---
	/// <summary>
	/// 現在のシーンの状態を参照する変数
	/// </summary>
	SceneState _currentScene;
	/// <summary>
	/// プレイヤーの状態を参照する変数
	/// </summary>
	PlayerManager _playerManager;
	/// <summary>
	/// エネミーの状態を参照する変数
	/// </summary>
	EnemyManager _enemyManager;
	/// <summary>
	/// 音響の状態を参照する変数
	/// </summary>
	AudioManager _audioManager;

	// --- ゲームタイマーデータ ---
	/// <summary>
	/// 制限時間を参照する変数
	/// </summary>
	int _gameTimer = 3600 * 3;
	/// <summary>
	/// フレーム数を秒数に変換して参照する変数
	/// </summary>
	int _surviveTime = _gameTimer / 60;
	/// <summary>
	/// タイム表示の位置Xを参照する変数
	/// </summary>
	int _timeFormatTransformX_Value = 10;
	/// <summary>
	/// タイム表示の位置Yを参照する変数
	/// </summary>
	int _timeFormatTransformY_Value = 30;
	/// <summary>
	/// ゲームの制限時間を参照する定数の変数
	/// </summary>
	const int _gameTimerMax = 3600 * 3;

	// --- 3Dモデルデータ ---
	/// <summary>
	/// 廃屋モデルを読み込み参照する変数
	/// </summary>
	int _houseModel = MV1LoadModel("Models/HouseModel/House.mv1");
	/// <summary>
	/// スカイボックスモデルを読み込み参照する変数
	/// </summary>
	int _skyboxModel = MV1LoadModel("Models/SkyboxModel/Skybox.mv1");

	// --- 廃屋データ ---	
	/// <summary>
	/// 廃屋モデルの大きさを参照する変数
	/// </summary>
	float _houseModel_ScaleValue = 1.0f;

	// --- スカイボックスデータ ---
	/// <summary>
	/// スカイボックスの大きさを参照する変数
	/// </summary>
	float _skyBoxScale = -100.0f;
	/// <summary>
	/// スカイボックスのZ軸回転の倍率を参照する変数
	/// </summary>
	float _skyBoxAngleZ_MagnificationValue = 1.0f;

	/// <summary>
	/// グローバル参照
	/// </summary>
public:

	// --- コンストラクタ ---
	/// <summary>
	/// ゲームシーンクラスのコンストラクタ
	/// </summary>
	GameSceneManager();

	// --- 初期化管理メソッド ---
	/// <summary>
	/// ゲームシーンの初期化を行う関数
	/// </summary>
	void Init();

	// --- 毎フレーム管理メソッド ---
	/// <summary>
	/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
	/// </summary>
	/// <param name="input"></param>
	void Update(const InputManager& _input);

	// --- 描画管理メソッド ---
	/// <summary>
	/// 描画を行う関数
	/// </summary>
	void Draw() const;
};