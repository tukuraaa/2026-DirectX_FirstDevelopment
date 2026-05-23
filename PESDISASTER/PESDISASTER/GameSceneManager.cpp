#include "GameSceneManager.h"
#include "DxLib.h"

/// <summary>
/// ゲームシーンクラスのコンストラクタ
/// </summary>
GameSceneManager::GameSceneManager() : _currentScene(SceneState::Title), _gameTimer(_gameTimerMax)
{
	Init();// コンストラクタで初期化関数を呼び出す
}

/// <summary>
/// ゲームシーンの初期化を行う関数
/// </summary>
void GameSceneManager::Init()
{
	// 廃屋モデルの大きさを設定
	MV1SetScale(_houseModel, VGet(_houseModel_ScaleValue, _houseModel_ScaleValue, _houseModel_ScaleValue));

	// --- スカイボックス設定 ---
	// スカイボックスの大きさを設定
	MV1SetScale(_skyboxModel, VGet(_skyBoxScale, _skyBoxScale, _skyBoxScale));
	// スカイボックスの初期回転を設定
	MV1SetRotationXYZ(_skyboxModel, VGet(0.0f, 0.0f, DX_PI_F * _skyBoxAngleZ_MagnificationValue));

	// コンストラクタの自動Initではなく、ここで本物のプレイヤーを渡して初期化する
	_enemyManager.Init(_playerManager);
}

/// <summary>
/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
/// </summary>
/// <param name="input"></param>
void GameSceneManager::Update(const InputManager& _input)
{
	// シーンごとの更新処理
	switch (_currentScene)
	{
	case SceneState::Title:

		_audioManager.StopBGM(_audioManager.BGM_HandleClear);
		_audioManager.StopBGM(_audioManager.BGM_HandleGameOver);

		_audioManager.PlayBGM(_audioManager.BGM_HandleTitle);

		// タイトル画面でエンターキーが押された場合
		if (_input.IsKeyDown(KEY_INPUT_RETURN))
		{
			_currentScene = SceneState::MainStage;

			// ---ゲーム開始時の初期化処理---
			_playerManager.Init();
			_enemyManager.Init(_playerManager);

			// 制限時間をリセット
			_gameTimer = _gameTimerMax;
		}

		break;

	case SceneState::MainStage:

		_audioManager.StopBGM(_audioManager.BGM_HandleTitle);

		_audioManager.PlayBGM(_audioManager.BGM_HandleMainStage);

		// 他クラスの更新処理
		_playerManager.Update(_input);
		_enemyManager.Update(_playerManager);

		// もしスペースキーが押された場合
		if (_input.IsKeyDown(KEY_INPUT_SPACE))
		{
			// もしプレイヤーが撃てる状態な場合
			if (_playerManager.TryShoot())
			{
				// エネミーを撃退する
				_enemyManager.Defeat(_playerManager);
			}
		}

		// もしゲームタイマーが0より大きい場合
		if (_gameTimer > 0)
		{
			// ゲームタイマーを減らす
			_gameTimer--;
		}
		else
		{
			_currentScene = SceneState::Clear;
		}

		// もし正面・右・後ろ・左の全てのスポットに敵が生存している場合
		if (_enemyManager.GameOverRule())
		{
			_currentScene = SceneState::GameOver;
		}

		break;

	case SceneState::Clear:

		_audioManager.StopBGM(_audioManager.BGM_HandleMainStage);

		_audioManager.PlayBGM(_audioManager.BGM_HandleClear);

		// ゲームタイマーをリセット
		_gameTimer = _gameTimerMax;

		// プレイヤー初期化
		_playerManager.Init();

		// もしエンターキーが新しく押された場合
		if (_input.IsKeyDown(KEY_INPUT_RETURN))
		{
			_currentScene = SceneState::Title;
		}

		break;

	case SceneState::GameOver:

		_audioManager.StopBGM(_audioManager.BGM_HandleMainStage);

		_audioManager.PlayBGM(_audioManager.BGM_HandleGameOver);

		// ゲームタイマーをリセット
		_gameTimer = _gameTimerMax;

		// プレイヤー初期化
		_playerManager.Init();

		// もしエンターキーが押された場合
		if (_input.IsKeyDown(KEY_INPUT_RETURN))
		{
			_currentScene = SceneState::Title;
		}

		break;
	}

	// プレイヤーカメラを適応する
	_playerManager.CameraAdapt();
}

/// <summary>
/// 描画処理を行う関数
/// </summary>
void GameSceneManager::Draw() const
{
	// シーンごとの描画処理
	switch (_currentScene)
	{
	case SceneState::Title:

		// --- タイトルテキストとエンターキーの誘導テキストを描画 ---
		DrawString(200, 200, "--- PESDISASTER ---", GetColor(255, 0, 0));
		DrawString(220, 280, "Press ENTER to Start", GetColor(255, 255, 255));

		break;

	case SceneState::MainStage:

		// スカイボックスをカメラの位置に合わせる
		MV1SetPosition(_skyboxModel, _playerManager.CamPos);

		// --- スカイボックスの描画設定 ---
		// スカイボックスを常に背景に描画するためにZバッファへの書き込みを無効化
		SetWriteZBuffer3D(FALSE);
		// スカイボックスを常に同じ明るさで描画するためにライティングを無効化
		SetUseLighting(FALSE);
		// スカイボックスの描画
		MV1DrawModel(_skyboxModel);
		// ライティングを再度有効化
		SetUseLighting(TRUE);
		// Zバッファへの書き込みを再度有効化
		SetWriteZBuffer3D(TRUE);

		// --- 廃屋の描画 ---
		// 廃屋の位置を設定
		MV1SetPosition(_houseModel, VGet(0.0f, 0.0f, 0.0f));
		// 廃屋の描画
		MV1DrawModel(_houseModel);

		// エネミー情報を描画する関数を呼び出す
		_enemyManager.Draw();

		// ゲームタイマーを描画
		DrawFormatString(10, 30, GetColor(255, 255, 255), "SURVIVE TIME: %d", _gameTimer / 60);

		// プレイヤー情報を描画する関数を呼び出す
		_playerManager.Draw();

		break;

	case SceneState::Clear:

		// --- クリアテキストとエンターキーの誘導テキストを描画 ---
		DrawString(220, 200, "*** CLEAR ***", GetColor(0, 255, 0));
		DrawString(180, 280, "Press ENTER to Title", GetColor(255, 255, 255));

		break;

	case SceneState::GameOver:

		DrawString(220, 200, "*** GAME OVER ***", GetColor(255, 0, 0));
		DrawString(180, 280, "Press ENTER to Title", GetColor(255, 255, 255));

		break;
	}
}