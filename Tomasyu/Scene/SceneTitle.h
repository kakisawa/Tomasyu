#pragma once
#include "SceneBase.h"
class Fade;
class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    ~SceneTitle();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

private:
    int m_titleGraph;   // タイトルロゴ画像
    int m_buttonGraph;  // Press...画像

    std::shared_ptr<Fade> m_pFade=std::make_shared<Fade>();
};

