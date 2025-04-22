#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneRanking.h"
#include "SceneOption.h"
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneExplanation.h"
#include "SceneDebug.h"
#include "../Util/Fade.h"
#include "../Util/MiniWindow.h"
#include "EffekseerForDXLib.h"

using namespace MyInputInfo;

namespace {



	const VECTOR kScenenSelectUIPos[5]{
			VGet(107.0f,177.0f,0.0f),		// ゲームを始める
			VGet(130.0f,616,0.0f),			// ランキング
			VGet(640.0f,617,0.0f),			// 設定
			VGet(129.0f,798.0f,0.0f),		// 操作説明
			VGet(641.0f,798.0f,0.0f)		// ゲームを終了する
	};

	const VECTOR kExampleGraph = VGet(958.0f, 162.0f, 0.0f);	// 例画像座標
	const VECTOR kExplanatoryText = VGet(950.0f, 708.0f, 0.0f);	// 説明文座標

	const VECTOR kCursorUIPos[5]{		// 選択中カーソルの座標
			VGet(517.0f,177.0f,0.0f),	// ゲームを始める
			VGet(386.0f,587.0f,0.0f),	// ランキング
			VGet(754.0f,587.0f,0.0f),	// 設定
			VGet(372.0f,762.0f,0.0f),	// 操作説明
			VGet(753.0f,762.0f,0.0f),	// ゲームを終了する
	};

	const VECTOR kPressAUIPos[5]{	// 選択中PressAボタンの座標
			VGet(840.0f,496.0f,0.0f),	// ゲームを始める
			VGet(555.0f,708.0f,0.0f),	// ランキング
			VGet(840.0f,708.0f,0.0f),	// 設定
			VGet(555.0f,895.0f,0.0f),	// 操作説明
			VGet(840.0f,895.0f,0.0f),	// ゲームを終了する
	};

	const VECTOR kTutorialCursorUIPos[2]{
		VGet(738.0f,602.0f,0.0f),			// はい
		VGet(1149.0f,602.0f,0.0f),			// いいえ
	};

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// 初期化用

	const char* const kSceneSelectUI[5]{
		"Data/Image/SceneSelect/GameStartUI.png",		// ゲームを始める
		"Data/Image/SceneSelect/RankingUI.png",			// ランキング
		"Data/Image/SceneSelect/SettingUI.png",			// 設定
		"Data/Image/SceneSelect/Operate.png",			// 操作説明
		"Data/Image/SceneSelect/GameFinishUI.png",		// ゲームを終了する
	};

	const char* const kSceneSelectGtaph[5]{
		"Data/Image/SceneSelect/SceneGameGraph.png",	// ゲームを始める
		"Data/Image/SceneSelect/SceneRankingGraph.png",	// ランキング
		"Data/Image/SceneSelect/SceneSetting.png",		// 設定
		"Data/Image/SceneSelect/SceneExplanation.png",	// 操作説明
		"Data/Image/SceneSelect/",						// ゲームを終了する
	};

	const char* const kSceneSelectIntroductionGtaph[5]{
		"Data/Image/SceneSelect/GameStartText.png",
		"Data/Image/SceneSelect/RankingText.png",
		"Data/Image/SceneSelect/SettingText.png",
		"Data/Image/SceneSelect/ExplanationText.png",
		"Data/Image/SceneSelect/GameFinishTextUI.png",
	};
}

