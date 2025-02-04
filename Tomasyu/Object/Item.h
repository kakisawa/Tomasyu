#pragma once
#include "../Util/Collision.h"
#include <memory>
#include <vector>
#include "DxLib.h"

namespace {
	constexpr int kItemKind = 6;	// アイテムの種類数
	constexpr float kItemRespawnTime = 200.0f;	// アイテムが復活するまでの時間
}

class Player;
class IsTime;
class Item
{
public:

	struct PosData
	{
		float posX;
		float posY;
		float posZ;
	}m_posData;


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤー情報</param>
	/// <param name="num">アイテムの上限数</param>
	Item(std::shared_ptr<Player> pPlayer,int num);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Item();

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
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// 浮遊
	/// </summary>
	void Floating();

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	void ColUpdate();

	// アイテムの種類
	enum class ItemKind {
		NoItem,			// アイテム無し
		IceFloor,		// 氷床
		landmine,		// 地雷
		SurpriseBox,	// びっくり箱
		RecoveryMedic,	// 回復薬
		Ammunition,		// 弾薬
	};

private:
	int m_maxItem;		// アイテムの上限数
	int m_baseModel;	// アイテムの基本モデル

	struct ItemInfo
	{
		int m_model = -1;						// モデル
		float m_angle = 0.0f;					// 移動角度
		bool m_isExist = false;				// 存在しているかのフラグ
		VECTOR m_pos = VGet(0.0f, 0.0f, 0.0);	// 座標
		VECTOR m_size = VGet(0.0f, 0.0f, 0.0);	// サイズ
		VECTOR m_colPos = VGet(0.0f, 0.0f, 0.0);// 当たり判定座標
		VECTOR m_move = VGet(0.0f, 0.0f, 0.0f);	// 移動量
		Collision m_col;						// 当たり判定
		std::shared_ptr<IsTime> m_respawnTime;	// 復活するまでの時間
	};

	std::vector<ItemInfo> m_item;		// アイテム(配列)

	std::shared_ptr<Player> m_pPlayer;	// プレイヤーポインタ
};