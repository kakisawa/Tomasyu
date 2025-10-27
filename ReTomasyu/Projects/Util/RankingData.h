#pragma once
#include <Dxlib.h>
#include <vector>

class RankingData
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	RankingData();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~RankingData() {};

	/// <summary>
	/// �X�R�A�����L���O�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	void ScoreLoad();

	/// <summary>
	/// �^�C�������L���O�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	void TimeLoad();

	/// <summary>
	/// �t�@�C����������
	/// </summary>
	/// <param name="time">����</param>
	/// <param name="score">�X�R�A</param>
	/// <param name="year">�N</param>
	/// <param name="month">��</param>
	/// <param name="day">��</param>
	/// <param name="hour">����</param>
	/// <param name="min">��</param>
	
	/// <summary>
	/// �X�R�A�t�@�C����������
	/// </summary>
	/// <param name="score">�X�R�A</param>
	/// <param name="year">�N</param>
	/// <param name="month">��</param>
	/// <param name="day">��</param>
	/// <param name="hour">����</param>
	/// <param name="min">��</param>
	void ScoreSave(int score, int year, int month, int day, int hour, int min);

	/// <summary>
	/// �^�C���t�@�C����������
	/// </summary>
	/// <param name="time">����</param>
	/// <param name="year">�N</param>
	/// <param name="month">��</param>
	/// <param name="day">��</param>
	/// <param name="hour">����</param>
	/// <param name="min">��</param>
	void TimeSave(int time, int year, int month, int day, int hour, int min);

	std::vector<std::tuple<int, int, int, int, int, int>> GetScoreRanking() const { return m_scoreRanking; }	// �X�R�A�����L���O�擾
	std::vector<std::tuple<int, int, int, int, int, int>> GetTimeRanking() const { return m_timeRanking; }	// �^�C�������L���O�擾

private:

	std::vector<std::tuple<int, int, int, int, int, int>> m_scoreRanking;	// �X�R�A�����L���O
	std::vector<std::tuple<int, int, int, int, int, int>> m_timeRanking;	// �^�C�������L���O
};