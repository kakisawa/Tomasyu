#pragma once
#include "../Util/Collision.h"
#include <array>
#include <DxLib.h>

namespace {
	constexpr int kItemKind = 6;	// アイテムの種類数
	constexpr float kItemRespawnTime = 200.0f;	// アイテムリスポーン時間
}

class Item
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Item();

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
	/// 浮遊
	/// </summary>
	void Floating();

	/// <summary>
	/// 当たり判定の値渡し
	/// </summary>
	/// <returns>Itemの当たり判定の値</returns>
	Collision GetCol() const { return m_col; }

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
	int m_model;		// モデル

	float m_angle;		// 移動角度
	
	VECTOR m_pos;		// 座標
	VECTOR m_colPos;	// 当たり判定座標
	VECTOR m_move;		// 移動量

	Collision m_col;	// 当たり判定
};