SceneSelect::SceneSelect() :
	m_selectGraphX(0),
	m_selectGraphY(0),
	m_sceneSelectGraph(-1),
	m_nextSceneGrapgh(-1),
	m_nextSceneIntroductionGraph(-1),
	m_cursorUIHandle(-1),
	m_pressAHandle(-1),
	m_bgHandle(-1),
	m_tutorialCheckHandle(-1),
	m_isTutorialCheck(true),
	m_isTutorialCursol(false),
	m_cursolPos(kInitVec),
	m_pressAPos(kInitVec),
	m_nextScene(nextScene::GameScene)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
	// 画像の読み込み
	for (int i = 0; i < m_sceneSelectUIHandle.size(); i++)
	{
		m_sceneSelectUIHandle[i] = LoadGraph(kSceneSelectUI[i]);
	}
	for (int i = 0; i < m_sceneSelectHandle.size(); i++)
	{
		m_sceneSelectHandle[i] = LoadGraph(kSceneSelectGtaph[i]);
	}

	for (int i = 0; i < m_sceneSelectIntroductionHandle.size(); i++)
	{
		m_sceneSelectIntroductionHandle[i] = LoadGraph(kSceneSelectIntroductionGtaph[i]);
	}

	m_nextSceneIntroductionGraph = LoadGraph("Data/Image/SceneSelect/説明文.png");
	m_cursorUIHandle = LoadGraph("Data/Image/SceneSelect/Cursor.png");
	m_pressAHandle = LoadGraph("Data/Image/SceneSelect/PressA.png");
	m_bgHandle = LoadGraph("Data/Image/SceneSelect/board2.png");
	m_tutorialCheckHandle = LoadGraph("Data/Image/SceneSelect/TutorialCheck.png");

	m_pMiniWindow->Init(m_tutorialCheckHandle);

	// 画像・座標のセット
	m_sceneSelectGraph = m_sceneSelectUIHandle[0];
	GetGraphSize(m_sceneSelectGraph, &m_selectGraphX, &m_selectGraphY);
	m_cursolPos = kCursorUIPos[0];
	m_pressAPos = kPressAUIPos[0];

	// サウンドの初期化・読み込み
	m_pSound->InitSound();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kSelectBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kSelectBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneSelect::Update(Input& input)
{
	// フェード処理
	m_pFade->FadeIn(m_pFade->GetFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);

	if (!m_pFade->GetFadeInFlag() && input.IsTrigger(InputInfo::OK)) {
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_isNextSceneFlag = true;
	}

	if (!m_pFade->GetFadeInFlag() && input.IsTrigger(InputInfo::Back)) {
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_nextScene = nextScene::TitleScene;
		m_isNextSceneFlag = true;
	}

	if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag())	// 次のシーン
	{
		if (m_nextScene == nextScene::TitleScene)
		{
			return std::make_shared<SceneTitle>();
		}
		else if (m_nextScene == nextScene::GameScene)
		{
			// チュートリアルをするかの確認
			TutorialCheck(input);

			if (input.IsTrigger(InputInfo::OK))
			{
				if (!m_isTutorialCheck) {
					return std::make_shared<SceneGame>();	// ゲームシーンへ行く
				}
				else if (m_isTutorialCheck)
				{
					return std::make_shared<SceneTutorial>();	// チュートリアルシーンへ行く
				}
			}
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
		m_nextSceneGrapgh = m_sceneSelectHandle[0];
		m_nextSceneIntroductionGraph = m_sceneSelectIntroductionHandle[0];
	}
	else if (m_nextScene == nextScene::ExplanationScene)
	{
		m_nextSceneGrapgh = m_sceneSelectHandle[3];
		m_nextSceneIntroductionGraph = m_sceneSelectIntroductionHandle[3];
	}
	else if (m_nextScene == nextScene::OptionScene)
	{
		m_nextSceneGrapgh = m_sceneSelectHandle[2];
		m_nextSceneIntroductionGraph = m_sceneSelectIntroductionHandle[2];
	}
	else if (m_nextScene == nextScene::RankingScene)
	{
		m_nextSceneGrapgh = m_sceneSelectHandle[1];
		m_nextSceneIntroductionGraph = m_sceneSelectIntroductionHandle[1];
	}
	else if (m_nextScene == nextScene::GameEnd)
	{
		m_nextSceneGrapgh = m_sceneSelectHandle[4];
		m_nextSceneIntroductionGraph = m_sceneSelectIntroductionHandle[4];
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
	DrawGraphF(0, 0, m_bgHandle, true);

	for (int i = 0; i < 5; i++)
	{
		DrawGraphF(kScenenSelectUIPos[i].x, kScenenSelectUIPos[i].y, m_sceneSelectUIHandle[i], true);
	}

	DrawGraphF(kExampleGraph.x, kExampleGraph.y, m_nextSceneGrapgh, true);
	DrawGraphF(kExplanatoryText.x, kExplanatoryText.y, m_nextSceneIntroductionGraph, true);
	DrawGraphF(m_cursolPos.x, m_cursolPos.y, m_cursorUIHandle, true);
	DrawGraphF(m_pressAPos.x, m_pressAPos.y, m_pressAHandle, true);

	m_pFade->Draw();

	// チュートリアルを行うかの確認画像を描画する
	if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag() && m_nextScene == nextScene::GameScene)
	{
		m_pMiniWindow->Draw();
		// カーソル画像
		DrawGraphF(m_cursolPos.x, m_cursolPos.y, m_cursorUIHandle, true);
	}
#ifdef _DEBUG
	DrawFormatString(0, 500, 0xffffff, "m_isTutorialCheck=%d", m_isTutorialCheck);
#endif // DEBUG
}

