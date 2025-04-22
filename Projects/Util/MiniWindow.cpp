#include "MiniWindow.h"
#include "../Game.h"
#include "DxLib.h"
#include <algorithm>

namespace {
	constexpr int kAddMiniWindow = 20;	// ウィンドウの増減値
	constexpr float kWidthCenter = Game::kScreenWidth / 2.0f;	// 横幅の中心点

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// ベクトル値の初期化
}

MiniWindow::MiniWindow():
	m_handle(-1),
	m_width(0),
	m_height(0),
	m_pos(kInitVec),
	m_movePos(kInitVec)
{
}

void MiniWindow::Init(int handle)
{
	// 読み込んだ画像の挿入
	m_handle = handle;

	// 画像の幅と高さを取得
	GetGraphSizeF(m_handle, &m_width, &m_height);

	m_pos.x = static_cast<float>((Game::kScreenWidth - m_width) / 2);
	m_pos.y = static_cast<float>((Game::kScreenHeight - m_height) / 2);

	m_movePos.x = kWidthCenter;	// 小ウィンドウの座標を初期位置にセットする
}

void MiniWindow::Draw()
{
	DrawExtendGraphF(m_movePos.x, m_pos.y,  
		Game::kScreenWidth - m_movePos.x,m_pos.y + m_height, m_handle, true);
}

void MiniWindow::CallMiiniWindow()
{
	// 小ウィンドウの幅を広げる
	m_movePos.x -= kAddMiniWindow;
	// 小ウィンドウの幅が一定まで広がるとそれ以上広がらないようにする
	m_movePos.x = std::max(m_movePos.x, m_pos.x);
}

void MiniWindow::CloseMiniWindow()
{
	m_movePos.x = kWidthCenter;	// 小ウィンドウの座標を初期位置にセットする
}
