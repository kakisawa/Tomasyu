﻿#pragma once
#include <memory>
#include <array>

class Time;
class Score
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Score();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Score();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::shared_ptr<Time> time);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// スコア加算
	/// </summary>
	/// <param name="num">加算するスコア</param>
	void AddScore(int num);

	/// <summary>
	/// 合計スコアの計算
	/// </summary>
	void CalculatingTotalScore();

	/// <summary>
	/// 合計スコアの描画
	/// </summary>
	void DrawClearScore();

	/// <summary>
	/// スコア渡し
	/// </summary>
	/// <returns>スコア</returns>
	const int GetScore() { return m_score; }

private:
	int m_score;				// スコア
	int m_remainingTimeScore;	// 残り時間スコア
	int m_totalScore;			// 合計スコア

	int m_totalScoreOneDigits;	// 合計スコアの一桁目
	int m_totalScoreTwoDigits;	// 合計スコアの二桁目
	int m_totalScoreThreeDigits;// 合計スコアの三桁目
	int m_totalScoreFourDigits;	// 合計スコアの四桁目
	int m_totalScoreFiveDigits;	// 合計スコアの五桁目

	std::array<int, 11>m_numberBlackUIHandle{};	// 黒数字UI画像
	

	std::shared_ptr<Time>m_pTime;
};