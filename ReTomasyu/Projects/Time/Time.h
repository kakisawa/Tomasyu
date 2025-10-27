#pragma once
#include <array>

class Time
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Time();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Time(){}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(){}

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
	/// 秒数にあたる画像をセットする
	/// </summary>
	void SetTimeNavyHandle();

	/// <summary>
	/// 秒数にあたる画像をセットする
	/// </summary>
	void SetTimeGreenHandle();

	/// <summary>
	/// クリアにかかった時間の描画
	/// </summary>
	void DrawClearTime();

	/// <summary>
	/// 残り時間を渡す
	/// </summary>
	/// <returns>残りカウント</returns>
	int GetRemainingTime() const {return m_remainingTime;}

	/// <summary>
	/// 経過時間を渡す
	/// </summary>
	/// <returns>経過カウント</returns>
	int GetElapsedTime() const { return m_elapsedTime; }

	/// <summary>
	/// 制限時間終了フラグを渡す
	/// </summary>
	/// <returns>制限時間終了フラグ</returns>
	bool GetTimeUp() const { return m_isTimeUp; }

private:

	int m_minutes;		// 分数一桁の値
	int m_secondsTen;	// 秒数十桁の値
	int m_secondsOne;	// 秒数一桁の値

	int m_elapsedMinutes;		// 経過分数一桁の値
	int m_elapsedSecondsTen;	// 経過秒数十桁の値
	int m_elapsedSecondsOne;	// 経過秒数一桁の値

	int m_timeHandleMinites;	// 分数一桁画像を入れる変数
	int m_timeHandleSecondsTen;	// 秒数十桁画像を入れる変数
	int m_timeHandleSecondsOne;	// 秒数一桁の画像を入れる変数

	int m_elapsedTimeHandleMinites;		// 経過分数一桁画像を入れる変数
	int m_elapsedTimeHandleSecondsTen;	// 経過秒数十桁画像を入れる変数
	int m_elapsedTimeHandleSecondsOne;	// 経過秒数一桁画像を入れる変数

	int m_timeBgHandle;
	std::array<int, 11>m_numberNavyUIHandle{};	// 時間の紺UI画像
	std::array<int, 11>m_numberGreenUIHandle{};	// 時間の黒UI画像

	int m_remainingTime;// 残りカウント
	int m_elapsedTime;	// 経過カウント

	bool m_isTimeUp;	// 制限時間終了フラグ
};