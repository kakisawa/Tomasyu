#include "DxLib.h"
#include "Manager/SceneManager.h"
#include "EffekseerForDXLib.h"
#include "../Manager/Effect.h"
#include "../Util/Input.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	// Windowモード設定(フルスクリーン)
	ChangeWindowMode(true);
	// ウインドウ名設定
	SetMainWindowText("Tomasyu");

	// 画面サイズの設定
	SetGraphMode(1920, 1080, 32);
	

	SetWindowSizeExtendRate(1.0);
	SetWindowSizeChangeEnableFlag(true, true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Effekseerの初期化
	Effekseer_Init(8000);

	// フルスクリーン切り替え時におかしくならないように
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Zバッファの設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	SetDrawScreen(DX_SCREEN_BACK);

	// シーン管理
	std::shared_ptr<SceneManager> m_pScene = std::make_shared<SceneManager>();
	m_pScene->Init();

	Input input;

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		input.Update();

		// ゲームの処理
		m_pScene->Update(input);
		m_pScene->Draw();
		

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}

	Effkseer_End();				   // Effekseerの終了処理
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}