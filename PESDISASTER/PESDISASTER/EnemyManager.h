#pragma once
#include "DxLib.h"
#include "PlayerManager.h"
#include "AudioManager.h"

/// <summary>
/// エネミーを管理するクラス
/// </summary>
class EnemyManager
{
	/// <summary>
	/// ローカル参照
	/// </summary>
private:
	// --- 他クラス参照データ ---
	/// <summary>
	/// 音響の状態を参照する変数
	/// </summary>
	AudioManager _audioManager;

	// --- エネミーデータ ---
	/// <summary>
	/// 敵の出現タイマーを参照する変数
	/// </summary>
	int _enemySpawnTimer;
	/// <summary>
	/// 敵の出現間隔を参照する変数
	/// </summary>
	int _enemySpawn_Interval;
	/// <summary>
	/// 敵モデルのアニメーションを紐づけ参照する変数
	/// </summary>
	int _enemyAnimAttach_Index;
	/// <summary>
	/// 出現方向の数を参照する変数
	/// </summary>
	int _encountDirectionNumber;

	// --- 3Dモデルデータ ---
	/// <summary>
	/// 敵モデルを読み込み参照する変数
	/// </summary>
	int _enemyModel;
	/// <summary>
	/// 4方向（0:正面, 1:右, 2:後ろ, 3:左）を管理する敵の生存状態の配列を参照する変数
	/// </summary>
	bool _isEnemyAlive[4];

	// --- アニメーション・位置データ ---
	/// <summary>
	/// 敵モデルのアニメーションの再生時間を管理する変数
	/// </summary>
	float _enemyAnimTime;
	/// <summary>
	/// 敵モデルのアニメーションの総時間を参照する変数
	/// </summary>
	float _enemyAnimTotal_Time;
	/// <summary>
	/// 敵とプレイヤーのX距離を参照する変数
	/// </summary>
	float _enemyDistanceX;
	/// <summary>
	/// 敵とプレイヤーのY距離を参照する変数
	/// </summary>
	float _enemyDistanceY;
	/// <summary>
	/// 敵とプレイヤーのZ距離を参照する変数
	/// </summary>
	float _enemyDistanceZ;
	/// <summary>
	/// 敵モデルの大きさを参照する変数
	/// </summary>
	float _enemyModel_ScaleValue;

	// --- 敵の座標データ ---
	/// <summary>
	/// 敵の位置を管理する配列（4方向：0:正面, 1:右, 2:後ろ, 3:左）を参照する変数
	/// </summary>
	VECTOR _enemyPositions[4];

	// --- 敵の角度データ ---
	/// <summary>
	/// 敵が常にプレイヤーを向くためのY軸回転角度（4方向）の配列を参照する変数
	/// </summary>
	float _enemyRotations[4];

	/// <summary>
	/// グローバル参照
	/// </summary>
public:

	// --- コンストラクタ ---
	/// <summary>
	/// エネミークラスのコンストラクタ
	/// </summary>
	EnemyManager();

	// --- 初期化管理メソッド ---
	/// <summary>
	/// エネミーの初期化を行う関数
	/// </summary>
	/// <param name="playerManager"></param>
	void Init(const PlayerManager& _playerManager);

	// --- 毎フレーム管理メソッド ---
	/// <summary>
	/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
	/// </summary>
	/// <param name="playerManager"></param>
	void Update(const PlayerManager& _playerManager);

	// --- エネミーダメージ関係メソッド ---
	/// <summary>
	/// エネミーを撃退する関数
	/// </summary>
	/// <param name="playerManager"></param>
	void Defeat(const PlayerManager& _playerManager);

	// --- 攻撃系メソッド ---
	/// <summary>
	/// プレイヤーをゲームオーバーにさせる条件の関数
	/// </summary>
	/// <returns></returns>
	bool GameOverRule();

	// --- 描画管理メソッド ---
	/// <summary>
	/// 描画を行う関数
	/// </summary>
	void Draw() const;
};