#include "MiniWindow.h"
#include "../Game.h"
#include <algorithm> // 追加
#include "DxLib.h"


MiniWindow::MiniWindow():
	m_handle(-1),
	m_width(0),
	m_height(0),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_movePos(VGet(0.0f, 0.0f, 0.0f))
{
}

MiniWindow::~MiniWindow()
{
}

void MiniWindow::Init(int handle)
{
	// 読み込んだ画像の挿入
	m_handle = handle;

	// 画像の幅と高さを取得
	GetGraphSize(m_handle, &m_width, &m_height);

	m_pos.x = (Game::kScreenWidth - m_width) / 2;
	m_pos.y = (Game::kScreenHeight - m_height) / 2;

	m_movePos.x = Game::kScreenWidth / 2;
}

void MiniWindow::Update()
{
}

void MiniWindow::Draw()
{
	DrawExtendGraph(
		m_movePos.x,m_pos.y,
		(Game::kScreenWidth- m_movePos.x), m_pos.y + m_height,
		m_handle,true
	);
}

void MiniWindow::End()
{
}

void MiniWindow::CallMiiniWindow()
{
	m_movePos.x-=20;

	m_movePos.x = std::max(m_movePos.x, m_pos.x);
}

void MiniWindow::CloseMiniWindow()
{
	m_movePos.x = Game::kScreenWidth / 2.0f;
}
