#pragma once
class IsTime
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="rimitTime">制限時間</param>
	IsTime(int rimitTime);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IsTime();

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>制限時間を超えているかどうか</returns>
	bool Update();

	/// <summary>
	/// 時間リセット
	/// </summary>
	void Reset();

private:
	int m_time;			// 経過時間
	int m_rimitTime;	// 制限時間
};

