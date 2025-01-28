#pragma once
#include <Dxlib.h>
#include <vector>

class RankingData
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RankingData();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RankingData() {};

	/// <summary>
	/// スコアランキングファイル読み込み
	/// </summary>
	/// <param name="path"></param>
	void ScoreLoad();

	/// <summary>
	/// タイムランキングファイル読み込み
	/// </summary>
	/// <param name="path"></param>
	void TimeLoad();

	/// <summary>
	/// ファイル書き込み
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="score">スコア</param>
	/// <param name="year">年</param>
	/// <param name="month">月</param>
	/// <param name="day">日</param>
	/// <param name="hour">時間</param>
	/// <param name="min">分</param>
	
	/// <summary>
	/// スコアファイル書き込み
	/// </summary>
	/// <param name="score">スコア</param>
	/// <param name="year">年</param>
	/// <param name="month">月</param>
	/// <param name="day">日</param>
	/// <param name="hour">時間</param>
	/// <param name="min">分</param>
	void ScoreSave(int score, int year, int month, int day, int hour, int min);

	/// <summary>
	/// タイムファイル書き込み
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="year">年</param>
	/// <param name="month">月</param>
	/// <param name="day">日</param>
	/// <param name="hour">時間</param>
	/// <param name="min">分</param>
	void TimeSave(int time, int year, int month, int day, int hour, int min);

	std::vector<std::tuple<int, int, int, int, int, int>> GetScoreRanking() const { return m_scoreRanking; }	// スコアランキング取得
	std::vector<std::tuple<int, int, int, int, int, int>> GetTimeRanking() const { return m_timeRanking; }	// タイムランキング取得

private:

	std::vector<std::tuple<int, int, int, int, int, int>> m_scoreRanking;	// スコアランキング
	std::vector<std::tuple<int, int, int, int, int, int>> m_timeRanking;	// タイムランキング
};