#include "GameScene.h"
#include "DxLib.h"

/// <summary>
/// ゲームシーンクラスのコンストラクタ
/// </summary>
GameScene::GameScene() : _currentScene(SceneState::Title), _gameTimer(_gameTimerMax)
{}

/// <summary>
/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
/// </summary>
/// <param name="input"></param>
void GameScene::Update(const Input& input)
{
	// シーンごとの更新処理
	switch (_currentScene)
	{
	case SceneState::Title:

		// タイトル画面でエンターキーが押された場合
		if (input.IsKeyDown(KEY_INPUT_RETURN))
		{
			_currentScene = SceneState::MainStage;
			// ゲーム開始時の初期化処理
			_player.Init();
			// 制限時間をリセット
			_gameTimer = _gameTimerMax;

			// 敵の数分のループ
			for (int i = 0; i < _directionNumber; i++)
			{
				// 敵を全て非生存状態にリセット
				_isEnemyAlive[i] = false;
			}

			// 敵の出現タイマーをリセット
			_enemySpawnTimer = 0;
		}

		break;

	case SceneState::MainStage:

		// プレイヤーの更新処理
		_player.Update(input);

		// もしスペースキーが押された場合
		if (input.IsKeyDown(KEY_INPUT_SPACE))
		{
			// もしプレイヤーが撃てる状態な場合
			if (_player.TryShoot())
			{
				// 現在カメラが「目標」としている方向のインデックスを計算 (0:正面, 1:右, 2:後ろ, 3:左)
				int _facingIndex = ((int)(_targetCameraAngleY / _moveCameraAngle) % _directionNumber + _directionNumber) % _directionNumber;

				// もしカメラが向いている方向に敵が生存している場合
				if (_isEnemyAlive[_facingIndex] == true)
				{
					// 敵を倒す
					_isEnemyAlive[_facingIndex] = false;

					// 敵の唸り声の周波数をランダムに変化させるための値を参照する変数を定義
					int _randomFrequency = 40000 + (rand() % 18001);
					// 敵の唸り声の周波数を設定
					SetFrequencySoundMem(_randomFrequency, _screamSoundHandle);
					PlaySoundMem(_screamSoundHandle, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			// もしプレイヤーが撃てない状態で、かつリロードミニゲームが待機状態の場合
			else if (_player.IsOutOfAmmo() && _player.GetReloadState() == ReloadState::Wait)
			{
				// 弾切れの音を再生
				PlaySoundMem(_nonMagazineSoundHandle, DX_PLAYTYPE_BACK, TRUE);
			}
		}

		// もし左キーが新しく押された場合
		if (CheckHitKey(KEY_INPUT_LEFT) == _inputNumber && _prevLeftKey == 0)
		{
			// カメラの「目標の角度」を左に回転させる
			_targetCameraAngleY -= _moveCameraAngle;
		}

		// もし右キーが新しく押された場合
		if (CheckHitKey(KEY_INPUT_RIGHT) == _inputNumber && _prevRightKey == 0)
		{
			// カメラの「目標の角度」を右に回転させる
			_targetCameraAngleY += _moveCameraAngle;
		}

		// もしカメラの「目標の角度」と現在の角度が等しくない場合
		if (_cameraAngleY < _targetCameraAngleY)
		{
			// カメラの水平角度を「目標の角度」に近づける
			_cameraAngleY += _moveCameraAngleSpeedValue;

			// もしカメラの水平角度が「目標の角度」を超えてしまった場合
			if (_cameraAngleY > _targetCameraAngleY)
			{
				// カメラの水平角度を「目標の角度」に合わせる
				_cameraAngleY = _targetCameraAngleY;
			}
		}
		// もしカメラの「目標の角度」と現在の角度が等しくない場合
		else if (_cameraAngleY > _targetCameraAngleY)
		{
			// カメラの水平角度を「目標の角度」に近づける
			_cameraAngleY -= _moveCameraAngleSpeedValue;

			// もしカメラの水平角度が「目標の角度」を超えてしまった場合
			if (_cameraAngleY < _targetCameraAngleY)
			{
				// カメラの水平角度を「目標の角度」に合わせる
				_cameraAngleY = _targetCameraAngleY;
			}
		}

		// 敵の出現タイマーを進める
		_enemySpawnTimer++;

		// もし敵の出現タイマーが出現間隔以上になった場合
		if (_enemySpawnTimer >= _enemySpawn_Interval)
		{
			// ---敵がいないスポットを探すための変数の定義---
			// 敵がいないスポットのインデックスを格納する配列を参照する変数を定義
			int _emptySpots[4];
			// 敵がいないスポットの数を参照する変数を定義
			int _emptyCount = 0;

			// 敵がいないスポットを探すループ
			for (int i = 0; i < _directionNumber; i++)
			{
				// もしそのスポットに敵がいない場合
				if (_isEnemyAlive[i] == false)
				{
					// 敵がいないスポットのインデックスを配列に格納
					_emptySpots[_emptyCount] = i;
					// 敵がいないスポットの数を増やす
					_emptyCount++;
				}
			}

			// もし敵がいないスポットがある場合
			if (_emptyCount > 0)
			{
				// 敵がいないスポットの中からランダムに出現させるスポットのインデックスを取得して参照する変数を定義
				int _spawn_Index = GetRand(_emptyCount - _subtractValue);
				// ランダムに選ばれたスポットに敵を出現させる
				_isEnemyAlive[_emptySpots[_spawn_Index]] = true;

				// 敵の唸り声の周波数をランダムに変化させるための値を参照する変数を定義
				int _randomFrequency = 21000 + (rand() % 18001);
				// 敵の唸り声の周波数を設定
				SetFrequencySoundMem(_randomFrequency, _screamSoundHandle);
				PlaySoundMem(_screamSoundHandle, DX_PLAYTYPE_BACK, TRUE);
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

		// 敵の数分のループ
		for (int i = 0; i < _directionNumber; i++)
		{
			// もしそのスポットに敵が生存している場合
			if (_isEnemyAlive[i] == true)
			{
				// ---敵モデルの位置と回転を設定して描画する---
				// 敵モデルの位置を設定
				MV1SetPosition(_enemyModel, _enemyPositions[i]);
				// 敵モデルの回転を設定して常にプレイヤーを向くようにする
				MV1SetRotationXYZ(_enemyModel, VGet(0.0f, _enemyRotations[i], 0.0f));
				// 敵モデルの描画
				MV1DrawModel(_enemyModel);
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
		if (_isEnemyAlive[0] && _isEnemyAlive[((_directionNumber - _subtractValue) - _subtractValue) - _subtractValue] && _isEnemyAlive[(_directionNumber - _subtractValue) - _subtractValue] && _isEnemyAlive[_directionNumber - _subtractValue])
		{
			_currentScene = SceneState::GameOver;
		}

		break;

	case SceneState::Clear:

	case SceneState::GameOver:

		// もしエンターキーが押された場合
		if (input.IsKeyDown(KEY_INPUT_RETURN))
		{
			_currentScene = SceneState::Title;
		}

		break;
	}
}

/// <summary>
/// 描画処理を行う関数
/// </summary>
void GameScene::Draw() const
{
	// シーンごとの描画処理
	switch (_currentScene)
	{
	case SceneState::Title:

		DrawString(200, 200, "--- PESDISASTER ---", GetColor(255, 0, 0));
		DrawString(220, 280, "Press ENTER to Start", GetColor(255, 255, 255));

		break;

	case SceneState::MainStage:

		// スカイボックスをカメラの位置に合わせる
		MV1SetPosition(_skyboxModel, _camPos);

		// ---スカイボックスの描画設定---
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

		// ---廃屋の描画---
		// 廃屋の位置を設定
		MV1SetPosition(_houseModel, VGet(0.0f, 0.0f, 0.0f));
		// 廃屋の描画
		MV1DrawModel(_houseModel);

		// プレイヤーのUIを描画する関数を呼び出す
		_player.DrawUI();
		// ゲームタイマーを描画
		DrawFormatString(10, 30, GetColor(255, 255, 255), "SURVIVE TIME: %d", _gameTimer / 60);

		break;

	case SceneState::Clear:

		DrawString(220, 200, "*** CLEAR ***", GetColor(0, 255, 0));
		DrawString(180, 280, "Press ENTER to Title", GetColor(255, 255, 255));

		break;

	case SceneState::GameOver:

		DrawString(220, 200, "*** GAME OVER ***", GetColor(255, 0, 0));
		DrawString(180, 280, "Press ENTER to Title", GetColor(255, 255, 255));

		break;
	}
}