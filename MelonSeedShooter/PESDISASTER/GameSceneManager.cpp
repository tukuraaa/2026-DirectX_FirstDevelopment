#include "GameSceneManager.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	/// <summary>
	/// 空のテクスチャを探すパスの候補(実行時のカレントディレクトリからの相対パス)
	/// </summary>
	const char* const SkyTextureCandidates[] =
	{
		"Models/SkyboxModel/SkyboxFairy.png",
		"Sprites/SkyboxFairy.png",
		"Sprites/Skybox/SkyboxFairy.png",
		"SkyboxFairy.png"
	};
}

// ============================================================
// 初期化
// ============================================================

/// <summary>
/// ゲームシーンクラスのコンストラクタ
/// </summary>
GameSceneManager::GameSceneManager() : _currentScene(SceneState::Title)
{
	Init();// コンストラクタで初期化関数を呼び出す
}

/// <summary>
/// ゲームシーンの初期化を行う関数
/// </summary>
void GameSceneManager::Init()
{
	// 空のテクスチャとライティングを設定
	LoadSkyTexture();
	SetupLighting();

	// プレイヤーを渡してエネミーを初期化する
	_enemyManager.Init(_playerManager);
}

/// <summary>
/// 空のテクスチャを読み込む関数
/// </summary>
void GameSceneManager::LoadSkyTexture()
{
	// もし既に読み込み済みの場合
	if (_skyTextureHandle != -1)
	{
		return;
	}

	// 候補のパスを順に試し、最初に読み込めたものを使う
	for (const char* _path : SkyTextureCandidates)
	{
		_skyTextureHandle = LoadGraph(_path);

		// もし読み込みに成功した場合
		if (_skyTextureHandle != -1)
		{
			break;
		}
	}
}

