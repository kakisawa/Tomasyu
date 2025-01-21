#include "SceneGame.h"
#include "SceneDebug.h"
#include "SceneSelect.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera.h"
#include "../Object/Map.h"
#include "../UI/UISceneGame.h"
#include "../Manager/Effect.h"
#include "../Time/Time.h"
#include "../Util/Fade.h"
#include "../Score.h"
#include "DxLib.h"

using namespace MyInputInfo;

namespace
{
	const VECTOR  kLogoPos = { 564.0f, 134.0f ,0.0f};	// ゲームクリア・オーバー時ロゴ画像座標

	const VECTOR kScorePos = { 560.0f, 730.0f, 0.0f };	// スコア画像座標

	const VECTOR kDefeatedTimePos = { 493.0f, 850.0f, 0.0f };	// ゲームクリア時間画像座標

	const  VECTOR kPressAPos = { 795.0f, 1005.0f, 0.0f };	// 「Aボタンで進む」画像座標

	const VECTOR kLogoBgPos = { 0.0f, 0.0f, 0.0f };	// ゲームクリア・オーバー背景画像座標
	const VECTOR kScoreBgPos = { 0.0f, 642.0f, 0.0f };	// スコア背景画像座標

	int count = 0;		// ゲームクリア演出時のシーン遷移用カウント
	// 3回Aボタンがクリックされたらセレクトシーンへ遷移する
	int second = 100;	// ゲームクリア演出時のロゴ画像を映すまでの待機時間

	float pos = 1.7f;	// ゲームクリアロゴ画像拡大表示演出用
}

SceneGame::SceneGame() :
	m_gameClearHandle(-1),
	m_gameOverHandle(-1),
	m_pressAHandle(-1),
	m_defeatedTimeHandle(-1),
	m_scoreHandle(-1),
	m_logoBgHandle(-1),
	m_scoreBgHandle(-1),
	m_isPause(false),
	m_isPlayBGM(true)
{
	m_pPlayer = std::make_shared<Player>(m_pEnemy);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pItem->Init();
	m_pCamera->Init();
	m_pPlayer->Init(m_pScore);
	m_pEnemy->Init();
	m_pMap->Init();
	m_pUI->Init(*m_pPlayer, *m_pEnemy);
	m_pTime->Init();
	m_pSound->InitBGM();
	m_pScore->Init(m_pTime);

	// BGMのロード
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameBGM);
	m_pSound->LoadBGM(SoundManager::BGM_Type::kGameClearBGM);
	// 通常バトル時のBGMを流す
	m_pSound->PlayBGM(SoundManager::BGM_Type::kGameBGM, DX_PLAYTYPE_LOOP);

	// エフェクトの読み込み
	Effect::GetInstance().Load();

	// 画像の読み込み
	m_gameClearHandle = LoadGraph("Data/Image/SceneGame/Clear/GameClear2.png");
	m_pressAHandle = LoadGraph("Data/Image/SceneGame/Clear/Aボタンで進む.png");
	m_defeatedTimeHandle = LoadGraph("Data/Image/SceneGame/Clear/DefeatedTime.png");
	m_scoreHandle = LoadGraph("Data/Image/SceneGame/Clear/Score.png");
	m_gameOverHandle = LoadGraph("Data/Image/SceneGame/Over/GameOver.png");
	m_logoBgHandle = LoadGraph("Data/Image/SceneGame/Clear/GameClearBg.png");
	m_scoreBgHandle = LoadGraph("Data/Image/SceneGame/Clear/ScoreBg.png");

}

