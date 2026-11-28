#include "EnemyManager.h"
#include "ModelUtil.h"
#include "DxLib.h"
#include <cstdlib>

// ============================================================
// 初期化
// ============================================================

/// <summary>
/// エネミークラスのコンストラクタ
/// </summary>
EnemyManager::EnemyManager()
{
	// --- モデルの読み込みと設定(起動時に1回だけ行う) ---
	_enemyModel = MV1LoadModel("Models/EnemyModel/Sparrow_Animations.mv1");
	// 敵モデルの大きさを設定
	MV1SetScale(_enemyModel, VGet(_enemyModel_ScaleValue, _enemyModel_ScaleValue, _enemyModel_ScaleValue));
	// 敵モデルが環境光をしっかり受けるようにする
	LiftModelAmbient(_enemyModel, _enemyAmbientValue);

	// --- アニメーションの紐づけ ---
	_enemyAnimAttach_Index = MV1AttachAnim(_enemyModel, _enemyAnimNumber, -1, FALSE);
	_enemyAnimTotal_Time = MV1GetAttachAnimTotalTime(_enemyModel, _enemyAnimAttach_Index);
}

/// <summary>
/// エネミーの状態をリセットする関数
/// </summary>
/// <param name="_playerManager">プレイヤー管理クラス</param>
void EnemyManager::Init(const PlayerManager& _playerManager)
{
	// 敵の出現タイマーをリセット
	_enemySpawnTimer = 0;
	// 敵モデルのアニメーションの再生時間をリセット
	_enemyAnimTime = 0.0f;

	// 敵の数分のループ
	for (int _i = 0; _i < _encountDirectionNumber; _i++)
	{
		// 敵を全て非生存状態にリセット
		_isEnemyAlive[_i] = false;
	}

	// 敵の出現位置と向きを計算
	SetupSpawnSpots(_playerManager);
}

/// <summary>
/// プレイヤーの位置を基準に、敵の4方向の位置と向きを計算する関数
/// </summary>
void EnemyManager::SetupSpawnSpots(const PlayerManager& _playerManager)
{
	// 基準となるプレイヤー(カメラ)の位置
	const float _centerX = _playerManager.CameraPositionX_Value;
	const float _centerZ = _playerManager.CameraPositionZ_Value;

	// --- 4方向の位置 ---
	_enemyPositions[0] = VGet(_centerX, _enemyDistanceY, _centerZ + _enemyDistanceZ); // 正面
	_enemyPositions[1] = VGet(_centerX + _enemyDistanceX, _enemyDistanceY, _centerZ); // 右
	_enemyPositions[2] = VGet(_centerX, _enemyDistanceY, _centerZ - _enemyDistanceZ); // 後ろ
	_enemyPositions[3] = VGet(_centerX - _enemyDistanceX, _enemyDistanceY, _centerZ); // 左

	// --- 常にプレイヤーを向くためのY軸回転角度 ---
	_enemyRotations[0] = 0.0f;                // 正面
	_enemyRotations[1] = DX_PI_F / 2.0f;      // 右
	_enemyRotations[2] = DX_PI_F;             // 後ろ
	_enemyRotations[3] = -DX_PI_F / 2.0f;     // 左
}

// ============================================================
// 更新
// ============================================================

/// <summary>
/// 毎フレーム呼び出されるエネミーの状態を更新する関数
/// </summary>
void EnemyManager::Update(const PlayerManager&)
{
	// 敵の出現タイマーを進める
	_enemySpawnTimer++;

	// もし敵の出現タイマーが出現間隔以上になった場合
	if (_enemySpawnTimer >= _enemySpawn_Interval)
	{
		SpawnEnemy();

		// 敵の出現タイマーをリセット
		_enemySpawnTimer = 0;
	}

	UpdateAnimation();
}

