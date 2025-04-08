#include "SceneOption.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/Fade.h"

using namespace MyInputInfo;

namespace {
	constexpr int kSoundMax = 255;	// 音量最大値
	constexpr int kChangeSoundNum = 7;	// 音量調整の数

	constexpr int kVolumeAddDec = kSoundMax/kChangeSoundNum;	// 音量調整の増減値


	const VECTOR kOptionGraphPos = VGet(216.0f, 78.0f, 0.0f);	// 設定画面画像座標
	const VECTOR kSelectWindowPos = VGet(960.0f, 640.0f, 0.0f);	// 画面サイズ画像座標

	const VECTOR kCursorGraphPos[4] = {  // カーソル座標
		VGet(572.0f,285.0f,0.0f),
		VGet(572.0f,440.0f,0.0f),
		VGet(572.0f,588.0f,0.0f),
		VGet(944.0f,722.0f,0.0f)
	};

	const VECTOR kBGMVolumePos[7] = {	// BGMボリュームUI座標
		VGet(944.0f,341.0f,0.0f),
		VGet(1029.0f,341.0f,0.0f),
		VGet(1114.0f,341.0f,0.0f),
		VGet(1199.0f,341.0f,0.0f),
		VGet(1284.0f,341.0f,0.0f),
		VGet(1369.0f,341.0f,0.0f),
		VGet(1454.0f,341.0f,0.0f),
	};

	const VECTOR kSEVolumePos[7] = {	// SEボリュームUI座標
		VGet(944.0f,490.0f,0.0f),
		VGet(1029.0f,490.0f,0.0f),
		VGet(1114.0f,490.0f,0.0f),
		VGet(1199.0f,490.0f,0.0f),
		VGet(1284.0f,490.0f,0.0f),
		VGet(1369.0f,490.0f,0.0f),
		VGet(1454.0f,490.0f,0.0f),
	};

	const char* const kOptionGraph = "Data/Image/SceneOption/SettingScreen.png";	// 設定画面画像パス
	const char* const kCursorGraph = "Data/Image/SceneOption/Cursor.png";			// カーソル画像パス
	const char* const kSound = "Data/Image/SceneOption/Volume.png";					// 音量画像パス

	const char* const kSelectWindow[2]
	{
		 "Data/Image/SceneOption/FullScreen.png",
		  "Data/Image/SceneOption/WindowMode.png",
	};// 画面サイズ画像パス
}

SceneOption::SceneOption() :
	m_optionHandle(-1),
	m_cursorHandle(-1),
	m_volumeHandle(-1),
	m_selectWindowHandlel(-1),
	m_select(Select::BGM),
	m_selectWindow(SelectWindow::FullScreen)
{
	m_cursorPos = kCursorGraphPos[0];
}

void SceneOption::Init()
{
	// 画像読み込み
	m_optionHandle = LoadGraph(kOptionGraph);
	m_cursorHandle = LoadGraph(kCursorGraph);
	m_volumeHandle = LoadGraph(kSound);
	for (int i = 0; i < m_selectWindowUI.size(); i++)
	{
		m_selectWindowUI[i] = LoadGraph(kSelectWindow[i]);
	}

	// サウンド初期化
	m_pSound->Init();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kSelectBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kSelectBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	// フェード処理
	m_pFade->FadeIn(m_pFade->GetFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);

	if (!m_pFade->GetFadeInFlag() && input.IsTrigger(InputInfo::OK))
	{
		m_isNextSceneFlag = true;
	}

	if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag())	// 次のシーン
	{
		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}

	// カーソル移動
	if (input.IsTrigger(InputInfo::Down)) {
		if (m_select==Select::BGM)
		{
			m_select = Select::SE;
			ChangeCursorInfo(1);
		}
		else if (m_select == Select::SE)
		{
			m_select = Select::Window;
			ChangeCursorInfo(2);
		}
		else if (m_select == Select::Window)
		{
			m_select = Select::Back;
			ChangeCursorInfo(3);
		}
		else if (m_select == Select::Back)
		{
			m_select = Select::BGM;
			ChangeCursorInfo(0);
		}
	}

	if (input.IsTrigger(InputInfo::Up)) {
		if (m_select == Select::BGM)
		{
			m_select = Select::Back;
			ChangeCursorInfo(3);
		}
		else if (m_select == Select::SE)
		{
			m_select = Select::BGM;
			ChangeCursorInfo(0);
		}
		else if (m_select == Select::Window)
		{
			m_select = Select::SE;
			ChangeCursorInfo(1);
		}
		else if (m_select == Select::Back)
		{
			m_select = Select::Window;
			ChangeCursorInfo(2);
		}
	}

	// 音量設定
	if (m_select == Select::BGM)
	{
		m_pSound->ChangeBGMVolume(input);
	}
	else if (m_select == Select::SE)
	{
		m_pSound->ChangeSEVolume(input);
	}


	// ウィンドウタイプ変更
	if (m_select == Select::Window)
	{
		if(input.IsTrigger(InputInfo::Right))
		{
			m_selectWindow += 1;
			if (m_selectWindow == SelectWindowNum) {
				m_selectWindow = SelectWindow::FullScreen;
			}
		}
		if (input.IsTrigger(InputInfo::Left))
		{
			m_selectWindow -= 1;
			if (m_selectWindow == -1) {
				m_selectWindow = SelectWindow::WindowMode;
			}
		}
	}

	if (m_selectWindow == SelectWindow::FullScreen) {
		ChangeWindowMode(false);
		m_selectWindowHandlel = m_selectWindowUI[0];
	}
	else if (m_selectWindow == SelectWindow::WindowMode)
	{
		ChangeWindowMode(true);
		m_selectWindowHandlel = m_selectWindowUI[1];
	}


	ChangeSoundVolume();
	SetVolumeUI();


