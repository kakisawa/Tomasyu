#include "Collision.h"

namespace {
	const int kArmParts = 2;	// 腕部分のパーツ数

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化
}

void Collision::Init()
{
	// 敵初期化
	m_colEnemy.m_body.m_pos = kInitVec;
	m_colEnemy.m_body.m_vertexPos = kInitVec;
	m_colEnemy.m_body.m_radius = 0.0f;

	for (int i = 0; i < kArmParts; i++)
	{
		m_colEnemy.m_leftArm[i].m_pos = kInitVec;
		m_colEnemy.m_leftArm[i].m_vertexPos = kInitVec;
		m_colEnemy.m_leftArm[i].m_radius = 0.0f;

		m_colEnemy.m_rightArm[i].m_pos = kInitVec;
		m_colEnemy.m_rightArm[i].m_vertexPos = kInitVec;
		m_colEnemy.m_rightArm[i].m_radius = 0.0f;
	}

	// プレイヤー初期化
	m_colPlayer.m_body.m_pos = kInitVec;
	m_colPlayer.m_body.m_vertexPos = kInitVec;
	m_colPlayer.m_body.m_radius = 0.0f;

	m_colPlayer.m_weapon.m_pos = kInitVec;
	m_colPlayer.m_weapon.m_vertexPos = kInitVec;
	m_colPlayer.m_weapon.m_radius = 0.0f;

	// アイテム初期化
	m_colItem.m_pos = kInitVec;
	m_colItem.m_vertexPos = kInitVec;
	m_colItem.m_radius = 0.0f;
}

void Collision::TypeChangeSphereUpdate(ColType& colType, const VECTOR pos, const float radius)
{
	colType.m_pos = pos;
	colType.m_radius = radius;
}

void Collision::TypeChangeCapsuleUpdate(ColType& colType, const VECTOR pos, const VECTOR upPos, const float radius)
{
	colType.m_pos = pos;
	colType.m_vertexPos = upPos;
	colType.m_radius = radius;
}

void Collision::TypeChangeSphereDraw(const ColType& colType, unsigned int color, bool isFill)
{
	DrawSphere3D(colType.m_pos, colType.m_radius, 32, color, color, isFill);
}

void Collision::TypeChangeCapsuleDraw(const ColType& colType, unsigned int color, bool isFill)
{
	DrawCapsule3D(colType.m_pos, colType.m_vertexPos, colType.m_radius, 32, color, color, isFill);
}

bool Collision::IsTypeChageSphereCollision(const ColType& colType, const Collision& col, const Collision::ColType& m_col)
{
	return HitCheck_Sphere_Sphere(colType.m_pos, colType.m_radius,
		m_col.m_pos, m_col.m_radius);
}

bool Collision::IsTypeChageCupsuleCollision(const ColType& colType, const Collision::ColType& m_col)
{
	return HitCheck_Capsule_Capsule(colType.m_pos, colType.m_vertexPos, colType.m_radius,
		m_col.m_pos, m_col.m_vertexPos, m_col.m_radius);
}

bool Collision::IsTypeChageSphereToCapsuleCollision(const ColType& colType,const Collision::ColType& m_col)
{
	return HitCheck_Sphere_Capsule(m_col.m_pos, m_col.m_radius,
		colType.m_pos, colType.m_vertexPos, colType.m_radius);
}