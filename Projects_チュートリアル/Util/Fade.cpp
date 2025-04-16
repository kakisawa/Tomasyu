#include "Fade.h"
#include "../Game.h"
#include "DxLib.h"

namespace {
	constexpr int kFadeMax = 255;	// フェード最大値

	constexpr int kNormalFadeAddDec = 5;	// 通常フェードの増減値
	constexpr int kHarfFadeAddDec = 3;		// 半透明用フェードの増減値
}

Fade::Fade() :
	m_fadeAlpha(kFadeMax),
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
	DrawBoxAA(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく
}

void Fade::FadeIn(bool flag)
{
	m_isFadeIn = flag;

	// フェードイン
	if (m_isFadeIn)
	{
		m_fadeAlpha -= kNormalFadeAddDec;
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
		m_fadeAlpha += kNormalFadeAddDec;
		if (m_fadeAlpha > kFadeMax)
		{
			m_fadeAlpha = kFadeMax;
			m_isNextSceneFlag = true;
			m_isFadeOut = false;
		}
	}
}

void Fade::HarfFade(bool flag)
{
	int harfFade = kFadeMax / 2;	// 半透明用フェード値

	m_isHarfFade = flag;

	// 半透明
	if (m_isHarfFade)
	{
		m_fadeAlpha += kHarfFadeAddDec;
		if (m_fadeAlpha > harfFade)
		{
			m_fadeAlpha = harfFade;
			m_isHarfFade = false;
		}
	}
}