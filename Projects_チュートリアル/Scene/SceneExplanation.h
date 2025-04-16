#pragma once
#include "SceneBase.h"
#include <array>

class Fade;
class SceneExplanation :
    public SceneBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneExplanation();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneExplanation(){}

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

private:
	int m_explanationHandle{};	// 説明画像ハンドル
	int m_count;	// 画像変更カウント

	std::array<int, 2> m_triangleUIHandle{};	// 三角形画像ハンドル
	std::array<int, 3> m_explanationUIHandle{};	// 説明画像ハンドル

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};