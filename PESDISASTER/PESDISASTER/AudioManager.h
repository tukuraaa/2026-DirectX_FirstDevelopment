#pragma once
#include "DxLib.h"

/// <summary>
/// 音響を管理するクラス
/// </summary>
class AudioManager
{
	/// <summary>
	/// ローカル参照
	/// </summary>
private:

	/// <summary>
	/// グローバル参照
	/// </summary>
public:

	// --- BGMデータ ---
	/// <summary>
	/// クリア画面のBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleClear = LoadSoundMem("Sounds/BGM/ClearSound.mp3");
	/// <summary>
	/// ゲームオーバー画面のBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleGameOver = LoadSoundMem("Sounds/BGM/GameOverSound.mp3");
	/// <summary>
	/// タイトル画面のBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleTitle = LoadSoundMem("Sounds/BGM/TitleSound.mp3");
	/// <summary>
	/// メインステージのBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleMainStage = LoadSoundMem("Sounds/BGM/MainStageSound.mp3");

	// --- SEデータ ---
	/// <summary>
	/// 敵出現の唸り声を読み込み参照する変数
	/// </summary>
	int SE_HandleScreamSound = LoadSoundMem("Sounds/SE/MonsterScream.mp3");
	/// <summary>
	/// リロードミニゲーム開始の音を参照する変数
	/// </summary>
	int SE_HandleReloadGameStartSound = LoadSoundMem("Sounds/SE/ReloadGameStartSound.mp3");
	/// <summary>
	/// リロード成功の音を読み込み参照する変数
	/// </summary>
	int SE_HandleReloadSound = LoadSoundMem("Sounds/SE/ReloadSound.mp3");
	/// <summary>
	/// 銃の発砲音を読み込み参照する変数
	/// </summary>
	int SE_HandleShotSound = LoadSoundMem("Sounds/SE/ShootSound.mp3");
	/// <summary>
	/// 弾切れの音を読み込み参照する変数
	/// </summary>
	int SE_HandleNonMagazineSound = LoadSoundMem("Sounds/SE/NonMagazineSound.mp3");

	// --- 音量データ ---
	/// <summary>
	/// 敵の唸り声のボリュームを参照する変数
	/// </summary>
	int ScreamVolume = 600;

	// --- 周波数データ ---
	/// <summary>
	/// 敵の出現時の唸り声の周波数をランダムに変化させるための値を参照する変数
	/// </summary>
	int RandomFrequencyEncountScream = 40000 + (rand() % 18001);
	/// <summary>
	/// 敵の撃退時の唸り声の周波数をランダムに変化させるための値を参照する変数
	/// </summary>
	int RandomFrequencyDefeatScream = 21000 + (rand() % 18001);

	// --- データ再生関係メソッド ---
	/// <summary>
	/// BGMを再生する関数
	/// </summary>
	/// <param name="_trackNumber"></param>
	void PlayBGM(int _trackNumber)
	{
		// もし指定の音声が再生されていない場合
		if (CheckSoundMem(_trackNumber) == 0)
		{
			// 音を再生
			PlaySoundMem(_trackNumber, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
	/// <summary>
	/// SEを再生する関数
	/// </summary>
	/// <param name="_trackNumber"></param>
	void PlaySE(int _trackNumber)
	{
		// 音を再生
		PlaySoundMem(_trackNumber, DX_PLAYTYPE_BACK, TRUE);
	}
	/// <summary>
	/// ランダムな周波数の音声を再生する関数
	/// </summary>
	void PlayRandoｍSound(int _frequency, int _trackNumber)
	{
		// --- ランダムに周波数を変化させながら再生する ---
		int RandomFrequency = _frequency;
		ChangeFrequency(RandomFrequency, _trackNumber);
		PlaySE(_trackNumber);
	}

	// --- データ停止メソッド ---
	/// <summary>
	/// BGMを停止する関数
	/// </summary>
	/// <param name="_trackNumber"></param>
	void StopBGM(int _trackNumber)
	{
		// 音を停止
		StopSoundMem(_trackNumber);
	}

	// --- データ内調整関係メソッド ---
	/// <summary>
	/// 音量を調整する関数
	/// </summary>
	/// <param name="_volume"></param>
	/// <param name="_trackNumber"></param>
	void ChangeVolume(int _volume, int _trackNumber)
	{
		// 音量を調整
		ChangeVolumeSoundMem(_volume, _trackNumber);
	}
	/// <summary>
	/// 周波数を調整する関数
	/// </summary>
	/// <param name="_frequency"></param>
	/// <param name="_trackNumber"></param>
	void ChangeFrequency(int _frequency, int _trackNumber)
	{
		// 周波数を調整
		SetFrequencySoundMem(_frequency, _trackNumber);
	}
};