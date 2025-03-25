#pragma once
#include "SceneBase.h"

class TitlePlayer;
class Map;
class TitleCamera;
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
	int m_titleLogoHandle; // タイトルロゴ画像
    int m_buttonHandle;  // Press...画像

	int m_boardModel;       // 掲示板モデル

	float m_boardAngle;     // 掲示板の角度

	VECTOR m_boardPos;

    // プレイヤー
    std::shared_ptr<TitlePlayer> m_pPlayer = std::make_shared<TitlePlayer>();
    // マップ
    std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
    // カメラ
    std::shared_ptr <TitleCamera> m_pCamera = std::make_shared<TitleCamera>();
    // フェード
    std::shared_ptr<Fade> m_pFade=std::make_shared<Fade>();
};