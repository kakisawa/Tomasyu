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
		AttackRightArm1,// 攻撃1
		AttackLeftArm1,	// 攻撃2
		AttackRightArm2,// 攻撃3
		AttackRightArm3,// 攻撃4
		BlownAway,		// 吹っ飛び
		Hit,			// 被ダメージ
		Death,			// 死亡
		MAX,
	};

	// アニメーション切り替えにかかる時間
	struct AnimChangeTime {
		int Idle=10;			// 待機
		int Walk=5;				// 歩く
		int AttackRightArm1 = 5;// 攻撃1
		int AttackLeftArm1 = 5;	// 攻撃2
		int AttackRightArm2 = 5;// 攻撃3
		int AttackRightArm3 = 5;// 攻撃4
		int Death = 5;			// 死亡
		// 下記仮
		int Run=5;				// 走る
		int Provocation = 5;	// 挑発
		int BlownAway = 5;		// 吹っ飛び
		int Hit = 5;			// 被ダメージ
	}m_animChangeTime;

	// アニメーションの再生速度
	struct AnimSpeed {
		float Default = 0.5f;			// 基本
		float Idle = 0.5f;				// 待機
		float Walk = 0.5f;				// 歩く
		float AttackRightArm1 = 0.5f;	// 攻撃1
		float AttackLeftArm1 = 0.5f;	// 攻撃2
		float AttackRightArm2 = 0.5f;	// 攻撃3
		float AttackRightArm3 = 0.5f;	// 攻撃4
		float Death = 0.5f;				// 死亡
		// 下記仮
		float Run=0.5f;					// 走る
		float Provocation = 0.5f;		// 挑発
		float BlownAway = 0.5f;			// 吹っ飛び
		float Hit = 0.5f;				// 被ダメージ
	}m_animSpeed;

	enum AttackKind {
		RightArm1,
		RightArm2,
		RightArm3,
		LeftArm1,
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pMap">マップ情報</param>
	/// <param name="pPlayer">プレイヤー情報</param>
	Enemy(const std::shared_ptr<Map> pMap, const std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Enemy(){}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// データの削除関係
	/// </summary>
	void End() override;

	/// <summary>
	/// Playerのポインタを設定する
	/// </summary>
	/// <param name="pPlayer">プレイヤー情報</param>
	void SetPlayer(std::shared_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	void ColUpdate();
	
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 移動処理更新
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// 一番近い座標を出す
	/// </summary>
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
	/// <param name="damage">ダメージ量</param>
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

private:
	int m_attackTimeCount;	// 攻撃をするまでにかかる時間
	int m_attackKind;		// 攻撃の種類

	float m_targetDistance;		// ターゲットポイントまでの距離
	float m_targetMoveDistance;	// ターゲットポイントまでの残りの距離

	bool m_isMove;					// 動いているかどうかのフラグ
	bool m_isColAttack;				// プレイヤーに攻撃が当たったかの判定
	bool m_isNextTargetPosSearch;	// 次のターゲットポイントを探すフラグ
	bool m_isSearchPlayer;			// 索敵範囲内にプレイヤーを見つけたかどうかのフラグ

	VECTOR m_rightShoulderPos;	// 右肩座標
	VECTOR m_rightElbowPos;		// 右肘座標
	VECTOR m_rightHandPos;		// 右手座標
	VECTOR m_leftShoulderPos;	// 左肩座標
	VECTOR m_leftElbowPos;		// 左肘座標
	VECTOR m_leftHandPos;		// 左手座標

	VECTOR m_targetPos;		// 目標座標
	VECTOR m_vecToPlayer;	// プレイヤーまでの座標
	
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Map> m_pMap;
};