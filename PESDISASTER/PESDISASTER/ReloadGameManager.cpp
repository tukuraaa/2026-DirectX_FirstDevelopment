#include <windows.h>
#include <cstdlib>

/// <summary>
/// リロードの列挙型の状態を参照する変数（最初は待機状態からスタート）
/// </summary>
ReloadState _currentState = ReloadState::Wait;

/// <summary>
/// 押すべきランダムなキーのコードを参照する変数
/// </summary>
int _targetKey = 0;
/// <summary>
/// 「R以外」の指定されたキーのリストを参照する変数
/// </summary>
const int _candidateKeys[] = { 'A', 'B', 'C', 'X', 'Y' };
/// <summary>
/// キーの候補の数を参照する変数
/// </summary>
const int _candidateCount = 5;
/// <summary>
/// キーの押下状態をチェックすためのマスクを参照する変数
/// </summary>
const int _keyPressedMask = 0x8000;
/// <summary>
/// キーコードを文字列に変換する際のバッファサイズを参照する変数
/// </summary>
const int _keyStr_Index = 2;

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
/// 初期化時に乱数の種を設定しておく関数
/// </summary>
void UpdateReloadKey()
{
	switch (_currentState)
	{
	case ReloadState::Wait:

		// もしRキーが押された場合
		if (GetAsyncKeyState('R') & _keyPressedMask)
		{
			// R以外の指定されたキーからランダムに1つ選ぶ
			int randomIndex = rand() % _candidateCount;// 0から候補の数-1までのランダムなインデックスを生成
			_targetKey = _candidateKeys[randomIndex];// _candidateKeys[randomIndex]でランダムに選ばれたキーコードを取得

			_currentState = ReloadState::Play;// 状態をプレイ中に変更
		}

		break;

	case ReloadState::Play:

		// もしランダムに選ばれたキーが押された場合
		if (GetAsyncKeyState(_targetKey) & _keyPressedMask)
		{
			_currentState = ReloadState::Clear;// 状態をクリアに変更
		}

		// ※必要であれば、ここで間違ったキーを押したときの処理（ゲームオーバーなど）も追加できます。

		break;

	case ReloadState::Clear:

		_currentState = ReloadState::Wait; // 最初からやり直し

		break;
	}
}

/// <summary>
/// 描画設定を行う関数
/// </summary>
void Render()
{
	// 画面のクリア処理などをここで行う

	DrawText("Rボタンを押してください", 10, 10);// 画面左上に常に表示

	switch (_currentState) {

	case ReloadState::Wait:

		// 待機中は特に真ん中には何も出さない、あるいはタイトルを出すなど

		break;

	case ReloadState::Play:

		// 選ばれたターゲットキーの文字を画面中央などに大きく表示
		char keyStr[_keyStr_Index] = { (char)_targetKey, '\0' };// 文字列に変換
		DrawText(keyStr, 400, 300); // 画面中央の座標を指定して描画

		break;

	case ReloadState::Clear:

		DrawText("ゲームクリア！ (Spaceでリトライ)", 300, 300);

		break;
	}

	// 画面のフリップ（Present）処理などをここで行う
}