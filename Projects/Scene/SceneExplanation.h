#pragma once
#include "SceneBase.h"
#include <array>

class Fade;
class SceneExplanation :
    public SceneBase
{
public:
    SceneExplanation();
    ~SceneExplanation();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

private:
	int m_explanationHandle{};	// 説明画像ハンドル

	int m_count;	// 画像変更カウント

	std::array<int, 2> m_triangleUIHandle{};	// 三角形画像ハンドル
	std::array<int, 3> m_explanationUIHandle{};	// 説明画像ハンドル

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();

};