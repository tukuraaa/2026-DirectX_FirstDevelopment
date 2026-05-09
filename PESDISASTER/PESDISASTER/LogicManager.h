#pragma once
#include <string>
#include <vector>

/// <summary>
/// ゲームの状態を表す列挙型
/// </summary>
enum class ReloadState
{
	Wait,       // ゲームプレイ中の状態
	Play,       // リロードミニゲーム中の状態
	Clear       // リロード完了の状態
};

/// <summary>
/// ロジックを管理するクラス
/// </summary>
class LogicManager
{
	/// <summary>
	/// 周りのコードからアクセスされる変数や関数を定義するセクション
	/// </summary>
public:
	/// <summary>
	/// ロジックマネージャーのコンストラクタを定義する関数
	/// </summary>
	LogicManager();

	/// <summary>
	/// 毎フレーム呼び出される更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 画面に表示するメイン文字のテキストを取得する関数
	/// </summary>
	/// <returns></returns>
	std::wstring GetDisplayText() const;
	/// <summary>
	/// ランダムに選ばれたキーの文字のテキストを取得する関数
	/// </summary>
	/// <returns></returns>
	std::wstring GetTargetKeyText() const;

	/// <summary>
	/// 周りのコードからアクセスされない変数や関数を定義するセクション
	/// </summary>
private:
	/// <summary>
	/// リロードの状態を管理する変数
	/// </summary>
	ReloadState _reloadState;

	/// <summary>
	/// ターゲットとなるキーを管理する変数
	/// </summary>
	wchar_t _targetKey;

	/// <summary>
	/// 候補キーのリストを管理する変数
	/// </summary>
	std::vector<wchar_t> _candidates;
};