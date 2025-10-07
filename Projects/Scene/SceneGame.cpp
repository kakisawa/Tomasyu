#include "SceneGame.h"
#include "SceneDebug.h"
#include "SceneSelect.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Item.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Map.h"
#include "../UI/UISceneGame.h"
#include "../Manager/Effect.h"
#include "../Time/Time.h"
#include "../Util/Fade.h"
#include "../Util/Score.h"
#include "../Util/RankingData.h"
#include "../Util/MiniWindow.h"
#include "DxLib.h"
#include <ctime>

using namespace MyInputInfo;

namespace
{
	constexpr int kGameClearLogoWaitTime = 100;	// ゲームクリア演出時ロゴを表示する待機時間
	constexpr float kGameClearLogoScale = 1.7f;	// ゲームクリアロゴの拡大率
	constexpr float kGameClearLogoBaseScale = 1.0f;	// ゲームクリアロゴのデフォルト拡大率
	constexpr float kGameClearLogoScaleDec = 0.03f;	// ゲームクリアロゴの減値

	const VECTOR kLogoPos = { 564.0f, 134.0f ,0.0f };			// ゲームクリア・オーバー時ロゴ画像座標
	const VECTOR kScorePos = { 560.0f, 730.0f, 0.0f };			// スコア画像座標
	const VECTOR kDefeatedTimePos = { 493.0f, 850.0f, 0.0f };	// ゲームクリア時間画像座標
	const VECTOR kPressAPos = { 795.0f, 1005.0f, 0.0f };		// 「Aボタンで進む」画像座標
	const VECTOR kLogoBgPos = { 0.0f, 0.0f, 0.0f };				// ゲームクリア・オーバー背景画像座標
	const VECTOR kScoreBgPos = { 0.0f, 642.0f, 0.0f };			// スコア背景画像座標
}

SceneGame::SceneGame() :
	m_gameClearHandle(-1),
	m_gameOverHandle(-1),
	m_pressAHandle(-1),
	m_defeatedTimeHandle(-1),
	m_scoreHandle(-1),
	m_logoBgHandle(-1),
	m_scoreBgHandle(-1),
	m_gameClearStagingCount(0),
	m_gameClearLogoWaitTime(kGameClearLogoWaitTime),
	m_gameClearLogoScale(kGameClearLogoScale),
	m_timeYear(0),
	m_timeMonth(0),
	m_timeDay(0),
	m_timeHour(0),
	m_timeMin(0),
	m_isPause(false),
	m_isPlayBGM(true)
{
	m_pEnemy = std::make_shared<Enemy>(m_pMap, nullptr);
	m_pPlayer = std::make_shared<Player>(nullptr, nullptr, m_pItem);

	// PlayerとEnemyの相手ポインタを設定する
	m_pPlayer->SetEnemyPointer(m_pEnemy);
	m_pEnemy->SetPlayer(m_pPlayer);

	m_pCamera = std::make_shared<Camera>(m_pPlayer);
	m_pUI = std::make_shared<UISceneGame>(m_pPlayer, m_pEnemy,m_pScore);

	m_pPlayer->SetCameraPointer(m_pCamera);
	m_pItem = std::make_shared<Item>(m_pPlayer, 3);
}

void SceneGame::Init()
{
	m_pItem->Init();
	m_pCamera->Init();
	m_pPlayer->Init(m_pScore);
	m_pEnemy->Init();
	m_pMap->Init();
	m_pUI->Init();
	m_pTime->Init();
	m_pSound->InitBGM();
	m_pScore->Init(m_pTime);

	// サウンドの初期化・読み込み
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
	m_pauseHandle = LoadGraph("Data/Image/SceneGame/Pause.png");

	m_pMiniWindow->Init(m_pauseHandle);

	// 日にちのセット
	time_t now = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &now);

	m_timeYear = localTime.tm_year + 1900;
	m_timeMonth = localTime.tm_mon + 1;
	m_timeDay = localTime.tm_mday;
	m_timeHour = localTime.tm_hour;
	m_timeMin = localTime.tm_min;
}

