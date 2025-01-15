#pragma once
#include "SceneBase.h"

class SceneDebug :
    public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug() {}

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
	virtual void End() {}

	/// <summary>
	/// 次のシーン切り替え
	/// </summary>
	/// <param name="input">input</param>
	void SwitchingScene(Input& input);

private:
	
	struct selectBox
	{
		VECTOR selectPos = VGet(0.0f, 0.0f, 0.0f);

	}m_selectBox;

	enum class nextScene
	{
		kTitleScene,
		kSelectScene,
		kOptionScene,
		kExplanationScene,
		kRankingScene,
		kGameScene,
		kGameEnd,
	}m_nextScene;
};