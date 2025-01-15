#include "SceneExplanation.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

using namespace MyInputInfo;

SceneExplanation::SceneExplanation()
{
}

SceneExplanation::~SceneExplanation()
{
}

void SceneExplanation::Init()
{
}

std::shared_ptr<SceneBase> SceneExplanation::Update(Input& input)
{
	if (input.IsTrigger(InputInfo::Back)) {			// Aボタン

		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}

#ifdef _DEBUG
	//if (input.IsTrigger(InputInfo::DebugStart)) {	// STARTボタン

	//	return std::make_shared<SceneDebug>();	// デバッグシーンへ行く
	//}
#endif // DEBUG

	return shared_from_this();
}

void SceneExplanation::Draw()
{

#ifdef _DEBUG
	/*DrawString(0, 0, "SceneExplanation", 0xffffff);*/
#endif // DEBUG

}

void SceneExplanation::End()
{
}
