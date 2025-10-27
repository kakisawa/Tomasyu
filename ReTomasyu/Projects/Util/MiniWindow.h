#pragma once
#include "DxLib.h"

class MiniWindow
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MiniWindow();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MiniWindow(){}
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">画像</param>
	void Init(int handle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(){}

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void End(){}

	/// <summary>
	/// ミニウィンドウの呼び出し
	/// </summary>
	void CallMiiniWindow();

	/// <summary>
	/// ミニウィンドウを閉じる
	/// </summary>
	void CloseMiniWindow();

private:
	int m_handle;	// ミニウィンドウのハンドル
	float m_width;	// ミニウィンドウの幅
	float m_height;	// ミニウィンドウの高さ

	VECTOR m_pos;	// ミニウィンドウの座標
	VECTOR m_movePos;	// ミニウィンドウの移動座標
};