#pragma once
#include "SceneBase.h"

class SceneDebug :
    public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneDebug();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneDebug() {}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	virtual std::shared_ptr<SceneBase> Update(Input& input);
	
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void End() {}

	/// <summary>
	/// 次のシーン切り替え
	/// </summary>
	/// <param name="input">入力</param>
	void SwitchingScene(Input& input);

private:
	
	struct selectBox		// セレクトボックス
	{
		VECTOR selectPos = VGet(0.0f, 0.0f, 0.0f);

	}m_selectBox;

	enum class nextScene	// 次のシーン
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