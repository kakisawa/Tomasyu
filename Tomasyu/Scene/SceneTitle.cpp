#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/Fade.h"

using namespace MyInputInfo;

namespace {
	constexpr int kTitlePosX = 380;	// タイトルロゴ座標X
	constexpr int kTitlePosY = 140;	// タイトルロゴ座標Y

	constexpr int kTitlePosX_Tentative = 250;

	constexpr int kButtonY = 880;	// Press...画像座標Y
}

SceneTitle::SceneTitle() :
	m_titleGraph(-1),
	m_buttonGraph(-1)
{
	m_isNextSceneFlag = false;
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Init()
{
	// 画像の読み込み
	m_titleGraph = LoadGraph("Data/Image/SceneTitle/Title.png");
	m_buttonGraph = LoadGraph("Data/Image/SceneTitle/PressAnyButton.png");

	m_pSound->InitBGM();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	// フェード処理
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);


	// Aボタンを押して、フェードインが終了したらゲームセレクトシーンへ行く
	if (!m_pFade->GatFadeInFlag() && input.IsTrigger(InputInfo::OK))
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kButtonSE, DX_PLAYTYPE_BACK);
		m_isNextSceneFlag = true;
	}
	if (m_pFade->GatNextSceneFlag()) {
		return std::make_shared<SceneSelect>();
	}



#ifdef _DEBUG
	//// Debug_STARTボタンを押すとデバッグシーンへ行く
	//if (input.IsTrigger(InputInfo::DebugStart)) {

	//	return std::make_shared<SceneDebug>();
	//}

#else
#endif // DEBUG

	return shared_from_this();
}

void SceneTitle::Draw()
{
	// タイトルロゴを描画
	//DrawGraph(kTitlePosX, kTitlePosY, m_titleGraph, true);
	DrawGraph(kTitlePosX_Tentative, kTitlePosY, m_titleGraph, true);


	// PressAnyButton画像を点滅しながら描画させる
	static int m_fadeAlpha;
	static bool isFade;

	if (isFade) {
		m_fadeAlpha += 3;

		if (m_fadeAlpha >= 255) {
			isFade = false;
		}
	}
	else {
		m_fadeAlpha -= 5;
		if (m_fadeAlpha <= 0) {
			isFade = true;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);	// 半透明で表示開始
	DrawGraph(0, kButtonY, m_buttonGraph, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく	


	// フェード処理
	m_pFade->Draw();

#ifdef _DEBUG

#endif // DEBUG
}

void SceneTitle::End()
{
	// 画像の削除
	DeleteGraph(m_titleGraph);
	DeleteGraph(m_buttonGraph);

	m_pSound->ReleaseSound();
}
