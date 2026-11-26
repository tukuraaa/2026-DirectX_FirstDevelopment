#pragma once
#include "DxLib.h"
#include <cmath>

/// <summary>
/// モデルの全マテリアルの環境光への反応を設定する関数
/// </summary>
inline void LiftModelAmbient(int _model, float _value)
{
	// マテリアルの数分のループ
	for (int _i = 0, _n = MV1GetMaterialNum(_model); _i < _n; _i++)
	{
		MV1SetMaterialAmbColor(_model, _i, GetColorF(_value, _value, _value, 1.0f));
	}
}

/// <summary>
/// モデルの大きさと中心を保持する構造体
/// </summary>
struct ModelBounds
{
	VECTOR Center;   // 中心座標
	float MaxExtent; // 一番長い辺の長さ(取得失敗時は0)
};

/// <summary>
/// モデルの大きさと中心を取得する関数
/// </summary>
inline ModelBounds GetModelBounds(int _model)
{
	VECTOR _min = VGet(1e9f, 1e9f, 1e9f);
	VECTOR _max = VGet(-1e9f, -1e9f, -1e9f);

	// メッシュの数分のループ
	for (int _i = 0, _n = MV1GetMeshNum(_model); _i < _n; _i++)
	{
		VECTOR _meshMin = MV1GetMeshMinPosition(_model, _i);
		VECTOR _meshMax = MV1GetMeshMaxPosition(_model, _i);
		_min = VGet(fminf(_min.x, _meshMin.x), fminf(_min.y, _meshMin.y), fminf(_min.z, _meshMin.z));
		_max = VGet(fmaxf(_max.x, _meshMax.x), fmaxf(_max.y, _meshMax.y), fmaxf(_max.z, _meshMax.z));
	}

	ModelBounds _result = { VGet(0.0f, 0.0f, 0.0f), 0.0f };

	// メッシュが取得できなかった場合
	if (_max.x < _min.x)
	{
		return _result;
	}

	_result.Center = VScale(VAdd(_min, _max), 0.5f);
	_result.MaxExtent = fmaxf(fmaxf(_max.x - _min.x, _max.y - _min.y), _max.z - _min.z);
	return _result;
}