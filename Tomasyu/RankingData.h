#pragma once
#include <Dxlib.h>
#include <vector>

class RankingData
{
public:
	/// <summary>
	/// 
	/// </summary>
	RankingData();

	/// <summary>
	/// 
	/// </summary>
	~RankingData() {};

	/// <summary>
	/// �t�@�C���ǂݍ���
	/// </summary>
	void Load();

	/// <summary>
	/// �t�@�C����������
	/// </summary>
	/// <param name="time">����</param>
	/// <param name="score">�X�R�A</param>
	void Save(int time,int score);

	int GetClearTime() const { return m_clearTime; }			// �N���A�^�C���擾
	int GetClearScore() const { return m_clearScore; }			// �N���A�X�R�A�擾

	std::vector<std::pair<int, int>> GetRanking() const { return m_ranking; }	// �����L���O�擾

private:
	int m_clearTime;	// �N���A�^�C��
	int m_clearScore;	// �N���A�X�R�A

	struct rankingData
	{
		int m_clearTime;
		int m_clearScore;
	}m_rankingData;




	std::vector<std::pair<int,int>> m_ranking;// �����L���O
};