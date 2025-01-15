#pragma once
#include "SceneBase.h"
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
    int m_rankingGraph; // ランキング画像

    int m_selectUI1;    // セレクトUI_最大スコア
    int m_selectUI3;    // セレクトUI_最短時間

    int m_cursorUI;     // カーソル画像

    int m_rankingUI;    // ランキングUI
    int m_rankingUI_Score;
    int m_rankingUI_Time;

    int m_rankingSelectUI;
    
    VECTOR m_selectBox1;
};

