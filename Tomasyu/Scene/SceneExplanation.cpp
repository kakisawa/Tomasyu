#include "SceneExplanation.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "../Util/Input.h"

using namespace MyInputInfo;

namespace {
	VECTOR kTrianglePos[2]{	// 三角形キー座標	
		VGet(81.0f, 486.0f, 0.0f),
		VGet(1728.0f, 486.0f, 0.0f) 
	};	// 三角形座標

	VECTOR kExplanationGraphPos = VGet(260.0f, 139.0f, 0.0f);	// 画像座標


	const char* const kExplanationUI[3] = {	// 画像パス
		"Data/Image/SceneExplanation/SceneExplanation1-3.png",
		"Data/Image/SceneExplanation/SceneExplanation2-3.png",
		"Data/Image/SceneExplanation/SceneExplanation3-3.png",
	};

	const char* const kTriangleUI[2] = {	// 三角形キー画像パス
		"Data/Image/SceneExplanation/LeftTriangle.png",
		"Data/Image/SceneExplanation/RightTriangle.png",
	};
}

SceneExplanation::SceneExplanation():
	m_explanationHandle(-1),
	m_count(0)
{
}

SceneExplanation::~SceneExplanation()
{
}

void SceneExplanation::Init()
{
	// 画像読み込み
	for (int i = 0; i < m_explanationUIHandle.size(); i++)
	{
		m_explanationUIHandle[i] = LoadGraph(kExplanationUI[i]);
	}
	for (int i = 0; i < m_triangleUIHandle.size(); i++)
	{
		m_triangleUIHandle[i] = LoadGraph(kTriangleUI[i]);
	}

	m_explanationHandle = m_explanationUIHandle[m_count];	// 説明画像
}

std::shared_ptr<SceneBase> SceneExplanation::Update(Input& input)
{
	if (input.IsTrigger(InputInfo::Back)) {			// Bボタン

		return std::make_shared<SceneSelect>();	// セレクトシーンへ行く
	}

	if (input.IsTrigger(InputInfo::Right)) {			// 右キー
		m_count++;
		if (m_count >= 3)
		{
			m_count = 0;
		}
		
	}

	if (input.IsTrigger(InputInfo::Left)) {			// 左キー
		m_count--;
		if (m_count < 0)
		{
			m_count = 2;
		}
	}

	m_explanationHandle = m_explanationUIHandle[m_count];	// 説明画像


#ifdef _DEBUG


#endif // DEBUG

	return shared_from_this();
}

void SceneExplanation::Draw()
{
	DrawGraphF(kExplanationGraphPos.x, kExplanationGraphPos.y, m_explanationHandle, true);	// 説明画像

	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kTrianglePos[i].x, kTrianglePos[i].y, m_triangleUIHandle[i], true);	// 三角形
	}
	

#ifdef _DEBUG
	DrawString(0, 0, "SceneExplanation", 0xffffff);
#endif // DEBUG

}

void SceneExplanation::End()
{
}
