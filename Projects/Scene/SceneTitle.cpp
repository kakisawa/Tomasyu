#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/Fade.h"

using namespace MyInputInfo;

namespace {
	constexpr int kTitlePosX = 798;	// タイトルロゴ座標X
	constexpr int kTitlePosY = 180;	// タイトルロゴ座標Y

	constexpr int kTitlePosX_Tentative = 250;

	constexpr int kButtonX = 650;	// Press...画像座標X
	constexpr int kButtonY = 880;	// Press...画像座標Y

}

SceneTitle::SceneTitle() :
	m_titleLogoHandle(-1),
	m_buttonHandle(-1),
	m_titleBgHandle(-1),
	m_wantedHandle(-1)
{
	m_isNextSceneFlag = false;
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Init()
{
	// 画像の読み込み
	m_titleLogoHandle = LoadGraph("Data/Image/SceneTitle/討魔衆.png");
	m_buttonHandle = LoadGraph("Data/Image/SceneTitle/AButton.png");
	m_titleBgHandle= LoadGraph("Data/Image/SceneTitle/TitleBg.png");
	m_wantedHandle = LoadGraph("Data/Image/SceneTitle/Wanted.png");

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
	DrawGraph(0, 0, m_titleBgHandle, true);
	// タイトルロゴを描画
	DrawGraph(kTitlePosX, kTitlePosY, m_titleLogoHandle, true);
	
	DrawGraph(55, 200, m_wantedHandle, true);
	

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
	DrawGraph(kButtonX, kButtonY, m_buttonHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// 不透明に戻しておく	


	// フェード処理
	m_pFade->Draw();

#ifdef _DEBUG

#endif // DEBUG
}

void SceneTitle::End()
{
	// 画像の削除
	DeleteGraph(m_titleLogoHandle);
	DeleteGraph(m_buttonHandle);
	DeleteGraph(m_titleBgHandle);
	DeleteGraph(m_wantedHandle);

	m_pSound->ReleaseSound();
}
