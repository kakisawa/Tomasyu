#include "SceneRanking.h"
#include "SceneSelect.h"
#include "../RankingData.h"
#include "SceneDebug.h"
#include <iostream>

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

	m_pRankingData->Load();

	
}

std::shared_ptr<SceneBase> SceneRanking::Update(Input& input)
{

	SelectRanking(input);



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

	// ランキングデータを取得
	std::vector<std::pair<int, int>> ranking = m_pRankingData->GetRanking();

	DrawFormatString(0, 500, 0xffffff, "Time:%d", ranking[0].first);
	DrawFormatString(0, 520, 0xffffff, "Second:%d", ranking[0].second);
	
	DrawFormatString(0, 540, 0xffffff, "Time:%d", ranking[1].first);
	DrawFormatString(0, 560, 0xffffff, "Second:%d", ranking[1].second);

	DrawFormatString(0, 580, 0xffffff, "Time:%d", ranking[2].first);
	DrawFormatString(0, 600, 0xffffff, "Second:%d", ranking[2].second);


	// ランキングデータを表示
	for (const auto& entry : ranking)
	{
		std::cout << "Time: " << entry.first << ", Score: " << entry.second << std::endl;

		
	}

#ifdef _DEBUG
	//DrawString(0, 0, "SceneRanking", 0xffffff);
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
