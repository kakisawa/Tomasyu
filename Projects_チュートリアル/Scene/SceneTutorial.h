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
private:
    /// <summary>
    /// チュートリアル用
    /// </summary>
    struct TutorialFlag {
        bool isTMove = false;				// 移動
        bool isTRoll = false;				// 回避
        bool isTGetItem = false;			// アイテム獲得
        bool isTChangeWeapon = false;		// 武器切り替え
        bool isTRockOn = false;				// ロックオン
        bool isTAttackHandGun = false;		// 銃攻撃(ハンドガン)
        bool isTAttackMachineGun = false;	// 銃攻撃(マシンガン)
        bool isTAttackKnife = false;		// ナイフ攻撃
        bool isTChangeItem = false;			// アイテム切り替え
        bool isTUseItem = false;			// アイテム使用
		bool isTOpenPause = false;			// ポーズを開く
    }m_tutorial;

public:
    enum class TutorialStep {
        Tutorial1,
		Tutorial2,
		Tutorial3,
        num,
	}m_tutorialNum;

    struct Tutorial {
        int m_step;
        int m_handle;
        std::vector<VECTOR> m_checkPos;
  };

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

    /// <summary>
    /// チュートリアル詳細の初期化
    /// </summary>
    void InitTutorial();

    /// <summary>
    /// チュートリアル画像の切り替え
    /// </summary>
    void ChangeTutorialDisplay(Input& input);

    /// <summary>
    /// チュートリアルの確認
    /// </summary>
    void CheckTutorial();

private:
    int m_pauseHandle;          // ポーズ画像

    int m_tutorialDisplay;      // チュートリアル説明画像(表示用)
	int m_tutorialCount;        // チュートリアルのカウント

    bool m_isPause;         // ポーズフラグ

    std::vector<int> m_checkHandle; // チュートリアル用チェックマーク画像
	std::vector<Tutorial> m_tutorialStep;	// チュートリアルの詳細

    enum class nextScene {
        None,               // 無し
        GameScene,
		SelectScene,
    }m_nextScene;

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