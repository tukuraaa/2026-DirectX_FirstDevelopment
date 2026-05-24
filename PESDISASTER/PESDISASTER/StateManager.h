#pragma once

// --- 列挙型クラス定義 ---
/// <summary>
/// ゲームシーンを管理する列挙型のクラス
/// </summary>
enum class SceneState
{
	Title,
	MainStage,
	Clear,
	GameOver
};
/// <summary>
/// リロードミニゲームの状態を管理する列挙型のクラス
/// </summary>
enum class ReloadState
{
	Wait,
	Play,
	Finished
};