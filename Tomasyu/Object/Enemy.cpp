#include "Enemy.h"
#include "Player.h"
#include "../Object/Map.h"
#include "../Util/LoadCsv.h"
#include <cassert>
#include <cmath>
#include <algorithm>

namespace {
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期価値
	constexpr float kInitFloat = 0.0f;				// float値初期化

	const VECTOR kBodyColUpPos = VGet(0.0f, 70.0f, 0.0f);	// 体当たり判定頂点

	constexpr int kAttackHandArm = 30;				// 右腕攻撃力
	constexpr int kAttackMachineArm = 30;			// 左腕攻撃力

	constexpr int kNextAttackTime = 100;			// 次の攻撃をするまでのカウント

	const char* kModelFilePath = "Data/Model/EnemyModel.mv1";

	bool isMove = true;			// 動いているかどうかのフラグ

	// 各攻撃パーツの部位パス
	const char* const kRightShoulder = "mixamorig:RightShoulder";
	const char* const kRightElbow = "mixamorig:RightForeArm";
	const char* const kRightHand = "mixamorig:RightHandMiddle4";

	const char* const kLeftShoulder = "mixamorig:LeftShoulder";
	const char* const kLeftElbow = "mixamorig:LeftForeArm";
	const char* const kLeftHand = "mixamorig:LeftHandMiddle4";
}

Enemy::Enemy():
	m_attackThePlayer(0),
	m_targetDistance(0.0f),
	m_targetMoveDistance(0.0f),
	m_attackTimeCount(kNextAttackTime),
	m_isAttackToPlayer(0),
	m_colPos(kInitVec),
	m_startPos(kInitVec),
	m_targetPos(kInitVec),
	m_rightShoulderPos(kInitVec),
	m_rightElbowPos(kInitVec),
	m_rightHandPos(kInitVec),
	m_leftShoulderPos(kInitVec),
	m_leftElbowPos(kInitVec),
	m_leftHandPos(kInitVec),
	m_isNextTargetPosSearch(true),
	m_isAttack(false)
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadCommonFileData(m_chara, "enemy");
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// 座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	MV1SetScale(m_model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));

	// 動かない状態
	m_status.situation.isMoving = false;
}

Enemy::~Enemy()
{
	// サウンドの解放
	m_pSound->ReleaseSound();
}

void Enemy::Init()
{
	ModelBase::Init();
	m_hp = m_chara.maxHp;		// HPに最大値を入れる
	m_attack = kAttackHandArm;	// 攻撃力を入れる

	// SEの初期化・ロード
	m_pSound->InitSE();
	m_pSound->LoadSE(SoundManager::SE_Type::kPunchSE2);
	m_pSound->LoadSE(SoundManager::SE_Type::kDeathrattle);

	// アニメーションの設定
	SetAnimation(static_cast<int>(EnemyAnim::Idle), m_animSpeed.Idle,true, false);
}

void Enemy::Update(const Map& map, const Player& player)
{
	// プレイヤーへの攻撃力を初期化
	m_attackThePlayer = 0;

	// エネミーが死んでいなかったら、プレイヤーが攻撃してきた際体力を減らす
	if (!m_status.situation.isDeath)
	{
		m_hp -= player.GetAttack();
	}

	// 更新処理
	Death();
	Attack();
	ColUpdate(player);
	SearchNearPosition(map);
	Move(map);
	PlaySE();

	// 攻撃が当たっていたらプレイヤーへ攻撃値を渡す
	if (m_isAttack && m_isAttackToPlayer)
	{
		m_attackThePlayer = m_attack;
		m_isAttack = false;
	}

	ChangeAnimIdle();
	ModelBase::Update();
}

