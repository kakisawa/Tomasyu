#include "RankingData.h"
#include <fstream>
#include <sstream>
#include <algorithm> // std::sort ���g�p���邽�߂ɕK�v

namespace {
	const char* fileName = "Data/csv/RankingData.csv";	// �ۑ�����t�@�C����
	constexpr int kRankingNum = 5;	// �ۑ����鏇�ʐ�
}

RankingData::RankingData()
{
}

void RankingData::Load()
{
	std::ifstream file(fileName);
	if (!file.is_open()) // �t�@�C���ǂݍ��ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���ǂݍ��ݎ��s\n");
#endif
	}
	else // �t�@�C���ǂݍ��ݐ���
	{
		std::string line;
		m_ranking.clear(); // ���łɗv�f������ꍇ�̓N���A����
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			int time, score;
			char comma;
			ss >> time >> comma >> score;
			m_ranking.push_back(std::make_pair(time, score));
		}
		file.close();
	}
}

void RankingData::Save(int time, int score)
{
	std::ofstream file(fileName, std::ios::out | std::ios::trunc);
	if (!file.is_open()) // �t�@�C���������ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���������ݎ��s\n");
#endif
	}
	else // �t�@�C���������ݐ���
	{
		m_ranking.push_back(std::make_pair(time, score));
		// �����L���O���\�[�g���ď�� kRankingNum ����ۑ�
		std::sort(m_ranking.begin(), m_ranking.end(), [](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first; // �N���A���ԂŃ\�[�g
			});
		if (m_ranking.size() > kRankingNum)
		{
			m_ranking.resize(kRankingNum);
		}
		for (const auto& entry : m_ranking)
		{
			file << entry.first << "," << entry.second << "\n";
		}
		file.close();
	}
}