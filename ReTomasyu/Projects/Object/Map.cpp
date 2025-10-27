#include "Map.h"
#include <cassert>

namespace {
	const VECTOR kPointPos1 = VGet(-200.0f, 0.0f, 800.0f);	// 左前
	const VECTOR kPointPos2 = VGet(200.0f, 0.0f, 1300.0f);	// 左後
	const VECTOR kPointPos3 = VGet(-200.0f, 0.0f, 1300.0f);	// 右後
	const VECTOR kPointPos4 = VGet(200.0f, 0.0f, 800.0f);	// 右前

	const VECTOR kMapPos = VGet(0.0f, -20.0f, 0.0f);	// マップ座標
	const VECTOR kMapSize = VGet(0.5f, 0.5f, 0.5f);	// マップサイズ
}


Map::Map():
	m_model(-1)
{
}

void Map::Init()
{
	// モデルの読み込み
	m_model = MV1LoadModel("Data/Model/Map/Map.mv1");
	assert(m_model != -1);

	// モデルの座標・サイズセット
	MV1SetPosition(m_model, kMapPos);
	MV1SetScale(m_model, kMapSize);

	// ポイントセット
	m_pointPos.point1 = kPointPos1;
	m_pointPos.point2 = kPointPos2;
	m_pointPos.point3 = kPointPos3;
	m_pointPos.point4 = kPointPos4;
}

void Map::Draw()
{
	// モデル描画
	MV1DrawModel(m_model);

#ifdef _DEBUG
	// 各ポイントへ球体を描画
	/*DrawSphere3D(m_pointPos.point1, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point2, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point3, 10.0f, 32, 0x0000ff, 0x0000ff, true);
	DrawSphere3D(m_pointPos.point4, 10.0f, 32, 0x0000ff, 0x0000ff, true);*/
#endif // DEBUG	
}

void Map::End()
{
	// モデルの削除
	MV1DeleteModel(m_model);
}
