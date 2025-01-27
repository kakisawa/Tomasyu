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
	/// ファイル読み込み
	/// </summary>
	void Load(const char* path);

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
	void Save(int time, int score, int year, int month, int day, int hour, int min);


	void ScoreSave(int score, int year, int month, int day, int hour, int min);

	void TimeSave(int time, int year, int month, int day, int hour, int min);


	int GetClearTime() const { return m_clearTime; }			// クリアタイム取得
	int GetClearScore() const { return m_clearScore; }			// クリアスコア取得

	std::vector<std::tuple<int, int, int, int, int, int, int>> GetRanking() const { return m_ranking; }	// ランキング取得

private:
	int m_clearTime;	// クリアタイム
	int m_clearScore;	// クリアスコア

	std::vector<std::tuple<int, int, int, int, int, int, int>> m_ranking; // ランキング

	std::vector<std::tuple<int, int, int, int, int, int>> m_scoreRanking; // ランキング
	std::vector<std::tuple<int, int, int, int, int, int>> m_timeRanking; // ランキング
};