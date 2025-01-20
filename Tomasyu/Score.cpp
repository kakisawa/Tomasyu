#include "Score.h"
#include "DxLib.h"

Score::Score():
	m_score(0)
{
}

Score::~Score()
{
}

void Score::Init()
{
}

void Score::Update()
{
}

void Score::Draw()
{
	DrawFormatString(0, 600, 0xffffff, "m_score=%d", m_score);
}

void Score::AddScore(int num)
{
	m_score += num;
}