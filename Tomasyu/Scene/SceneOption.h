#pragma once
#include "SceneBase.h"
class SceneOption :
    public SceneBase
{
public:
    SceneOption();
    ~SceneOption();

    void Init() override;
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;
    void Draw() override;
    virtual void End() override;

    /// <summary>
    /// カーソルの位置を変更する関数
    /// </summary>
    /// <param name="num"></param>
    void ChangeCursorInfo(int num);

private:
    int m_optionGraph;
    int m_cursorGraph;

    struct Cursor   // 選択中のセレクト
    {
        VECTOR m_selectBox1;    // 四角始点
        VECTOR m_selectBox2;    // 四角終点
    }c1;

    enum class Select {
        BGM,
        SE,
        Window,
        Back,
    }m_select;

};