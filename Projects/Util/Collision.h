#pragma once
#include <DxLib.h>

class Collision
{
public:

	struct ColType
	{
		VECTOR m_pos = VGet(0.0f, 0.0f, 0.0f);		// 座標
		VECTOR m_vertexPos = VGet(0.0f, 0.0f, 0.0f);	// 頂点座標
		float m_radius = 0.0f;		// 半径
	};

	struct Player			// プレイヤーの当たり判定部位
	{
		ColType m_body;		// 体
		ColType m_weapon;	// 武器
	}m_colPlayer;

	struct Enemy			// 敵の当たり判定部位
	{
		ColType m_body;			// 体
		ColType m_rightArm[2];	// 右腕
		ColType m_leftArm[2];	// 左腕
		ColType m_search;		// 索敵範囲
		ColType m_hitting;		// 接触範囲
	}m_colEnemy;

	ColType m_colItem;		// アイテムの当たり判定
	ColType m_colBullet;	// 弾の当たり判定

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Collision() {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Collision() {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 変更後:球体の当たり判定更新
	/// </summary>
	/// <param name="colType">当たり判定の属性</param>
	/// <param name="pos">座標</param>
	/// <param name="radius">半径</param>
	void TypeChangeSphereUpdate(ColType& colType, const VECTOR pos, const float radius);

	/// <summary>
	/// 変更後:カプセルの当たり判定更新
	/// </summary>
	/// <param name="colType">当たり判定の属性</param>
	/// <param name="pos">座標</param>
	/// <param name="upPos">頂点座標</param>
	/// <param name="radius">半径</param>
	void TypeChangeCapsuleUpdate(ColType& colType,const VECTOR pos, const VECTOR upPos, const float radius);

	/// <summary>
	/// 変更後:球体の当たり判定描画
	/// </summary>
	/// <param name="colType">当たり判定の属性</param>
	/// <param name="color">色</param>
	/// <param name="isFill">色を塗りつぶすか</param>
	void TypeChangeSphereDraw(const ColType& colType, unsigned int color, bool isFill);

	/// <summary>
	/// カプセルの当たり判定描画
	/// </summary>
	/// <param name="colType">当たり判定の属性</param>
	/// <param name="color">色</param>
	/// <param name="isFill">色を塗りつぶすか</param>
	void TypeChangeCapsuleDraw(const ColType& colType, unsigned int color, bool isFill);

	/// <summary>
	/// 球体の当たり判定
	/// </summary>
	/// <param name="colType">当たり判定の属性</param>
	/// <param name="col">もう一つの値</param>
	/// <returns>当たっていたらtrueを返す</returns>
	bool IsTypeChageSphereCollision(const ColType& colType,const Collision& col,const Collision::ColType& m_col);

	/// <summary>
	/// カプセルの当たり判定
	/// </summary>
	/// <param name="colType">当たり判定の属性</param>
	/// <param name="col">もう一つの当たり判定</param>
	/// <returns>当たっていたらtrueを返す</returns>
	bool IsTypeChageCupsuleCollision(const ColType& colType, const Collision::ColType& m_col);

	/// <summary>
	/// 球体とカプセルの当たり判定
	/// </summary>
	/// <param name="colType">当たり判定の属性(Cupsule)</param>
	/// <param name="col">もう一つの当たり判定(Sphere)</param>
	/// <returns>当たっていたらtrueを返す</returns>
	bool IsTypeChageSphereToCapsuleCollision(const ColType& colType, const Collision::ColType& m_col);

private:
};