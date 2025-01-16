#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneRanking.h"
#include "SceneOption.h"
#include "SceneGame.h"
#include "SceneExplanation.h"
#include "SceneDebug.h"
#include "../Util/Fade.h"
#include "EffekseerForDXLib.h"

using namespace MyInputInfo;

namespace {

	VECTOR kScenenSelectUIPos[5]{
		VGet(90.0f,110.0f,0.0f),		// ゲームを始める
		VGet(110.0f,590,0.0f),			// ランキング
		VGet(620.0f,590,0.0f),			// 設定
		VGet(110.0f,770.0f,0.0f),		// 操作説明
		VGet(620.0f,770.0f,0.0f)		// ゲームを終了する
	};

	VECTOR kExampleGraph = VGet(975.0f, 75.0f, 0.0f);		// 例画像座標
	VECTOR kExplanatoryText = VGet(975.0f, 720.0f, 0.0f);	// 説明文座標
	VECTOR kPressAPos = VGet(700.0f, 1000.0f,0.0f);			// Aボタンを押す座標

	VECTOR kCursorUIPos[5]{
		VGet(kScenenSelectUIPos[0].x - 2,kScenenSelectUIPos[0].y - 2,0.0f),	// ゲームを始める
		VGet(kScenenSelectUIPos[1].x - 2,kScenenSelectUIPos[1].y - 2,0.0f),	// ランキング
		VGet(kScenenSelectUIPos[2].x - 2,kScenenSelectUIPos[2].y - 2,0.0f),	// 設定
		VGet(kScenenSelectUIPos[3].x - 2,kScenenSelectUIPos[3].y - 2,0.0f),	// 操作説明
		VGet(kScenenSelectUIPos[4].x - 2,kScenenSelectUIPos[4].y - 2,0.0f),	// ゲームを終了する
	};

	const char* const kSceneSelectUI[5]{
		"Data/Image/SceneSelect/GameStartUI.png",
		"Data/Image/SceneSelect/RankingUI.png",
		"Data/Image/SceneSelect/SettingUI.png",
		"Data/Image/SceneSelect/Operate.png",
		"Data/Image/SceneSelect/GameFinishUI.png",
	};

	const char* const kSceneSelectGtaph[5]{
		"Data/Image/SceneSelect/SceneGameGraph.png",
		"Data/Image/SceneSelect/SceneRankingGraph.png",
		"Data/Image/SceneSelect/",
		"Data/Image/SceneSelect/",
		"Data/Image/SceneSelect/",
	};

	const char* const kSceneSelectIntroductionGtaph[5]{
		"Data/Image/SceneSelect/",
		"Data/Image/SceneSelect/",
		"Data/Image/SceneSelect/",
		"Data/Image/SceneSelect/",
		"Data/Image/SceneSelect/",
	};
}

SceneSelect::SceneSelect():
	m_selectGraphX(0),
	m_selectGraphY(0),
	m_sceneSelectGraph(-1),
	m_nextSceneGrapgh(-1),
	m_nextSceneIntroduction(-1),
	m_cursorUI(-1),
	m_pressAHandle(-1),
	m_nextScene(nextScene::GameScene)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
	for (int i = 0; i < m_sceneSelectUI.size(); i++)
	{
		m_sceneSelectUI[i]= LoadGraph(kSceneSelectUI[i]);
	}
	for (int i = 0; i < m_sceneSelectGtaph.size(); i++)
	{
		m_sceneSelectGtaph[i] = LoadGraph(kSceneSelectGtaph[i]);
	}

	for (int i = 0; i < m_sceneSelectIntroductionGtaph.size(); i++)
	{
		m_sceneSelectIntroductionGtaph[i]= LoadGraph(kSceneSelectIntroductionGtaph[i]);
	}


	m_nextSceneIntroduction = LoadGraph("Data/Image/SceneSelect/説明文.png");
	m_cursorUI = LoadGraph("Data/Image/SceneSelect/Cursor1.png");
	m_pressAHandle = LoadGraph("Data/Image/SceneGame/PressA.png");


	m_sceneSelectGraph = m_sceneSelectUI[0];
	GetGraphSize(m_sceneSelectGraph, &m_selectGraphX, &m_selectGraphY);
	c1.m_selectBox1 = kCursorUIPos[0];
	c1.m_selectBox2 = VGet(kCursorUIPos[0].x + m_selectGraphX, kCursorUIPos[0].y + m_selectGraphY, 0.0f);

	m_pSound->InitSound();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kSelectBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kSelectBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());
	
	if (input.IsTrigger(InputInfo::OK)) {			// STARTボタン
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_isNextSceneFlag = true;
	}


	if (m_isNextSceneFlag)						// 次のシーン
	{
		if (m_nextScene == nextScene::TitleScene) 
		{
			return std::make_shared<SceneTitle>();
		}
		else if (m_nextScene == nextScene::GameScene)
		{
			return std::make_shared<SceneGame>();	// ゲームシーンへ行く
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			return std::make_shared<SceneExplanation>();	// 操作説明シーンへ行く
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			return std::make_shared<SceneOption>();	// オプションシーンへ行く
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			return std::make_shared<SceneRanking>();	// ランキングシーンへ行く
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			/* ここで終了させるより、メインのループから出したほうがいいか?*/
			Effkseer_End();				   // Effekseerの終了処理
			DxLib_End();				// ＤＸライブラリ使用の終了処理
		}
	}

	
	if (m_nextScene == nextScene::GameScene)
	{
		m_nextSceneGrapgh = m_sceneSelectGtaph[0];
	}
	else if (m_nextScene == nextScene::ExplanationScene)
	{
		m_nextSceneGrapgh = m_sceneSelectGtaph[3];
	}
	else if (m_nextScene == nextScene::OptionScene)
	{
		m_nextSceneGrapgh = m_sceneSelectGtaph[2];
	}
	else if (m_nextScene == nextScene::RankingScene)
	{
		m_nextSceneGrapgh = m_sceneSelectGtaph[1];
	}
	else if (m_nextScene == nextScene::GameEnd)
	{
		m_nextSceneGrapgh = m_sceneSelectGtaph[4];
	}


	SwitchingScene(input);
	

