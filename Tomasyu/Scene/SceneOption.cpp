#include "SceneOption.h"
#include "SceneSelect.h"
#include "SceneDebug.h"

using namespace MyInputInfo;

namespace {

	const VECTOR kOptionGraphPos = VGet(256.0f, 131.0f, 0.0f);	

	const VECTOR kCursorGraphPos1[4] = {
		VGet(370.0f,322.0f,0.0f),
		VGet(408.0f,478.0f,0.0f),
		VGet(265.0f,620.0f,0.0f),
		VGet(865.0f,803.0f,0.0f)
	};

	const VECTOR kCursorGraphPos2[4] = {
		VGet(620.0f,442.0f,0.0f),
		VGet(584.0f,585.0f,0.0f),
		VGet(722.0f,742.0f,0.0f),
		VGet(1075.0f,923.0f,0.0f)
	};


	const char* const kOptionGraph = "Data/Image/SceneOption/設定画面.png";
	const char* const kCursorGraph = "Data/Image/SceneOption/Cursor.png";
}

SceneOption::SceneOption() :
	m_optionGraph(-1),
	m_cursorGraph(-1),
	m_select(Select::BGM)
{
	c1.m_selectBox1 = kCursorGraphPos1[0];
	c1.m_selectBox2 = kCursorGraphPos2[0];
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
	m_optionGraph = LoadGraph(kOptionGraph);
	m_cursorGraph = LoadGraph(kCursorGraph);
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	if (m_select==Select::Back&&input.IsTrigger(InputInfo::OK)) {

		return std::make_shared<SceneSelect>();	//セレクトシーンへ向かう
	}

	if (input.IsTrigger(InputInfo::Down)) {
		if (m_select==Select::BGM)
		{
			m_select = Select::SE;
			ChangeCursorInfo(1);
		}
		else if (m_select == Select::SE)
		{
			m_select = Select::Window;
			ChangeCursorInfo(2);
		}
		else if (m_select == Select::Window)
		{
			m_select = Select::Back;
			ChangeCursorInfo(3);
		}
		else if (m_select == Select::Back)
		{
			m_select = Select::BGM;
			ChangeCursorInfo(0);
		}
	}

	if (input.IsTrigger(InputInfo::Up)) {
		if (m_select == Select::BGM)
		{
			m_select = Select::Back;
			ChangeCursorInfo(3);
		}
		else if (m_select == Select::SE)
		{
			m_select = Select::BGM;
			ChangeCursorInfo(0);
		}
		else if (m_select == Select::Window)
		{
			m_select = Select::SE;
			ChangeCursorInfo(1);
		}
		else if (m_select == Select::Back)
		{
			m_select = Select::Window;
			ChangeCursorInfo(2);
		}
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
	DrawGraph(kOptionGraphPos.x, kOptionGraphPos.y, m_optionGraph, true);

	DrawExtendGraphF(c1.m_selectBox1.x, c1.m_selectBox1.y,
		c1.m_selectBox2.x, c1.m_selectBox2.y, m_cursorGraph, true);

#ifdef _DEBUG
#endif // DEBUG
}

void SceneOption::End()
{
	DeleteGraph(m_optionGraph);
	DeleteGraph(m_cursorGraph);
}

void SceneOption::ChangeCursorInfo(int num)
{
	c1.m_selectBox1 = kCursorGraphPos1[num];
	c1.m_selectBox2 = kCursorGraphPos2[num];
}