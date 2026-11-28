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
	/// 音響の状態を参照する変数(全クラスで共有する唯一のインスタンス)
	/// </summary>
	AudioManager& _audioManager = AudioManager::Instance();

	// --- 出現データ(定数) ---
	/// <summary>
	/// 出現方向の数(0:正面, 1:右, 2:後ろ, 3:左)を参照する定数
	/// </summary>
	static constexpr int _encountDirectionNumber = 4;
	/// <summary>
	/// 敵の出現間隔(フレーム数)を参照する定数
	/// </summary>
	static constexpr int _enemySpawn_Interval = 300;
	/// <summary>
	/// 敵とプレイヤーのX距離を参照する定数
	/// </summary>
	static constexpr float _enemyDistanceX = 150.0f;
	/// <summary>
	/// 敵とプレイヤーのY距離を参照する定数
	/// </summary>
	static constexpr float _enemyDistanceY = 170.0f;
	/// <summary>
	/// 敵とプレイヤーのZ距離を参照する定数
	/// </summary>
	static constexpr float _enemyDistanceZ = 150.0f;

	// --- モデルデータ(定数) ---
	/// <summary>
	/// 敵モデルの大きさを参照する定数
	/// </summary>
	static constexpr float _enemyModel_ScaleValue = 0.9f;
	/// <summary>
	/// 敵モデルの環境光への反応の強さ(0.0〜1.0)を参照する定数
	/// </summary>
	static constexpr float _enemyAmbientValue = 0.9f;
	/// <summary>
	/// 再生するアニメーションの番号を参照する定数
	/// </summary>
	static constexpr int _enemyAnimNumber = 6;
	/// <summary>
	/// アニメーションを1フレームで進める量を参照する定数
	/// </summary>
	static constexpr float _enemyAnimSpeed = 0.5f;

	// --- 3Dモデルデータ ---
	/// <summary>
	/// 敵モデルを読み込み参照する変数
	/// </summary>
	int _enemyModel = -1;

	// --- アニメーションデータ ---
	/// <summary>
	/// 敵モデルのアニメーションを紐づけ参照する変数
	/// </summary>
	int _enemyAnimAttach_Index = -1;
	/// <summary>
	/// 敵モデルのアニメーションの再生時間を管理する変数
	/// </summary>
	float _enemyAnimTime = 0.0f;
	/// <summary>
	/// 敵モデルのアニメーションの総時間を参照する変数
	/// </summary>
	float _enemyAnimTotal_Time = 0.0f;

	// --- 敵の状態データ ---
	/// <summary>
	/// 敵の出現タイマーを参照する変数
	/// </summary>
	int _enemySpawnTimer = 0;
	/// <summary>
	/// 4方向(0:正面, 1:右, 2:後ろ, 3:左)の敵の生存状態を管理する配列の変数
	/// </summary>
	bool _isEnemyAlive[_encountDirectionNumber] = {};
	/// <summary>
	/// 敵の位置を管理する配列(4方向)の変数
	/// </summary>
	VECTOR _enemyPositions[_encountDirectionNumber] = {};
	/// <summary>
	/// 敵が常にプレイヤーを向くためのY軸回転角度(4方向)の配列の変数
	/// </summary>
	float _enemyRotations[_encountDirectionNumber] = {};

	// --- 初期化補助メソッド ---
	/// <summary>
	/// プレイヤーの位置を基準に、敵の4方向の位置と向きを計算する関数
	/// </summary>
	void SetupSpawnSpots(const PlayerManager& _playerManager);

	// --- 更新補助メソッド ---
	/// <summary>
	/// 敵がいないスポットの中からランダムに1か所選んで敵を出現させ、唸り声を再生する関数
	/// </summary>
	void SpawnEnemy();
	/// <summary>
	/// 敵モデルのアニメーションを進める関数
	/// </summary>
	void UpdateAnimation();
	/// <summary>
	/// カメラが「目標」としている方向のインデックスを取得する関数
	/// </summary>
	/// <returns>0:正面, 1:右, 2:後ろ, 3:左</returns>
	int GetFacingIndex(const PlayerManager& _playerManager) const;

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
	/// エネミーの状態をリセットする関数
	/// </summary>
	/// <param name="_playerManager">プレイヤー管理クラス</param>
	void Init(const PlayerManager& _playerManager);

	// --- 毎フレーム管理メソッド ---
	/// <summary>
	/// 毎フレーム呼び出されるエネミーの状態を更新する関数
	/// </summary>
	/// <param name="_playerManager">プレイヤー管理クラス(現在は未使用)</param>
	void Update(const PlayerManager& _playerManager);

	// --- エネミーダメージ関係メソッド ---
	/// <summary>
	/// カメラが向いている方向のエネミーを撃退する関数
	/// </summary>
	/// <param name="_playerManager">プレイヤー管理クラス</param>
	void Defeat(const PlayerManager& _playerManager);

	// --- 攻撃系メソッド ---
	/// <summary>
	/// プレイヤーをゲームオーバーにさせる条件(全方向に敵がいる)を満たしているかを返す関数
	/// </summary>
	/// <returns>満たしている場合はtrue</returns>
	bool GameOverRule() const;

	// --- 描画管理メソッド ---
	/// <summary>
	/// 描画を行う関数
	/// </summary>
	void Draw() const;
};