#pragma once
#include "DxLib.h"
#include <array>
#include <map>
#include <memory>

class Player;
class Enemy;
class Score;
class UISceneGame
{
	enum class GunType {	// 銃の種類
		HandGun,			// ハンドガン
		MachineGun,			// マシンガン
	};

	struct BalanceBullet {
		int m_playerHundredsHandle = 0;	// プレイヤーの残弾数100の位
		int m_playerTensHandle = 0;		// プレイヤーの残弾数10の位
		int m_playerOneHandle = 0;		// プレイヤーの残弾数1の位
	};

	struct NowScore {
		int m_tenThousandsHandle = 0;	// スコアの10000の位
		int m_thousandsHandle = 0;		// スコアの1000の位
		int m_hundredsHandle = 0;		// スコアの100の位
		int m_tensHandle = 0;			// スコアの10の位
		int m_oneHandle = 0;			// スコアの1の位
	}m_score;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤー情報</param>
	/// <param name="pEnemy">対象情報</param>
	/// <param name="pScore">スコア情報</param>
	UISceneGame(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy>pEnemy, std::shared_ptr<Score> pScore);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UISceneGame();

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
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// HP・スタミナバーUIの更新
	/// </summary>
	void UpdateBarUI();

	/// <summary>
	/// アイテムUIの更新
	/// </summary>
	void UpdateItemUI();

	/// <summary>
	/// 武器UIの更新
	/// </summary>
	void UpdateWeaponUI();

	/// <summary>
	/// 選択中アイテム名UIの表示
	/// </summary>
	void SetUI_SelectItem();

	/// <summary>
	/// プレイヤーが獲得したアイテムのUIを設定する
	/// </summary>
	void SetUI_GetItem();

	/// <summary>
	/// プレイヤーの残弾UIを設定する
	/// </summary>
	void SetUI_RemainingBullets();

	/// <summary>
	/// プレイヤーのスコアを設定する
	/// </summary>
	void SetScore();

	/// <summary>
	/// プレイヤーの残弾UIを表示する
	/// </summary>
	/// <param name="type">銃の種類</param>
	/// <param name="num">残弾数</param>
	void SetUI_RemainingBulletsHandle(GunType type, int num);

private:
	int m_useWeaponChara;		// 使用中の武器の表示文字
	int m_useItemChara;			// 使用中のアイテムの表示文字
	int m_playerRemainingBullets_handgun;		// プレイヤーのハンドガン残弾
	int m_playerRemainingBullets_machinegun;	// プレイヤーのマシンガン残弾

	float m_playerHp_Green;		// プレイヤーの緑HPバー表示用
	float m_playerHp_Red;		// プレイヤーの赤HPバー表示用
	float m_playerStamina;		// プレイヤーのスタミナバー表示用
	float m_enemyHp;			// 敵のHPバー表示用

	std::array<int, 3>m_itemUI{};	// 獲得したアイテムのUIを入れる容器

	std::map<GunType, BalanceBullet> balanceBullets = {};	// 銃の残弾管理

	std::array<int, 9>m_itemCharaUIHnadle{};	// 文字画像UI用ハンドル
	std::array<int, 14>m_playerToolUIHandle{};	// 武器・アイテム画像UI用ハンドル
	std::array<int, 7>m_barUIHandle{};			// HP・スタミナバー画像UI用ハンドル
	std::array<int, 10>m_numberUIHandle{};		// 残弾表示UI用ハンドル

	VECTOR m_cursorUI1Pos;		// 武器カーソルUI座標
	VECTOR m_cursorUI2Pos;		// アイテムカーソルUI座標


	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Score> m_pScore;
};