void SceneSelect::End()
{
	// 画像の削除
	m_pSound->ReleaseSound();

	DeleteGraph(m_nextSceneGrapgh);
	DeleteGraph(m_nextSceneIntroductionGraph);
	DeleteGraph(m_pressAHandle);
	DeleteGraph(m_cursorUIHandle);
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_tutorialCheckHandle);

	for (int i = 0; i < m_sceneSelectUIHandle.size(); i++)
	{
		DeleteGraph(m_sceneSelectUIHandle[i]);
	}
	for (int i = 0; i < m_sceneSelectHandle.size(); i++)
	{
		DeleteGraph(m_sceneSelectHandle[i]);
	}
	for (int i = 0; i < m_sceneSelectIntroductionHandle.size(); i++)
	{
		DeleteGraph(m_sceneSelectIntroductionHandle[i]);
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
			m_sceneSelectGraph = m_sceneSelectUIHandle[1];

			ChangeCursorInfo(1);
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[3];
			ChangeCursorInfo(3);
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[0];
			ChangeCursorInfo(0);
		}

		// 右側
		if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameEnd;
			m_sceneSelectGraph = m_sceneSelectUIHandle[4];
			ChangeCursorInfo(4);
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[0];
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
			m_sceneSelectGraph = m_sceneSelectUIHandle[3];
			ChangeCursorInfo(3);
		}
		else if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[1];
			ChangeCursorInfo(1);
		}
		else if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[0];
			ChangeCursorInfo(0);
		}

		// 右側
		if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::OptionScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[2];
			ChangeCursorInfo(2);
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::GameScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[0];
			ChangeCursorInfo(0);
		}
	}

	// 右キー・左キーを押すと次のシーンの変更をする
	if (input.IsTrigger(InputInfo::Right) || input.IsTrigger(InputInfo::Left))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		if (m_nextScene == nextScene::RankingScene)
		{
			m_nextScene = nextScene::OptionScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[2];
			ChangeCursorInfo(2);
		}
		else if (m_nextScene == nextScene::OptionScene)
		{
			m_nextScene = nextScene::RankingScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[1];
			ChangeCursorInfo(1);
		}

		if (m_nextScene == nextScene::ExplanationScene)
		{
			m_nextScene = nextScene::GameEnd;
			m_sceneSelectGraph = m_sceneSelectUIHandle[4];
			ChangeCursorInfo(4);
		}
		else if (m_nextScene == nextScene::GameEnd)
		{
			m_nextScene = nextScene::ExplanationScene;
			m_sceneSelectGraph = m_sceneSelectUIHandle[3];
			ChangeCursorInfo(3);
		}
	}
}

void SceneSelect::ChangeCursorInfo(int num)
{
	// 画像の読み込み・カーソル座標の変更
	GetGraphSize(m_sceneSelectGraph, &m_selectGraphX, &m_selectGraphY);
	m_cursolPos = kCursorUIPos[num];
	m_pressAPos = kPressAUIPos[num];
}

void SceneSelect::TutorialCheck(Input& input)
{
	if (!m_isTutorialCursol) {
		m_cursolPos = kTutorialCursorUIPos[0];
		m_isTutorialCursol = true;
	}

	// チュートリアルをするか選択させる
	m_pMiniWindow->CallMiiniWindow();
	
	if (input.IsTrigger(InputInfo::Right)|| input.IsTrigger(InputInfo::Left))
	{
		if (m_cursolPos.x == kTutorialCursorUIPos[0].x)
		{
			m_cursolPos = kTutorialCursorUIPos[1];
			m_isTutorialCheck = false;
		}
		else if (m_cursolPos.x == kTutorialCursorUIPos[1].x)
		{
			m_cursolPos = kTutorialCursorUIPos[0];
			m_isTutorialCheck = true;
		}
	}
}
