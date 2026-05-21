#pragma once
#include "DxLib.h"
#include "Common.h"
#include "Input.h"

/// <summary>
/// プレイヤーの状態を管理するクラス
/// </summary>
class Player
{
	/// <summary>
	/// ローカルの変数
	/// </summary>
private:

	/// <summary>
	/// リロードミニゲームの状態を参照する変数
	/// </summary>
	ReloadState _reloadState;

	/// <summary>
	/// 残弾数を参照する変数
	/// </summary>
	int _currentAmmo;
	/// <summary>
	/// ターゲットのキーを参照する変数
	/// </summary>
	int _targetKey;
	/// <summary>
	/// リロードミニゲーム開始の音を参照する変数
	/// </summary>
	int _reloadGameStartSoundHandle = LoadSoundMem("Sounds/SE/ReloadGameStartSound.mp3");
	/// <summary>
	/// リロード成功の音を読み込み参照する変数
	/// </summary>
	int _reloadSoundHandle = LoadSoundMem("Sounds/SE/ReloadSound.mp3");
	/// <summary>
	/// 銃の発砲音を読み込み参照する変数
	/// </summary>
	int _shotSoundHandle = LoadSoundMem("Sounds/SE/ShootSound.mp3");
	/// <summary>
	/// キーの候補を参照する配列の変数
	/// </summary>
	int _candidateKeys[5] = { KEY_INPUT_A, KEY_INPUT_B, KEY_INPUT_C, KEY_INPUT_X, KEY_INPUT_Y };
	/// <summary>
	/// 残弾数の最大値を参照する定数の変数
	/// </summary>
	const int _maxAmmo = 9;

	/// <summary>
	/// ターゲットのキーの文字を参照する変数
	/// </summary>
	char _targetKeyChar;
	/// <summary>
	/// ターゲットのキーの文字の候補を参照する配列の変数
	/// </summary>
	char _candidateChars[5] = { 'A', 'B', 'C', 'X', 'Y' };

	/// <summary>
	/// グローバルの関数
	/// </summary>
public:

	/// <summary>
	/// プレイヤークラスのコンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// リロードミニゲームの状態を取得する関数
	/// </summary>
	/// <returns></returns>
	ReloadState GetReloadState() const
	{
		// リロードミニゲームの状態を返す
		return _reloadState;
	}

	/// <summary>
	/// プレイヤーの初期化を行う関数
	/// </summary>
	void Init();
	/// <summary>
	/// 毎フレーム呼び出されるプレイヤーの状態を更新する関数
	/// </summary>
	/// <param name="input"></param>
	void Update(const Input& input);
	/// <summary>
	/// UIを描画する関数
	/// </summary>
	void DrawUI() const;

	/// <summary>
	/// 発砲を試みる関数
	/// </summary>
	/// <returns></returns>
	bool TryShoot();
	/// <summary>
	/// 残弾数をリロードする関数
	/// </summary>
	/// <returns></returns>
	bool IsOutOfAmmo() const
	{
		// 残弾数が0かどうかを返す
		return _currentAmmo == 0;
	}
};