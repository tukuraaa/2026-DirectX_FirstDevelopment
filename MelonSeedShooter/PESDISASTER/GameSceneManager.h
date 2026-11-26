#pragma once
#include "StateManager.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "AudioManager.h"

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
	AudioManager& _audioManager = AudioManager::Instance();

	// --- ゲームタイマーデータ ---
		// --- ゲームタイマーデータ ---
	/// <summary>
	/// ゲームの制限時間を参照する定数(先に宣言する)
	/// </summary>
	static constexpr int _gameTimerMax = 3600 * 3;
	/// <summary>
	/// 制限時間を参照する変数
	/// </summary>
	int _gameTimer = _gameTimerMax;
	/// <summary>
	/// タイム表示の位置Xを参照する変数
	/// </summary>
	int _timeFormatTransformX_Value = 10;
	/// <summary>
	/// タイム表示の位置Yを参照する変数
	/// </summary>
	int _timeFormatTransformY_Value = 30;

	// --- 3Dモデルデータ ---
	/// <summary>
	/// スイカモデルを読み込み参照する変数
	/// </summary>
	int _waterMelonModel = MV1LoadModel("Models/StageObjectModel/WaterMelon.mv1");
	/// <summary>
	/// スカイボックスモデルを読み込み参照する変数
	/// </summary>
	int _skyboxModel = MV1LoadModel("Models/SkyboxModel/Skybox.mv1");

	// --- 廃屋データ ---	
	/// <summary>
	/// 廃屋モデルの大きさを参照する変数
	/// </summary>
	float _waterMelonScaleValue = 1.0f;

	// --- スカイボックスデータ ---
/// <summary>
/// スカイボックスの倍率を参照する変数(起動時にモデルの大きさから自動計算する)
/// </summary>
	float _skyBoxScale = 100.0f;
	/// <summary>
	/// スカイボックスの一辺の長さを参照する変数
	/// </summary>
	float _skyBoxSize = 2000.0f;
	/// <summary>
	/// スカイボックスモデルの中心座標(等倍時)を参照する変数
	/// </summary>
	VECTOR _skyBoxCenter = VGet(0.0f, 0.0f, 0.0f);
	/// <summary>
	/// スカイボックスを明るくする加算ブレンドの強さ(0〜255)を参照する変数
	/// </summary>
	int _skyBoxBoostValue = 60;

	// --- スカイシリンダーデータ ---
/// <summary>
/// 空のテクスチャを読み込み参照する変数(パスは実際の置き場所に合わせる)
/// </summary>
	int _skyTextureHandle = LoadGraph("Sprites/SkyboxFairy.png");

	/// <summary>
	/// スカイシリンダーを描画する関数
	/// </summary>
	void DrawSkyCylinder() const;

	// --- ライティングデータ ---
	/// <summary>
	/// カメラの向きに追従する補助光のハンドルを参照する変数
	/// </summary>
	int _fillLightHandle = -1;

	// --- 画像データ ---
	/// <summary>
	/// タイトル画像を読み込み参照する変数
	/// </summary>
	int _titleSpriteHandle = LoadGraph("Sprites/TitleSprite.png");
	/// <summary>
	/// ゲームオーバー画像を読み込み参照する変数
	/// </summary>
	int _gameOverSpriteHandle = LoadGraph("Sprites/GameOverSprite.png");
	/// <summary>
	/// ゲームクリア画像を読み込み参照する変数
	/// </summary>
	int _gameClearSpriteHandle = LoadGraph("Sprites/GameClearSprite.png");

	// --- 初期化補助メソッド ---
	void SetupLighting();
	void SetupSkybox();

	// --- 描画補助メソッド ---
	void DrawTitle() const;
	void DrawMainStage() const;
	void DrawSkybox() const;
	void DrawResult(bool _isClear) const;
	void DrawSkyboxDebug() const;// 確認用(解決したら削除)

	int _color;

	/// <summary>
	/// タイトルシーンにあるテキストのX軸表示位置を参照する変数
	/// </summary>
	int _titleFormatTransformX_Value = 640;
	/// <summary>
	/// タイトルシーンにあるテキストのY軸表示位置を参照する変数
	/// </summary>
	int _titleFormatTransformY_Value = 480;

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