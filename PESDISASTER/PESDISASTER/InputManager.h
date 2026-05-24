#pragma once
#include "DxLib.h"

// --- クラス定義 ---
/// <summary>
/// 入力を管理するクラス
/// </summary>
class InputManager
{
	/// <summary>
	/// ローカル変数
	/// </summary>
private:

	// --- キー入力データ ---
	/// <summary>
	/// 入力の状態を保存する配列を参照する変数
	/// </summary>
	char _currentKeys[256];
	/// <summary>
	/// 入力の前の状態を保存する配列を参照する変数
	/// </summary>
	char _prevKeys[256];

	/// <summary>
	/// グローバル関数
	/// </summary>
	/// 
public:

	// --- 毎フレーム管理メソッド ---
	/// <summary>
	/// 毎フレーム呼び出される更新関数
	/// </summary>
	void Update()
	{
		// 256個のキー分のループ
		for (int _i = 0; _i < 256; _i++)
		{
			// 前の状態を保存
			_prevKeys[_i] = _currentKeys[_i];
		}

		// 現在の入力を取得
		GetHitKeyStateAll(_currentKeys);
	}

	// --- 入力判定管理メソッド ---
	/// <summary>
	/// 押されているかを判定する関数
	/// </summary>
	/// <param name="keyCode"></param>
	/// <returns></returns>
	bool IsKeyHold(int _keyCode)const
	{
		// キーが押されているかを判定
		return _currentKeys[_keyCode] != 0;
	}
	/// <summary>
	/// 押された瞬間かを判定する関数
	/// </summary>
	/// <param name="keyCode"></param>
	/// <returns></returns>
	bool IsKeyDown(int _keyCode)const
	{
		// キーが押された瞬間かを判定
		return _currentKeys[_keyCode] != 0 && _prevKeys[_keyCode] == 0;
	}
};