#pragma once
#include "../ModelBase.h"
#include "DxLib.h"

class TitlePlayer :
    public ModelBase,public std::enable_shared_from_this<TitlePlayer>
{
public:
    struct Situation {
        bool isIdle = false;    //待機中
    };

    // プレイヤーの状態
    struct PlayerStatus {
        int animNo = -1;        // アニメーション番号
        float animSpeed = 0.0f; // アニメーション再生スピード
        Situation situasion;    // 状態
        bool isLoop = false;    // ループ再生
    };

    // アニメーション
    enum class PlayerAnim {
        Idle,   // 待機
    };

    // アニメーションの再生速度
    struct AnimSpeed {
        float Default = 0.5f;   // 基本
        float Idle = 0.5f;      // 待機
    };

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    TitlePlayer();

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~TitlePlayer(){}

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

    /// <summary>
    /// データの削除関係
    /// </summary>
    void End() override;

private:

};

