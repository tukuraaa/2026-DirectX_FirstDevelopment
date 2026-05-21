#include "Player.h"
#include <cstdlib>

/// <summary>
/// プレイヤークラスのコンストラクタ
/// </summary>
Player::Player()
{
	Init();// コンストラクタで初期化関数を呼び出す
}

/// <summary>
/// プレイヤーの初期化を行う関数
/// </summary>
void Player::Init()
{
	// 残弾数を最大値に設定
	_currentAmmo = _maxAmmo;

	_reloadState = ReloadState::Wait;
}
/// <summary>
/// 毎フレーム呼び出されるプレイヤーの状態を更新する関数
/// </summary>
/// <param name="input"></param>
void Player::Update(const Input& input)
{
	// リロードミニゲームの状態に応じた処理
	switch (_reloadState)
	{
	case ReloadState::Wait:

		// もし、弾が満タンでなく、かつRキーが押された場合
		if (_currentAmmo < _maxAmmo && input.IsKeyDown(KEY_INPUT_R))
		{
			// 0から4のランダムな整数を生成して参照する変数を定義
			int r = rand() % 5;
			// ランダムに選ばれたキーをターゲットキーとして設定
			_targetKey = _candidateKeys[r];
			// ランダムに選ばれたキーの文字をターゲットキーの文字として設定
			_targetKeyChar = _candidateChars[r];

			_reloadState = ReloadState::Play;

			// リロードミニゲーム開始の音を再生
			PlaySoundMem(_reloadGameStartSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		}

		break;

	case ReloadState::Play:

		// もし正解キーが押された場合
		if (input.IsKeyDown(_targetKey))
		{
			_reloadState = ReloadState::Finished;
		}

		break;

	case ReloadState::Finished:

		// ミニゲーム完了時に残弾数を最大までリセット
		_currentAmmo = _maxAmmo;

		_reloadState = ReloadState::Wait;

		// リロード成功の音を再生
		PlaySoundMem(_reloadSoundHandle, DX_PLAYTYPE_BACK, TRUE);

		break;
	}
}

/// <summary>
/// プレイヤー関係のUIを描画する関数
/// </summary>
void Player::DrawUI() const
{
	DrawString(10, 10, "Press R to Reload", GetColor(255, 255, 255));
	// 残弾数を描画
	DrawFormatString(10, 50, GetColor(255, 255, 255), "AMMO: %d / %d", _currentAmmo, _maxAmmo);

	// もし弾が尽きていて、かつリロードミニゲームが待機状態の場合
	if (_currentAmmo == 0 && _reloadState == ReloadState::Wait)
	{
		DrawString(10, 70, "OUT OF AMMO! PRESS R!", GetColor(255, 0, 0));
	}

	// もしリロードミニゲームの状態がPlayの場合
	if (_reloadState == ReloadState::Play)
	{
		// ランダムに表示されるボタンの誘導テキストを描画
		DrawFormatString(300, 240, GetColor(255, 255, 0), "Push [ %c ] !", _targetKeyChar);
	}
}

/// <summary>
/// 射撃を試みる関数
/// </summary>
/// <returns></returns>
bool Player::TryShoot()
{
	// もし残弾数が0より大きく、かつリロードミニゲームの状態がWaitの場合
	if (_currentAmmo > 0 && _reloadState == ReloadState::Wait)
	{
		// 残弾数を1減らす
		_currentAmmo--;
		// 発砲が成功したことを返す
		return true;
	}

	// 発砲できない場合はfalseを返す
	return false;
}