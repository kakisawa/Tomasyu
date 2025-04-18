#include "SceneTutorial.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include"../Object/Player/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Map.h"
#include "../UI/UISceneGame.h"
#include "../Manager/Effect.h"
#include "../Util/Fade.h"
#include "../Util/Score.h"
#include "../Util/MiniWindow.h"
#include "DxLib.h"
#include <ctime>
#include <cassert>

using namespace MyInputInfo;

namespace {
	const char* const kTutorialHandlePath[3] = {	// チュートリアル画像パス
		"Data/Image/SceneTutorial/Tutorial1.png" ,
		"Data/Image/SceneTutorial/Tutorial2.png" ,
		"Data/Image/SceneTutorial/Tutorial3.png" ,
	};

	const VECTOR kTutorialPos = VGet(0.0f, 310.0f, 0.0f);
}

SceneTutorial::SceneTutorial() :
	m_pauseHandle(-1),
	m_tutorialDisplay(-1),
	m_isPause(false)
{
	m_pEnemy = std::make_shared<Enemy>(m_pMap, nullptr);
	m_pPlayer = std::make_shared<Player>(nullptr, nullptr, m_pItem);

	// PlayerとEnemyの相手ポインタを設定する
	m_pPlayer->SetEnemyPointer(m_pEnemy);
	m_pEnemy->SetPlayer(m_pPlayer);

	m_pCamera = std::make_shared<Camera>(m_pPlayer);
	m_pUI = std::make_shared<UISceneGame>(m_pPlayer, m_pEnemy, m_pScore);

	m_pPlayer->SetCameraPointer(m_pCamera);
	m_pItem = std::make_shared<Item>(m_pPlayer, 3);
}

SceneTutorial::~SceneTutorial()
{
}

void SceneTutorial::Init()
{
	m_pItem->Init();
	m_pCamera->Init();
	m_pPlayer->Init(m_pScore);
	m_pEnemy->Init();
	m_pMap->Init();
	m_pUI->Init();
	m_pSound->InitBGM();
	m_pScore->InitTutorial();

	// サウンド関係


	// エフェクトの読み込み
	Effect::GetInstance().Load();

	// 画像の読み込み
	m_pauseHandle = LoadGraph("Data/Image/SceneTutorial/TutorialPause.png");
	assert(m_pauseHandle != -1);

	for (int i = 0; i < m_tutorialHandle.size(); i++)
	{
		m_tutorialHandle[i] = LoadGraph(kTutorialHandlePath[i]);
		assert(m_tutorialHandle[i] != -1);
	}

	m_pMiniWindow->Init(m_pauseHandle);
	m_tutorialDisplay = m_tutorialHandle[0];
}

std::shared_ptr<SceneBase> SceneTutorial::Update(Input& input)
{
	// スタートボタンを押したらポーズ状態にする
	if (input.IsTrigger(InputInfo::DebugStart)) {
		if (m_isPause)
		{
			m_isPause = false;
			m_pMiniWindow->CloseMiniWindow();
			m_pFade->HarfFade(false);
			m_pFade->FadeIn(true);
		}
		else
		{
			m_isPause = true;
		}
	}

	//ポーズ状態
	if (m_isPause)
	{
		// 半透明にし、ミニウィンドウを表示する
		m_pFade->HarfFade(true);
		m_pMiniWindow->CallMiiniWindow();

		if (input.IsTrigger(InputInfo::OK))
		{
			return std::make_shared<SceneGame>();	// ゲームシーンへ行く
		}

		if (input.IsTrigger(InputInfo::Back))
		{
			return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
		}
	}
	else
	{
		m_pFade->FadeIn(true);
		m_pMap->Update();
		m_pItem->Update();
		m_pPlayer->Update(input);
		m_pEnemy->Update();
		m_pEnemy->TutorialHp();
		m_pCamera->Update();
		m_pUI->Update();
		// エフェクトの更新
		Effect::GetInstance().Update();


		ChangeTutorialDisplay(input);
	}

	return shared_from_this();
}

void SceneTutorial::Draw()
{
	m_pMap->Draw();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pScore->Draw();
	Effect::GetInstance().Draw();	// エフェクト表示
	m_pUI->Draw();

	DrawGraphF(kTutorialPos.x, kTutorialPos.y, m_tutorialDisplay, true);

	m_pFade->Draw();

	// ポーズ中の描画
	if (m_isPause)
	{
		m_pMiniWindow->Draw();
	}
}

void SceneTutorial::End()
{
	m_pUI->End();
	m_pMap->End();
	m_pPlayer->End();
	m_pScore->End();
	m_pEnemy->End();
	m_pSound->ReleaseSound();

	// 画像の削除
	DeleteGraph(m_pauseHandle);

	for (int i = 0; i < m_tutorialHandle.size(); i++)
	{
		DeleteGraph(m_tutorialHandle[i]);
	}
}

void SceneTutorial::ChangeTutorialDisplay(Input& input)
{
	if (input.IsTrigger(InputInfo::DebugBack))
	{
		if (m_tutorialDisplay == m_tutorialHandle[0])
		{
			m_tutorialDisplay = m_tutorialHandle[1];
		}
		else if (m_tutorialDisplay == m_tutorialHandle[1])
		{
			m_tutorialDisplay = m_tutorialHandle[2];
		}
		else if (m_tutorialDisplay == m_tutorialHandle[2])
		{
			m_tutorialDisplay = m_tutorialHandle[0];
		}
	}
}

void SceneTutorial::CheckTutorial()
{
}