#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{
	// 画像描画
	DrawGraphF(kOptionGraphPos.x, kOptionGraphPos.y, m_optionHandle, true);
	DrawGraphF(kSelectWindowPos.x, kSelectWindowPos.y, m_selectWindowHandlel, true);
	DrawGraphF(m_cursorPos.x, m_cursorPos.y, m_cursorHandle, true);

	for (int i = 0; i < m_bgmVolumeUI.size(); i++)
	{
		DrawGraphF(kBGMVolumePos[i].x, kBGMVolumePos[i].y, m_bgmVolumeUI[i], true);
	}

	for (int i = 0; i < m_seVolumeUI.size(); i++)
	{
		DrawGraphF(kSEVolumePos[i].x, kSEVolumePos[i].y, m_seVolumeUI[i], true);
	}

	// フェード処理
	m_pFade->Draw();

#ifdef _DEBUG
	//DrawFormatString(0, 460, 0xffffff, "m_selectWindow=%d", m_selectWindow);
	//DrawFormatString(0, 480, 0xffffff, "BGM=%d", m_pSound->GetBgmVolume());
	//DrawFormatString(0, 500, 0xffffff, "SE=%d", m_pSound->GetSeVolume());
	//DrawFormatString(0, 520, 0xffffff, "SE=%d", (255 / 7) * 5);
	//DrawFormatString(0, 540, 0xffffff, "SE=%d", (255 / 7) * 6);
	//DrawFormatString(0, 560, 0xffffff, "SE=%d",(255 / 7) * 7);
#endif // DEBUG
}

void SceneOption::End()
{
	// 画像削除
	DeleteGraph(m_optionHandle);
	DeleteGraph(m_cursorHandle);

	m_pSound->ReleaseSound();
}

void SceneOption::ChangeCursorInfo(int num)
{
	m_cursorPos = kCursorGraphPos[num];
}

void SceneOption::ChangeSoundVolume()
{
	m_pSound->SetBgmVolume();
	m_pSound->SetSeVolume();
}

void SceneOption::SetVolumeUI()
{
	// 初期化
	for (int i = 0; i < m_bgmVolumeUI.size(); i++)
	{
		m_bgmVolumeUI[i] = m_volumeHandle;
	}
	for (int i = 0; i < m_seVolumeUI.size(); i++)
	{
		m_seVolumeUI[i] = m_volumeHandle;
	}

	// SEの音量
	if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 0)) {

	}
	else if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 1)) {
		m_seVolumeUI[0] = -1;
	}
	else if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 2)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
	}
	else if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 3)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
	}
	else if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 4)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
	}
	else if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 5)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
		m_seVolumeUI[4] = -1;
	}
	else if (m_pSound->GetSeVolume() <= (kVolumeAddDec * 6)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
		m_seVolumeUI[4] = -1;
		m_seVolumeUI[5] = -1;
	}
	else if (m_pSound->GetSeVolume() <= kSoundMax) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
		m_seVolumeUI[4] = -1;
		m_seVolumeUI[5] = -1;
		m_seVolumeUI[6] = -1;
	}


	// BGMの音量
	if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 0)) 
	{}
	else if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 1)) 
	{
		m_bgmVolumeUI[0] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 2)) 
	{
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 3)) 
	{
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 4)) 
	{
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 5)) 
	{
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
		m_bgmVolumeUI[4] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= (kVolumeAddDec * 6)) 
	{
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
		m_bgmVolumeUI[4] = -1;
		m_bgmVolumeUI[5] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= kSoundMax)
	{
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
		m_bgmVolumeUI[4] = -1;
		m_bgmVolumeUI[5] = -1;
		m_bgmVolumeUI[6] = -1;
	}
	
}
