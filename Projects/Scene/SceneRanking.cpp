#include "SceneRanking.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/RankingData.h"
#include "../Util/Fade.h"
#include <iostream>
#include <tuple>

using namespace MyInputInfo;

namespace {
	constexpr int kUIMoveX = 605;
	constexpr int kCursorAddX = 4;
	
	const VECTOR kRankingPos = VGet(637.0f, 91.0f, 0.0f);
	const VECTOR kUIPos = VGet(462.0f, 236.0f, 0.0f);
	const VECTOR kCursorPos = VGet(457.5f, 231.5f, 0.0f);
	const VECTOR kRankingUIPos = VGet(142.0f, 358.0f, 0.0f);
	const VECTOR kRankingUI_Change = VGet(1322.0f, 389.0f, 0.0f);

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);
}

SceneRanking::SceneRanking() :
	m_bg(-1),
	m_rankingGraph(-1),
	m_cursorGraph(-1),
	m_selectUI1(-1),
	m_selectUI3(-1),
	m_rankingUI(-1),
	m_rankingSelectUI(-1),
	m_rankingUI_Score(-1),
	m_rankingUI_Time(-1),
	m_rankingDataChange(rankingDataChange::Score),
	m_selectBox1(kInitVec)
{
}

void SceneRanking::Init()
{
	// 画像の読み込み
	m_bg = LoadGraph("Data/Image/SceneRanking/Bg.png");
	m_rankingGraph = LoadGraph("Data/Image/SceneRanking/ランキング.png");
	m_selectUI1 = LoadGraph("Data/Image/SceneRanking/最大スコア.png");
	m_selectUI3 = LoadGraph("Data/Image/SceneRanking/最短時間.png");
	m_cursorGraph = LoadGraph("Data/Image/SceneRanking/カーソル.png");
	m_rankingUI = LoadGraph("Data/Image/SceneRanking/ランキングデータ.png");
	m_rankingUI_Score= LoadGraph("Data/Image/SceneRanking/スコア.png");
	m_rankingUI_Time = LoadGraph("Data/Image/SceneRanking/討伐時間.png");

	m_selectBox1 = kCursorPos;
	m_rankingSelectUI = m_rankingUI_Score;

	m_pSound->InitSound();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kRankingBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kRankingBGM, DX_PLAYTYPE_LOOP);

	m_pRankingData->ScoreLoad();
	m_pRankingData->TimeLoad();	
}

std::shared_ptr<SceneBase> SceneRanking::Update(Input& input)
{
	m_pFade->FadeIn(m_pFade->GetFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);

	SelectRanking(input);

	// ランキングデータを取得
	m_scoreRanking = m_pRankingData->GetScoreRanking();
	m_timeRanking = m_pRankingData->GetTimeRanking();


	if (!m_pFade->GetFadeInFlag() && input.IsTrigger(InputInfo::Back)) 
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_isNextSceneFlag = true;
	}

	if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag())						// 次のシーン
	{
		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}

#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {	// STARTボタン

	//	return std::make_shared<SceneDebug>();	// デバッグシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneRanking::Draw()
{
	DrawGraph(0, 0, m_bg, true);
	DrawGraphF(kRankingPos.x, kRankingPos.y, m_rankingGraph, true);

	DrawGraphF(kUIPos.x, kUIPos.y, m_selectUI1, true);
	DrawGraphF(kUIPos.x + kUIMoveX, kUIPos.y, m_selectUI3, true);

	DrawGraphF(m_selectBox1.x, m_selectBox1.y, m_cursorGraph, true);

	DrawGraphF(kRankingUIPos.x, kRankingUIPos.y, m_rankingUI, true);
	DrawGraphF(kRankingUI_Change.x, kRankingUI_Change.y, m_rankingSelectUI, true);

	
	if (m_rankingDataChange == rankingDataChange::Score) {
		if (m_scoreRanking.size() > 0) {

			for (int i = 0; i < m_scoreRanking.size(); i++)
			{
				// プレイ日時描画
				SetFontSize(70);
				DrawFormatString(660, 490 + (i * 100), 0xffffff, "%d/%d/%d %d:%d",
					std::get<1>(m_scoreRanking[i]), std::get<2>(m_scoreRanking[i]), std::get<3>(m_scoreRanking[i]),
					std::get<4>(m_scoreRanking[i]), std::get<5>(m_scoreRanking[i]));

				// スコア描画
				SetFontSize(80);
				DrawFormatString(1430, 490 + (i * 100), 0xffffff, "%d", std::get<0>(m_scoreRanking[i]));
			}
		}
	}
	else if (m_rankingDataChange == rankingDataChange::Time) {
		if (m_timeRanking.size() > 0) {

			for (int i = 0; i < m_timeRanking.size(); i++)
			{
				// プレイ日時描画
				SetFontSize(70);
				DrawFormatString(660, 490 + (i * 100), 0xffffff, "%d/%d/%d %d:%d",
					std::get<1>(m_timeRanking[i]), std::get<2>(m_timeRanking[i]), std::get<3>(m_timeRanking[i]),
					std::get<4>(m_timeRanking[i]), std::get<5>(m_timeRanking[i]));

				// 時間描画
				SetFontSize(80);
				DrawFormatString(1430, 490 + (i * 100), 0xffffff, "%d:%d",
					std::get<0>(m_timeRanking[i]) / 3600, (std::get<0>(m_timeRanking[i]) % 3600) / 60);
			}
		}
	}

	SetFontSize(14);

	// フェード処理
	m_pFade->Draw();

#ifdef _DEBUG
#endif // DEBUG
}

void SceneRanking::End()
{
	DeleteGraph(m_bg);
	DeleteGraph(m_rankingGraph);
	DeleteGraph(m_selectUI1);
	DeleteGraph(m_selectUI3);
	DeleteGraph(m_cursorGraph);
	DeleteGraph(m_rankingUI);
	DeleteGraph(m_rankingUI_Score);
	DeleteGraph(m_rankingUI_Time);

	m_pSound->ReleaseSound();
}

void SceneRanking::SelectRanking(Input& input)
{
	if (input.IsTrigger(InputInfo::Right))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		m_rankingDataChange++;
		if (m_rankingDataChange >= rankingDataChange::RankingDataNum)
		{
			m_rankingDataChange = rankingDataChange::Score;
		}

		m_selectBox1.x += kUIMoveX;
		if (m_selectBox1.x > kCursorPos.x + kUIMoveX)
		{
			m_selectBox1.x = kCursorPos.x;
		}
	}

	if (input.IsTrigger(InputInfo::Left))
	{

		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		m_rankingDataChange--;
		if (m_rankingDataChange < 0)
		{
			m_rankingDataChange = rankingDataChange::Time;
		}

		m_selectBox1.x -= kUIMoveX;
		if (m_selectBox1.x < kCursorPos.x)
		{
			m_selectBox1.x = kCursorPos.x + kUIMoveX;
		}
	}

	if (m_selectBox1.x == kCursorPos.x)
	{
		m_rankingSelectUI = m_rankingUI_Score;
	}
	else if (m_selectBox1.x == kCursorPos.x + kUIMoveX)
	{
		m_rankingSelectUI = m_rankingUI_Time;
	}
}
