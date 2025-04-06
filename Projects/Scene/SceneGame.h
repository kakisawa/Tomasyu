#pragma once
#include "SceneBase.h"

class Player;
class Enemy;
class Item;
class Camera;
class Map;
class UISceneGame;
class Time;
class Fade;
class Score;
class RankingData;
class MiniWindow;
class SceneGame :
    public SceneBase
{
public:
    SceneGame();
    ~SceneGame();

    virtual void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    virtual void Draw() override;
    virtual void End() override;

    void GameClearDraw();
	void GameOverDraw();

private:
    int m_gameClearHandle;      // ゲームクリアロゴ画像
    int m_gameOverHandle;       // ゲームオーバーロゴ画像
    int m_pressAHandle;         // Aボタンで進む画像
    int m_defeatedTimeHandle;   // 討伐時間画像
    int m_scoreHandle;          // スコア画像
	int m_logoBgHandle;         // ゲームクリア背景画像
	int m_scoreBgHandle;        // スコア背景画像
	int m_pauseHandle;          // ポーズ画像

	int m_gameClearStagingCount;    // ゲームクリア演出用カウント
	int m_gameClearLogoWaitTime;       // ゲームクリア演出待機時間
	float m_gameClearLogoScale;     // ゲームクリアロゴ拡大率

    bool m_isPause;         // ポーズフラグ
    bool m_isPlayBGM;       // BGMを鳴らすフラグ

	int m_timeYear;     // 年
	int m_timeMonth;    // 月
	int m_timeDay;      // 日
	int m_timeHour;     // 時間
	int m_timeMin;      // 分

    // カメラ
    std::shared_ptr<Camera> m_pCamera;
    // プレイヤー
    std::shared_ptr<Player> m_pPlayer;
    // 敵
    std::shared_ptr<Enemy> m_pEnemy;
    // アイテム
    std::shared_ptr<Item> m_pItem;
    // マップ
    std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
    // UI
    std::shared_ptr<UISceneGame> m_pUI;
    // 制限時間
    std::shared_ptr<Time> m_pTime = std::make_shared<Time>();
    // フェード
    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
    // スコア
    std::shared_ptr<Score> m_pScore = std::make_shared<Score>();
	// ランキングデータ
	std::shared_ptr<RankingData> m_pRankingData = std::make_shared<RankingData>();
    // ミニウィンドウ
	std::shared_ptr<MiniWindow> m_pMiniWindow = std::make_shared<MiniWindow>();
};