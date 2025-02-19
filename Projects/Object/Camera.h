#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera() {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラ位置補正
	/// </summary>
	void FixCameraPos();

	/// <summary>
	/// 右スティックを使用したカメラの更新
	/// </summary>
	void RightStickCameraUpdate();

	/// <summary>
	/// 左スティックを使用したカメラの更新
	/// </summary>
	void LeftstickCameraUpdate();

	/// <summary>
	/// ターゲットの座標をセットする
	/// </summary>
	/// <param name="target">ターゲットの座標</param>
	void SetTarget(const VECTOR& target){ m_targetPos = target; }

	/// <summary>
	/// 座標渡し
	/// </summary>
	/// <returns>カメラの座標</returns>
	const VECTOR& GetPosition() const { return m_pos; }

	/// <summary>
	/// 注視点座標渡し
	/// </summary>
	/// <returns>注視点座標</returns>
	const VECTOR& GetTarget()	const { return m_targetPos; }

private:
	float m_angleH;	 // 水平角度
	float m_angleV;	 // 垂直角度

	DINPUT_JOYSTATE input;	// ジョイパットの入力1
	DINPUT_JOYSTATE input2;	// ジョイパットの入力2

	VECTOR m_pos;				// カメラ座標
	VECTOR m_targetPos;			// 注視点座標
	VECTOR m_enemyTargetPos;	// ターゲット座標

	std::shared_ptr<Player> m_pPlayer;		// プレイヤーのポインタ
};