#include <windows.h>
#include "LogicManager.h"

/// <summary>
/// ウィンドウのメッセージを処理する関数
/// </summary>
/// <param name="hwnd"></param>
/// <param name="uint"></param>
/// <param name="wparam"></param>
/// <param name="lparam"></param>
/// <returns></returns>
LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _uint, WPARAM _wparam, LPARAM _lparam)
{
	// もしウィンドウが破棄された場合
	if (_uint == WM_DESTROY)
	{
		PostQuitMessage(0);// アプリケーションを終了するためのメッセージを送る
		return 0;
	}

	return DefWindowProc(_hwnd, _uint, _wparam, _lparam);// デフォルトのウィンドウプロシージャを呼び出す
}

/// <summary>
/// ゲームを開始するためのエントリーポイント関数
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int WINAPI WinMain(HINSTANCE _hinstance_Instance, HINSTANCE _hinstancePrev_Instance, LPSTR _lpstr, int _nCmdShow)
{
	// ウィンドウの登録と作成
	const wchar_t _cLASS_NAME[] = L"MyGameWindow";// ウィンドウクラスの構造体を初期化
	WNDCLASS _wndclass = {};// ウィンドウプロシージャを設定
	_wndclass.lpfnWndProc = WindowProc;// ウィンドウクラスのインスタンスを設定
	_wndclass.hInstance = _hinstance_Instance;// ウィンドウクラスの名前を設定
	_wndclass.lpszClassName = _cLASS_NAME;// ウィンドウクラスを登録
	RegisterClass(&_wndclass);// ウィンドウを作成
	HWND _hwnd = CreateWindowEx(0, _cLASS_NAME, L"DirectX12 R-Key Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, _hinstance_Instance, nullptr);// ウィンドウのハンドルを取得

	// もしウィンドウの作成に失敗した場合
	if (_hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(_hwnd, _nCmdShow);// ウィンドウを表示

	// ゲームの準備
	LogicManager _logic;// ゲームロジックのインスタンスを作成
	MSG _msg = {};// メッセージ構造体を初期化

	// ゲームループ
	while (_msg.message != WM_QUIT)
	{
		// もしメッセージがある場合
		if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);// メッセージを翻訳
			DispatchMessage(&_msg);// メッセージをウィンドウプロシージャに送る
		}
		else
		{
			_logic.Update();// ロジックの更新

			// ② 描画処理（DirectX 12のコマンドを積んで画面に映す）
			// Render(logic); 

			// もし表示するテキストがある場合
			if (_logic.GetTargetKeyText() != L"")
			{
				SetWindowText(_hwnd, (L"ターゲットキー: " + _logic.GetTargetKeyText()).c_str());// ウィンドウのタイトルを更新
			}
		}
	}

	return 0;
}