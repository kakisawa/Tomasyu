#include "Time.h"
#include "DxLib.h"

namespace {
	constexpr int kMaxtimeLimit = 10800;	// 制限時間(3分=180秒)

	const VECTOR kGameClearTimePos[4] = {
		VGet(1060.0f, 848.0f, 0.0f),
		VGet(1144.0f, 848.0f, 0.0f),
		VGet(1183.0f, 848.0f, 0.0f),
		VGet(1247.0f, 848.0f, 0.0f),
		};	// ゲームクリア時間画像座標


	const char* const kNumberNavyUI[11]{
		"Data/Image/SceneGame/時間/0.png",
		"Data/Image/SceneGame/時間/1.png",
		"Data/Image/SceneGame/時間/2.png",
		"Data/Image/SceneGame/時間/3.png",
		"Data/Image/SceneGame/時間/4.png",
		"Data/Image/SceneGame/時間/5.png",
		"Data/Image/SceneGame/時間/6.png",
		"Data/Image/SceneGame/時間/7.png",
		"Data/Image/SceneGame/時間/8.png",
		"Data/Image/SceneGame/時間/9.png",
		"Data/Image/SceneGame/時間/点.png",
	};
	const char* const kNumberBlackUI[11]{
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
	};

	constexpr int kMinutes = 3600;	// 1分の値
	constexpr int kSecondOne = 60;	// 1秒の値
}

Time::Time():
	m_minutes(0),
	m_elapsedTime(0),
	m_secondsTen(0),
	m_secondsOne(0),
	m_elapsedMinutes(0),
	m_elapsedSecondsTen(0),
	m_elapsedSecondsOne(0),
	m_timeHandleMinites(-1),
	m_timeHandleSecondsTen(-1),
	m_timeHandleSecondsOne(-1),
	m_remainingTime(kMaxtimeLimit),
	m_isTimeUp(false)
{
	// 数字UI画像読み込み
	for (int i = 0; i < m_numberNavyUIHandle.size(); i++)
	{
		m_numberNavyUIHandle[i] = LoadGraph(kNumberNavyUI[i]);
		m_numberGreenUIHandle[i] = LoadGraph(kNumberBlackUI[i]);
	}
	m_timeBgHandle = LoadGraph("Data/Image/SceneGame/時間/Time.png");
}

Time::~Time()
{
}

void Time::Init()
{
}

void Time::Update()
{
	m_remainingTime--;
	m_elapsedTime++;

	// 表示用時間計算
	m_minutes = m_remainingTime / kMinutes;
	m_secondsTen = ((m_remainingTime % kMinutes) / kSecondOne) / 10;
	m_secondsOne= ((m_remainingTime % kMinutes) / kSecondOne) % 10;

	// 経過時間計算
	m_elapsedMinutes = m_elapsedTime / kMinutes;
	m_elapsedSecondsTen = ((m_elapsedTime % kMinutes) / kSecondOne) / 10;
	m_elapsedSecondsOne = ((m_elapsedTime % kMinutes) / kSecondOne) % 10;

	SetTimeNavyHandle();
	SetTimeGreenHandle();

	// 制限時間が0になったら
	if (m_remainingTime <= 0.0f)
	{
		m_isTimeUp = true;
	}
}

void Time::Draw()
{
	DrawGraph(10, 20, m_timeBgHandle, true);

	DrawGraph(35, 45, m_timeHandleMinites, true);
	DrawGraph(90, 50, m_numberNavyUIHandle[10], true);
	DrawGraph(110, 45, m_timeHandleSecondsTen, true);
	DrawGraph(155, 45, m_timeHandleSecondsOne, true);

#ifdef _DEBUG
	//DrawFormatString(0, 1000, 0xffffff, "m_second=%d", m_elapsedTime);
	//DrawFormatString(0, 1020, 0xffffff, "残り時間:分=%d", m_elapsedMinutes);
	//DrawFormatString(0, 1040, 0xffffff, "残り時間:十秒=%d", m_elapsedSecondsTen);
	//DrawFormatString(0, 1060, 0xffffff, "残り時間:秒=%d", m_elapsedSecondsOne);

	/*DrawFormatString(0, 1000, 0xffffff, "m_second=%.2f", m_second / 60);
	DrawFormatString(0, 1020, 0xffffff,"残り時間:分=%d", m_minutes);
	DrawFormatString(0, 1040, 0xffffff,"残り時間:十秒=%d", m_secondsTen);
	DrawFormatString(0, 1060, 0xffffff,"残り時間:秒=%d", m_secondsOne);*/

#endif // DEBUG
}

void Time::End()
{
	// 数字UI画像削除
	for (int i = 0; i < m_numberNavyUIHandle.size(); i++)
	{
		DeleteGraph(m_numberNavyUIHandle[i]);
		DeleteGraph(m_numberGreenUIHandle[i]);
	}

	DeleteGraph(m_timeBgHandle);
}

void Time::SetTimeNavyHandle()
{
	// 分数
	for (int i = 0; i < 3; i++)
	{
		if (m_minutes == i)
		{
			m_timeHandleMinites = m_numberNavyUIHandle[i];
		}
	}

	// 秒数
	for (int i = 0; i < 10; i++)
	{
		if (m_secondsTen == i)
		{
			m_timeHandleSecondsTen = m_numberNavyUIHandle[i];
		}

		if (m_secondsOne == i)
		{
			m_timeHandleSecondsOne = m_numberNavyUIHandle[i];
		}
	}
}

void Time::SetTimeGreenHandle()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_elapsedMinutes == i)
		{
			m_elapsedTimeHandleMinites = m_numberGreenUIHandle[i];
		}
	}


	// 秒数
	for (int i = 0; i < 10; i++)
	{
		if (m_elapsedSecondsTen == i)
		{
			m_elapsedTimeHandleSecondsTen = m_numberGreenUIHandle[i];
		}

		if (m_elapsedSecondsOne == i)
		{
			m_elapsedTimeHandleSecondsOne = m_numberGreenUIHandle[i];
		}
	}
}

void Time::DrawClearTime()
{
	DrawGraph(kGameClearTimePos[0].x, kGameClearTimePos[0].y, m_elapsedTimeHandleMinites, true);
	DrawGraph(kGameClearTimePos[1].x, kGameClearTimePos[1].y, m_numberGreenUIHandle[10], true);
	DrawGraph(kGameClearTimePos[2].x, kGameClearTimePos[2].y, m_elapsedTimeHandleSecondsTen, true);
	DrawGraph(kGameClearTimePos[3].x, kGameClearTimePos[3].y, m_elapsedTimeHandleSecondsOne, true);
}
