#include "ModelBase.h"
#include <iostream>

namespace 
{
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化
	constexpr int kInitInt = 0;						// int値初期化

	constexpr float kAnimBlendMax = 1.0f;			// アニメーションの切り替えにかかる最大フレーム数
}

ModelBase::ModelBase() :
	m_model(-1),
	m_hp(kInitInt),
	m_attack(kInitInt),
	m_attackThePlayer(kInitInt),
	m_angle(kInitFloat),
	m_nextAnimTime(kInitFloat),
	m_pos(kInitVec),
	m_move(kInitVec),
	m_targetDir(kInitVec),
	m_animChangeFrame(kInitInt),
	m_animChangeFrameTotal(kInitInt),
	m_isLoopFinish(false),
	m_deathFlag(false)
{
	m_pSound = std::make_shared<SoundManager>();
}

ModelBase::~ModelBase()
{
}

void ModelBase::Init()
{
	// モデルの位置設定
	MV1SetPosition(m_model, m_pos);

	// 当たり判定の初期化
	m_col.Init();
}

void ModelBase::Update()
{
	// モデルの位置設定
	MV1SetPosition(m_model, m_pos);

	// アニメーション情報の更新
	UpdateAnim(m_animPrev);
	UpdateAnim(m_animNext);

	m_animChangeFrame++;		// アニメーションの切り替えフレーム加算
	
	// アニメーションの切り替えにかける総フレーム
	if (m_animChangeFrame > m_animChangeFrameTotal)
	{
		m_animChangeFrame = std::min(m_animChangeFrame, m_animChangeFrameTotal);
	}

	UpdateAnimBlendRate();
}

void ModelBase::Draw()
{
	MV1DrawModel(m_model);
}

void ModelBase::End()
{
	// モデルの削除
	MV1DeleteModel(m_model);
}

void ModelBase::ChangeAnimation(const int animNo, const float animSpeed,const bool isLoop, const bool isForceChange, const int changeFrame)
{
	// 指定されたアニメーションが既に選択されていた場合、処理を返す
	if (!isForceChange)
	{
		if (m_animNext.animNo == animNo)	return;
	}

	// 前のアニメーションが残っていたら終了させる
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_animPrev.attachNo);
		InitAnim(m_animPrev);
	}
	m_animPrev = m_animNext;		// 前のアニメーションに新しいアニメーションの情報を入れる

	// 新しいアニメーションに新しい情報を入れる
	m_animNext.animNo = animNo;
	m_animNext.animSpeed = animSpeed;
	m_animNext.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_model, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;
	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	// アニメーションのブレンド率を設定
	UpdateAnimBlendRate();
}

void ModelBase::SetAnimation(const int animNo, const float animSpeed,const bool isLoop, const bool isForceChange)
{
	// 指定されたアニメーションが既に選択されていた場合、処理を返す
	if (!isForceChange)
	{
		if (m_animNext.animNo == animNo)return;
	}

	// 以前のアニメーションが残っていれば終了
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_animPrev.attachNo);
		InitAnim(m_animPrev);
	}
	if (m_animNext.attachNo != -1)
	{
		MV1DetachAnim(m_model, m_animNext.attachNo);
		InitAnim(m_animNext);
	}

	// 新しくアニメーションを設定
	m_animNext.animNo = animNo;
	m_animNext.animSpeed = animSpeed;
	m_animNext.attachNo = MV1AttachAnim(m_model, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_model, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

bool ModelBase::IsAnimEnd()
{
	// Loopアニメーションの場合は常にfalseを返す
	if (m_animNext.isLoop)	return false;

	// 現在のアニメーション再生時間を取得する
	m_nextAnimTime= MV1GetAttachAnimTime(m_model, m_animNext.attachNo);

	// 再生時間が総再生時間を超えていた場合trueを返す
	if (m_nextAnimTime >= m_animNext.totalTime)
	{
		return true;
	}

	return false;
}

bool ModelBase::IsLoopAnimEnd()
{
	if (m_isLoopFinish)
	{
		return true;
	}

	return false;
}

void ModelBase::InitAnim(AnimData& anim)
{
	// アニメーション情報の初期化
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.animSpeed = 0.0f;
	anim.time = 0.0f;
	anim.totalTime = 0.0f;
	anim.isLoop = false;

	m_nextAnimTime = 0.0f;
}

void ModelBase::UpdateAnim(const AnimData& anim)
{
	// アニメーションが設定されていない場合は何もしない
	if (anim.animNo == -1)	return;

	m_isLoopFinish = false;

	m_nextAnimTime = MV1GetAttachAnimTime(m_model, anim.attachNo);
	m_nextAnimTime += anim.animSpeed;
	if (m_nextAnimTime > anim.totalTime)
	{
		// ループ再生する場合
		if (anim.isLoop)
		{
			m_nextAnimTime -= anim.totalTime;
			m_isLoopFinish = true;
		}
		else
		{
			m_nextAnimTime = std::min(m_nextAnimTime, anim.totalTime);
		}
	}

	// アニメーションの再生時間を設定
	MV1SetAttachAnimTime(m_model, anim.attachNo, m_nextAnimTime);
}

void ModelBase::UpdateAnimBlendRate()
{
	// アニメーション変化のフレーム数に応じたブレンド率を設定する
	float rate = static_cast<float> (m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	rate = std::min(rate, kAnimBlendMax);

	// アニメーションのブレンド率を設定する
	MV1SetAttachAnimBlendRate(m_model, m_animPrev.attachNo, kAnimBlendMax - rate);
	MV1SetAttachAnimBlendRate(m_model, m_animNext.attachNo, rate);
}