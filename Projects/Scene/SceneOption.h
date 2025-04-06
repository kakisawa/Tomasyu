#pragma once
#include "SceneBase.h"

class Fade;
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

	void ChangeSoundVolume();

	void SetVolumeUI();

private:
    int m_optionHandle;
    int m_cursorHandle;
    int m_volumeHandle;
    int m_selectWindowHandlel;

    std::array<int, 7> m_bgmVolumeUI{};   //
	std::array<int, 7> m_seVolumeUI{};   //

    std::array<int, 2>m_selectWindowUI{};
        
	VECTOR m_cursorPos;    // カーソルの位置

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

    enum SelectWindow {
        FullScreen,
        WindowMode,
        SelectWindowNum,
    };

    int m_selectWindow;

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};