/// <summary>
/// 敵がいないスポットの中からランダムに1か所選んで敵を出現させ、唸り声を再生する関数
/// </summary>
void EnemyManager::SpawnEnemy()
{
	// --- 敵がいないスポットを集める ---
	int _emptySpots[_encountDirectionNumber];
	int _emptyCount = 0;

	// 敵の数分のループ
	for (int _i = 0; _i < _encountDirectionNumber; _i++)
	{
		// もしそのスポットに敵がいない場合
		if (_isEnemyAlive[_i] == false)
		{
			_emptySpots[_emptyCount] = _i;
			_emptyCount++;
		}
	}

	// もし敵がいないスポットがない場合
	if (_emptyCount == 0)
	{
		return;
	}

	// 敵がいないスポットの中からランダムに選んで敵を出現させる
	int _spawnIndex = rand() % _emptyCount;
	_isEnemyAlive[_emptySpots[_spawnIndex]] = true;

	// 出現するたびに周波数をランダムに変えて、唸り声を再生する
	_audioManager.PlayRandomPitchSE(_audioManager.SE_HandleScreamSound, _audioManager.ScreamFrequencyMin, _audioManager.ScreamFrequencyMax);
}

/// <summary>
/// 敵モデルのアニメーションを進める関数
/// </summary>
void EnemyManager::UpdateAnimation()
{
	// 敵のアニメーションの再生時間を進める
	_enemyAnimTime += _enemyAnimSpeed;

	// もし敵のアニメーションの再生時間が総時間以上になった場合
	if (_enemyAnimTime >= _enemyAnimTotal_Time)
	{
		// 敵のアニメーションの再生時間をループさせる
		_enemyAnimTime -= _enemyAnimTotal_Time;
	}

	// 敵モデルのアニメーションの再生時間を設定
	MV1SetAttachAnimTime(_enemyModel, _enemyAnimAttach_Index, _enemyAnimTime);
}

// ============================================================
// 撃退・ゲームオーバー判定
// ============================================================

/// <summary>
/// カメラが「目標」としている方向のインデックスを取得する関数
/// </summary>
/// <returns>0:正面, 1:右, 2:後ろ, 3:左</returns>
int EnemyManager::GetFacingIndex(const PlayerManager& _playerManager) const
{
	// 目標の角度を1回の回転量で割って方向の番号にし、負の値も0〜3に収める
	int _step = (int)(_playerManager.TargetCameraAngleY / _playerManager.MoveCameraAngle);
	return (_step % _encountDirectionNumber + _encountDirectionNumber) % _encountDirectionNumber;
}

/// <summary>
/// カメラが向いている方向のエネミーを撃退する関数
/// </summary>
/// <param name="_playerManager">プレイヤー管理クラス</param>
void EnemyManager::Defeat(const PlayerManager& _playerManager)
{
	int _facingIndex = GetFacingIndex(_playerManager);

	// もしカメラが向いている方向に敵が生存している場合
	if (_isEnemyAlive[_facingIndex] == true)
	{
		// 敵を倒す
		_isEnemyAlive[_facingIndex] = false;

		// 被弾音を再生
		_audioManager.PlaySE(_audioManager.SE_HandleDamageSound);
	}
}

/// <summary>
/// プレイヤーをゲームオーバーにさせる条件(全方向に敵がいる)を満たしているかを返す関数
/// </summary>
/// <returns>満たしている場合はtrue</returns>
bool EnemyManager::GameOverRule() const
{
	// 敵の数分のループ
	for (int _i = 0; _i < _encountDirectionNumber; _i++)
	{
		// もし敵がいないスポットが1か所でもある場合
		if (_isEnemyAlive[_i] == false)
		{
			return false;
		}
	}

	// 正面・右・後ろ・左の全てのスポットに敵が生存している
	return true;
}

// ============================================================
// 描画
// ============================================================

/// <summary>
/// 描画を行う関数
/// </summary>
void EnemyManager::Draw() const
{
	// 敵の数分のループ
	for (int _i = 0; _i < _encountDirectionNumber; _i++)
	{
		// もしそのスポットに敵がいない場合は次へ
		if (_isEnemyAlive[_i] == false)
		{
			continue;
		}

		// --- 敵モデルの位置と回転を設定して描画する ---
		MV1SetPosition(_enemyModel, _enemyPositions[_i]);
		// 常にプレイヤーを向くように回転させる
		MV1SetRotationXYZ(_enemyModel, VGet(0.0f, _enemyRotations[_i], 0.0f));
		MV1DrawModel(_enemyModel);
	}
}