/// <summary>
/// 明るい雰囲気のライティングを設定する関数
/// </summary>
void GameSceneManager::SetupLighting()
{
	// 環境光を底上げして、影の面が暗く潰れないようにする
	SetGlobalAmbientLight(GetColorF(0.50f, 0.52f, 0.58f, 0.0f));

	// --- メインライト(太陽光): 上からやや斜めに、暖かい白で照らす ---
	SetLightDirection(VNorm(VGet(0.3f, -1.0f, 0.4f)));
	SetLightDifColor(GetColorF(1.0f, 0.96f, 0.88f, 0.0f));
	SetLightSpcColor(GetColorF(0.3f, 0.3f, 0.3f, 0.0f));
	SetLightAmbColor(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// もし補助光がまだ作られていない場合
	if (_fillLightHandle == -1)
	{
		// --- 補助光(青白い光): プレイヤーから見える面が必ず明るくなるようにする ---
		// 向きは毎フレームの描画時にカメラへ合わせる
		_fillLightHandle = CreateDirLightHandle(VGet(0.0f, 0.0f, 1.0f));
		SetLightDifColorHandle(_fillLightHandle, GetColorF(0.55f, 0.65f, 0.80f, 0.0f));
		SetLightSpcColorHandle(_fillLightHandle, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
		SetLightAmbColorHandle(_fillLightHandle, GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

// ============================================================
// 更新
// ============================================================

/// <summary>
/// 毎フレーム呼び出されるゲームシーンの状態を更新する関数
/// </summary>
/// <param name="_input">入力管理クラス</param>
void GameSceneManager::Update(const InputManager& _input)
{
	// シーンごとの更新処理
	switch (_currentScene)
	{
	case SceneState::Title:
		UpdateTitle(_input);
		break;

	case SceneState::MainStage:
		UpdateMainStage(_input);
		break;

	case SceneState::Clear:
		UpdateResult(_input, _audioManager.BGM_HandleClear);
		break;

	case SceneState::GameOver:
		UpdateResult(_input, _audioManager.BGM_HandleGameOver);
		break;
	}

	// プレイヤーカメラを適応する
	_playerManager.CameraAdapt();
}

/// <summary>
/// タイトルシーンの更新を行う関数
/// </summary>
void GameSceneManager::UpdateTitle(const InputManager& _input)
{
	// 他のシーンのBGMを止めて、タイトルのBGMを流す
	_audioManager.StopBGM(_audioManager.BGM_HandleClear);
	_audioManager.StopBGM(_audioManager.BGM_HandleGameOver);
	_audioManager.PlayBGM(_audioManager.BGM_HandleTitle);

	// もしエンターキーが押された場合
	if (_input.IsKeyDown(KEY_INPUT_RETURN))
	{
		_currentScene = SceneState::MainStage;

		// --- ゲーム開始時の初期化処理 ---
		_playerManager.Init();
		_enemyManager.Init(_playerManager);

		// 制限時間をリセット
		_gameTimer = _gameTimerMax;
	}
}

/// <summary>
/// メインステージシーンの更新を行う関数
/// </summary>
void GameSceneManager::UpdateMainStage(const InputManager& _input)
{
	// タイトルのBGMを止めて、メインステージのBGMを流す
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
		// 制限時間まで耐えたのでクリア
		_currentScene = SceneState::Clear;
	}

	// もし正面・右・後ろ・左の全てのスポットに敵が生存している場合
	if (_enemyManager.GameOverRule())
	{
		_currentScene = SceneState::GameOver;
	}
}

/// <summary>
/// クリア・ゲームオーバーシーン共通の更新を行う関数
/// </summary>
/// <param name="_bgmHandle">流すBGMのハンドル</param>
void GameSceneManager::UpdateResult(const InputManager& _input, int _bgmHandle)
{
	// メインステージのBGMを止めて、結果画面のBGMを流す
	_audioManager.StopBGM(_audioManager.BGM_HandleMainStage);
	_audioManager.PlayBGM(_bgmHandle);

	// 次のゲームに備えて、ゲームタイマーとプレイヤーを初期化する
	_gameTimer = _gameTimerMax;
	_playerManager.Init();

	// もしエンターキーが押された場合
	if (_input.IsKeyDown(KEY_INPUT_RETURN))
	{
		_currentScene = SceneState::Title;
	}
}

// ============================================================
// 描画
// ============================================================

/// <summary>
/// 描画処理を行う関数
/// </summary>
void GameSceneManager::Draw() const
{
	// シーンごとの描画処理
	switch (_currentScene)
	{
	case SceneState::Title:
		DrawTitle();
		break;

	case SceneState::MainStage:
		DrawMainStage();
		break;

	case SceneState::Clear:
		DrawResult(true);
		break;

	case SceneState::GameOver:
		DrawResult(false);
		break;
	}
}

/// <summary>
/// タイトルシーンを描画する関数
/// </summary>
void GameSceneManager::DrawTitle() const
{
	// 画面サイズと画像サイズを取得
	int _screenW, _screenH;
	GetDrawScreenSize(&_screenW, &_screenH);

	int _imgW, _imgH;
	GetGraphSize(_titleSpriteHandle, &_imgW, &_imgH);

	// 画面の横幅いっぱいに収める倍率(縦横比は維持)
	float _scale = (float)_screenW / _imgW;

	// 画面中央にタイトルを描画
	DrawRotaGraph(_screenW / 2, _screenH / 2, _scale, 0.0f, _titleSpriteHandle, TRUE);

	// 見出しとエンターキーの誘導テキストを描画
	DrawString(570, 612, "ENTERでタイトルへ", GetColor(255, 255, 255));
}

/// <summary>
/// メインステージシーンを描画する関数
/// </summary>
void GameSceneManager::DrawMainStage() const
{
	// --- 3D描画(奥から順に描く) ---
	// 背景の空
	DrawSkyCylinder();

	// 補助光をカメラの向きに合わせる(少し上から照らす)
	VECTOR _front = GetCameraFrontVector();
	SetLightDirectionHandle(_fillLightHandle, VNorm(VGet(_front.x, _front.y - _fillLightTiltValue, _front.z)));

	// エネミー
	_enemyManager.Draw();

	// --- UI描画(3Dより手前に描く) ---
	// ゲームタイマー
	DrawFormatString(_timeFormatTransformX_Value, _timeFormatTransformY_Value, GetColor(0, 0, 0), "防衛残り時間：%d", _gameTimer / 60);
	// ゲームルール
	DrawFormatString(10, 10, GetColor(0, 0, 0), "ゲームルール：スズメに囲まれるな！追い返して耐え続けろ！");

	// プレイヤー情報
	_playerManager.Draw();

	// もし空のテクスチャが読み込めていない場合(原因特定用。解決したら削除してよい)
	if (_skyTextureHandle == -1)
	{
		DrawString(10, 100, "空のテクスチャ(SkyboxFairy.png)を読み込めません", GetColor(255, 80, 80));
	}
}

/// <summary>
/// スカイシリンダー(カメラを囲む円筒の空)を描画する関数
/// </summary>
void GameSceneManager::DrawSkyCylinder() const
{
	// もしテクスチャの読み込みに失敗している場合
	if (_skyTextureHandle == -1)
	{
		return;
	}

	// 頂点とポリゴンの番号を入れる配列(上下2点 × (分割数+1) 列)
	constexpr int _vertexCount = (_skySegments + 1) * 2;
	constexpr int _polygonCount = _skySegments * 2;
	VERTEX3D _vertices[_vertexCount];
	unsigned short _indices[_polygonCount * 3];

	// 円筒の中心は常にカメラの位置にする(どれだけ動いても空が近づかない)
	VECTOR _center = GetCameraPosition();

	// --- 頂点を作る ---
	for (int _i = 0; _i <= _skySegments; _i++)
	{
		// 円周上の位置(0.0〜1.0)と、その角度
		float _t = (float)_i / _skySegments;
		float _angle = _t * DX_TWO_PI_F;

		// 円周を一周する間に「絵 → 左右反転した絵」と並べて、継ぎ目をなくす(三角波)
		// _t = 0 の位置(カメラ正面 +Z方向)に絵の中心が来るようにずらす
		float _s = fmodf(_t + _skyPhase, 1.0f);
		float _u = 1.0f - fabsf(1.0f - 2.0f * _s);

		// 上(0)と下(1)の2点を作る
		for (int _k = 0; _k < 2; _k++)
		{
			VERTEX3D& _vertex = _vertices[_i * 2 + _k];

			_vertex.pos = VGet(_center.x + sinf(_angle) * _skyRadius,
				_center.y + (_k == 0 ? _skyHalfHeight : -_skyHalfHeight),
				_center.z + cosf(_angle) * _skyRadius);
			_vertex.norm = VGet(0.0f, 0.0f, -1.0f);
			_vertex.dif = GetColorU8(255, 255, 255, 255);
			_vertex.spc = GetColorU8(0, 0, 0, 0);
			_vertex.u = _u;
			_vertex.v = (float)_k;
			_vertex.su = 0.0f;
			_vertex.sv = 0.0f;
		}
	}

	// --- 四角形ごとに三角形2枚分の頂点番号を作る ---
	for (int _i = 0; _i < _skySegments; _i++)
	{
		unsigned short _topLeft = (unsigned short)(_i * 2);
		unsigned short _bottomLeft = _topLeft + 1;
		unsigned short _topRight = _topLeft + 2;
		unsigned short _bottomRight = _topLeft + 3;

		unsigned short* _polygon = &_indices[_i * 6];
		_polygon[0] = _topLeft;    _polygon[1] = _bottomLeft;  _polygon[2] = _topRight;
		_polygon[3] = _topRight;   _polygon[4] = _bottomLeft;  _polygon[5] = _bottomRight;
	}

	// --- 描画設定 ---
	// 常に最背面に描くため、Zバッファ・ライティング・裏面カリングを切る
	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
	SetUseLighting(FALSE);
	SetUseBackCulling(DX_CULLING_NONE);

	// 空を描画
	DrawPolygonIndexed3D(_vertices, _vertexCount, _indices, _polygonCount, _skyTextureHandle, FALSE);

	// 加算ブレンドでもう一度重ねて、空を明るく持ち上げる
	SetDrawBlendMode(DX_BLENDMODE_ADD, _skyBoostValue);
	DrawPolygonIndexed3D(_vertices, _vertexCount, _indices, _polygonCount, _skyTextureHandle, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// --- 描画設定を元に戻す ---
	SetUseBackCulling(DX_CULLING_LEFT);
	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

/// <summary>
/// クリア・ゲームオーバーシーン共通のテキストを描画する関数
/// </summary>
/// <param name="_isClear">クリアかどうか</param>
void GameSceneManager::DrawResult(bool _isClear) const
{
	// 画面サイズと画像サイズを取得
	int _screenW, _screenH;
	GetDrawScreenSize(&_screenW, &_screenH);

	if (_isClear)
	{
		int _imgW, _imgH;
		GetGraphSize(_gameClearSpriteHandle, &_imgW, &_imgH);

		// 画面の横幅いっぱいに収める倍率(縦横比は維持)
		float _scale = (float)_screenW / _imgW;

		// 画面中央にタイトルを描画
		DrawRotaGraph(_screenW / 2, _screenH / 2, _scale, 0.0f, _gameClearSpriteHandle, TRUE);
	}
	else
	{
		int _imgW, _imgH;
		GetGraphSize(_gameOverSpriteHandle, &_imgW, &_imgH);

		// 画面の横幅いっぱいに収める倍率(縦横比は維持)
		float _scale = (float)_screenW / _imgW;

		// 画面中央にタイトルを描画
		DrawRotaGraph(_screenW / 2, _screenH / 2, _scale, 0.0f, _gameOverSpriteHandle, TRUE);
	}

	// 見出しとエンターキーの誘導テキストを描画
	DrawString(570, 622, "ENTERでタイトルへ", GetColor(255, 255, 255));
}