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
    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneTitle();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneTitle(){}

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
    ///  描画
    /// </summary>
    void Draw() override;

    /// <summary>
    /// 終了
    /// </summary>
    virtual void End() override;

private:
	int m_titleLogoHandle; // タイトルロゴ画像
    int m_buttonHandle;     // Press...画像
	int m_boardModel;       // 掲示板モデル

	float m_boardAngle;     // 掲示板の角度

	VECTOR m_boardPos;      // 掲示板画像の座標

    // プレイヤー
    std::shared_ptr<TitlePlayer> m_pPlayer = std::make_shared<TitlePlayer>();
    // マップ
    std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
    // カメラ
    std::shared_ptr <TitleCamera> m_pCamera = std::make_shared<TitleCamera>();
    // フェード
    std::shared_ptr<Fade> m_pFade=std::make_shared<Fade>();
};