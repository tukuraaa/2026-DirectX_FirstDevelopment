#include "EnemyManager.h"
#include "DxLib.h"

/// <summary>
/// エネミークラスのコンストラクタ
/// </summary>
EnemyManager::EnemyManager()
{}

/// <summary>
/// エネミーの初期化を行う関数
/// </summary>
void EnemyManager::Init(const PlayerManager& _playerManager)
{
	// --- 基本データの初期化 ---
	// 敵の出現タイマーをリセット
	_enemySpawnTimer = 0;
	// 敵の出現間隔をリセット
	_enemySpawn_Interval = 300;
	// 出現方向の数をリセット
	_encountDirectionNumber = 4;
	// 敵モデルのアニメーションの再生時間をリセット
	_enemyAnimTime = 0.0f;
	// 敵モデルの大きさをリセット
	_enemyModel_ScaleValue = 0.9f;

	// --- 敵とプレイヤーの距離をリセット ---
	_enemyDistanceX = 100.0f;
	_enemyDistanceY = 140.0f;
	_enemyDistanceZ = 100.0f;

	// 敵の数分のループ
	for (int i = 0; i < _encountDirectionNumber; i++)
	{
		// 敵を全て非生存状態にリセット
		_isEnemyAlive[i] = false;
	}

	// モデルのロード
	_enemyModel = MV1LoadModel("Models/EnemyModel/Enemy.mv1");

	// 敵モデルの大きさを設定
	MV1SetScale(_enemyModel, VGet(_enemyModel_ScaleValue, _enemyModel_ScaleValue, _enemyModel_ScaleValue));

	// --- ロードしたモデルを使ってアニメーションを設定 ---
	_enemyAnimAttach_Index = MV1AttachAnim(_enemyModel, 0, -1, FALSE);
	_enemyAnimTotal_Time = MV1GetAttachAnimTotalTime(_enemyModel, _enemyAnimAttach_Index);

	// --- 本物のプレイヤーの位置を基準に、敵の4方向の位置を計算 ---
	_enemyPositions[0] = VGet(0.0f + _playerManager.CameraPositionX_Value, _enemyDistanceY, _enemyDistanceZ + _playerManager.CameraPositionZ_Value); // 正面
	_enemyPositions[1] = VGet(_enemyDistanceX + _playerManager.CameraPositionX_Value, _enemyDistanceY, 0.0f + _playerManager.CameraPositionZ_Value); // 右
	_enemyPositions[2] = VGet(0.0f + _playerManager.CameraPositionX_Value, _enemyDistanceY, -_enemyDistanceZ + _playerManager.CameraPositionZ_Value); // 後ろ
	_enemyPositions[3] = VGet(-_enemyDistanceX + _playerManager.CameraPositionX_Value, _enemyDistanceY, 0.0f + _playerManager.CameraPositionZ_Value); // 左

	// --- 回転の設定 ---
	_enemyRotations[0] = 0.0f;
	_enemyRotations[1] = (180.0f / 2) * DX_PI_F / 180.0f;
	_enemyRotations[2] = 180.0f * DX_PI_F / 180.0f;
	_enemyRotations[3] = (-180.0f / 2) * DX_PI_F / 180.0f;

	// 敵の唸り声のボリュームを設定
	_audioManager.ChangeVolume(_audioManager.SE_HandleScreamSound, _audioManager.ScreamVolume);
}

