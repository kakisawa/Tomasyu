#pragma once
#include "SceneBase.h"

class Fade;
class SceneOption :
    public SceneBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    SceneOption();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SceneOption(){}

    /// <summary>
    /// 初期化
    /// </summary>
    void Init() override;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="input"></param>
    virtual std::shared_ptr<SceneBase> Update(Input& input) override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    /// <summary>
    /// 終了
    /// </summary>
    virtual void End() override;

    /// <summary>
    /// カーソルの位置を変更する関数
    /// </summary>
    /// <param name="num">カーソル座標の番号</param>
    void ChangeCursorInfo(int num);

	/// <summary>
	/// 音量設定
	/// </summary>
	void ChangeSoundVolume();

	/// <summary>
	/// 音量UIの設定
	/// </summary>
	void SetVolumeUI();

private:
    int m_optionHandle;         // 設定画面画像
    int m_cursorHandle;         // カーソル画像
    int m_volumeHandle;         // 音量画像
    int m_selectWindowHandlel;  // 画面サイズ画像

    int m_selectWindow; // 選択中の画面サイズ

    std::array<int, 7> m_bgmVolumeUI{};     // BGMボリュームUI
	std::array<int, 7> m_seVolumeUI{};      // SEボリュームUI
    std::array<int, 2>m_selectWindowUI{};   // 画面サイズUI
        
	VECTOR m_cursorPos;    // カーソルの位置

    enum class Select {
        BGM,
        SE,
        Window,
        Back,
    }m_select;  // セレクト

    enum SelectWindow {
        FullScreen,
        WindowMode,
        SelectWindowNum,
    };  // 画面サイズ

    std::shared_ptr<Fade> m_pFade = std::make_shared<Fade>();
};