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
	/// �t�@�C���ǂݍ���
	/// </summary>
	void Load(const char* path);

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
	void Save(int time, int score, int year, int month, int day, int hour, int min);


	void ScoreSave(int score, int year, int month, int day, int hour, int min);

	void TimeSave(int time, int year, int month, int day, int hour, int min);


	int GetClearTime() const { return m_clearTime; }			// �N���A�^�C���擾
	int GetClearScore() const { return m_clearScore; }			// �N���A�X�R�A�擾

	std::vector<std::tuple<int, int, int, int, int, int, int>> GetRanking() const { return m_ranking; }	// �����L���O�擾

private:
	int m_clearTime;	// �N���A�^�C��
	int m_clearScore;	// �N���A�X�R�A

	std::vector<std::tuple<int, int, int, int, int, int, int>> m_ranking; // �����L���O

	std::vector<std::tuple<int, int, int, int, int, int>> m_scoreRanking; // �����L���O
	std::vector<std::tuple<int, int, int, int, int, int>> m_timeRanking; // �����L���O
};