void Enemy::Draw()
{
	ModelBase::Draw();

#ifdef _DEBUG
	m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_body, 0x0000ff, false);

	if(m_status.situation.isAttack){
		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_rightArm[0], 0x00ff00, false);
		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_rightArm[1], 0x00ff00, false);

		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_leftArm[0], 0x00ff00, false);
		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_leftArm[1], 0x00ff00, false);
	}

	//DrawFormatString(0, 140, 0xffffff, "Enemy:HP=%d", m_hp);
	//DrawFormatString(0, 780, 0xffffff, "Enemy:m_pos.x=%.2f:z=%.2f", m_pos.x, m_pos.z);
	DrawFormatString(0, 800, 0xffffff, "Enemy:m_targetMoveDistance=%.2f", m_targetMoveDistance);
	DrawFormatString(0, 1000, 0xffffff, "Enemy:m_targetDistance=%.2f", m_targetDistance);
	DrawFormatString(0, 1020, 0xffffff, "Enemy:m_isNextTargetPosSearch=%d", m_isNextTargetPosSearch);
	DrawFormatString(0, 1040, 0xffffff, "Enemy:m_startPos.x=%.2f:z=%.2f", m_startPos.x, m_startPos.z);
	DrawFormatString(0, 1060, 0xffffff, "Enemy:m_targetPos.x=%.2f:z=%.2f", m_targetPos.x, m_targetPos.z);
	//DrawFormatString(0, 900, 0xffffff, "Enemy:m_move.x=%.2f:z=%.2f", m_move.x, m_move.z);
	//DrawFormatString(0, 920, 0xffffff, "Enemy:m_animNext.animNo=%d", m_animNext.animNo);
	//DrawFormatString(0, 940, 0xffffff, "Enemy:m_isAttackToPlayer=%d", m_isAttackToPlayer);
	//DrawFormatString(0, 960, 0xffffff, "Enemy:m_isAttack=%d", m_isAttack);
#endif // DEBUG
}

void Enemy::ColUpdate(const Player& player)
{
	// 死んだら処理しない
	if (m_status.situation.isDeath)return;

	// 当たり判定用座標調整
	m_colPos = VAdd(m_pos, kBodyColUpPos);
	// 体当たり判定更新
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_body, m_pos, m_colPos, m_chara.bodyColRad);

	// プレイヤーの当たり判定獲得
	Collision playerCol = player.GetCol();

	// プレイヤーに敵の攻撃が当たったかどうかの判定
	m_isAttackToPlayer = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], playerCol.m_colPlayer.m_body);
}

void Enemy::Move(const Map& map)
{
	// DOTO:↓はまだ挑戦中
		// 出来ることなら、複数ポイントをグルグル回りつつ一定範囲にプレイヤーが近づいたら接近して攻撃するようにしたい

	if (m_status.situation.isAttack) return;

	m_move = kInitVec;
	
	if (isMove)
	{

		if (m_pos.x != m_targetPos.x) {
			if (m_pos.x >= m_targetPos.x)
			{
				m_move.x = -m_chara.walkSpeed;
			}
			else if (m_pos.x <= m_targetPos.x)
			{
				m_move.x = +m_chara.walkSpeed;
			}

			
		}
		else if (m_pos.z != m_targetPos.z)
		{
			if (m_pos.z >= m_targetPos.z)
			{
				m_move.z = -m_chara.walkSpeed;
			}
			else if (m_pos.z <= m_targetPos.z)
			{
				m_move.z = +m_chara.walkSpeed;
			}
		}



	}

	// 正規化と移動速度の適用
	if (VSize(m_move) > 0.0f) 
	{
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}

	m_pos = VAdd(m_pos, m_move);

	// 移動処理の更新
	MoveUpdate();
}

void Enemy::MoveUpdate()
{
	// 移動値を入れる
	float movingSpeed = std::max(abs(m_move.x), abs(m_move.z));

	// プレイヤーの移動状態を初期化する
	m_status.situation.isMoving = false;

	// 移動値があった場合
	if (movingSpeed != 0.0f) 
	{

		// プレイヤーの移動状態をtrueにする
		m_status.situation.isMoving = true;

		// プレイヤーが移動している時のみ、移動アニメーションを入れる
		ChangeAnimNo(EnemyAnim::Walk, m_animSpeed.Walk, true, m_animChangeTime.Walk);
	}
}

void Enemy::SearchNearPosition(const Map& map)
{
	// エネミーが現在いる位置から、一番近いポイントを探しその地点へ向かう
	// DOTO:こちらも挑戦中の為、コメント無し
	
	if (m_isNextTargetPosSearch)
	{
		m_isNextTargetPosSearch = false;

		m_startPos = m_pos;

		VECTOR target1;
		VECTOR target2;
		VECTOR target3;
		VECTOR target4;

		target1 = VSub(map.GetPointPos().point1, m_pos);
		target2 = VSub(map.GetPointPos().point2, m_pos);
		target3 = VSub(map.GetPointPos().point3, m_pos);
		target4 = VSub(map.GetPointPos().point4, m_pos);

		float target1_1;
		float target2_1;
		float target3_1;
		float target4_1;

		target1_1 = abs(target1.x) + abs(target1.z);
		target2_1 = abs(target2.x) + abs(target2.z);
		target3_1 = abs(target3.x) + abs(target3.z);
		target4_1 = abs(target4.x) + abs(target4.z);

		m_targetDistance = std::min({ target1_1,target2_1,target3_1,target4_1 });
		
		if (m_targetDistance == target1_1)
		{
			m_targetPos = map.GetPointPos().point1;
		}
		else if (m_targetDistance == target2_1)
		{
			m_targetPos = map.GetPointPos().point2;
		}
		else if (m_targetDistance == target3_1)
		{
			m_targetPos = map.GetPointPos().point3;
		}
		else if (m_targetDistance == target4_1)
		{
			m_targetPos = map.GetPointPos().point4;
		}
	}

	if (m_targetMoveDistance >= abs(m_targetDistance))
	{
		m_isNextTargetPosSearch = true;
	}

	m_targetMoveDistance = abs( (m_startPos.x + m_startPos.z)- (m_pos.x + m_pos.z));
}

