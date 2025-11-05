#include "NewCamera.h"
#include "../Player/Player.h"
#include "../Enemy.h"
#include <cmath>
#include <algorithm>
#include <memory>

namespace {
	constexpr float kCameraHeight = 65.0f;		// カメラの高さ
	constexpr float kPlayerHeight = 50.0f;		// 通常カメラ注視点のy軸
	constexpr float kTargetHeight = 450.0f;		// ロックオンカメラ注視点のy軸
	constexpr float kCameraNear = 1.0f;			// カメラ手前クリップ距離
	constexpr float kCameraFar = 10000.0f;		// カメラ最奥クリップ距離
	constexpr float kDist = 70.0f;				// カメラからプレイヤーまでの距離
	constexpr float kRightStickAngle = 0.08f;	// 右スティックでカメラを動かす角度
	constexpr float kLeftStickAngle = 0.005f;	// 左スティックでカメラを動かす角度
	constexpr float kInitAngleH = DX_PI_F;		// カメラの初期平行角度
	constexpr float kInitAngleV = 0.3f;			// カメラの初期垂直角度
	constexpr float kMinAngleV = -DX_PI_F * 0.5f + 0.5f;	// 最小の垂直角度
	constexpr float kMaxAngleV = DX_PI_F * 0.5f - 1.0f;		// 最大の垂直角度
	constexpr float kRightStickHorizontalRate = 0.00020f;	// 右スティック水平動作を角度変化に変換する比率
	constexpr float kRightStickVerticalRate = 0.00017f;	// 右スティック垂直動作を角度変化に変換する比率
	constexpr float kCameraSensitivity = 3.0f;	// カメラ感度

	constexpr float kCameraFollowSpeed = 0.2f;	// カメラが付いてくる速度
	constexpr float kPrevCameraFollowSpeed = 1.0f - kCameraFollowSpeed;	// カメラの初速度
	constexpr float kCameraTargetFollowSpeed = 0.2f;	// カメラが注視点を追いかける速度
	constexpr float kPrevCameraTargetFollowSpeed = 1.0f - kCameraTargetFollowSpeed;	// カメラが注視点を追いかける初速度

	/*ロックオン時*/
	constexpr float kLockOnAngleH = -DX_PI_F * 0.5f + 0.1f;										// ロックオン時にカメラを少し傾けるための値
	constexpr float kLockOnAngleV = -DX_PI_F / 2.0f + 1.3f;										// ロックオン時にカメラを少し傾けるための値
	constexpr float kCameraTargetLockOnFollowSpeed = 0.5f;										// ロックオン時にカメラが付いてくる速度
	constexpr float kPrevCameraTargetLockOnFollowSpeed = 1.0f - kCameraTargetLockOnFollowSpeed;	// ロックオン時にカメラが付いてくる初速度

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトルの初期化値
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ライトの指向性
}

NewCamera::NewCamera(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy) :
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV),
	m_cameraToTargetLength(0.0f),
	m_cameraSensitivity(kCameraSensitivity),
	m_isLookOn(false),
	m_pos(kInitVec),
	m_targetPos(kInitVec),
	m_prevPos(kInitVec),
	m_nextPos(kInitVec),
	m_pPlayer(pPlayer),
	m_pEnemy(pEnemy)
{
}

void NewCamera::Init()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void NewCamera::Update()
{
	// 前の座標を保存する
	m_prevPos = m_pos;

	// 注視点座標を設定する(高さは別途足す)
	VECTOR viewPointPos = m_pPlayer->GetPos();
	viewPointPos.y += kCameraHeight;

	// 角度更新
	LeftStickCameraUpdate();
	UpdateAngle();

	if (m_pPlayer->GetLockOn()) 
	{
		// カメラロックオン時の更新
		LookOnUpdate(m_pEnemy->GetPos(), viewPointPos);
	}
	else 
	{
		// 通常時更新
		AngleToPlayerMove();
		NormalUpdate(viewPointPos);
	}

	// 座標の確定
	UpdatePos(viewPointPos);

	// カメラの情報を反映させる
	m_pos = VAdd(VScale(m_prevPos, kPrevCameraFollowSpeed),
		VScale(m_nextPos, kCameraFollowSpeed));

	// 座標設定
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	// 標準ライトのタイプをディレクショナルライトにする
	ChangeLightTypeDir(kLightDirection);
}

void NewCamera::Draw()
{
	DrawFormatString(0, 500, 0xffffff, "Camera:m_pos.x/y/z=%.2f/%.2f/%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 520, 0xffffff, "Camera:m_targetPos.x/y/z=%.2f/%.2f/%.2f",
		m_targetPos.x, m_targetPos.y, m_targetPos.z);
	DrawFormatString(0, 600, 0xffffff, "m_angleH=%.2f", m_angleH);
	DrawFormatString(0, 620, 0xffffff, "m_angleV=%.2f", m_angleV);

	DrawSphere3D(m_targetPos, 3.0f, 32, 0xff00ff, 0xff00ff, true);
}

