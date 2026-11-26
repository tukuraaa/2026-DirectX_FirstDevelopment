#pragma once
#include "DxLib.h"

/// <summary>
/// モデルの全マテリアルの環境光への反応を設定する関数
/// </summary>
/// <param name="_model">モデルのハンドル</param>
/// <param name="_value">環境光への反応の強さ(0.0〜1.0)</param>
inline void LiftModelAmbient(int _model, float _value)
{
	// マテリアルの数分のループ
	for (int _i = 0, _n = MV1GetMaterialNum(_model); _i < _n; _i++)
	{
		MV1SetMaterialAmbColor(_model, _i, GetColorF(_value, _value, _value, 1.0f));
	}
}