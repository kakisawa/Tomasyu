﻿#include "SceneRanking.h"
#include "SceneSelect.h"
#include "../RankingData.h"
#include "SceneDebug.h"
#include <iostream>
#include <tuple>

using namespace MyInputInfo;

namespace {
	const VECTOR kRankingPos = VGet(640.0f, 38.0f, 0.0f);


	const VECTOR kUIPos = VGet(464.0f, 214.0f, 0.0f);
	const VECTOR kCursorPos = VGet(462.0f, 212.0f, 0.0f);

	const VECTOR kRankingUIPos = VGet(142.0f, 358.0f, 0.0f);
	const VECTOR kRankingUI_Change = VGet(1322.0f, 389.0f, 0.0f);

	constexpr int kUIMoveX = 605;
	constexpr int kCursorAddX = 4;
}

SceneRanking::SceneRanking() :
	m_rankingGraph(-1),
	m_cursorUI(-1),
	m_selectUI1(-1),
	m_selectUI3(-1),
	m_rankingUI(-1),
	m_rankingSelectUI(-1),
	m_rankingUI_Score(-1),
	m_rankingUI_Time(-1)
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::Init()
{
	m_rankingGraph = LoadGraph("Data/Image/UI/ランキング.png");
	m_selectUI1 = LoadGraph("Data/Image/UI/最大スコア.png");
	m_selectUI3 = LoadGraph("Data/Image/UI/最短時間.png");
	m_cursorUI = LoadGraph("Data/Image/UI/カーソル.png");
	m_rankingUI = LoadGraph("Data/Image/UI/ランキングデータ.png");
	m_rankingUI_Score= LoadGraph("Data/Image/UI/スコア.png");
	m_rankingUI_Time = LoadGraph("Data/Image/UI/討伐時間.png");

	m_selectBox1 = kCursorPos;
	m_rankingSelectUI = m_rankingUI_Score;

	m_pSound->InitBGM();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kRankingBGM);

	m_pSound->PlayBGM(SoundManager::BGM_Type::kRankingBGM, DX_PLAYTYPE_LOOP);

	m_pRankingData->ScoreLoad();
	m_pRankingData->TimeLoad();	
}

std::shared_ptr<SceneBase> SceneRanking::Update(Input& input)
{

	SelectRanking(input);

	// ランキングデータを取得
	m_scoreRanking = m_pRankingData->GetScoreRanking();
	m_timeRanking = m_pRankingData->GetTimeRanking();


	if (input.IsTrigger(InputInfo::Back)) {			// Bボタン

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
	DrawGraphF(kRankingPos.x, kRankingPos.y, m_rankingGraph, true);

	DrawGraphF(kUIPos.x, kUIPos.y, m_selectUI1, true);
	DrawGraphF(kUIPos.x + kUIMoveX, kUIPos.y, m_selectUI3, true);

	DrawGraphF(m_selectBox1.x, m_selectBox1.y, m_cursorUI, true);

	DrawGraphF(kRankingUIPos.x, kRankingUIPos.y, m_rankingUI, true);
	DrawGraphF(kRankingUI_Change.x, kRankingUI_Change.y, m_rankingSelectUI, true);

	


	if (m_scoreRanking.size() > 0) {

		for (int i = 0; i < m_scoreRanking.size(); i++)
		{
			DrawFormatString(0, 500 + (i * 40), 0xffffff, "Score: %d", std::get<0>(m_scoreRanking[i]));
			DrawFormatString(0, 520 + (i * 40), 0xffffff, "Date: %d/%d/%d/%d/%d",
				std::get<1>(m_scoreRanking[i]), std::get<2>(m_scoreRanking[i]), std::get<3>(m_scoreRanking[i]),
				std::get<4>(m_scoreRanking[i]),std::get<5>(m_scoreRanking[i]));
		}
	}

	if (m_timeRanking.size() > 0) {

		for (int i = 0; i < m_timeRanking.size(); i++)
		{
			DrawFormatString(200, 500 + (i * 40), 0xffffff, "Time: %d:%d", std::get<0>(m_timeRanking[i])/3600, (std::get<0>(m_timeRanking[i])%3600)/60);
			DrawFormatString(200, 520 + (i * 40), 0xffffff, "Date: %d/%d/%d/%d/%d",
				std::get<1>(m_timeRanking[i]), std::get<2>(m_timeRanking[i]), std::get<3>(m_timeRanking[i]),
				std::get<4>(m_timeRanking[i]), std::get<5>(m_timeRanking[i]));
		}
	}

	

#ifdef _DEBUG
#endif // DEBUG
}

void SceneRanking::End()
{
	DeleteGraph(m_rankingGraph);
	DeleteGraph(m_selectUI1);
	DeleteGraph(m_selectUI3);
	DeleteGraph(m_cursorUI);
	DeleteGraph(m_rankingUI);
	DeleteGraph(m_rankingUI_Score);
	DeleteGraph(m_rankingUI_Time);

	m_pSound->ReleaseSound();
}

void SceneRanking::SelectRanking(Input& input)
{
	if (input.IsTrigger(InputInfo::Right))
	{
		m_selectBox1.x += kUIMoveX;
		if (m_selectBox1.x > kCursorPos.x + kUIMoveX)
		{
			m_selectBox1.x = kCursorPos.x;
		}
	}

	if (input.IsTrigger(InputInfo::Left))
	{
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
