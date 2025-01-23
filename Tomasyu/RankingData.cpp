#include "RankingData.h"
#include <fstream>
#include <sstream>
#include <algorithm> // std::sort を使用するために必要

namespace {
	const char* fileName = "Data/csv/RankingData.csv";	// 保存するファイル名
	constexpr int kRankingNum = 5;	// 保存する順位数
}

RankingData::RankingData()
{
}

void RankingData::Load()
{
	std::ifstream file(fileName);
	if (!file.is_open()) // ファイル読み込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル読み込み失敗\n");
#endif
	}
	else // ファイル読み込み成功
	{
		std::string line;
		m_ranking.clear(); // すでに要素がある場合はクリアする
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
	if (!file.is_open()) // ファイル書き込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル書き込み失敗\n");
#endif
	}
	else // ファイル書き込み成功
	{
		m_ranking.push_back(std::make_pair(time, score));
		// ランキングをソートして上位 kRankingNum 件を保存
		std::sort(m_ranking.begin(), m_ranking.end(), [](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first; // クリア時間でソート
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