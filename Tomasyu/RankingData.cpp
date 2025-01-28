#include "RankingData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>

namespace {
	const char* kTimeFilePath = "Data/csv/TimeRankingData.csv";	// スコアランキングデータを保存するファイル名
	const char* kScoreFilePath = "Data/csv/ScoreRankingData.csv";	// タイムランキングデータを保存するファイル名
	constexpr int kRankingNum = 5;	// 保存する順位数
}

RankingData::RankingData()
{
	ScoreLoad();
	TimeLoad();
}

void RankingData::ScoreLoad()
{
	std::ifstream file(kScoreFilePath);


	if (!file.is_open()) // ファイル読み込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル読み込み失敗\n");
#endif
	}
	else // ファイル読み込み成功
	{
		std::string line;
		m_scoreRanking.clear(); // すでに要素がある場合はクリアする
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			int score = 0, year = 0, month = 0, day = 0, hour = 0, min = 0;
			char comma;
			ss >>  score >> comma >> year >> comma >> month >> comma >> day >> comma >> hour >> comma >> min;
			if (ss.fail()) {
#ifdef _DEBUG
				printfDx("データの読み込みに失敗しました: %s\n", line.c_str());
#endif
				continue; // 読み込みに失敗した場合は次の行へ
			}
			m_scoreRanking.emplace_back(score, year, month, day, hour, min);
		}
		file.close();
	}
}

void RankingData::TimeLoad()
{
	std::ifstream file(kTimeFilePath);


	if (!file.is_open()) // ファイル読み込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル読み込み失敗\n");
#endif
	}
	else // ファイル読み込み成功
	{
		std::string line;
		m_timeRanking.clear(); // すでに要素がある場合はクリアする
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			int time = 0, year = 0, month = 0, day = 0, hour = 0, min = 0;
			char comma;
			ss >> time >> comma >>  year >> comma >> month >> comma >> day >> comma >> hour >> comma >> min;
			if (ss.fail()) {
#ifdef _DEBUG
				printfDx("データの読み込みに失敗しました: %s\n", line.c_str());
#endif
				continue; // 読み込みに失敗した場合は次の行へ
			}
			m_timeRanking.emplace_back(time, year, month, day, hour, min);
		}
		file.close();
	}
}

void RankingData::ScoreSave(int score, int year, int month, int day, int hour, int min)
{

	// 負の値がある場合は何もせず終了
	if ( score <= 0 || year <= 0 || month <= 0 || day <= 0 || hour <= 0 || min <= 0) {
#ifdef _DEBUG
		printfDx("負の値が含まれているため、保存をスキップしました\n");
#endif
		return; // 処理を中断
	}

	m_scoreRanking.emplace_back(score, year, month, day, hour, min);
	// ランキングをソートして上位 kRankingNum 件を保存
	std::sort(m_scoreRanking.begin(), m_scoreRanking.end(), [](const std::tuple<int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int>& rhs) {
		return std::get<0>(lhs) > std::get<0>(rhs); // クリア時間でソート
		});
	if (m_scoreRanking.size() > kRankingNum)
	{
		m_scoreRanking.resize(kRankingNum);
	}

	std::ofstream file(kScoreFilePath, std::ios::out | std::ios::trunc);


	if (!file.is_open()) // ファイル書き込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル書き込み失敗\n");
#endif
	}
	else // ファイル書き込み成功
	{
		for (const auto& entry : m_scoreRanking)
		{
			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
				<< std::get<4>(entry) << "," << std::get<5>(entry) << "\n";
		}
		file.close();
	}
}

void RankingData::TimeSave(int time, int year, int month, int day, int hour, int min)
{
	// 負の値がある場合は何もせず終了
	if (time <= 0 ||year <= 0 || month <= 0 || day <= 0 || hour <= 0 || min <= 0) {
#ifdef _DEBUG
		printfDx("負の値が含まれているため、保存をスキップしました\n");
#endif
		return; // 処理を中断
	}

	m_timeRanking.emplace_back(time, year, month, day, hour, min);
	// ランキングをソートして上位 kRankingNum 件を保存
	std::sort(m_timeRanking.begin(), m_timeRanking.end(), [](const std::tuple<int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int>& rhs) {
		return std::get<0>(lhs) < std::get<0>(rhs); // クリア時間でソート
		});
	if (m_timeRanking.size() > kRankingNum)
	{
		m_timeRanking.resize(kRankingNum);
	}

	std::ofstream file(kTimeFilePath, std::ios::out | std::ios::trunc);


	if (!file.is_open()) // ファイル書き込み失敗
	{
#ifdef _DEBUG
		printfDx("ファイル書き込み失敗\n");
#endif
	}
	else // ファイル書き込み成功
	{
		for (const auto& entry : m_timeRanking)
		{
			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
				<< std::get<4>(entry) << "," << std::get<5>(entry) <<"\n";
		}
		file.close();
	}
}
