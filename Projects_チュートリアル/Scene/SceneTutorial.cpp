#include "SceneTutorial.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include"../Object/Player/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Map.h"
#include "../UI/UISceneGame.h"
#include "../Manager/Effect.h"
#include "../Util/Fade.h"
#include "../Util/Score.h"
#include "../Util/MiniWindow.h"
#include "DxLib.h"
#include <ctime>
#include <cassert>

using namespace MyInputInfo;

namespace {
	const char* const kTutorialHandlePath[3] = {	// チュートリアル画像パス
		"Data/Image/SceneTutorial/Tutorial1.png" ,
		"Data/Image/SceneTutorial/Tutorial2.png" ,
		"Data/Image/SceneTutorial/Tutorial3.png" ,
	};

	// チュートリアルごとの座標リスト
	const std::vector<std::vector<VECTOR>> kTutorialCheckPos = {
		{ // チュートリアル1の座標
			VGet(21.0f, 451.0f, 0.0f),
			VGet(21.0f, 565.0f, 0.0f),
			VGet(21.0f, 663.0f, 0.0f),
			VGet(0.0f, -100.0f, 0.0f)
		},
		{ // チュートリアル2の座標
			VGet(14.0f, 422.0f, 0.0f),
			VGet(14.0f, 509.0f, 0.0f),
			VGet(14.0f, 591.0f, 0.0f),
			VGet(14.0f, 699.0f, 0.0f)
		},
		{ // チュートリアル3の座標
			VGet(19.0f, 430.0f, 0.0f),
			VGet(19.0f, 556.0f, 0.0f),
			VGet(19.0f, 638.0f, 0.0f),
			VGet(19.0f, 716.0f, 0.0f)
		}
	};

	const VECTOR kTutorialPos = VGet(0.0f, 354.5f, 0.0f);
}

SceneTutorial::SceneTutorial() :
	m_pauseHandle(-1),
	m_tutorialDisplay(-1),
	m_tutorialCount(0),
	m_tutorialNum(TutorialStep::Tutorial1),
	m_isPause(false),
	m_nextScene(nextScene::None)
{
	m_pEnemy = std::make_shared<Enemy>(m_pMap, nullptr);
	m_pPlayer = std::make_shared<Player>(nullptr, nullptr, m_pItem);

	// PlayerとEnemyの相手ポインタを設定する
	m_pPlayer->SetEnemyPointer(m_pEnemy);
	m_pEnemy->SetPlayer(m_pPlayer);

	m_pCamera = std::make_shared<Camera>(m_pPlayer);
	m_pUI = std::make_shared<UISceneGame>(m_pPlayer, m_pEnemy, m_pScore);

	m_pPlayer->SetCameraPointer(m_pCamera);
	m_pItem = std::make_shared<Item>(m_pPlayer, 3);
}

SceneTutorial::~SceneTutorial()
{
}

void SceneTutorial::Init()
{
	m_pItem->Init();
	m_pCamera->Init();
	m_pPlayer->Init(m_pScore);
	m_pEnemy->Init();
	m_pMap->Init();
	m_pUI->Init();
	m_pSound->InitBGM();
	m_pScore->InitTutorial();

	// サウンド関係


	// エフェクトの読み込み
	Effect::GetInstance().Load();

	// 画像の読み込み
	m_pauseHandle = LoadGraph("Data/Image/SceneTutorial/TutorialPause.png");
	assert(m_pauseHandle != -1);

	// チュートリアル用チェックマーク画像の読み込み
	m_checkHandle.resize(4);
	for (int i = 0; i < m_checkHandle.size(); i++) {
		m_checkHandle[i] = LoadGraph("Data/Image/SceneTutorial/CheckMark.png");
		assert(m_checkHandle[i] != -1);
	}


	m_tutorialStep.resize(3);

	for (int i = 0; i < m_tutorialStep.size(); i++)
	{
		m_tutorialStep[i].m_step = i;
		m_tutorialStep[i].m_handle = LoadGraph(kTutorialHandlePath[i]);
		assert(m_tutorialStep[i].m_handle != -1);

		m_tutorialStep[i].m_checkPos = kTutorialCheckPos[i];
	}

	m_pMiniWindow->Init(m_pauseHandle);
	m_tutorialDisplay = m_tutorialStep[0].m_handle;
}

