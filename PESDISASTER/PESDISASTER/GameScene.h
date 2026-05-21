#pragma once
#include "Common.h"
#include "Input.h"
#include "Player.h"

/// <summary>
/// ゲームシーンを管理するクラス
/// </summary>
class GameScene
{
	/// <summary>
	/// ローカルの変数
	/// </summary>
private:

	/// <summary>
	/// 現在のシーンの状態を参照する変数
	/// </summary>
	SceneState _currentScene;

	/// <summary>
	/// プレイヤーの状態を参照する変数
	/// </summary>
	Player _player;

	/// <summary>
	/// // カメラの位置を取得して参照する変数
	/// </summary>
	VECTOR _camPos = GetCameraPosition();
	/// <summary>
	/// 敵の位置を管理する配列（4方向：0:正面, 1:右, 2:後ろ, 3:左）を参照する変数
	/// </summary>
	VECTOR _enemyPositions[4] =
	{
		VGet(0.0f + _cameraPositionX_Value, _enemyDistanceY, _enemyDistanceZ + _cameraPositionZ_Value),// 0:正面
		VGet(_enemyDistanceX + _cameraPositionX_Value, _enemyDistanceY, 0.0f + _cameraPositionZ_Value),// 1:右
		VGet(0.0f + _cameraPositionX_Value,  _enemyDistanceY, -_enemyDistanceZ + _cameraPositionZ_Value),// 2:後ろ
		VGet(-_enemyDistanceX + _cameraPositionX_Value, _enemyDistanceY, 0.0f + _cameraPositionZ_Value)// 3:左
	};

	/// <summary>
	/// ゲームの経過時間を参照する変数
	/// </summary>
	int _gameTimer;
	/// <summary>
	/// 敵の出現タイマーを参照する変数
	/// </summary>
	int _enemySpawnTimer = 0;
	/// <summary>
	/// 敵出現の唸り声を読み込み参照する変数
	/// </summary>
	int _screamSoundHandle = LoadSoundMem("Sounds/SE/MonsterScream.mp3");
	/// <summary>
	/// 弾切れの音を読み込み参照する変数
	/// </summary>
	int _nonMagazineSoundHandle = LoadSoundMem("Sounds/SE/NonMagazineSound.mp3");
	/// <summary>
/// ボタン入力の成功数を参照する変数
/// </summary>
	int _inputNumber = 1;
	/// <summary>
/// 前のフレームの左キーの状態を参照する変数
/// </summary>
	int _prevLeftKey = 0;
	/// <summary>
/// 前のフレームの右キーの状態を参照する変数
/// </summary>
	int _prevRightKey = 0;
	/// <summary>
	/// 敵の出現間隔を参照する変数
	/// </summary>
	int _enemySpawn_Interval = 300;
	/// <summary>
	/// 方向の数を参照する変数
	/// </summary>
	int _directionNumber = 4;
	/// <summary>
/// 引く数を参照する変数
/// </summary>
	int _subtractValue = 1;
	/// <summary>
	/// 敵モデルを読み込み参照する変数
	/// </summary>
	int _enemyModel = MV1LoadModel("Models/EnemyModel/Enemy.mv1");
	/// <summary>
/// 敵のアニメーションの紐付け関係のインデックスを参照する変数
/// </summary>
	int _enemyAnimAttachAbout_Index = -1;
	/// <summary>
	/// 敵モデルのアニメーションを紐づけ参照する変数
	/// </summary>
	int _enemyAnimAttach_Index = MV1AttachAnim(_enemyModel, 0, _enemyAnimAttachAbout_Index, FALSE);
	/// <summary>
/// 割る数を参照する変数
/// </summary>
	int _divisorValue = 2;
	/// <summary>
	/// 廃屋モデルを読み込み参照する変数
	/// </summary>
	int _houseModel = MV1LoadModel("Models/HouseModel/House.mv1");
	/// <summary>
	/// スカイボックスモデルを読み込み参照する変数
	/// </summary>
	int _skyboxModel = MV1LoadModel("Models/SkyboxModel/Skybox.mv1");
	/// <summary>
	/// ゲームの制限時間を参照する定数の変数
	/// </summary>
	const int _gameTimerMax = 3600 * 3;

	/// <summary>
	/// 4方向（0:正面, 1:右, 2:後ろ, 3:左）を管理する敵の生存状態の配列を参照する変数
	/// </summary>
	bool _isEnemyAlive[4] = { false, false, false, false };

	/// <summary>
/// カメラの「目標の角度」を参照する変数
/// </summary>
	float _targetCameraAngleY = 0.0f;
	/// <summary>
/// カメラ角度の動かす量をを参照する変数
/// </summary>
	float _moveCameraAngle = 90.0f;
	/// <summary>
/// カメラの水平角度を参照する変数
/// </summary>
	float _cameraAngleY = 0.0f;
	/// <summary>
/// カメラ水平角度の回転速度を参照する変数
/// </summary>
	float _moveCameraAngleSpeedValue = 5.0f;
	/// <summary>
	/// 敵モデルのアニメーションの再生時間を管理する変数
	/// </summary>
	float _enemyAnimTime = 0.0f;
	/// <summary>
	/// 敵モデルのアニメーションの総時間を参照する変数
	/// </summary>
	float _enemyAnimTotal_Time = MV1GetAttachAnimTotalTime(_enemyModel, _enemyAnimAttach_Index);
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
	/// 敵とプレイヤーのX距離を参照する変数
	/// </summary>
	float _enemyDistanceX = 100.0f;
	/// <summary>
	/// 敵とプレイヤーのY距離を参照する変数
	/// </summary>
	float _enemyDistanceY = 140.0f;
	/// <summary>
	/// 敵とプレイヤーのZ距離を参照する変数
	/// </summary>
	float _enemyDistanceZ = 100.0f;
	/// <summary>
/// 敵の回転角度の最大値を参照する変数
/// </summary>
	float _enemyRotationRagianValueMax = 180.0f;
	/// <summary>
	/// 敵が常にプレイヤーを向くためのY軸回転角度（4方向）の配列を参照する変数
	/// </summary>
	float _enemyRotations[4] =
	{
		0.0f,// 0:正面
		(_enemyRotationRagianValueMax / _divisorValue) * DX_PI_F / _enemyRotationRagianValueMax,// 1:右
		_enemyRotationRagianValueMax * DX_PI_F / _enemyRotationRagianValueMax,// 2:後ろ
		(-_enemyRotationRagianValueMax / _divisorValue) * DX_PI_F / _enemyRotationRagianValueMax// 3:左
	};

	/// <summary>
	/// グローバルの関数
	/// </summary>
public:

	/// <summary>
	/// ゲームシーンクラスのコンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
	/// </summary>
	/// <param name="input"></param>
	void Update(const Input& input);
	/// <summary>
	/// 描画を行う関数
	/// </summary>
	void Draw() const;
};