#pragma once
#include "SceneBase.h"

class RankingData;
class Fade;
class SceneRanking :
    public SceneBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneRanking();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneRanking(){}

    /// <summary>
    /// 初期化
    /// </summary>
    void Init() override;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="input">入力</param>
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    /// <summary>
    /// 終了
    /// </summary>
    virtual void End() override;

    /// <summary>
    /// ランキングの変更
    /// </summary>
    /// <param name="input">入力</param>
    void SelectRanking(Input& input);

private:
    int m_bg;           // 背景画像
    int m_rankingGraph; // ランキング画像
    int m_cursorGraph;  // カーソル画像

    int m_selectUI1;    // セレクトUI_最大スコア
    int m_selectUI3;    // セレクトUI_最短時間
    int m_rankingUI;        // ランキングUI
    int m_rankingUI_Score;  // スコアランキングUI
    int m_rankingUI_Time;   // タイムランキングUI
    int m_rankingSelectUI;  // 選択中のランキングUI

    int m_rankingDataChange;// 選択中のランキング種類
    
    enum rankingDataChange {
        Score,
        Time,
        RankingDataNum,
    };// ランキングの種類

    VECTOR m_selectBox1;

    std::vector<std::tuple<int, int, int, int, int, int>> m_scoreRanking;	// スコアランキング
    std::vector<std::tuple<int, int, int, int, int, int>> m_timeRanking;	// タイムランキング

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
    std::shared_ptr<RankingData> m_pRankingData = std::make_shared<RankingData>();
};

