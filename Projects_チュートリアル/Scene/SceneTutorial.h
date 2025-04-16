#pragma once
#include "SceneBase.h"

class Player;
class Enemy;
class Item;
class Camera;
class Map;
class UISceneGame;
class Fade;
class Score;
class MiniWindow;
class SceneTutorial :
    public SceneBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneTutorial();

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~SceneTutorial();

    /// <summary>
    /// 初期化
    /// </summary>
    virtual void Init() override;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="input">入力</param>
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;

    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw() override;

    /// <summary>
    /// 終了
    /// </summary>
    virtual void End() override;


private:
    int m_pauseHandle;          // ポーズ画像

    bool m_isPause;         // ポーズフラグ

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
    // フェード
    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
    // スコア
    std::shared_ptr<Score> m_pScore = std::make_shared<Score>();
    // ミニウィンドウ
    std::shared_ptr<MiniWindow> m_pMiniWindow = std::make_shared<MiniWindow>();
};