#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneSelect::Draw()
{
	for (int i = 0; i < 5; i++)
	{
		DrawGraphF(kScenenSelectUIPos[i].x, kScenenSelectUIPos[i].y, m_sceneSelectUI[i], true);
	}

	DrawGraphF(kExampleGraph.x, kExampleGraph.y, m_nextSceneGrapgh, true);
	DrawGraphF(kExplanatoryText.x, kExplanatoryText.y, m_nextSceneIntroduction, true);
	DrawExtendGraphF(c1.m_selectBox1.x, c1.m_selectBox1.y, c1.m_selectBox2.x, c1.m_selectBox2.y, m_cursorUI, true);
	DrawGraphF(kPressAPos.x, kPressAPos.y, m_pressAHandle, true);

	m_pFade->Draw();

#ifdef _DEBUG
#endif // DEBUG
}

void SceneSelect::End()
{
	m_pSound->ReleaseSound();

	DeleteGraph(m_nextSceneGrapgh);
	DeleteGraph(m_nextSceneIntroduction);
	DeleteGraph(m_pressAHandle);
	DeleteGraph(m_cursorUI);

	for (int i = 0; i < m_sceneSelectUI.size(); i++)
	{
		DeleteGraph(m_sceneSelectUI[i]);
	}
	for (int i = 0; i < m_sceneSelectGtaph.size(); i++)
	{
		DeleteGraph(m_sceneSelectGtaph[i]);
	}
	for (int i = 0; i < m_sceneSelectIntroductionGtaph.size(); i++)
	{
		DeleteGraph(m_sceneSelectIntroductionGtaph[i]);
	}
}

void SceneSelect::SwitchingScene(Input& input)
{
	// 下キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Down))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		// 左側
		if (m_nextScene == nextScene::GameScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUI[1];
			
			ChangeCursorInfo(1);
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUI[3];
			ChangeCursorInfo(3);
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}

		// 右側
		if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameEnd;
			m_sceneSelectGraph = m_sceneSelectUI[4];
			ChangeCursorInfo(4);
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}
	}

	// 上キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Up))
	{	
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		// 左側
		if (m_nextScene == nextScene::GameScene)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUI[3];
			ChangeCursorInfo(3);
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUI[1];
			ChangeCursorInfo(1);
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}

		// 右側
		if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::OptionScene;
			m_sceneSelectGraph = m_sceneSelectUI[2];
			ChangeCursorInfo(2);
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUI[0];
			ChangeCursorInfo(0);
		}
	}

	// 右キー・左キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Right)|| input.IsTrigger(InputInfo::Left))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::OptionScene;
			m_sceneSelectGraph = m_sceneSelectUI[2];
			ChangeCursorInfo(2);
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUI[1];
			ChangeCursorInfo(1);
		}

		if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::GameEnd;
			m_sceneSelectGraph = m_sceneSelectUI[4];
			ChangeCursorInfo(4);
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUI[3];
			ChangeCursorInfo(3);
		}
	}
}

void SceneSelect::ChangeCursorInfo(int num)
{
	GetGraphSize(m_sceneSelectGraph, &m_selectGraphX, &m_selectGraphY);
	c1.m_selectBox1 = kCursorUIPos[num];
	c1.m_selectBox2 = VGet(kCursorUIPos[num].x + m_selectGraphX+2, kCursorUIPos[num].y + m_selectGraphY+2, 0.0f);
}
