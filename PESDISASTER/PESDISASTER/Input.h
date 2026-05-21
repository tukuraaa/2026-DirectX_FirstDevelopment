#pragma once
#include "DxLib.h"

/// <summary>
/// 入力を管理するクラス
/// </summary>
class Input
{
	/// <summary>
	/// ローカル変数
	/// </summary>
private:

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

	/// <summary>
	/// 毎フレーム呼び出される更新関数
	/// </summary>
	void Update()
	{
		// 256個のキー分のループ
		for (int i = 0; i < 256; i++)
		{
			// 前の状態を保存
			_prevKeys[i] = _currentKeys[i];
		}

		// 現在の入力を取得
		GetHitKeyStateAll(_currentKeys);
	}

	/// <summary>
	/// 押されているかを判定する関数
	/// </summary>
	/// <param name="keyCode"></param>
	/// <returns></returns>
	bool IsKeyHold(int keyCode)const
	{
		// キーが押されているかを判定
		return _currentKeys[keyCode] != 0;
	}
	/// <summary>
	/// 押された瞬間かを判定する関数
	/// </summary>
	/// <param name="keyCode"></param>
	/// <returns></returns>
	bool IsKeyDown(int keyCode)const
	{
		// キーが押された瞬間かを判定
		return _currentKeys[keyCode] != 0 && _prevKeys[keyCode] == 0;
	}
};