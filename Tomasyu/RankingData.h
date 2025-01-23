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
	/// ファイル読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// ファイル書き込み
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="score">スコア</param>
	void Save(int time,int score);

	int GetClearTime() const { return m_clearTime; }			// クリアタイム取得
	int GetClearScore() const { return m_clearScore; }			// クリアスコア取得

	std::vector<std::pair<int, int>> GetRanking() const { return m_ranking; }	// ランキング取得

private:
	int m_clearTime;	// クリアタイム
	int m_clearScore;	// クリアスコア

	struct rankingData
	{
		int m_clearTime;
		int m_clearScore;
	}m_rankingData;




	std::vector<std::pair<int,int>> m_ranking;// ランキング
};