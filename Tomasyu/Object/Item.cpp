#include "Item.h"
#include "../Time/IsTime.h"
#include "../Util/LoadCsv.h"
#include "Player.h"
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

	const VECTOR kItemPos[2]{
		VGet(100.0f,0.0f,-300.0f),
		VGet(-100.0f,0.0f,-300.0f),
	};
}

Item::Item(std::shared_ptr<Player> pPlayer, int num) :
	m_pPlayer(pPlayer),
	m_maxItem(num)
{
	
}

Item::~Item()
{
}

void Item::Init()
{
	m_baseModel= MV1LoadModel(kModelFilePath);
	assert(m_baseModel != -1);

	m_itemPos.resize(m_maxItem);

	// アイテムの最大配列を指定
	m_item.resize(m_maxItem);
	for (size_t i = 0; i < m_item.size(); ++i)
	{
		auto& item = m_item[i];
		// モデルの読み込み
		item.m_model = MV1DuplicateModel(m_baseModel);
		assert(item.m_model != -1);
		// 座標の初期化
		item.m_pos = kItemPos[i]; // kItemPosの値を設定
		// 移動量の初期化
		item.m_move = kInitVec;
		// 角度の初期化
		item.m_angle = kInitFloat;
		// 存在しているかどうか
		item.m_isExist = true;

		// サイズ設定
		MV1SetScale(item.m_model, kModelSize);
		// 当たり判定
		item.m_col.TypeChangeSphereUpdate(item.m_col.m_itemCol, item.m_pos, kBodyColRad);
		// 復活までにかかる時間
		item.m_respawnTime = std::make_shared<IsTime>(kItemRespawnTime);
	}
}

void Item::Update()
{
	for (auto& item : m_item)
	{
		item.m_colPos = VAdd(item.m_pos, kColPosAdjustment);
		item.m_col.TypeChangeSphereUpdate(item.m_col.m_itemCol, item.m_pos, kBodyColRad);

		if (!item.m_isExist&& item.m_respawnTime->Update()) {
			item.m_isExist = true;
			item.m_respawnTime->Reset();
		}
	}



	Floating();
	ColUpdate();
}

void Item::Draw()
{
	for (auto& item : m_item)
	{
		if (!item.m_isExist) continue;

		MV1DrawModel(item.m_model);	// モデルの描画
	}

#ifdef _DEBUG
	//m_col.TypeChangeSphereDraw(m_col.m_itemCol,0xff00ff, false);
	//DrawFormatString(0, 600, 0xffffff, "item:m_pos=%.2f", m_pos.y);
	//DrawFormatString(0, 620, 0xffffff, "item:m_angle=%.2f", m_angle);
#endif
}

void Item::End()
{
	// モデルの削除
	if (m_baseModel != -1)
	{
		MV1DeleteModel(m_baseModel);
		m_baseModel = -1;
	}

	for (auto& item : m_item)
	{
		if (item.m_model != -1)
		{
			MV1DeleteModel(item.m_model);
			item.m_model = -1;
		}
	}
}

void Item::Floating()
{
	// 浮遊角度の調整
	for (auto& item : m_item)
	{
		// 浮遊角度の調整
		item.m_angle += kAddAngle;
		float angle = item.m_angle * (DX_PI_F / 180.0f);
		item.m_move.y = static_cast<float>(sin(angle)) * kFloatingAdjustment;

		// 座標更新
		item.m_pos = VAdd(item.m_pos, item.m_move);
		MV1SetPosition(item.m_model, item.m_pos);
	}
}

void Item::ColUpdate()
{
	Collision playerCol = m_pPlayer->GetCol();
	bool isItem = false;

	for (auto& item : m_item)
	{
		if (!item.m_isExist) continue;

		// プレイヤーとアイテムが当たっていたら
		if (item.m_col.IsTypeChageSphereToCapsuleCollision
		(playerCol.m_colPlayer.m_body, item.m_col.m_itemCol)) {

			isItem = true;
			item.m_isExist = false;
		}
	}

	m_pPlayer->SetGetItemFlag(isItem);
}
