#pragma once
#include "DxLib.h"

class TitleCamera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleCamera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleCamera(){}

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
	/// カメラの座標をセットする
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const VECTOR& pos) { m_pos = pos; }

private:
	float m_angleH;	 // 水平角度
	float m_angleV;	 // 垂直角度

	VECTOR m_pos;				// カメラ座標
	VECTOR m_targetPos;			// 注視点座標
};