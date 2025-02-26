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
	/// <param name="pos">注視点座標</param>
	void Init(VECTOR pos);

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

	VECTOR m_pos;				// カメラ座標
	VECTOR m_targetPos;			// 注視点座標
};