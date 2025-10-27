#pragma once
#include "DxLib.h"
#include "../Util/Input.h"
#include "../Manager/SoundManager.h"
#include <memory>

class SceneBase:
	public std::enable_shared_from_this<SceneBase>
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase(){}

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(){}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(){}

	/// <summary>
	/// 終了
	/// </summary>
	virtual void End(){}

protected:
	bool m_isNextSceneFlag;		// 次のシーンに移動するフラグ
	
	std::shared_ptr<SoundManager> m_pSound = std::make_shared<SoundManager>();
};