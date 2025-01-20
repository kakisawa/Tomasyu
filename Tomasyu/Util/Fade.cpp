#include "Fade.h"
#include "DxLib.h"

Fade::Fade() :
	m_fadeAlpha(255),
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_isHarfFade(false),
	m_isNextSceneFlag(false)
{
}

Fade::~Fade()
{
}

void Fade::Init()
{
}

void Fade::Update()
{
}

void Fade::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawBoxAA(0, 0, 1920, 1080, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Fade::FadeIn(bool flag)
{
	m_isFadeIn = flag;

	// フェードイン
	if (m_isFadeIn)
	{
		m_fadeAlpha -= 5;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_isFadeIn = false;
		}
	}
}

void Fade::FadeOut(bool flag)
{
	m_isFadeOut = flag;

	// フェードアウト
	if (m_isFadeOut)
	{
		m_fadeAlpha += 5;
		if (m_fadeAlpha > 255)
		{
			m_fadeAlpha = 255;
			m_isNextSceneFlag = true;
			m_isFadeOut = false;
		}
	}
}

void Fade::HarfFade(bool flag)
{
	m_isHarfFade = flag;

	if (m_isHarfFade)
	{
		m_fadeAlpha += 3;
		if (m_fadeAlpha > 180)
		{
			m_fadeAlpha = 180;
			m_isHarfFade = false;
		}
	}
}