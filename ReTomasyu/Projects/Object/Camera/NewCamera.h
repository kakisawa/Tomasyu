#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy;
class NewCamera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤー情報</param>
	/// <param name="pEnemy">敵情報</param>
	NewCamera(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~NewCamera(){}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 通常カメラ更新
	/// </summary>
	/// <param name="target">注視点座標</param>
	void NormalUpdate(VECTOR target);

	/// <summary>
	/// ロックオン時カメラ更新
	/// </summary>
	/// <param name="target">注視点座標</param>
	void LookOnUpdate(VECTOR target);
	
	/// <summary>
	/// 角度更新
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// 座標の更新
	/// </summary>
	void UpdatePos();

	/// <summary>
	/// 左スティックを使用したカメラの更新
	/// </summary>
	void LeftStickCameraUpdate();

	/// <summary>
	/// ターゲット座標をセットする
	/// </summary>
	/// <param name="target"></param>
	void SetTargetPos(const VECTOR& target) { m_targetPos = target; }

	/// <summary>
	/// 座標渡し
	/// </summary>
	/// <returns>カメラの座標</returns>
	const VECTOR& GetPos() const { return m_pos; }

	/// <summary>
	/// 注視点座標渡し
	/// </summary>
	/// <returns>注視点座標</returns>
	const VECTOR& GetTargetPos()	const { return m_targetPos; }

private:

	float m_angleH;	 // 水平角度
	float m_angleV;	 // 垂直角度
	float m_cameraToTargetLength;	// カメラからターゲットまでの最小距離

	bool m_isLookOn;	// 直前までロックオンをしていたか

	VECTOR m_pos;		// カメラの座標
	VECTOR m_targetPos;	// 注視点座標

	VECTOR m_prevPos;	// 前の座標
	VECTOR m_nextPos;	// 次の座標

	DINPUT_JOYSTATE rightInput;	// 右ジョイパットの入力
	DINPUT_JOYSTATE leftInput;	// 左ジョイパットの入力

	std::shared_ptr<Player> m_pPlayer;	// プレイヤーのポインタ
	std::shared_ptr<Enemy> m_pEnemy;	// 敵のポインタ
};