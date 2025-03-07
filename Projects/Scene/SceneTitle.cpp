#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Object/Camera/TitleCamera.h"
#include "../Object/Map.h"
#include "../Util/Fade.h"
#include <cassert>

using namespace MyInputInfo;

namespace {
	const char* kPlayerModelFilePath = "Data/Model/PlayerModel.mv1";	// プレイヤーモデルパス
	const char* kBoardModelFilePath = "Data/Model/BulletinBoard.mv1";	// 掲示板モデルパス

	constexpr int kTitlePosX_Tentative = 250;

	constexpr float kPlayerAngle = 0.0f+ DX_PI_F;	// プレイヤーの角度


	const VECTOR kTitleLogoPos = VGet(130.0f, 152.0f, 0.0f);	// タイトルロゴ座標
	const VECTOR kButtonPos = VGet(650.0f, 880.0f, 0.0f);	// タイトルロゴサイズ
	const VECTOR kPlayerPos = VGet(40.0f, 0.0f, 270.0f);	// プレイヤー座標
	const VECTOR kBoardPos = VGet(0.0f, 0.0f, 300.0f);		// 掲示板座標
	const VECTOR kPlayerSize = VGet(0.5f, 0.5f, 0.5f);	// プレイヤーサイズ
	const VECTOR kBoardSize = VGet(1.0f,1.0f,1.0f);		// 掲示板サイズ
}

SceneTitle::SceneTitle() :
	m_playerPos(kPlayerPos),
	m_boardPos(kBoardPos),
	m_buttonHandle(-1),
	m_playerModel(-1),
	m_boardModel(-1),
	m_titleLogoHandle(-1),
	m_plyerAngle(kPlayerAngle),
	m_boardAngle(kPlayerAngle)
{

	m_isNextSceneFlag = false;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_pMap->Init();
	m_pCamera->Init(m_boardPos);

	// 画像の読み込み
	m_titleLogoHandle = LoadGraph("Data/Image/SceneTitle/討魔衆.png");
	m_buttonHandle = LoadGraph("Data/Image/SceneTitle/AButton.png");

	// モデルの読み込み
	m_playerModel = MV1LoadModel(kPlayerModelFilePath);
	assert(m_playerModel != -1);
	m_boardModel = MV1LoadModel(kBoardModelFilePath);
	assert(m_boardModel != -1);

	// モデルの座標セット
	MV1SetPosition(m_playerModel, m_playerPos);
	MV1SetPosition(m_boardModel, m_boardPos);

	// モデルのサイズセット
	MV1SetScale(m_playerModel, kPlayerSize);
	MV1SetScale(m_boardModel, kBoardSize);

	MV1SetRotationXYZ(m_playerModel, VGet(0.0f, m_plyerAngle, 0.0f));
	MV1SetRotationXYZ(m_boardModel, VGet(0.0f, m_boardAngle, 0.0f));
	
	m_pSound->InitSound();
	m_pSound->LoadBGM(SoundManager::BGM_Type::kTitleBGM);
	m_pSound->LoadSE(SoundManager::SE_Type::kButtonSE);
	m_pSound->PlayBGM(SoundManager::BGM_Type::kTitleBGM, DX_PLAYTYPE_LOOP);
}

std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	// フェード処理
	m_pFade->FadeIn(m_pFade->GatFadeInFlag());
	m_pFade->FadeOut(m_isNextSceneFlag);

	m_pMap->Update();
	m_pCamera->Update();

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
	m_pMap->Draw();

	// モデル描画
	MV1DrawModel(m_playerModel);
	MV1DrawModel(m_boardModel);

	DrawGraphF(kTitleLogoPos.x, kTitleLogoPos.y, m_titleLogoHandle, true);

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
	DrawGraphF(kButtonPos.x, kButtonPos.y, m_buttonHandle, true);
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

	MV1DeleteModel(m_playerModel);
	MV1DeleteModel(m_boardModel);

	m_pMap->End();
	m_pSound->ReleaseSound();
}
