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
	/// 音響の状態を参照する変数(全クラスで共有する唯一のインスタンス)
	/// </summary>
	AudioManager& _audioManager = AudioManager::Instance();

	// --- ゲームタイマーデータ ---
	/// <summary>
	/// ゲームの制限時間(フレーム数)を参照する定数
	/// ※ _gameTimer より先に宣言すること(初期化の順番が宣言順で決まるため)
	/// </summary>
	static constexpr int _gameTimerMax = 3600 * 3;
	/// <summary>
	/// 残りの制限時間(フレーム数)を参照する変数
	/// </summary>
	int _gameTimer = _gameTimerMax;
	/// <summary>
	/// タイム表示の位置Xを参照する変数
	/// </summary>
	int _timeFormatTransformX_Value = 10;
	/// <summary>
	/// タイム表示の位置Yを参照する変数
	/// </summary>
	int _timeFormatTransformY_Value = 40;

	// --- スカイシリンダーデータ ---
	/// <summary>
	/// 空のテクスチャのハンドルを参照する変数(読み込み失敗時は-1)
	/// </summary>
	int _skyTextureHandle = -1;
	/// <summary>
	/// 円筒の円周の分割数を参照する定数
	/// </summary>
	static constexpr int _skySegments = 64;
	/// <summary>
	/// 円筒の半径を参照する定数
	/// </summary>
	static constexpr float _skyRadius = 1000.0f;
	/// <summary>
	/// 円筒の高さの半分を参照する定数(大きいほど絵が縦に伸びる)
	/// </summary>
	static constexpr float _skyHalfHeight = 700.0f;
	/// <summary>
	/// 絵を円周方向にずらす量(0.0〜1.0)を参照する定数
	/// ※ 1.0 / _skySegments の倍数にすると、絵の中心と継ぎ目が頂点に揃って綺麗に見える
	/// </summary>
	static constexpr float _skyPhase = 0.25f;
	/// <summary>
	/// 空を明るく持ち上げる加算ブレンドの強さ(0〜255)を参照する定数
	/// </summary>
	static constexpr int _skyBoostValue = 60;

	// --- ライティングデータ ---
	/// <summary>
	/// カメラの向きに追従する補助光のハンドルを参照する変数
	/// </summary>
	int _fillLightHandle = -1;
	/// <summary>
	/// 補助光を斜め下に傾ける量を参照する定数
	/// </summary>
	static constexpr float _fillLightTiltValue = 0.3f;

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
	/// <summary>
	/// 空のテクスチャを読み込む関数
	/// </summary>
	void LoadSkyTexture();
	/// <summary>
	/// 明るい雰囲気のライティングを設定する関数
	/// </summary>
	void SetupLighting();

	// --- 更新補助メソッド ---
	/// <summary>
	/// タイトルシーンの更新を行う関数
	/// </summary>
	void UpdateTitle(const InputManager& _input);
	/// <summary>
	/// メインステージシーンの更新を行う関数
	/// </summary>
	void UpdateMainStage(const InputManager& _input);
	/// <summary>
	/// クリア・ゲームオーバーシーン共通の更新を行う関数
	/// </summary>
	/// <param name="_bgmHandle">流すBGMのハンドル</param>
	void UpdateResult(const InputManager& _input, int _bgmHandle);

	// --- 描画補助メソッド ---
	/// <summary>
	/// タイトルシーンを描画する関数
	/// </summary>
	void DrawTitle() const;
	/// <summary>
	/// メインステージシーンを描画する関数
	/// </summary>
	void DrawMainStage() const;
	/// <summary>
	/// スカイシリンダー(カメラを囲む円筒の空)を描画する関数
	/// </summary>
	void DrawSkyCylinder() const;
	/// <summary>
	/// クリア・ゲームオーバーシーン共通のテキストを描画する関数
	/// </summary>
	/// <param name="_isClear">クリアかどうか</param>
	void DrawResult(bool _isClear) const;

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
	/// <param name="_input">入力管理クラス</param>
	void Update(const InputManager& _input);

	// --- 描画管理メソッド ---
	/// <summary>
	/// 描画を行う関数
	/// </summary>
	void Draw() const;
};