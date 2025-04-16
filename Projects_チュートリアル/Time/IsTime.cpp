#include "IsTime.h"

IsTime::IsTime(int rimitTime):
	m_time(0),
	m_rimitTime(rimitTime)
{
}

bool IsTime::Update()
{
	// 制限時間を超えていたらtrueを返す
	if (m_time >= m_rimitTime)	return true;
	m_time++;

	return false;
}

void IsTime::Reset()
{
	m_time = 0;
}