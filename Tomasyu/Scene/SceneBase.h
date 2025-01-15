#pragma once
#include "DxLib.h"
#include "../Util/Input.h"
#include "../Manager/SoundManager.h"
#include <memory>

class SceneBase:
	public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase(){}

	virtual void Init(){}
	virtual std::shared_ptr<SceneBase> Update(Input& input) = 0;
	virtual void Draw(){}
	virtual void End(){}

protected:
	bool m_isNextSceneFlag;		// 次のシーンに移動するフラグ
	
	std::shared_ptr<SoundManager> m_pSound = std::make_shared<SoundManager>();

};