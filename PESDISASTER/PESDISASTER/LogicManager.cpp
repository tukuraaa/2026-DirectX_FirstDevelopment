#include "LogicManager.h"
#include <ctime>

/// <summary>
/// ロジックマネージャーのコンストラクタを定義する関数
/// </summary>
LogicManager::LogicManager() : _reloadState(ReloadState::Wait), _targetKey(L' '), _candidates({ L'A', L'B', L'C', L'X', L'Y' })
{
	srand((unsigned int)time(NULL));// 乱数のシードを初期化
}

/// <summary>
/// 毎フレーム呼び出される更新関数
/// </summary>
void LogicManager::Update()
{
	bool isR_Pressed = (GetAsyncKeyState('R') & 0x8000);// 常にRキーの入力を監視

	switch (_reloadState)
	{
	case ReloadState::Wait:

		// もしRキーが押された場合
		if (isR_Pressed)
		{
			// R以外の候補からランダムに選択
			_targetKey = _candidates[rand() % _candidates.size()];// 候補からランダムにキーを選ぶ
			_reloadState = ReloadState::Play;
		}

		break;

	case ReloadState::Play:

		// もしターゲットキーが押された場合
		if (GetAsyncKeyState(_targetKey) & 0x8000)
		{
			_reloadState = ReloadState::Clear;
		}

		break;

	case ReloadState::Clear:

		// もしスペースキーが押された場合
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			_reloadState = ReloadState::Wait;
		}

		break;
	}
}

/// <summary>
/// テキストを表示する関数
/// </summary>
/// <returns></returns>
std::wstring LogicManager::GetDisplayText() const
{
	std::wstring base = L"常に表示: Rボタンを押してください\n";// 状態に合わせて常時表示テキストを結合

	// もしリロードが完了している場合
	if (_reloadState == ReloadState::Clear)
	{
		return base + L"GAME CLEAR! (Spaceでリトライ)";
	}

	return base;
}

/// <summary>
/// 選択されたキーを取得する関数
/// </summary>
/// <returns></returns>
std::wstring LogicManager::GetTargetKeyText() const
{
	// もしリロードミニゲーム中の場合
	if (_reloadState == ReloadState::Play)
	{
		return std::wstring(1, _targetKey); // 選ばれたキーを1文字の文字列として返す
	}

	return L"";
}