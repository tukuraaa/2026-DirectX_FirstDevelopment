#pragma once
#include "DxLib.h"
#include <cstdlib>

/// <summary>
/// 音響を管理するクラス(ゲーム全体で1つだけ生成する)
/// </summary>
class AudioManager
{
	/// <summary>
	/// ローカル参照
	/// </summary>
private:

	// --- コンストラクタ ---
	/// <summary>
	/// 音響クラスのコンストラクタ(外から生成できないようにする)
	/// </summary>
	AudioManager()
	{
		// 敵の唸り声の音量を設定(再生のたびに設定し直す必要はない)
		ChangeVolume(ScreamVolume, SE_HandleScreamSound);
	}

	// --- 読み込み補助メソッド ---
	/// <summary>
	/// BGMをメモリに全展開せず、ファイルからストリーム再生する形で読み込む関数
	/// </summary>
	/// <param name="_path">BGMファイルのパス</param>
	/// <returns>サウンドハンドル(失敗時は-1)</returns>
	static int LoadBGM(const char* _path)
	{
		SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);
		int _handle = LoadSoundMem(_path);
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);// 設定をデフォルトに戻す
		return _handle;
	}

	/// <summary>
	/// グローバル参照
	/// </summary>
public:

	// --- インスタンス取得メソッド ---
	/// <summary>
	/// 唯一のインスタンスを返す関数
	/// </summary>
	static AudioManager& Instance()
	{
		static AudioManager _instance;
		return _instance;
	}
	// コピーを禁止する
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	// --- BGMデータ(ストリーム再生) ---
	/// <summary>
	/// クリア画面のBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleClear = LoadBGM("Sounds/BGM/ClearSound.mp3");
	/// <summary>
	/// ゲームオーバー画面のBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleGameOver = LoadBGM("Sounds/BGM/GameOverSound.mp3");
	/// <summary>
	/// タイトル画面のBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleTitle = LoadBGM("Sounds/BGM/TitleSound.mp3");
	/// <summary>
	/// メインステージのBGMを読み込み参照する変数
	/// </summary>
	int BGM_HandleMainStage = LoadBGM("Sounds/BGM/MainStageSound.mp3");

	// --- SEデータ(短いのでメモリに読み込む) ---
	/// <summary>
	/// 敵出現の唸り声を読み込み参照する変数
	/// </summary>
	int SE_HandleScreamSound = LoadSoundMem("Sounds/SE/MonsterScream.mp3");
	/// <summary>
	/// リロードミニゲーム開始の音を読み込み参照する変数
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
	/// <summary>
	/// 被弾の音を読み込み参照する変数
	/// </summary>
	int SE_HandleDamageSound = LoadSoundMem("Sounds/SE/DamageSound.mp3");

	// --- 音量データ ---
	/// <summary>
	/// 敵の唸り声のボリュームを参照する定数(0〜255、255が最大)
	/// </summary>
	static constexpr int ScreamVolume = 255;

	// --- 周波数データ ---
	/// <summary>
	/// 敵の唸り声の周波数の最小値(Hz)を参照する定数
	/// </summary>
	static constexpr int ScreamFrequencyMin = 40000;
	/// <summary>
	/// 敵の唸り声の周波数の最大値(Hz)を参照する定数
	/// </summary>
	static constexpr int ScreamFrequencyMax = 58000;

	// --- データ再生関係メソッド ---
	/// <summary>
	/// BGMを再生する関数
	/// </summary>
	/// <param name="_trackNumber">再生するBGMのハンドル</param>
	void PlayBGM(int _trackNumber)
	{
		// もし指定の音声が再生されていない場合
		if (CheckSoundMem(_trackNumber) == 0)
		{
			// 音をループ再生
			PlaySoundMem(_trackNumber, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
	/// <summary>
	/// SEを再生する関数
	/// </summary>
	/// <param name="_trackNumber">再生するSEのハンドル</param>
	void PlaySE(int _trackNumber)
	{
		// 音を頭から1回再生
		PlaySoundMem(_trackNumber, DX_PLAYTYPE_BACK, TRUE);
	}
	/// <summary>
	/// 再生のたびに周波数をランダムに変えてSEを再生する関数
	/// </summary>
	/// <param name="_trackNumber">再生するSEのハンドル</param>
	/// <param name="_minFrequency">周波数の最小値(Hz)</param>
	/// <param name="_maxFrequency">周波数の最大値(Hz)</param>
	void PlayRandomPitchSE(int _trackNumber, int _minFrequency, int _maxFrequency)
	{
		// 最小〜最大の範囲から周波数を選ぶ(呼ばれるたびに選び直す)
		// ※ 最大が最小以下の場合は、最小の周波数に固定する
		int _range = (_maxFrequency > _minFrequency) ? (_maxFrequency - _minFrequency + 1) : 1;
		int _frequency = _minFrequency + rand() % _range;

		// 周波数を変えてから再生する
		ChangeFrequency(_frequency, _trackNumber);
		PlaySE(_trackNumber);
	}

	// --- データ停止メソッド ---
	/// <summary>
	/// BGMを停止する関数
	/// </summary>
	/// <param name="_trackNumber">停止するBGMのハンドル</param>
	void StopBGM(int _trackNumber)
	{
		// 音を停止
		StopSoundMem(_trackNumber);
	}

	// --- データ内調整関係メソッド ---
	/// <summary>
	/// 音量を調整する関数
	/// </summary>
	/// <param name="_volume">音量(0〜255)</param>
	/// <param name="_trackNumber">調整する音のハンドル</param>
	void ChangeVolume(int _volume, int _trackNumber)
	{
		// DxLibの音量は0〜255なので、範囲外の値は丸める
		int _clampedVolume = (_volume < 0) ? 0 : ((_volume > 255) ? 255 : _volume);

		// 音量を調整
		ChangeVolumeSoundMem(_clampedVolume, _trackNumber);
	}
	/// <summary>
	/// 周波数を調整する関数
	/// </summary>
	/// <param name="_frequency">周波数(Hz)</param>
	/// <param name="_trackNumber">調整する音のハンドル</param>
	void ChangeFrequency(int _frequency, int _trackNumber)
	{
		// 周波数を調整
		SetFrequencySoundMem(_frequency, _trackNumber);
	}
};