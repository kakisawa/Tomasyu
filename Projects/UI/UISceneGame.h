#pragma once
#include "DxLib.h"
#include <array>
#include <map>
#include <memory>

class Player;
class Enemy;
class UISceneGame
{
	enum class GunType {
		HandGun,
		MachineGun,
	};

	struct BalanceBullet {
		int m_playerHundredsHandle;	// プレイヤーの残弾数百の位
		int m_playerTensHandle;		// プレイヤーの残弾数十の位
		int m_playerOneHandle;		// プレイヤーの残弾数一の位
	};

public:
	UISceneGame(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy>pEnemy);
	virtual ~UISceneGame();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player"></param>
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
	/// <param name="player"></param>
	void UpdateBarUI();

	/// <summary>
	/// アイテムUIの更新
	/// </summary>
	/// <param name="player"></param>
	void UpdateItemUI();

	/// <summary>
	/// 武器UIの更新
	/// </summary>
	/// <param name="player"></param>
	void UpdateWeaponUI();

	/// <summary>
	/// 選択中アイテム名UIの表示
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void SetUI_SelectItem();

	/// <summary>
	/// プレイヤーが獲得したアイテムのUIを設定する
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void SetUI_GetItem();

	/// <summary>
	/// プレイヤーの残弾UIを設定する
	/// </summary>
	void SetUI_RemainingBullets();

	/// <summary>
	/// プレイヤーの残弾UIを表示する
	/// </summary>
	/// <param name="num">残弾数</param>
	void SetUI_RemainingBulletsHandle(GunType type,int num);

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
	std::array<int, 12>m_playerToolUIHandle{};	// 武器・アイテム画像UI用ハンドル
	std::array<int, 7>m_barUIHandle{};			// HP・スタミナバー画像UI用ハンドル

	std::array<int, 10>m_numberUIHandle{};	// 残弾表示UI用ハンドル

	VECTOR m_cursorUI1Pos;		// 武器カーソルUI座標
	VECTOR m_cursorUI2Pos;		// アイテムカーソルUI座標


	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
};

