#include "SceneOption.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

using namespace MyInputInfo;

SceneOption::SceneOption()
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	if (input.IsTrigger(InputInfo::Back)) {			// STARTボタン

		return std::make_shared<SceneSelect>();	// ゲームシーンへ行く
	}

#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {			// STARTボタン

	//	return std::make_shared<SceneDebug>();	// ゲームシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneOption::Draw()
{

#ifdef _DEBUG
	//DrawString(0, 0, "SceneOption", 0xffffff);
#endif // DEBUG
}

void SceneOption::End()
{
}