std::shared_ptr<SceneBase> SceneGame::Update(Input& input)
{
	// スタートボタンを押したらポーズ状態にする
	if (input.IsTrigger(InputInfo::DebugStart)) {
		if (m_isPause) {
			m_isPause = false;
		}
		else {
			m_isPause = true;
		}
	}

		// エネミーが死んでいなければ
	if (!m_pEnemy->GetDeathFlag())
	{
		if (!m_pPlayer->GetDeathFlag()||m_pTime->GetTimeUp()) {
			m_pFade->FadeIn(true);
			m_pMap->Update();
			m_pPlayer->Update(*m_pEnemy, *m_pItem, *m_pCamera, input);
			m_pEnemy->Update(*m_pMap, *m_pPlayer);
			m_pCamera->Update(*m_pPlayer);
			m_pItem->Update();
			m_pUI->Update(*m_pPlayer, *m_pEnemy);
			m_pTime->Update();

			// エフェクトの更新
			Effect::GetInstance().Update();
		}
	}

	// プレイヤーが死亡したら・制限時間を過ぎてしまったら
	if (m_pPlayer->GetDeathFlag() || m_pTime->GetTimeUp()) {
		// フェード演出を行う
		m_pFade->HarfFade(true);
		// フェード演出が終わったら、BGMを変更する
		if (!m_pFade->GetHarfFadeFlag()) {
			if (m_isPlayBGM) {
			m_pSound->StopBGM(SoundManager::BGM_Type::kGameBGM);
			m_isPlayBGM = false;
			}

			if (input.IsTrigger(InputInfo::OK))
			{
				return std::make_shared<SceneSelect>();	// ゲームオーバーシーンへ行く
			}
		}
	}

	// エネミーを倒したら
	if (m_pEnemy->GetDeathFlag())
	{
		// フェード演出を行う
		m_pFade->HarfFade(true);
		// フェード演出が終わったら、BGMを変更する
		if (!m_pFade->GetHarfFadeFlag()) {
			m_pSound->StopBGM(SoundManager::BGM_Type::kGameBGM);
			if (m_isPlayBGM) {
				m_pSound->PlayBGM(SoundManager::BGM_Type::kGameClearBGM, DX_PLAYTYPE_LOOP);
				m_isPlayBGM = false;
			}
			// ゲームクリア時の演出
			second--;

			m_pScore->Update();

			if (second <= 0) {
				pos -= 0.03f;
				if (pos <= 1.0f)
				{
					// Aボタンが押されたら、次の処理を行う
						// 3回押されたらセレクトシーンへ遷移する
					if (input.IsTrigger(InputInfo::OK))
					{
						count++;
					}

					if (count >= 3)
					{
						return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
					}
				}
			}

		}
	}


#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneGame::Draw()
{
	m_pMap->Draw();
	m_pItem->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();

	m_pScore->Draw();

	// エフェクト表示
	Effect::GetInstance().Draw();

	m_pUI->Draw();
	m_pTime->Draw();

	m_pFade->Draw();

	// プレイヤーが死亡orタイムアップしたら
	if (m_pPlayer->GetDeathFlag()) 
	{
		if (!m_pFade->GetHarfFadeFlag()) {
			DrawGraph(kLogoPos.x, kLogoPos.y, m_gameOverHandle, true);
			DrawGraph(kPressAPos.x, kPressAPos.y, m_pressAHandle, true);
		}
	}


	// 敵が死亡したら
	if (m_pEnemy->GetDeathFlag()) {

		if (!m_pFade->GetHarfFadeFlag()) {
			if (second <= 0) {
				// ゲームクリアロゴを描画する
				DrawGraph(kLogoBgPos.x, kLogoBgPos.y, m_logoBgHandle, true);

				DrawRotaGraph3(kLogoPos.x, kLogoPos.y, 0, 0,
					std::max(1.0f, pos), std::max(1.0f, pos),
					0, m_gameClearHandle, true, false);
				if (pos <= 1.0f)
				{
					DrawGraph(kPressAPos.x, kPressAPos.y, m_pressAHandle, true);
				}
			}
			// Aボタンを押すごとに画像を追加する
			if (count >= 1) {
				DrawGraph(kScoreBgPos.x, kScoreBgPos.y, m_scoreBgHandle, true);

				DrawGraph(kScorePos.x, kScorePos.y, m_scoreHandle, true);
				m_pScore->DrawClearScore();
				if (count >= 2) {
					DrawGraph(kDefeatedTimePos.x, kDefeatedTimePos.y, m_defeatedTimeHandle, true);
					m_pTime->DrawClearTime();
				}
			}

		}
	}

#ifdef DEBUG
	//DrawString(0, 0, "SceneGame", 0xffffff);
#endif // DEBUG
}

void SceneGame::End()
{
	m_pUI->End();
	m_pSound->ReleaseSound();

	m_pPlayer->End();
	m_pScore->End();

	DeleteGraph(m_gameClearHandle);
	DeleteGraph(m_pressAHandle);
	DeleteGraph(m_defeatedTimeHandle);
	DeleteGraph(m_scoreHandle);
	DeleteGraph(m_gameOverHandle);
	DeleteGraph(m_logoBgHandle);
	DeleteGraph(m_scoreBgHandle);
}