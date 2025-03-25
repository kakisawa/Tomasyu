#include "TitlePlayer.h"
#include <cassert>

namespace {
	const char* kPlayerModelFilePath = "Data/Model/PlayerModel.mv1";	// プレイヤーモデルパス
	
	const VECTOR kPlayerPos = VGet(40.0f, 0.0f, 270.0f);// プレイヤー座標
	const VECTOR kPlayerSize = VGet(0.5f, 0.5f, 0.5f);	// プレイヤーサイズ
	constexpr float kPlayerAngle = 0.0f + DX_PI_F;	// プレイヤーの角度
}

TitlePlayer::TitlePlayer()
{
	m_pos = kPlayerPos;
	m_angle = kPlayerAngle;
	m_size = kPlayerSize;

	// プレイヤーモデルの読み込み
	m_model = MV1LoadModel(kPlayerModelFilePath);
	assert(m_model != -1);
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Init()
{
	ModelBase::Init();

	// 初期アニメーションの設定(待機状態)
	SetAnimation(static_cast<int>(PlayerAnim::Idle), 0.5f, true, false);
}

void TitlePlayer::Update()
{
	// 角度設定
	MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle, 0.0f));

	// アニメーションの更新
	ModelBase::Update();
}

void TitlePlayer::Draw()
{
	ModelBase::Draw();
}

void TitlePlayer::End()
{
	ModelBase::End();
}