std::shared_ptr<SceneBase> SceneGame::Update(Input& input)
{
	// スタートボタンを押したらポーズ状態にする
	if (input.IsTrigger(InputInfo::DebugStart)) {
		if (m_isPause) 
		{
			m_isPause = false;
			m_pMiniWindow->CloseMiniWindow();
			m_pFade->HarfFade(false);
			m_pFade->FadeIn(true);
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
		m_pFade->HarfFade(true);
		m_pMiniWindow->CallMiiniWindow();

		if (input.IsTrigger(InputInfo::OK))
		{
			return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
		}
	}
	else 
	{
		// エネミーが死んでいなければ
		if (!m_pEnemy->GetDeathFlag() && !m_pTime->GetTimeUp())
		{
			if (!m_pPlayer->GetDeathFlag() || m_pTime->GetTimeUp()) 
			{
				m_pFade->FadeIn(true);
				m_pMap->Update();
				m_pItem->Update();
				m_pPlayer->Update(input);
				m_pEnemy->Update();
				m_pCamera->Update(m_pEnemy->GetPos());
				m_pUI->Update();
				m_pTime->Update();

				// エフェクトの更新
				Effect::GetInstance().Update();
			}
		}

		// プレイヤーが死亡したら・制限時間を過ぎてしまったら
		if (m_pPlayer->GetDeathFlag() || m_pTime->GetTimeUp()) 
		{
			// フェード演出を行う
			m_pFade->HarfFade(true);
			// フェード演出が終わったら、BGMを変更する
			if (!m_pFade->GetHarfFadeFlag()) 
			{
				if (m_isPlayBGM) {
					m_pSound->StopBGM(SoundManager::BGM_Type::kGameBGM);
					m_isPlayBGM = false;
				}

				if (input.IsTrigger(InputInfo::OK))
				{
					return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
				}
			}
		}

		// エネミーを倒したら
		if (m_pEnemy->GetDeathFlag())
		{
			// フェード演出を行う
			m_pFade->HarfFade(true);
			// フェード演出が終わったら、BGMを変更する
			if (!m_pFade->GetHarfFadeFlag())
			{
				m_pSound->StopBGM(SoundManager::BGM_Type::kGameBGM);
				if (m_isPlayBGM) 
				{
					m_pSound->PlayBGM(SoundManager::BGM_Type::kGameClearBGM, DX_PLAYTYPE_LOOP);
					m_isPlayBGM = false;
				}
				// ゲームクリア時の演出
				m_gameClearLogoWaitTime--;

				m_pScore->Update();

				if (m_gameClearLogoWaitTime <= 0) 
				{
					m_gameClearLogoScale -= kGameClearLogoScaleDec;
					if (m_gameClearLogoScale <= kGameClearLogoBaseScale)
					{
						// Aボタンが押されたら、次の処理を行う
							// 3回押されたらセレクトシーンへ遷移する
						if (input.IsTrigger(InputInfo::OK))
						{
							m_gameClearStagingCount++;
						}

						// スコア類の保存
						if (m_gameClearStagingCount >= 3)
						{
							m_pRankingData->TimeSave(m_pTime->GetElapsedTime(),
								m_timeYear, m_timeMonth, m_timeDay, m_timeHour, m_timeMin);

							m_pRankingData->ScoreSave(m_pScore->GetTotalScore(),
								m_timeYear, m_timeMonth, m_timeDay, m_timeHour, m_timeMin);

							return std::make_shared<SceneSelect>();	// ゲームセレクトシーンへ行く
						}
					}
				}
			}
		}
	}

#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン
	//	m_pEnemy->SetDeathFlag(true);
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
	Effect::GetInstance().Draw();	// エフェクト表示
	m_pUI->Draw();
	m_pTime->Draw();
	m_pCamera->DebugDraw();
	m_pFade->Draw();

	GameClearDraw();
	GameOverDraw();

	// ポーズ中の描画
	if (m_isPause) 
	{
		m_pMiniWindow->Draw();
	}

#ifdef _DEBUG
	/*DrawFormatString(0, 300, 0xffffff, "m_timeYear=%d", m_timeYear);
	DrawFormatString(0, 320, 0xffffff, "m_timeMonth=%d", m_timeMonth);
	DrawFormatString(0, 340, 0xffffff, "m_timeDay=%d", m_timeDay);
	DrawFormatString(0, 360, 0xffffff, "m_timeHour=%d", m_timeHour);
	DrawFormatString(0, 380, 0xffffff, "m_timeMin=%d", m_timeMin);*/
#endif // DEBUG
}

void SceneGame::End()
{
	m_pUI->End();
	m_pMap->End();
	m_pPlayer->End();
	m_pScore->End();
	m_pEnemy->End();
	m_pSound->ReleaseSound();

	// 画像の削除
	DeleteGraph(m_gameClearHandle);
	DeleteGraph(m_pressAHandle);
	DeleteGraph(m_defeatedTimeHandle);
	DeleteGraph(m_scoreHandle);
	DeleteGraph(m_gameOverHandle);
	DeleteGraph(m_logoBgHandle);
	DeleteGraph(m_scoreBgHandle);
	DeleteGraph(m_pauseHandle);
}

void SceneGame::GameClearDraw()
{
	// 敵が死亡したら
	if (m_pEnemy->GetDeathFlag())
	{
		if (!m_pFade->GetHarfFadeFlag()) 
		{
			if (m_gameClearLogoWaitTime <= 0) 
			{
				// ゲームクリアロゴを描画する
				DrawGraphF(kLogoBgPos.x, kLogoBgPos.y, m_logoBgHandle, true);

				DrawRotaGraph3F(kLogoPos.x, kLogoPos.y, 0.0f, 0.0f,
					std::max(kGameClearLogoBaseScale, m_gameClearLogoScale), std::max(kGameClearLogoBaseScale, m_gameClearLogoScale),
					0.0f, m_gameClearHandle, true, false);
				if (m_gameClearLogoScale <= kGameClearLogoBaseScale)
				{
					DrawGraphF(kPressAPos.x, kPressAPos.y, m_pressAHandle, true);
				}
			}
			// Aボタンを押すごとに画像を追加する
			if (m_gameClearStagingCount >= 1)
			{
				DrawGraphF(kScoreBgPos.x, kScoreBgPos.y, m_scoreBgHandle, true);

				DrawGraphF(kScorePos.x, kScorePos.y, m_scoreHandle, true);
				m_pScore->DrawClearScore();
				if (m_gameClearStagingCount >= 2) 
				{
					DrawGraphF(kDefeatedTimePos.x, kDefeatedTimePos.y, m_defeatedTimeHandle, true);
					m_pTime->DrawClearTime();
				}
			}
		}
	}
}

void SceneGame::GameOverDraw()
{
	// プレイヤーが死亡orタイムアップしたら
	if (m_pPlayer->GetDeathFlag() || m_pTime->GetTimeUp())
	{
		if (!m_pFade->GetHarfFadeFlag())
		{
			DrawGraphF(kLogoPos.x, kLogoPos.y, m_gameOverHandle, true);
			DrawGraphF(kPressAPos.x, kPressAPos.y, m_pressAHandle, true);
		}
	}
}