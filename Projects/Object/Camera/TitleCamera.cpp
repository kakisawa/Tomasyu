#include "TitleCamera.h"
#include <cmath>
#include <algorithm>

namespace {
	constexpr float kCameraNear = 0.1f;			// カメラ手前クリップ距離
	constexpr float kCameraFar = 5000.0f;		// カメラ最奥クリップ距離

	constexpr float kDist = -80.0f;				// カメラからプレイヤーまでの距離

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期価値
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ライトの指向性
}

TitleCamera::TitleCamera():
	m_pos(kInitVec),
	m_targetPos(kInitVec)
{
	// 	カメラの手前クリップ距離と奥クリップ距離を設定する
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void TitleCamera::Init(VECTOR pos)
{
	m_targetPos = pos;
	m_targetPos = VAdd(m_targetPos, VGet(10.0f, 50.0f, 0.0f));
	m_pos = VGet(50.0f, 60.0f, 230.0f);
}

void TitleCamera::Update()
{
	// カメラの情報をライブラリのカメラに反映させる
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);

	// 	標準ライトのタイプをディレクショナルライトにする
	ChangeLightTypeDir(kLightDirection);
}