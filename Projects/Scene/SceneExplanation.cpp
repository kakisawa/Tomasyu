﻿#include "SceneExplanation.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/Input.h"
#include "../Util/Fade.h"

using namespace MyInputInfo;

namespace {

	constexpr int kFadeMax = 255;	// フェード最大値
	constexpr int kFadeAdd = 3;		// 半透明用フェードの増値
	constexpr int kFadeDec = 5;		// 半透明用フェードの減値

	VECTOR kTrianglePos[2]{	// 三角形キー座標	
		VGet(81.0f, 486.0f, 0.0f),
		VGet(1728.0f, 486.0f, 0.0f) 
	};	// 三角形座標

	VECTOR kExplanationGraphPos = VGet(260.0f, 139.0f, 0.0f);	// 画像座標

	const char* const kExplanationUI[2] = {
		"Data/Image/SceneExplanation/SceneExplanation2-3.png",
		"Data/Image/SceneExplanation/SceneExplanation3-3.png",
	};	// 画像パス

	const char* const kTriangleUI[2] = {
		"Data/Image/SceneExplanation/LeftTriangle.png",
		"Data/Image/SceneExplanation/RightTriangle.png",
	};	// 三角形キー画像パス
}

SceneExplanation::SceneExplanation():
	m_explanationHandle(-1),
	m_count(0)
{
}

void SceneExplanation::Init()
{
	// 画像読み込み
	for (int i = 0; i < m_explanationUIHandle.size(); i++)
	{
		m_explanationUIHandle[i] = LoadGraph(kExplanationUI[i]);
	}
	for (int i = 0; i < m_triangleUIHandle.size(); i++)
	{
		m_triangleUIHandle[i] = LoadGraph(kTriangleUI[i]);
	}

	m_explanationHandle = m_explanationUIHandle[m_count];	// 説明画像

	// サウンドの初期化・読み込み
	m_pSound->InitSound();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kSelectBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kSelectSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kSelectBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneExplanation::Update(Input& input)
{
	// フェード処理
	m_pFade->FadeIn(m_pFade->GetFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);

	if (!m_pFade->GetFadeInFlag() && input.IsTrigger(InputInfo::Back))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_isNextSceneFlag = true;
	}

	if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag())	// 次のシーン
	{
		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}

	if (input.IsTrigger(InputInfo::Right)) {			// 右キー

		// SEを鳴らす
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		m_count++;
		if (m_count >= 2)
		{
			m_count = 0;
		}
	}

	if (input.IsTrigger(InputInfo::Left)) {			// 左キー

		// SEを鳴らす
		m_pSound->PlaySE(SoundManager::SE_Type::kSelectSE, DX_PLAYTYPE_BACK);

		m_count--;
		if (m_count < 0)
		{
			m_count = 2;
		}
	}

	m_explanationHandle = m_explanationUIHandle[m_count];	// 説明画像

#ifdef _DEBUG
#endif // DEBUG
	return shared_from_this();
}

void SceneExplanation::Draw()
{
	DrawGraphF(kExplanationGraphPos.x, kExplanationGraphPos.y, m_explanationHandle, true);	// 説明画像

	// 左右三角点滅描画
	static int m_fadeAlpha;
	static bool isFade;

	if (isFade) {
		m_fadeAlpha += kFadeAdd;

		if (m_fadeAlpha >= kFadeMax) {
			isFade = false;
		}
	}
	else {
		m_fadeAlpha -= kFadeDec;
		if (m_fadeAlpha <= 0) {
			isFade = true;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kTrianglePos[i].x, kTrianglePos[i].y, m_triangleUIHandle[i], true);	// 三角形
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく	


	// フェード処理
	m_pFade->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "SceneExplanation", 0xffffff);
#endif // DEBUG
}

void SceneExplanation::End()
{
	// 画像の削除
	DeleteGraph(m_explanationHandle);
	for (int i = 0; i < m_triangleUIHandle.size(); i++) 
	{
		DeleteGraph(m_triangleUIHandle[i]);
	}
	for (int i = 0; i < m_explanationUIHandle.size(); i++)
	{
		DeleteGraph(m_explanationUIHandle[i]);
	}
}
