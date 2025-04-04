﻿#include "SceneOption.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/Fade.h"

using namespace MyInputInfo;

namespace {

	const VECTOR kOptionGraphPos = VGet(256.0f, 131.0f, 0.0f);	
	const VECTOR kSelectWindowPos = VGet(880.0f, 640.0f, 0.0f);

	const VECTOR kCursorGraphPos1[4] = {
		VGet(370.0f,322.0f,0.0f),
		VGet(408.0f,478.0f,0.0f),
		VGet(265.0f,620.0f,0.0f),
		VGet(865.0f,803.0f,0.0f)
	};

	const VECTOR kCursorGraphPos2[4] = {
		VGet(620.0f,442.0f,0.0f),
		VGet(584.0f,585.0f,0.0f),
		VGet(722.0f,742.0f,0.0f),
		VGet(1075.0f,923.0f,0.0f)
	};

	const VECTOR kBGMVolumePos[7] = {
		VGet(866.0f,341.0f,0.0f),
		VGet(951.0f,341.0f,0.0f),
		VGet(1036.0f,341.0f,0.0f),
		VGet(1121.0f,341.0f,0.0f),
		VGet(1206.0f,341.0f,0.0f),
		VGet(1291.0f,341.0f,0.0f),
		VGet(1376.0f,341.0f,0.0f),
	};

	const VECTOR kSEVolumePos[7] = {
		VGet(866.0f,489.0f,0.0f),
		VGet(951.0f,489.0f,0.0f),
		VGet(1036.0f,489.0f,0.0f),
		VGet(1121.0f,489.0f,0.0f),
		VGet(1206.0f,489.0f,0.0f),
		VGet(1291.0f,489.0f,0.0f),
		VGet(1376.0f,489.0f,0.0f),
	};


	const char* const kOptionGraph = "Data/Image/SceneOption/設定画面.png";
	const char* const kCursorGraph = "Data/Image/SceneOption/Cursor.png";
	const char* const kSound = "Data/Image/SceneOption/Volume.png";

	const char* const kSelectWindow[2]{
		 "Data/Image/SceneOption/FullScreen.png",
		  "Data/Image/SceneOption/WindowMode.png",
	};
}

SceneOption::SceneOption() :
	m_optionHandle(-1),
	m_cursorHandle(-1),
	m_volumeHandle(-1),
	m_selectWindowHandlel(-1),
	m_select(Select::BGM),
	m_selectWindow(SelectWindow::FullScreen)
{
	c1.m_selectBox1 = kCursorGraphPos1[0];
	c1.m_selectBox2 = kCursorGraphPos2[0];
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
	m_optionHandle = LoadGraph(kOptionGraph);
	m_cursorHandle = LoadGraph(kCursorGraph);
	m_volumeHandle = LoadGraph(kSound);
	for (int i = 0; i < m_selectWindowUI.size(); i++)
	{
		m_selectWindowUI[i] = LoadGraph(kSelectWindow[i]);
	}


	m_pSound->Init();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kSelectBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kSelectBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);

	if (!m_pFade->GatFadeInFlag() && input.IsTrigger(InputInfo::OK))
	{
		m_isNextSceneFlag = true;
	}

	if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag())						// 次のシーン
	{
		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}

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

	if (m_select == Select::BGM)
	{
		m_pSound->ChangeBGMVolume(input);
	}
	else if (m_select == Select::SE)
	{
		m_pSound->ChangeSEVolume(input);
	}


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
	DrawGraphF(kOptionGraphPos.x, kOptionGraphPos.y, m_optionHandle, true);

	DrawGraphF(kSelectWindowPos.x, kSelectWindowPos.y, m_selectWindowHandlel, true);
	

	DrawExtendGraphF(c1.m_selectBox1.x, c1.m_selectBox1.y,
		c1.m_selectBox2.x, c1.m_selectBox2.y, m_cursorHandle, true);


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
	DeleteGraph(m_optionHandle);
	DeleteGraph(m_cursorHandle);

	m_pSound->ReleaseSound();
}

void SceneOption::ChangeCursorInfo(int num)
{
	c1.m_selectBox1 = kCursorGraphPos1[num];
	c1.m_selectBox2 = kCursorGraphPos2[num];
}

void SceneOption::ChangeSoundVolume()
{
	m_pSound->SetBgmVolume();
	m_pSound->SetSeVolume();
}

void SceneOption::SetVolumeUI()
{
	for (int i = 0; i < m_bgmVolumeUI.size(); i++)
	{
		m_bgmVolumeUI[i] = m_volumeHandle;
	}
	for (int i = 0; i < m_seVolumeUI.size(); i++)
	{
		m_seVolumeUI[i] = m_volumeHandle;
	}

	// SEの音量
	if (m_pSound->GetSeVolume() <= ((255 / 7) * 0)) {

	}
	else if (m_pSound->GetSeVolume() <= ((255 / 7) * 1)) {
		m_seVolumeUI[0] = -1;
	}
	else if (m_pSound->GetSeVolume() <= ((255 / 7) * 2)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
	}
	else if (m_pSound->GetSeVolume() <= ((255 / 7) * 3)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
	}
	else if (m_pSound->GetSeVolume() <= ((255 / 7) * 4)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
	}
	else if (m_pSound->GetSeVolume() <= ((255 / 7) * 5)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
		m_seVolumeUI[4] = -1;
	}
	else if (m_pSound->GetSeVolume() <= ((255 / 7) * 6)) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
		m_seVolumeUI[4] = -1;
		m_seVolumeUI[5] = -1;
	}
	else if (m_pSound->GetSeVolume() <= 255) {
		m_seVolumeUI[0] = -1;
		m_seVolumeUI[1] = -1;
		m_seVolumeUI[2] = -1;
		m_seVolumeUI[3] = -1;
		m_seVolumeUI[4] = -1;
		m_seVolumeUI[5] = -1;
		m_seVolumeUI[6] = -1;
	}


	// BGMの音量
	if (m_pSound->GetBgmVolume() <= ((255 / 7) * 0)) {
		
	}
	else if (m_pSound->GetBgmVolume() <= ((255 / 7) * 1)) {
		m_bgmVolumeUI[0] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= ((255 / 7) * 2)) {
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= ((255 / 7) * 3)) {
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= ((255 / 7) * 4)) {
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= ((255 / 7) * 5)) {
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
		m_bgmVolumeUI[4] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= ((255 / 7) * 6)) {
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
		m_bgmVolumeUI[4] = -1;
		m_bgmVolumeUI[5] = -1;
	}
	else if (m_pSound->GetBgmVolume() <= 255){
		m_bgmVolumeUI[0] = -1;
		m_bgmVolumeUI[1] = -1;
		m_bgmVolumeUI[2] = -1;
		m_bgmVolumeUI[3] = -1;
		m_bgmVolumeUI[4] = -1;
		m_bgmVolumeUI[5] = -1;
		m_bgmVolumeUI[6] = -1;
	}
	
}
