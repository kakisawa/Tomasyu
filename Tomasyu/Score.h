#pragma once
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
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// スコア加算
	/// </summary>
	/// <param name="num">加算するスコア</param>
	void AddScore(int num);

	/// <summary>
	/// スコア渡し
	/// </summary>
	/// <returns>スコア</returns>
	const int GetScore() { return m_score; }

private:
	int m_score;	// スコア
};