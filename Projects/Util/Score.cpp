﻿#include "Score.h"
#include "../Time/Time.h"
#include "DxLib.h"

namespace {
	constexpr int kRemainingTimScore = 50;	// 1秒あたりの定数スコア
	constexpr int kSecondConversion = 60;	// 秒換算用

	constexpr int kNumberHandleNum = 10;	// 数字画像の最大数

	const VECTOR kScorePos[5] = {
	VGet(1028.0f, 714.0f, 0.0f),
	VGet(1096.0f, 714.0f, 0.0f),
	VGet(1166.0f, 714.0f, 0.0f),
	VGet(1229.0f, 714.0f, 0.0f),
	VGet(1295.0f, 714.0f, 0.0f),
	};	// ゲームクリア時間画像座標

	const char* const kNumberBlackPath[11]{
	"Data/Image/SceneGame/Clear/0.png",
	"Data/Image/SceneGame/Clear/1.png",
	"Data/Image/SceneGame/Clear/2.png",
	"Data/Image/SceneGame/Clear/3.png",
	"Data/Image/SceneGame/Clear/4.png",
	"Data/Image/SceneGame/Clear/5.png",
	"Data/Image/SceneGame/Clear/6.png",
	"Data/Image/SceneGame/Clear/7.png",
	"Data/Image/SceneGame/Clear/8.png",
	"Data/Image/SceneGame/Clear/9.png",
	"Data/Image/SceneGame/Clear/点.png",
	};	// 「黒数字・点画像」パス
}

Score::Score():
	m_score(0),
	m_remainingTimeScore(0),
	m_totalScore(0),
	m_totalScoreOneDigits(0),
	m_totalScoreTwoDigits(0),
	m_totalScoreThreeDigits(0),
	m_totalScoreFourDigits(0),
	m_totalScoreFiveDigits(0),
	m_totalScoreHandleOneDigits(-1),
	m_totalScoreHandleTwoDigits(-1),
	m_totalScoreHandleThreeDigits(-1),
	m_totalScoreHandleFourDigits(-1),
	m_totalScoreHandleFiveDigits(-1)
{
}

void Score::Init(std::shared_ptr<Time> time)
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberBlackUIHandle.size(); i++)
	{
		m_numberBlackUIHandle[i] = LoadGraph(kNumberBlackPath[i]);
	}

	m_pTime = time;		// ポインタの値入れ
}

void Score::InitTutorial()
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberBlackUIHandle.size(); i++)
	{
		m_numberBlackUIHandle[i] = LoadGraph(kNumberBlackPath[i]);
	}
}

void Score::Update()
{
	int time = m_pTime->GetRemainingTime() / kSecondConversion;	// 時間(秒)

	m_remainingTimeScore = time * kRemainingTimScore;			// 時間と定数スコアをかける
	m_totalScore = m_score + m_remainingTimeScore;				// 残り時間スコアとスコアを足す


	CalculatingTotalScore();
	SetTotalScoreHandle();
}

void Score::Draw()
{
#ifdef _DEBUG
	/*DrawFormatString(0, 600, 0xffffff, "m_score=%d", m_score);
	DrawFormatString(0, 620, 0xffffff, "m_remainingTimeScore=%d", m_remainingTimeScore);
	DrawFormatString(0, 640, 0xffffff, "m_pTime->GetRemainingTime=%d", m_pTime->GetRemainingTime()/kSecondConversion);*/
#endif // DEBUG
}

void Score::End()
{
	// 数字UI画像削除
	for (int i = 0; i < m_numberBlackUIHandle.size(); i++)
	{
		DeleteGraph(m_numberBlackUIHandle[i]);
	}
}

void Score::AddScore(int num)
{
	m_score += num;
}

void Score::CalculatingTotalScore()
{
	m_totalScoreFiveDigits = m_totalScore / 10000;
	m_totalScoreFourDigits = (m_totalScore % 10000) / 1000;
	m_totalScoreThreeDigits = (m_totalScore % 1000) / 100;
	m_totalScoreTwoDigits = (m_totalScore % 100) / 10;
	m_totalScoreOneDigits = m_totalScore % 10;

}

void Score::DrawClearScore()
{
	DrawGraphF(kScorePos[0].x, kScorePos[0].y, m_totalScoreHandleFiveDigits, true);
	DrawGraphF(kScorePos[1].x, kScorePos[1].y, m_totalScoreHandleFourDigits, true);
	DrawGraphF(kScorePos[2].x, kScorePos[2].y, m_totalScoreHandleThreeDigits, true);
	DrawGraphF(kScorePos[3].x, kScorePos[3].y, m_totalScoreHandleTwoDigits, true);
	DrawGraphF(kScorePos[4].x, kScorePos[4].y, m_totalScoreHandleOneDigits, true);

#ifdef DEBUG
	DrawFormatString(0, 680, 0xffffff, "m_totalScore=%d", m_totalScore);
#endif // DEBUG
}

void Score::SetTotalScoreHandle()
{
	for (int i = 0; i < kNumberHandleNum; i++)
	{
		// 一桁目
		if (m_totalScoreOneDigits == i)
		{
			m_totalScoreHandleOneDigits = m_numberBlackUIHandle[i];
		}

		// 二桁目
		if (m_totalScoreTwoDigits == i)
		{
			m_totalScoreHandleTwoDigits = m_numberBlackUIHandle[i];
		}

		// 三桁目
		if (m_totalScoreThreeDigits == i)
		{
			m_totalScoreHandleThreeDigits = m_numberBlackUIHandle[i];
		}

		// 四桁目
		// 四・五桁目の値が両方とも0の場合はなにも入れない
		if (m_totalScoreFiveDigits == 0 && m_totalScoreFourDigits == 0)
		{
			m_totalScoreHandleTwoDigits = -1;
		}
		else if (m_totalScoreFourDigits == i)
		{
			m_totalScoreHandleFourDigits = m_numberBlackUIHandle[i];
		}

		// 五桁目
		// 五桁目の値が0の場合はなにも入れない
		if (m_totalScoreFiveDigits == 0)
		{
			m_totalScoreHandleFiveDigits = -1;
		}
		else if (m_totalScoreFiveDigits == i)
		{
			m_totalScoreHandleFiveDigits = m_numberBlackUIHandle[i];
		}
	}
}
