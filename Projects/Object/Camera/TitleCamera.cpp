#include "TitleCamera.h"
#include <cmath>
#include <algorithm>

namespace {
	constexpr float kCameraNear = 1.0f;			// カメラ手前クリップ距離
	constexpr float kCameraFar = 10000.0f;		// カメラ最奥クリップ距離
	constexpr float kDist = -80.0f;				// カメラからプレイヤーまでの距離
	constexpr float kRightStickAngle = 0.03f;	// カメラを動かす角度
	constexpr float kLeftStickAngle = 0.01f;	// カメラを動かす角度

	constexpr float kInitAngleH = 1.7f;		// カメラの初期平行角度
	constexpr float kInitAngleV = 0.3f;		// カメラの初期垂直角度
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 0.5f;	// 最小の垂直角度
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 1.0f;	// 最大の垂直角度


	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期価値
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ライトの指向性
}

TitleCamera::TitleCamera():
	m_pos(VGet(0.0f,0.0f,0.0f)),
	m_targetPos(VGet(0.0f, 70.0f, 1000.0f)),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV)
{
}

void TitleCamera::Init()
{
	// 	カメラの手前クリップ距離と奥クリップ距離を設定する
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void TitleCamera::Update()
{
	// カメラの情報をライブラリのカメラに反映させる
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);

	// 	標準ライトのタイプをディレクショナルライトにする
	ChangeLightTypeDir(kLightDirection);

	m_pos = VAdd(m_pos, m_targetPos);
}