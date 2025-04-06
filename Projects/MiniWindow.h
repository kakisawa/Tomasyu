#pragma once
#include "DxLib.h"

class MiniWindow
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MiniWindow();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MiniWindow();
	
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
	/// 終了
	/// </summary>
	void End();

private:

	int m_width; // ミニウィンドウの幅
	int m_height; // ミニウィンドウの高さ


	VECTOR m_pos;
};