std::shared_ptr<SceneBase> SceneTutorial::Update(Input& input)
{
	// フェード処理
	m_pFade->FadeIn(m_pFade->GetFadeInFlag());

	// スタートボタンを押したらポーズ状態にする
	if (input.IsTrigger(InputInfo::DebugStart)) {
		if (m_isPause)
		{
			m_isPause = false;
			m_pMiniWindow->CloseMiniWindow();
			m_pFade->HarfFade(false);
			m_pFade->FadeIn(true);
			m_tutorial.isTOpenPause = true;	// チュートリアルのポーズフラグをtrueにする
		}
		else
		{
			m_isPause = true;
		}
	}

	//ポーズ状態
	if (m_isPause)
	{
		// 半透明にし、ミニウィンドウを表示する
		if (!m_isNextSceneFlag) {
			m_pFade->HarfFade(true);
		}
		m_pMiniWindow->CallMiiniWindow();
		m_pFade->FadeOut(m_isNextSceneFlag);

		if (input.IsTrigger(InputInfo::OK))
		{
			m_nextScene = nextScene::GameScene;
			m_isNextSceneFlag = true;
		}
		if (input.IsTrigger(InputInfo::Back))
		{
			m_nextScene = nextScene::SelectScene;
			m_isNextSceneFlag = true;
		}

		if (m_isNextSceneFlag && m_pFade->GatNextSceneFlag())	// 次のシーン
		{
			if (m_nextScene == nextScene::GameScene)
			{
				return std::make_shared<SceneGame>();	// ゲームシーンへ行く
			}
			else if (m_nextScene == nextScene::SelectScene)
			{
				return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
			}
		}

		
	}
	else
	{
		m_pFade->FadeIn(true);
		m_pMap->Update();
		m_pItem->Update();
		m_pPlayer->Update(input);
		m_pEnemy->UpdateTutorial();
		m_pEnemy->TutorialHp();
		m_pCamera->Update();
		m_pUI->Update();
		// エフェクトの更新
		Effect::GetInstance().Update();

		CheckTutorial();

		ChangeTutorialDisplay(input);
	}

	return shared_from_this();
}

void SceneTutorial::Draw()
{
	m_pMap->Draw();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pScore->Draw();
	Effect::GetInstance().Draw();	// エフェクト表示
	m_pUI->Draw();

	DrawGraphF(kTutorialPos.x, kTutorialPos.y, m_tutorialDisplay, true);

	// m_tutorialStep の各チェックマークを描画
	if (m_tutorialNum == TutorialStep::Tutorial1)
	{
		if (m_tutorial.isTMove) {
			DrawGraphF(m_tutorialStep[0].m_checkPos[0].x, m_tutorialStep[0].m_checkPos[0].y, m_checkHandle[0], true);
		}
		if (m_tutorial.isTRoll) {
			DrawGraphF(m_tutorialStep[0].m_checkPos[1].x, m_tutorialStep[0].m_checkPos[1].y, m_checkHandle[1], true);
		}
		if (m_tutorial.isTGetItem) {
			DrawGraphF(m_tutorialStep[0].m_checkPos[2].x, m_tutorialStep[0].m_checkPos[2].y, m_checkHandle[2], true);
		}
	}
	else if (m_tutorialNum == TutorialStep::Tutorial2)
	{
		if (m_tutorial.isTChangeWeapon) {
			DrawGraphF(m_tutorialStep[1].m_checkPos[0].x, m_tutorialStep[1].m_checkPos[0].y, m_checkHandle[0], true);
		}
		if (m_tutorial.isTRockOn) {
			DrawGraphF(m_tutorialStep[1].m_checkPos[1].x, m_tutorialStep[1].m_checkPos[1].y, m_checkHandle[1], true);
		}
		if (m_tutorial.isTAttackHandGun) {
			DrawGraphF(m_tutorialStep[1].m_checkPos[2].x, m_tutorialStep[1].m_checkPos[2].y, m_checkHandle[2], true);
		}
		if (m_tutorial.isTAttackMachineGun) {
			DrawGraphF(m_tutorialStep[1].m_checkPos[3].x, m_tutorialStep[1].m_checkPos[3].y, m_checkHandle[3], true);
		}
	}
	else if (m_tutorialNum == TutorialStep::Tutorial3)
	{
		if (m_tutorial.isTAttackKnife) {
			DrawGraphF(m_tutorialStep[2].m_checkPos[0].x, m_tutorialStep[2].m_checkPos[0].y, m_checkHandle[0], true);
		}
		if (m_tutorial.isTChangeItem) {
			DrawGraphF(m_tutorialStep[2].m_checkPos[1].x, m_tutorialStep[2].m_checkPos[1].y, m_checkHandle[1], true);
		}
		if (m_tutorial.isTUseItem) {
			DrawGraphF(m_tutorialStep[2].m_checkPos[2].x, m_tutorialStep[2].m_checkPos[2].y, m_checkHandle[2], true);
		}
		if (m_tutorial.isTOpenPause) {
			DrawGraphF(m_tutorialStep[2].m_checkPos[3].x, m_tutorialStep[2].m_checkPos[3].y, m_checkHandle[3], true);
		}
	}

	m_pFade->Draw();

	// ポーズ中の描画
	if (m_isPause)
	{
		m_pMiniWindow->Draw();
		if (m_isNextSceneFlag) {
			m_pFade->Draw();
		}
	}
}