/// <summary>
/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
/// </summary>
/// <param name="input"></param>
void EnemyManager::Update(const PlayerManager& _playerManager)
{
	// 敵の出現タイマーを進める
	_enemySpawnTimer++;

	// もし敵の出現タイマーが出現間隔以上になった場合
	if (_enemySpawnTimer >= _enemySpawn_Interval)
	{
		// --- 敵がいないスポットを探すための変数の定義 ---
		// 敵がいないスポットのインデックスを格納する配列を参照する変数を定義
		int _emptySpots[4];
		// 敵がいないスポットの数を参照する変数を定義
		int _emptyCount = 0;

		// 敵がいないスポットを探すループ
		for (int _i = 0; _i < _encountDirectionNumber; _i++)
		{
			// もしそのスポットに敵がいない場合
			if (_isEnemyAlive[_i] == false)
			{
				// 敵がいないスポットのインデックスを配列に格納
				_emptySpots[_emptyCount] = _i;
				// 敵がいないスポットの数を増やす
				_emptyCount++;
			}
		}

		// もし敵がいないスポットがある場合
		if (_emptyCount > 0)
		{
			// 敵がいないスポットの中からランダムに出現させるスポットのインデックスを取得して参照する変数を定義
			int _spawn_Index = GetRand(_emptyCount - 1);
			// ランダムに選ばれたスポットに敵を出現させる
			_isEnemyAlive[_emptySpots[_spawn_Index]] = true;

			// 敵の唸り声の周波数をランダムに変化させながら再生
			_audioManager.PlayRandoｍSound(_audioManager.RandomFrequencyEncountScream, _audioManager.SE_HandleScreamSound);
		}

		// 敵の出現タイマーをリセット
		_enemySpawnTimer = 0;
	}

	// 敵のアニメーションの再生時間を進める
	_enemyAnimTime += 0.5f;

	// もし敵のアニメーションの再生時間が総時間以上になった場合
	if (_enemyAnimTime >= _enemyAnimTotal_Time)
	{
		// 敵のアニメーションの再生時間をループさせる
		_enemyAnimTime -= _enemyAnimTotal_Time;
	}

	// 敵モデルのアニメーションの再生時間を設定
	MV1SetAttachAnimTime(_enemyModel, _enemyAnimAttach_Index, _enemyAnimTime);
}

/// <summary>
/// エネミーを撃退する関数
/// </summary>
void EnemyManager::Defeat(const PlayerManager& _playerManager)
{
	// 現在カメラが「目標」としている方向のインデックスを計算 (0:正面, 1:右, 2:後ろ, 3:左)
	int _facingIndex = ((int)(_playerManager.TargetCameraAngleY / _playerManager.MoveCameraAngle) % _encountDirectionNumber + _encountDirectionNumber) % _encountDirectionNumber;

	// もしカメラが向いている方向に敵が生存している場合
	if (_isEnemyAlive[_facingIndex] == true)
	{
		// 敵を倒す
		_isEnemyAlive[_facingIndex] = false;

		// 敵の唸り声の周波数をランダムに変化させながら再生
		_audioManager.PlayRandoｍSound(_audioManager.RandomFrequencyDefeatScream, _audioManager.SE_HandleScreamSound);
	}
}

bool EnemyManager::GameOverRule()
{
	// もし正面・右・後ろ・左の全てのスポットに敵が生存している場合
	if (_isEnemyAlive[0] && _isEnemyAlive[((_encountDirectionNumber - 1) - 1) - 1] && _isEnemyAlive[(_encountDirectionNumber - 1) - 1] && _isEnemyAlive[_encountDirectionNumber - 1])
	{
		// 成功を返す
		return true;
	}

	// 失敗を返す
	return false;
}

/// <summary>
/// エネミー関係のUIを描画する関数
/// </summary>
void EnemyManager::Draw() const
{
	// 敵の数分のループ
	for (int _i = 0; _i < _encountDirectionNumber; _i++)
	{
		// もしそのスポットに敵が生存している場合
		if (_isEnemyAlive[_i] == true)
		{
			// --- 敵モデルの位置と回転を設定して描画する ---
			// 敵モデルの位置を設定
			MV1SetPosition(_enemyModel, _enemyPositions[_i]);
			// 敵モデルの回転を設定して常にプレイヤーを向くようにする
			MV1SetRotationXYZ(_enemyModel, VGet(0.0f, _enemyRotations[_i], 0.0f));
			// 敵モデルの描画
			MV1DrawModel(_enemyModel);
		}
	}
}