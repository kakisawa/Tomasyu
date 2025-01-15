#include "Item.h"
#include <DxLib.h>
#include <cassert>

namespace {
	constexpr float kAddAngle = 2.0f;							// 角度の追加値
	constexpr float kFloatingAdjustment = 0.1f;					// 浮遊移動量の調整用
	constexpr float kBodyColRad = 20.0f;						// 当たり判定の半径
	const VECTOR kInitPos = VGet(100.0f, 15.0f, 100.0f);		// モデルの初期座標
	const VECTOR kModelSize = VGet(50.0f, 50.0f, 50.0f);		// モデルのサイズ
	const VECTOR kColPosAdjustment = VGet(0.0f, 13.0f, 0.0f);	// 当たり判定の調整用

	constexpr float kInitFloat = 0.0f;							// Floatの初期化値
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);				// ベクトルの初期化値

	const char* kModelFilePath = "Data/Model/ItemBox.mv1";
}

Item::Item() :
	m_model(-1),
	m_angle(kInitFloat),
	m_pos(kInitPos),
	m_colPos(kInitPos),
	m_move(kInitVec)
{
}

Item::~Item()
{
}

void Item::Init()
{
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// 当たり判定の初期化
	m_col.Init();

	// モデルの位置、サイズの初期化
	MV1SetPosition(m_model, kInitPos);
	MV1SetScale(m_model, kModelSize);
}

void Item::Update()
{
	// 当たり判定の更新
	m_colPos = VAdd(m_pos, kColPosAdjustment);
	m_col.TypeChangeSphereUpdate(m_col.m_itemCol , m_colPos, kBodyColRad);

	Floating();
}

void Item::Draw()
{
	MV1DrawModel(m_model);	// モデルの描画
	
#ifdef _DEBUG
	//m_col.TypeChangeSphereDraw(m_col.m_itemCol,0xff00ff, false);
	//DrawFormatString(0, 600, 0xffffff, "item:m_pos=%.2f", m_pos.y);
	//DrawFormatString(0, 620, 0xffffff, "item:m_angle=%.2f", m_angle);
#endif
}

void Item::Floating()
{
	// 浮遊角度の調整
	m_angle += kAddAngle;
	float angle = m_angle * (DX_PI_F / 180.0f);
	m_move.y = static_cast<float>(sin(angle)) * kFloatingAdjustment;

	// 座標更新
	m_pos = VAdd(m_pos, m_move);
	MV1SetPosition(m_model, m_pos);	
}