void SceneTutorial::End()
{
	m_pUI->End();
	m_pMap->End();
	m_pPlayer->End();
	m_pScore->End();
	m_pEnemy->End();
	m_pSound->ReleaseSound();

	// 画像の削除
	DeleteGraph(m_pauseHandle);
	for (int i = 0; i < m_checkHandle.size(); i++) {
		DeleteGraph(m_checkHandle[i]);
	}

	for (int i = 0; i < m_tutorialStep.size(); i++)
	{
		DeleteGraph(m_tutorialStep[i].m_handle);
	}
}

void SceneTutorial::InitTutorial()
{
}

void SceneTutorial::ChangeTutorialDisplay(Input& input)
{
	if (input.IsTrigger(InputInfo::DebugBack))
	{
		m_tutorialCount++;
		if (m_tutorialCount >= m_tutorialStep.size())
		{
			m_tutorialCount = 0;
		}

		m_tutorialNum = static_cast<TutorialStep>(m_tutorialCount);


		if (m_tutorialNum == TutorialStep::Tutorial1)
		{
			m_tutorialDisplay = m_tutorialStep[0].m_handle;
		}
		else if (m_tutorialNum == TutorialStep::Tutorial2)
		{
			m_tutorialDisplay = m_tutorialStep[1].m_handle;
		}
		else if (m_tutorialNum == TutorialStep::Tutorial3)
		{
			m_tutorialDisplay = m_tutorialStep[2].m_handle;
		}
	}
}

void SceneTutorial::CheckTutorial()
{
	// プレイヤーのチュートリアル情報を取得
	const auto& playerTutorial = m_pPlayer->GetTutorial();

	if (m_tutorialNum == TutorialStep::Tutorial1)
	{
		// 各フラグがすでに true の場合は変更しない
		if (!m_tutorial.isTMove) {
			m_tutorial.isTMove = playerTutorial.isTMove; // 移動
		}
		if (!m_tutorial.isTRoll) {
			m_tutorial.isTRoll = playerTutorial.isTRoll; // 回避
		}
		if (!m_tutorial.isTGetItem) {
			m_tutorial.isTGetItem = playerTutorial.isTGetItem; // アイテム獲得
		}
	}
	else if (m_tutorialNum == TutorialStep::Tutorial2)
	{
		if (!m_tutorial.isTChangeWeapon) {
			m_tutorial.isTChangeWeapon = playerTutorial.isTChangeWeapon; // 武器切り替え
		}
		if (!m_tutorial.isTRockOn) {
			m_tutorial.isTRockOn = playerTutorial.isTRockOn; // ロックオン
		}
		if (!m_tutorial.isTAttackHandGun) {
			m_tutorial.isTAttackHandGun = playerTutorial.isTAttackHandGun; // 銃攻撃(ハンドガン)
		}
		if (!m_tutorial.isTAttackMachineGun) {
			m_tutorial.isTAttackMachineGun = playerTutorial.isTAttackMachineGun; // 銃攻撃(マシンガン)
		}
	}
	else if (m_tutorialNum == TutorialStep::Tutorial3)
	{
		if (!m_tutorial.isTAttackKnife) {
			m_tutorial.isTAttackKnife = playerTutorial.isTAttackKnife; // ナイフ攻撃
		}
		if (!m_tutorial.isTChangeItem) {
			m_tutorial.isTChangeItem = playerTutorial.isTChangeItem; // アイテム切り替え
		}
		if (!m_tutorial.isTUseItem) {
			m_tutorial.isTUseItem = playerTutorial.isTUseItem; // アイテム使用
		}	// アイテム使用
	}
}
