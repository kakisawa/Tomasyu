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
    int m_titleLogoHandle;   // タイトルロゴ画像
    int m_buttonHandle;  // Press...画像
    int m_titleBgHandle;
    int m_wantedHandle;

    std::shared_ptr<Fade> m_pFade=std::make_shared<Fade>();
};

