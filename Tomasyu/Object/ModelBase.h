﻿#pragma once
#include "../Util/Collision.h"
#include "../Manager/SoundManager.h"
#include <memory>
#include "DxLib.h"

class ModelBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ModelBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了関係
	/// </summary>
	virtual void End();

	/// <summary>
	/// プレイヤーデータ
	/// </summary>
	struct CharaCommonData
	{
		int maxHp = 0;			// 最大HP
		float initPosX = 0.0f;		// 初期化座標X
		float initPosY = 0.0f;		// 初期化座標Y
		float initPosZ = 0.0f;		// 初期化座標Z
		float modelSize = 0.0f;	// モデルサイズ
		float walkSpeed = 0.0f;	// 歩く速度
		float rotaSpeed = 0.0f;	// 回転速度
		float bodyColRad = 0.0f;	// 体当たり判定半径
		float bodyColUpY = 0.0f;	// 当たり判定上座標
	}m_chara;

	/// <summary>
	/// アニメーションを変更させる
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	/// <param name="changeFrame">何フレームかけて変化させるのか</param>
	void ChangeAnimation(const int animNo, const float animSpeed, const bool isLoop, const bool isForceChange, const int changeFrame);

	/// <summary>
	/// アニメーションを設定する
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isLoop">ループ再生させるか</param>
	/// <param name="isForceChange">既に指定されたアニメーションが再生されている場合も変更するか</param>
	void SetAnimation(const int animNo, const float animSpeed, const bool isLoop, const bool isForceChange);

	/// <summary>
	/// アニメーションが終了しているかどうか
	/// </summary>
	/// <returns>アニメーションが終了したか</returns>
	bool IsAnimEnd();

	/// <summary>
	/// ループアニメーションが終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool IsLoopAnimEnd();

	/// <summary>
	/// HPを渡す
	/// </summary>
	/// <returns></returns>
	int GetHp() const { return m_hp; }

	/// <summary>
	/// 敵へ攻撃値を渡す
	/// </summary>
	/// <returns></returns>
	int GetAttack() const { return m_attackThePlayer; }

	// 死亡しているかどうかのフラグを渡す
	bool GetDeathFlag() const { return m_deathFlag; }

	// 向いている方向を渡す
	VECTOR GetTargetDir() const { return m_targetDir; }

	/// <summary>
	/// 当たり判定の値渡し
	/// </summary>
	/// <returns>プレイヤーの当たり判定の値</returns>
	Collision GetCol() const { return m_col; }

protected:
	int m_model;			// モデル
	int m_hp;				// HP
	int m_attack;			// 攻撃力
	/*要修正*/
	int m_attackThePlayer;	// 敵への攻撃力

	float m_angle;			// プレイヤー向き角度
	float m_nextAnimTime;	// 新しいアニメーションの再生時間

	bool m_deathFlag;		// 死亡しているかのフラグ

	VECTOR m_pos;			// 座標
	VECTOR m_move;			// 移動量
	VECTOR m_targetDir;		// プレイヤーが向くべき方向のベクトル

	Collision m_col;		// 当たり判定

	// アニメーション関係
	int m_animChangeFrame;		// 現在の切り替えフレーム数
	int m_animChangeFrameTotal;	// 切り替えにかける総フレーム数

	bool m_isLoopFinish;		// ループ再生中に1回のアニメーションが終了したかのフラグ

	/// <summary>
	/// アニメーション情報
	/// </summary>
	struct AnimData {
		int animNo = -1;		// アニメーション番号
		int attachNo = -1;	// アタッチ番号
		float animSpeed = 0.0f;// アニメーションの再生速度
		float time = 0.0f;		// 再生時間
		float totalTime = 0.0f;// 総再生時間
		float isLoop = false;	// ループさせるか
	};

	// アニメーションのアタッチ番号
	AnimData m_animPrev;	// 前のアニメーション情報
	AnimData m_animNext;	// 新しいアニメーション情報

	/// <summary>
	/// アニメーション情報の初期化
	/// </summary>
	/// <param name="anim">アニメーション情報</param>
	void InitAnim(AnimData& anim);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="anim">アニメーション情報</param>
	/// <param name="dt"></param>
	void UpdateAnim(const AnimData& anim);

	/// <summary>
	/// アニメーションのブレンド率の設定
	/// </summary>
	void UpdateAnimBlendRate();

	std::shared_ptr<SoundManager> m_pSound = std::make_shared<SoundManager>();
};