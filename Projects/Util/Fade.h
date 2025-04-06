#pragma once
class Fade
{
public:
	Fade();
	virtual ~Fade();

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
	/// フェードイン
	/// </summary>
	/// <param name="flag"></param>
	void FadeIn(bool flag);

	/// <summary>
	/// フェードアウト
	/// </summary>
	/// <param name="flag"></param>
	void FadeOut(bool flag);

	/// <summary>
	/// 半透明
	/// </summary>
	/// <param name="flag"></param>
	void HarfFade(bool flag);

	/// <summary>
	/// フェードアウトフラグをセットする
	/// </summary>
	/// <param name="flag"></param>
	void SetFadeOutFlag(bool flag) { m_isFadeOut = flag; }

	/// <summary>
	/// フェードインフラグを獲得する
	/// </summary>
	/// <returns></returns>
	bool GetFadeInFlag() const { return m_isFadeIn; }

	/// <summary>
	/// フェードアウトフラグを獲得する
	/// </summary>
	/// <returns></returns>
	bool GetFadeOutFlag() const { return m_isFadeOut; }

	/// <summary>
	/// 半透明フラグを獲得する
	/// </summary>
	/// <returns></returns>
	bool GetHarfFadeFlag() const { return m_isHarfFade; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GatNextSceneFlag() const { return m_isNextSceneFlag; }

private:
	int m_fadeAlpha;        // フェードイン、アウト
	bool m_isFadeIn;		// フェードイン用のフラグ
	bool m_isFadeOut;		// フェードアウト用のフラグ
	bool m_isHarfFade;		// 半透明用のフラグ

	bool m_isNextSceneFlag;	// 次のシーンへ行くフラグ
};

