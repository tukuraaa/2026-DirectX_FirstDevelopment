#include "LogicManager.h"
#include <ctime>

/// <summary>
/// ロジックマネージャーのコンストラクタを定義する関数
/// </summary>
LogicManager::LogicManager() : _reloadState(ReloadState::Wait), _targetKey(L' '), _candidates({ L'A', L'B', L'C', L'X', L'Y' }) 
{
	srand((unsigned int)time(NULL));// 乱数のシードを初期化
}

void LogicManager::Update() {
	// 常にRキーの入力を監視（GetAsyncKeyStateは手軽に判定できます）
	bool isRPressed = (GetAsyncKeyState('R') & 0x8000);

	switch (_reloadState) {
	case ReloadState::Wait:
		if (isRPressed) {
			// R以外の候補からランダムに選択
			_targetKey = _candidates[rand() % _candidates.size()];
			_reloadState = ReloadState::Play;
		}
		break;

	case ReloadState::Play:
		// 指定されたキーが押されたかチェック
		if (GetAsyncKeyState(_targetKey) & 0x8000) {
			_reloadState = ReloadState::Clear;
		}
		break;

	case ReloadState::Clear:
		// 何かキー（例えばSpace）を押したら最初に戻る
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			_reloadState = ReloadState::Wait;
		}
		break;
	}
}

std::wstring LogicManager::GetDisplayText() const {
	// 「Rボタンを押してください」は常に表示する方針なので、状態に合わせて結合
	std::wstring base = L"常に表示: Rボタンを押してください\n";

	if (_reloadState == ReloadState::Clear) {
		return base + L"GAME CLEAR! (Spaceでリトライ)";
	}
	return base;
}

std::wstring LogicManager::GetTargetKeyText() const {
	if (_reloadState == ReloadState::Play) {
		return std::wstring(1, _targetKey); // 選ばれたキーを1文字の文字列として返す
	}
	return L"";
}