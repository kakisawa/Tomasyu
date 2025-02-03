#pragma once
#include "ModelBase.h"

class Player;
class Map;
class Enemy :
	public ModelBase
{
public:
	struct Situation {
		bool isMoving = false;	// 移動中
		bool isAttack = false;	// 攻撃中
		bool isDeath = false;	// 死亡中
	};

	// 敵の状態
	struct EnemyStatus {
		int animNo = -1;		// アニメーション番号
		float animSpeed = 0.0f;	// アニメーション再生スピード
		Situation situation;	// 状態
		bool isLoop = false;	// ループ再生
	}m_status;

	// アニメーション
	enum class EnemyAnim {
		Idle,			// 待機
		Walk,			// 歩く
		Run,			// 走る
		Provocation,	// 挑発
		Attack1,		// 攻撃1
		Attack2,		// 攻撃2
		Attack3,		// 攻撃3
		Attack4,		// 攻撃4
		BlownAway,		// 吹っ飛び
		Hit,			// 被ダメージ
		Death,			// 死亡
		MAX,
	};

	// アニメーション切り替えにかかる時間
	struct AnimChangeTime {
		int Idle=10;			// 待機
		int Walk=5;				// 歩く
		int Death = 5;			// 死亡
		// 下記仮
		int Run=5;				// 走る
		int Provocation = 5;	// 挑発
		int Attack1 = 5;		// 攻撃1
		int Attack2 = 5;		// 攻撃2
		int Attack3 = 5;		// 攻撃3
		int Attack4 = 5;		// 攻撃4
		int BlownAway = 5;		// 吹っ飛び
		int Hit = 5;			// 被ダメージ
		
	}m_animChangeTime;

	// アニメーションの再生速度
	struct AnimSpeed {
		float Default = 0.5f;		// 基本
		float Idle = 0.5f;			// 待機
		float Walk = 0.5f;			// 歩く
		float Death = 0.5f;			// 死亡
		// 下記仮
		float Run=0.5f;				// 走る
		float Provocation = 0.5f;	// 挑発
		float Attack1 = 0.5f;		// 攻撃1
		float Attack2 = 0.5f;		// 攻撃2
		float Attack3 = 0.5f;		// 攻撃3
		float Attack4 = 0.5f;		// 攻撃4
		float BlownAway = 0.5f;		// 吹っ飛び
		float Hit = 0.5f;			// 被ダメージ
	}m_animSpeed;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy(const std::shared_ptr<Map> pMap, const std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="map">マップ情報</param>
	/// <param name="player">プレイヤー情報</param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Playerのポインタを設定する
	/// </summary>
	/// <param name="pPlayer"></param>
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	/// <param name="player">プレイヤー情報</param>
	void ColUpdate();
	
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="map">マップ情報</param>
	void Move();

	/// <summary>
	/// 移動処理更新
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// 一番近い座標を出す
	/// </summary>
	/// <param name="map">マップ情報</param>
	void SearchNearPosition();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Angle();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	void OnDamage(int damage);

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Death();

	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="anim">選択するアニメーション</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isAnimLoop">ループさせるか</param>
	/// <param name="changeTime">切り替えにかかる時間</param>
	void ChangeAnimNo(const EnemyAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime);

	/// <summary>
	/// アニメーションを待機状態に変更する
	/// </summary>
	void ChangeAnimIdle();

	// SEを鳴らす
	void PlaySE();

	/// <summary>
	/// 座標渡し
	/// </summary>
	/// <returns>座標</returns>
	VECTOR GetPos() const { return m_pos; }

	/// <summary>
	/// 当たり判定渡し
	/// </summary>
	/// <returns>当たり判定</returns>
	Collision GetCol() const { return m_col; }

	/// <summary>
	/// プレイヤーへ攻撃値を渡す
	/// </summary>
	/// <returns></returns>
	int GetAttack() const { return m_attackThePlayer; }

private:

	VECTOR m_startPos;	// 移動開始座標
	VECTOR m_targetPos;	// 目標座標

	VECTOR m_colPos;			// 当たり判定用座標

	VECTOR m_rightShoulderPos;	// 右肩座標
	VECTOR m_rightElbowPos;		// 右肘座標
	VECTOR m_rightHandPos;		// 右手座標

	VECTOR m_leftShoulderPos;	// 左肩座標
	VECTOR m_leftElbowPos;		// 左肘座標
	VECTOR m_leftHandPos;		// 左手座標

	VECTOR m_vecToPlayer;		// プレイヤーまでの座標

	int m_attackTimeCount;	// 攻撃をするまでにかかる時間

	int m_attackThePlayer;		// プレイヤーへの攻撃力

	float m_targetDistance;		// ターゲットポイントまでの距離
	float m_targetMoveDistance;	// ターゲットポイントまでの残りの距離

	bool m_isAttack;				// 攻撃をしたかの判定
	bool m_isAttackToPlayer;		// プレイヤーに攻撃が当たったかの判定
	bool m_isNextTargetPosSearch;	// 次のターゲットポイントを探すフラグ
	bool m_isSearchPlayer;			// 索敵範囲内にプレイヤーを見つけたかどうかのフラグ

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Map> m_pMap;
};