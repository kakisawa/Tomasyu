#pragma once
#include "SceneBase.h"


class RankingData;
class Fade;
class SceneRanking :
    public SceneBase
{
public:
    SceneRanking();
    ~SceneRanking();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

    void SelectRanking(Input& input);

private:
    int m_bg;
    int m_rankingGraph; // ランキング画像
 
    int m_selectUI1;    // セレクトUI_最大スコア
    int m_selectUI3;    // セレクトUI_最短時間

    int m_cursorUI;     // カーソル画像

    int m_rankingUI;    // ランキングUI
    int m_rankingUI_Score;
    int m_rankingUI_Time;

    int m_rankingSelectUI;
    
    enum rankingDataChange {
        Score,
        Time,
        RankingDataNum,
    };
    int m_rankingDataChange;

    VECTOR m_selectBox1;

	std::shared_ptr<RankingData> m_pRankingData = std::make_shared<RankingData>();

    std::vector<std::tuple<int, int, int, int, int, int>> m_scoreRanking;	// スコアランキング
    std::vector<std::tuple<int, int, int, int, int, int>> m_timeRanking;	// タイムランキング

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};

