#pragma once

// --- 状態を管理する列挙型クラス ---
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