#pragma once
#include "../Scene/SceneBase.h"

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager() {}

	void Init();
	void Update(Input& input);
	void Draw();
	void End();

private:
	std::shared_ptr<SceneBase> m_pScene;
};