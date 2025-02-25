#include "Camera.h"
#include "../Player.h"
#include <cmath>
#include <algorithm>

namespace {
	// カメラ情報
	constexpr float kCameraNear = 1.0f;			// カメラ手前クリップ距離
	constexpr float kCameraFar = 10000.0f;		// カメラ最奥クリップ距離
	constexpr float kDist = -80.0f;				// カメラからプレイヤーまでの距離
	constexpr float kRightStickAngle = 0.03f;	// カメラを動かす角度
	constexpr float kLeftStickAngle = 0.01f;	// カメラを動かす角度

	constexpr float kInitAngleH = 1.7f;		// カメラの初期平行角度
	constexpr float kInitAngleV = 0.3f;		// カメラの初期垂直角度
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 0.5f;	// 最小の垂直角度
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 1.0f;	// 最大の垂直角度

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);				// ベクトルの初期価値
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ライトの指向性
	const VECTOR kTargetHeightPos = VGet(0.0f, 70.0f, 0.0f);	// カメラの注視点座標(高さ調整用)
}

Camera::Camera(std::shared_ptr<Player> pPlayer) :
	m_pos(kInitVec),
	m_targetPos(kInitVec),
	m_enemyTargetPos(kInitVec),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV),
	m_pPlayer(pPlayer)
{
}

void Camera::Init()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void Camera::Update()
{
	// カメラの角度手動入力/更新
	LeftstickCameraUpdate();
	RightStickCameraUpdate();

	// カメラの注視点を設定
	if (m_pPlayer->GetLockOn())
	{
		// プレイヤーの位置取得
		VECTOR playerPos = m_pPlayer->GetPos();

		// カメラの位置をプレイヤーの後ろに設定
		VECTOR cameraPos = VAdd(playerPos, kTargetHeightPos);

		// カメラの注視点を更新
		SetCameraPositionAndTarget_UpVecY(cameraPos, m_targetPos);
	}
	else
	{
		// 注視点をプレイヤーの座標+高さにする
		m_targetPos = VAdd(m_pPlayer->GetPos(), kTargetHeightPos);

		// カメラ位置補正
		FixCameraPos();

		// カメラの情報をライブラリのカメラに反映させる
		SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	}

	// 	標準ライトのタイプをディレクショナルライトにする
	ChangeLightTypeDir(kLightDirection);

#ifdef _DEBUG
	//DrawFormatString(0, 100, 0xffffff, "Camera:m_pos.x/y/z=%.2f/%.2f/%.2f", m_pos.x, m_pos.y, m_pos.z);
	//DrawFormatString(0, 120, 0xffffff, "Camera:m_targetPos.x/y/z=%.2f/%.2f/%.2f",
	//	m_targetPos.x, m_targetPos.y, m_targetPos.z);
	//DrawFormatString(0, 140, 0xffffff, "Player:m_pos.x/y/z=%.2f/%.2f/%.2f",
	//	m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, m_pPlayer->GetPos().z);
	//DrawFormatString(0, 160, 0xffffff, "m_angleH=%.2f", m_angleH);
#endif // DEBUG
}

void Camera::FixCameraPos()
{
	// 水平方向の回転
	auto rotY = MGetRotY(m_angleH);
	// 垂直方向の回転
	auto rotZ = MGetRotZ(m_angleV);

	// X軸にカメラからプレイヤーまでの距離分伸びたベクトルを垂直方向に回転する(Z軸回転)
	m_pos = VTransform(VGet(-kDist, 0.0f, 0.0f), rotZ);
	// 水平方向(Y軸回転)に回転する
	m_pos = VTransform(m_pos, rotY);

	// 注視点の座標を足す
	m_pos = VAdd(m_pos, m_targetPos);
}

void Camera::RightStickCameraUpdate()
{
	// プレイヤーがロックオンしているときは処理しない
	if (m_pPlayer->GetLockOn())	return;

	//入力状態初期化
	input.Rx = 0;
	input.Ry = 0;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (input.Rx < 0.0f)			// 右スティックを右に倒したら右回転する
	{
		m_angleH -= kRightStickAngle;
	}
	if (input.Rx > 0.0f)			// 右スティックを左に倒したら左回転する
	{
		m_angleH += kRightStickAngle;
	}
	if (input.Ry < 0.0f)			// 右スティックを下に倒したら上方向に回る
	{
		m_angleV -= kRightStickAngle;
		// ある一定角度以上にならないようにする
		m_angleV = (std::max)(m_angleV, kMaxAngleV);
	}
	if (input.Ry > 0.0f)			// 右スティックを上に倒したら下方向に回る
	{
		m_angleV += kRightStickAngle;
		// ある一定角度以下にならないようにする
		m_angleV = (std::min)(kMinAngleV, m_angleV);
	}
}

void Camera::LeftstickCameraUpdate()
{
	// プレイヤーがロックオンしているときは処理しない
	if (m_pPlayer->GetLockOn())return;

	//入力状態初期化
	input2.X = 0;
	input2.Y = 0;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input2);

	if (input2.X < 0.0f)
	{
		m_angleH -= kLeftStickAngle;	// 左スティックを右に倒したら右回転する
	}
	if (input2.X > 0.0f)
	{
		m_angleH += kLeftStickAngle;	// 左スティックを左に倒したら左回転する
	}
}