void NewCamera::NormalUpdate(VECTOR target)
{
	// ターゲット座標更新
	m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (target.x * kCameraTargetFollowSpeed);
	m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (target.y * kCameraTargetFollowSpeed);
	m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (target.z * kCameraTargetFollowSpeed);
}

void NewCamera::LookOnUpdate(VECTOR target, VECTOR playerPos)
{
	// 注視点の高さ調整
	target.y += kTargetHeight;
	playerPos.y += kPlayerHeight;

	// ターゲット座標とプレイヤー座標の中間点を出す
	VECTOR midPos = VScale(VAdd(playerPos, target), 0.5f);

	// ターゲット座標更新
	m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (midPos.x * kCameraTargetFollowSpeed);
	m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (midPos.y * kCameraTargetFollowSpeed);
	m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (midPos.z * kCameraTargetFollowSpeed);
}

void NewCamera::UpdateAngle()
{
	// ロックオン時は何もしない
	if (m_pPlayer->GetLockOn()) return;

	// 入力状態初期化
	rightInput.X = 0;
	rightInput.Y = 0;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &rightInput);

	// 回転を1～1000から0.001～1の範囲にする
	float rotX = kRightStickHorizontalRate * rightInput.Rx * m_cameraSensitivity;
	float rotY =  kRightStickVerticalRate * rightInput.Ry * m_cameraSensitivity;

	// カメラの回転スピードをかける
	rotX *= kRightStickAngle;
	rotY *= kRightStickAngle;

	m_angleH += rotX;
	if (rightInput.Rx < 0.0f)
	{
		// -180°以下になったら角度地が大きくなりすぎないように360°を足す
		if (m_angleH < -DX_PI_F)
		{
			m_angleH += DX_TWO_PI_F;
		}
	}
	if (rightInput.Rx > 0.0f)
	{
		// 180°以上になったら角度地が大きくなりすぎないように360°を引く
		if (m_angleH > DX_PI_F)
		{
			m_angleH -= DX_TWO_PI_F;
		}
	}

	m_angleV -= rotY;
	// 一定角度以下ににはならないようにする
	if (m_angleV < kMinAngleV)
	{
		m_angleV = kMinAngleV;
	}
	// 一定角度以上にはならないようにする
	if (m_angleV > kMaxAngleV)
	{
		m_angleV = kMaxAngleV;
	}
}

void NewCamera::AngleToPlayerMove()
{
	// プレイヤーの移動方向ベクトルを取得
	VECTOR moveVec = m_pPlayer->GetMove();

	// 移動がない場合はスキップ
	if (VSize(moveVec) < 0.05f) return;

	// プレイヤーの移動方向から水平角度を算出
	float moveAngle = atan2f(moveVec.x, moveVec.z);

	// 現在のカメラ角度との差を求める
	float diff = moveAngle - m_angleH;

	// 角度差を -π～π の範囲に正規化
	if (diff > DX_PI_F) diff -= DX_TWO_PI_F;
	if (diff < -DX_PI_F) diff += DX_TWO_PI_F;

	if (moveVec.z < 0.0f) return;

	// 真横以上の移動時は補正を緩和
	if (fabs(diff) > DX_PI_F * 0.45f) return;

	// プレイヤー移動方向にカメラを少し追従させる（遅延追従）
	constexpr float kFollowRate = 0.08f; // 小さいほど滑らか
	m_angleH += diff * kFollowRate;
}

void NewCamera::UpdatePos(VECTOR pos)
{
	constexpr float kCameraYOffset = 50.0f;	// カメラを「プレイヤーの頭上」にセットするための高さ

	// 垂直方向の回転(X軸)
	MATRIX RotX = MGetRotX(m_angleV);
	// 水平方向の回転(Y軸)
	MATRIX RotY = MGetRotY(m_angleH);

	/*カメラの座標の計算
	(X軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
	垂直方向回転させた後に水平方向回転して
	それに注視点の座標を足す)*/
	m_cameraToTargetLength = std::min(m_cameraToTargetLength + 5.0f, kDist);

	m_nextPos = VAdd(VTransform(VTransform(VGet(0.0f, kCameraYOffset, m_cameraToTargetLength), RotX), RotY), pos);
}

void NewCamera::LeftStickCameraUpdate()
{
	// 入力状態初期化
	leftInput.X = 0;
	leftInput.Y = 0;

	GetJoypadDirectInputState(DX_INPUT_PAD1, &leftInput);

	if (leftInput.X < 0.0f)
	{
		m_angleH -= kLeftStickAngle;	// 左スティックを右に倒したら右回転する
	}
	if (leftInput.X > 0.0f)
	{
		m_angleH += kLeftStickAngle;	// 左スティックを左に倒したら左回転する
	}
}