#include "SceneManager.h"
#include "../Scene/SceneDebug.h"
#include "../Scene/SceneTitle.h"
#include "../Util/Input.h"
#include "DxLib.h"

SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

void SceneManager::Init()
{
	// 最初のシーンのメモリを確保する
#ifdef _DEBUG

	// デバッグ時はデバッグシーンへ遷移する
	m_pScene = std::make_shared<SceneDebug>();
#else
	// タイトルシーンへ遷移
	m_pScene = std::make_shared<SceneTitle>();
#endif // DEBUG

	m_pScene->Init();	// シーンのInitを呼び出す
}

void SceneManager::Update(Input& input)
{
	// シーンのUpdateを呼び出す
	std::shared_ptr<SceneBase> pNext = m_pScene->Update(input);	
	if (pNext != m_pScene)
	{
		// 現在のシーンの終了処理
		m_pScene->End();

		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}
}

void SceneManager::Draw()
{
	m_pScene->Draw();	// シーンのDrawを呼び出す
}

void SceneManager::End()
{
	m_pScene->End();	// シーンのEndを呼び出す
}