void Enemy::Attack()
{
	// 死んだら処理しない
	if (m_status.situation.isDeath) return;

	// 攻撃に使用する腕の場所の獲得
	int rightShoulder = MV1SearchFrame(m_model, kRightShoulder);
	int rightElbow= MV1SearchFrame(m_model, kRightElbow);
	int rightHand = MV1SearchFrame(m_model, kRightHand);

	int leftShoulder = MV1SearchFrame(m_model, kLeftShoulder);
	int leftElbow = MV1SearchFrame(m_model, kLeftElbow);
	int leftHand = MV1SearchFrame(m_model, kLeftHand);

	// 攻撃をする腕の場所をセット
	m_rightShoulderPos = MV1GetFramePosition(m_model, rightShoulder);
	m_rightElbowPos = MV1GetFramePosition(m_model, rightElbow);
	m_rightHandPos = MV1GetFramePosition(m_model, rightHand);

	m_leftShoulderPos = MV1GetFramePosition(m_model, leftShoulder);
	m_leftElbowPos = MV1GetFramePosition(m_model, leftElbow);
	m_leftHandPos = MV1GetFramePosition(m_model, leftHand);

	// 当たり判定の更新
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_rightArm[0], m_rightShoulderPos, m_rightElbowPos, 6);
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_rightArm[1], m_rightElbowPos, m_rightHandPos, 6);
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_leftArm[0], m_leftShoulderPos, m_leftElbowPos, 6);
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_leftArm[1], m_leftElbowPos, m_leftHandPos, 6);
	
	// 攻撃してくるまでの間隔
	m_attackTimeCount--;

	// 攻撃までのカウントが0になったら攻撃をする
	if (m_attackTimeCount == 0)
	{
		m_status.situation.isAttack = true;
		ChangeAnimNo(EnemyAnim::Attack1, m_animSpeed.Attack1, false, m_animChangeTime.Attack1);
		
		m_isAttack = true;	
	}

	// 攻撃アニメーションが終了したら、再度攻撃までのカウントを行う
	if (m_status.situation.isAttack && IsAnimEnd())
	{
		m_status.situation.isAttack = false;
		m_attackTimeCount = kNextAttackTime;
		m_isAttack = false;
	}
}

void Enemy::OnDamage(int damage)
{
	m_hp -= damage;
}

void Enemy::Death()
{
	// 死亡時処理
	if (m_hp <= 0) 
	{
		m_status.situation.isDeath = true;
		ChangeAnimNo(EnemyAnim::Death, m_animSpeed.Death, false, m_animChangeTime.Death);
		// 体当たり判定更新
		m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_body, kInitVec, kInitVec, kInitFloat);
	}

	// 死亡アニメーションが終わったら死亡フラグをtrueにする
	if (m_status.situation.isDeath && IsAnimEnd())
	{
		m_deathFlag = true;
	}
}


void Enemy::ChangeAnimNo(const EnemyAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.animSpeed = animSpeed;
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, animSpeed, m_status.isLoop, false, changeTime);
}

void Enemy::ChangeAnimIdle()
{
	// 待機アニメーションに変更する
	if (!m_status.situation.isMoving&&!m_status.situation.isDeath&&!m_status.situation.isAttack) 
	{
		ChangeAnimNo(EnemyAnim::Idle, m_animSpeed.Idle, true, m_animChangeTime.Idle);
	}
}

void Enemy::PlaySE()
{
	// プレイヤーへ攻撃が当たったら殴りSEを鳴らす
	if (m_isAttack && m_isAttackToPlayer)
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kPunchSE2, DX_PLAYTYPE_BACK);
	}

	// エネミーが死亡したら断末魔SEを鳴らす
	if (m_status.situation.isDeath) 
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kDeathrattle, DX_PLAYTYPE_BACK);
	}
}
