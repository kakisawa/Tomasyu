#pragma once
#include "DxLib.h"

class Map
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Map();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Map();

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

	// エネミーが移動するためのポイント
	struct PointPos {
		VECTOR point1;
		VECTOR point2;
		VECTOR point3;
		VECTOR point4;
	}m_pointPos;

	/// <summary>
	/// エネミーが移動するためのポイント渡し
	/// </summary>
	/// <returns>エネミーが移動するためのポイント</returns>
	PointPos GetPointPos() const { return m_pointPos; }

private:
	int m_model1;	// モデル
};

