#pragma once
#include "../Util/Input.h"
#include<map>
#include<string>

class SoundManager
{
public:

	//BGMの種類
	enum class BGM_Type
	{
		kTitleBGM,		// タイトルBGM
		kSelectBGM,		// セレクトBGM
		kRankingBGM,	// ランキングBGM
		kOptionBGM,		// 設定BGM
		kGameBGM,		// ゲームBGM
		kGameClearBGM,	// ゲームクリアBGM
		kGameOverBGM,	// ゲームオーバーBGM
		MAX
	};

	//SEの種類
	enum class SE_Type
	{
		// 全体
		kSelectSE,		// セレクトSE
		kButtonSE,		// 決定SE
		kBackSE,		// キャンセルSE

		// プレイヤー
		kHandGunSE,		// ハンドガンSE
		kMachineGunSE,	// マシンガンSE
		kKnifeSE,		// ナイフSE
		kInstallationSE,// 罠アイテムSE
		kDrinkSE,		// 回復SE
		kSummonSE,		// 召喚SE
		kDamageReceivedSE,	// 被ダメージSE

		// エネミー
		kPunchSE1,		// 殴りSE1
		kPunchSE2,		// 殴りSE2
		kDeathrattle,	// 死亡時

		// TODO:↑全体・プレイヤー・敵でSE分けるのもあり

		MAX,
	};

	enum Select
	{
		kBgmVolume,   // BGM
		kSeVolume,    // SE
		kBack,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Input input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// BGMを調整した音量に変換する
	/// </summary>
	void SetBgmVolume();

	/// <summary>
	/// SEを調整した音量に変換する
	/// </summary>
	void SetSeVolume();

	/// <summary>
	/// 音量変更
	/// </summary>
	void ChangeSound();

	/// <summary>
	/// オプション用設定
	/// </summary>
	void SelectOption();

	/// <summary>
	/// 選択中のメニュー渡し
	/// </summary>
	/// <returns>選択中のメニュー</returns>
	int GetSoundSelect() const { return m_select; }

	/// <summary>
	/// BGM音量渡し
	/// </summary>
	/// <returns>BGM音量</returns>
	float GetBgmVolume() const { return m_bgmVolume; }

	/// <summary>
	/// SE音量渡し
	/// </summary>
	/// <returns>SE音量</returns>
	float GetSeVolume() const { return m_seVolume; }

	//サウンドの初期化
	void InitSound(void);

	//BGMのパスの初期化
	void InitBGM(void);

	/// <summary>
	/// BGMのロード
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	void LoadBGM(BGM_Type bgm);

	//SEのパスの初期化
	void InitSE(void);

	/// <summary>
	/// SEのロード	
	/// </summary>
	/// <param name="_bgm">SEの種類</param>
	void LoadSE(SE_Type se);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	/// <param name="_playType">再生形式</param>
	/// <param name="_volumePar">音量(%表記)</param>
	/// <param name="_topPositionFlag">最初から再生を始めるか (true:最初から)</param>
	void PlayBGM(BGM_Type bgm, int playType, int volumePar = 100, bool topPositionFlag = true);

	/// <summary>
	/// SEの再生
	/// </summary>
	/// <param name="_se">SEの種類</param>
	/// <param name="_playType">再生形式</param>
	/// <param name="_volumePar">音量(%表記)</param>
	/// <param name="_topPositionFlag">最初から再生を始めるか (true:最初から)</param>
	void PlaySE(SE_Type se, int playType, int volumePar = 100, bool topPositionFlag = true);

	/// <summary>
	/// BGMが再生中かどうかを返す
	/// </summary>
	/// <param name="_bgm">BGMの種類</param>
	/// <returns>true:再生中</returns>
	bool CheckPlayBGM(BGM_Type bgm);

	/// <summary>
	/// SEが再生中かどうかを返す
	/// </summary>
	/// <param name="_se">SEの種類</param>
	/// <returns>true:再生中</returns>
	bool CheckPlaySE(SE_Type se);

	/// <summary>
	/// BGMを止める
	/// </summary>
	/// <param name="_bgm">止めるBGM</param>
	void StopBGM(BGM_Type bgm);

	/// <summary>
	/// SEを止める
	/// </summary>
	/// <param name="_se">止めるSE</param>
	void StopSE(SE_Type se);

	/// <summary>
	/// サウンドの解放
	/// </summary>
	/// <param name=""></param>
	void ReleaseSound(void);
private:
	int m_select;		// 選択中のメニュー
	float m_seVolume;	// SE音量
	float m_bgmVolume;	// BGM音量

	Input m_input;

	//サウンドのパス
	std::map<BGM_Type, std::string> m_bgmPass;	//BGMのパス
	std::map<SE_Type, std::string> m_sePass;	//SEのパス

	//サウンドのハンドル
	std::map<BGM_Type, int> m_bgm;		//BGMハンドル
	std::map<SE_Type, int> m_se;		//SEハンドル
};