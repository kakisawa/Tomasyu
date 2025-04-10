#pragma once
#include "SceneBase.h"
class Fade;
class SceneSelect :
    public SceneBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneSelect();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneSelect();

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
    /// 次のシーン切り替え
    /// </summary>
    /// <param name="input">input</param>
    void SwitchingScene(Input& input);

    /// <summary>
    /// カーソル情報の変更
    /// </summary>
    /// <param name="num"></param>
    void ChangeCursorInfo(int num);

private:
    int m_sceneSelectGraph;             // 選択中のセレクト画像情報を入れる用
    int m_selectGraphX, m_selectGraphY; // 選択中のセレクト画像のサイズ獲得用

    int m_nextSceneGrapgh;              // セレクト中のシーンの画像
    int m_nextSceneIntroductionGraph;   // セレクト中のシーンの説明

    int m_pressAHandle;    // Aボタンを押す画像
    int m_cursorUIHandle;  // カーソル画像
	int m_bgHandle;        // 背景画像

	VECTOR m_cursolPos;	  // カーソル座標
	VECTOR m_pressAPos;   // カーソル(Aボタンを押す)座標


    std::array<int, 5> m_sceneSelectUIHandle{};             // シーンセレクトUI
    std::array<int, 5> m_sceneSelectHandle{};               // シーンセレクト画像
    std::array<int, 5> m_sceneSelectIntroductionHandle{};   // シーンセレクト説明画像

    enum class nextScene {
        None,               // 無し
        TitleScene,         // タイトルシーン
        GameScene,          // ゲームシーン
        ExplanationScene,   // 操作説明シーン
        OptionScene,        // オプションシーン
        RankingScene,       // ランキングシーン
        GameEnd,            // ゲーム終了
    }m_nextScene;

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};