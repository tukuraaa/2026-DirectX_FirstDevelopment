#include "GameSceneManager.h"
#include "ModelUtil.h"
#include <cmath>
#include "DxLib.h"

/// <summary>
/// ゲームシーンクラスのコンストラクタ
/// </summary>
GameSceneManager::GameSceneManager() : _currentScene(SceneState::Title)
{
	Init();
}

/// <summary>
/// ゲームシーンの初期化を行う関数
/// </summary>
void GameSceneManager::Init()
{
	// スイカモデルの設定
	MV1SetScale(_waterMelonModel, VGet(_waterMelonScaleValue, _waterMelonScaleValue, _waterMelonScaleValue));
	LiftModelAmbient(_waterMelonModel, 0.9f);

	SetupSkybox();
	SetupLighting();

	// 本物のプレイヤーを渡してエネミーを初期化する
	_enemyManager.Init(_playerManager);
}

/// <summary>
/// スカイボックスの大きさと向きを設定する関数
/// </summary>
void GameSceneManager::SetupSkybox()
{
	// もしロードに失敗している場合
	if (_skyboxModel == -1)
	{
		return;
	}

	// 全メッシュの裏面カリングを無効化(どの向きの面でも描画されるようにする)
	for (int _i = 0, _n = MV1GetMeshNum(_skyboxModel); _i < _n; _i++)
	{
		MV1SetMeshBackCulling(_skyboxModel, _i, DX_CULLING_NONE);
	}

	// 計測のため、変形をすべて初期状態に戻す
	MV1SetScale(_skyboxModel, VGet(1.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(_skyboxModel, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(_skyboxModel, VGet(0.0f, 0.0f, 0.0f));
	ModelBounds _bounds = GetModelBounds(_skyboxModel);

	// もしモデルの大きさが取得できた場合
	if (_bounds.MaxExtent > 0.0f)
	{
		// 元のモデルの大きさに関係なく、一辺が_skyBoxSizeになる倍率にする
		_skyBoxScale = _skyBoxSize / _bounds.MaxExtent;
		_skyBoxCenter = _bounds.Center;
	}

	// Z軸だけ反転して内側から見た向きにする(以前の「-倍率 + Z軸180°回転」と同じ結果)
	MV1SetScale(_skyboxModel, VGet(_skyBoxScale, _skyBoxScale, -_skyBoxScale));

	// テクスチャをmv1の参照に頼らず、明示的に読み込んで貼る(置き場所は実際のパスに合わせる)
	int _skyTexture = LoadGraph("Models/SkyboxModel/SkyboxFairy.png");

	// もしテクスチャの読み込みに成功した場合
	if (_skyTexture != -1)
	{
		MV1SetTextureGraphHandle(_skyboxModel, 0, _skyTexture, FALSE);
	}

	DrawFormatString(10, 160, _color, "texture graph handle:%d", MV1GetTextureGraphHandle(_skyboxModel, 0));
}

/// <summary>
/// 明るい雰囲気のライティングを設定する関数
/// </summary>
void GameSceneManager::SetupLighting()
{
	// 環境光を底上げして、影の面が暗く潰れないようにする
	SetGlobalAmbientLight(GetColorF(0.50f, 0.52f, 0.58f, 0.0f));

	// メインライト(太陽光): 上からやや斜めに、暖かい白で照らす
	SetLightDirection(VNorm(VGet(0.3f, -1.0f, 0.4f)));
	SetLightDifColor(GetColorF(1.0f, 0.96f, 0.88f, 0.0f));
	SetLightSpcColor(GetColorF(0.3f, 0.3f, 0.3f, 0.0f));
	SetLightAmbColor(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// 補助光(青白い光): プレイヤーから見える面が必ず明るくなるようにする
	if (_fillLightHandle == -1)
	{
		_fillLightHandle = CreateDirLightHandle(VGet(0.0f, 0.0f, 1.0f));
		SetLightDifColorHandle(_fillLightHandle, GetColorF(0.55f, 0.65f, 0.80f, 0.0f));
		SetLightSpcColorHandle(_fillLightHandle, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
		SetLightAmbColorHandle(_fillLightHandle, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

// ===== Update() はそのまま =====

/// <summary>
/// 描画処理を行う関数
/// </summary>
void GameSceneManager::Draw() const
{
	switch (_currentScene)
	{
	case SceneState::Title:     DrawTitle();     break;
	case SceneState::MainStage: DrawMainStage(); break;
	case SceneState::Clear:     DrawResult(true);   break;
	case SceneState::GameOver:  DrawResult(false); break;
	}
}

/// <summary>
/// タイトル画面を描画する関数
/// </summary>
void GameSceneManager::DrawTitle() const
{
	int _screenW, _screenH;
	GetDrawScreenSize(&_screenW, &_screenH);

	int _imgW, _imgH;

	GetGraphSize(_titleSpriteHandle, &_imgW, &_imgH);
	// 画面の横幅いっぱいに収める倍率(縦横比は維持)
	float _scale = (float)_screenW / _imgW;
	DrawRotaGraph(_screenW / 2, _screenH / 2, _scale, 0.0f, _titleSpriteHandle, TRUE);

	DrawFormatString(_titleFormatTransformX_Value, _titleFormatTransformY_Value, GetColor(255, 255, 255), "ENTERでスタート");
}

/// <summary>
/// メインステージを描画する関数
/// </summary>
void GameSceneManager::DrawMainStage() const
{
	// 背景(最初に描く)
	DrawSkybox();

	// 補助光をカメラの向きに合わせる(少し上から照らす)
	VECTOR _front = GetCameraFrontVector();
	SetLightDirectionHandle(_fillLightHandle, VNorm(VGet(_front.x, _front.y - 0.3f, _front.z)));

	// 廃屋の描画
	MV1SetPosition(_waterMelonModel, VGet(0.0f, 0.0f, 0.0f));
	MV1DrawModel(_waterMelonModel);

	_enemyManager.Draw();

	DrawFormatString(_timeFormatTransformX_Value, _timeFormatTransformY_Value, GetColor(255, 255, 255), "防衛残り時間：%d", _gameTimer / 60);

	_playerManager.Draw();

	DrawSkyCylinder();
}

/// <summary>
/// スカイボックスを描画する関数
/// </summary>
void GameSceneManager::DrawSkybox() const
{
	// 切り分けテスト: モデル・テクスチャを使わない単色の球
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
	SetUseLighting(FALSE);
	SetUseBackCulling(DX_CULLING_NONE);
	DrawSphere3D(GetCameraPosition(), 1000.0f, 32, GetColor(255, 0, 255), GetColor(255, 0, 255), TRUE);
	SetUseBackCulling(DX_CULLING_LEFT);
	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

/// <summary>
/// クリア・ゲームオーバー画面の共通テキストを描画する関数
/// </summary>
void GameSceneManager::DrawResult(bool _isClear) const
{
	int _screenW, _screenH;
	GetDrawScreenSize(&_screenW, &_screenH);

	int _imgW, _imgH;

	if(_isClear)
	{
		GetGraphSize(_gameClearSpriteHandle, &_imgW, &_imgH);
		// 画面の横幅いっぱいに収める倍率(縦横比は維持)
		float _scale = (float)_screenW / _imgW;
		DrawRotaGraph(_screenW / 2, _screenH / 2, _scale, 0.0f, _gameClearSpriteHandle, TRUE);
	}
	else
	{
		GetGraphSize(_gameOverSpriteHandle, &_imgW, &_imgH);
		// 画面の横幅いっぱいに収める倍率(縦横比は維持)
		float _scale = (float)_screenW / _imgW;
		DrawRotaGraph(_screenW / 2, _screenH / 2, _scale, 0.0f, _gameOverSpriteHandle, TRUE);
	}

	DrawString(180, 280, "ENTERでタイトルへ", GetColor(255, 255, 255));
}

/// <summary>
/// スカイボックスの状態を画面に表示する確認用の関数
/// </summary>
void GameSceneManager::DrawSkyboxDebug() const
{
	unsigned int _color = GetColor(255, 255, 0);
	VECTOR _cam = GetCameraPosition();

	DrawFormatString(10, 100, _color, "Skybox handle:%d mesh:%d texture:%d", _skyboxModel, MV1GetMeshNum(_skyboxModel), MV1GetTextureNum(_skyboxModel));
	DrawFormatString(10, 120, _color, "scale:%.2f center:(%.1f, %.1f, %.1f)", _skyBoxScale, _skyBoxCenter.x, _skyBoxCenter.y, _skyBoxCenter.z);
	DrawFormatString(10, 140, _color, "camera near:%.1f far:%.1f pos:(%.0f, %.0f, %.0f)", GetCameraNear(), GetCameraFar(), _cam.x, _cam.y, _cam.z);

	DrawFormatString(10, 160, GetColor(255, 255, 0), "sky texture handle:%d", _skyTextureHandle);
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
/// スカイシリンダー(カメラを囲む円筒)を描画する関数
/// </summary>
void GameSceneManager::DrawSkyCylinder() const
{
	// もしテクスチャの読み込みに失敗している場合
	if (_skyTextureHandle == -1)
	{
		return;
	}

	const int _segments = 64;          // 円周の分割数
	const float _radius = 1000.0f;     // 円筒の半径
	const float _halfHeight = 700.0f;  // 円筒の高さの半分
	const float _phase = 0.25f;        // 絵の中央が正面(+Z方向)に来るようにずらす量

	VERTEX3D _vertices[(_segments + 1) * 2];
	unsigned short _indices[_segments * 6];
	VECTOR _center = GetCameraPosition();

	// 頂点を作る(上下2点 × 一周分)
	for (int _i = 0; _i <= _segments; _i++)
	{
		float _t = (float)_i / _segments;
		float _angle = _t * DX_TWO_PI_F;

		// 一周で「絵 → 反転した絵」と並べ、左右の継ぎ目をなくす(三角波)
		float _s = fmodf(_t + _phase, 1.0f);
		float _u = 1.0f - fabsf(1.0f - 2.0f * _s);

		// 上(0)と下(1)の2点を作る
		for (int _k = 0; _k < 2; _k++)
		{
			VERTEX3D& _v = _vertices[_i * 2 + _k];
			_v.pos = VGet(_center.x + sinf(_angle) * _radius,
				_center.y + (_k == 0 ? _halfHeight : -_halfHeight),
				_center.z + cosf(_angle) * _radius);
			_v.norm = VGet(0.0f, 0.0f, -1.0f);
			_v.dif = GetColorU8(255, 255, 255, 255);
			_v.spc = GetColorU8(0, 0, 0, 0);
			_v.u = _u;
			_v.v = (float)_k;
			_v.su = 0.0f;
			_v.sv = 0.0f;
		}
	}

	// 四角形ごとに三角形2枚の番号を作る
	for (int _i = 0; _i < _segments; _i++)
	{
		unsigned short _a = (unsigned short)(_i * 2);
		unsigned short _b = _a + 1;
		unsigned short _c = _a + 2;
		unsigned short _d = _a + 3;

		unsigned short* _p = &_indices[_i * 6];
		_p[0] = _a; _p[1] = _b; _p[2] = _c;
		_p[3] = _c; _p[4] = _b; _p[5] = _d;
	}

	// 常に最背面に描くため、Zバッファ・ライティング・裏面カリングを切る
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
	SetUseLighting(FALSE);
	SetUseBackCulling(DX_CULLING_NONE);

	DrawPolygonIndexed3D(_vertices, (_segments + 1) * 2, _indices, _segments * 2, _skyTextureHandle, FALSE);

	// 加算ブレンドで重ねて、空を明るく持ち上げる
	SetDrawBlendMode(DX_BLENDMODE_ADD, _skyBoxBoostValue);
	DrawPolygonIndexed3D(_vertices, (_segments + 1) * 2, _indices, _segments * 2, _skyTextureHandle, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 設定を元に戻す
	SetUseBackCulling(DX_CULLING_LEFT);
	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}