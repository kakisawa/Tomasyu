#include "Enemy.h"
#include "Player.h"
#include "../Object/Map.h"
#include "../Util/LoadCsv.h"
#include "../Manager/Effect.h"
#include <cmath>
#include <random>
#include <cassert>
#include <algorithm>

namespace {
	constexpr int kAttackHandArm = 30;						// 右腕攻撃力
	constexpr int kAttackMachineArm = 30;					// 左腕攻撃力
	constexpr int kNextAttackTime = 100;					// 次の攻撃をするまでのカウント
	constexpr float kInitAngle = -DX_PI_F / 2.0f * 90.0f;	// プレイヤーの初期角度*90(向きを反対にする)

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);			// Vector値初期価値
	const VECTOR kBodyColUpPos = VGet(0.0f, 70.0f, 0.0f);	// 体当たり判定頂点
	
	// モデルパス
	const char* kModelFilePath = "Data/Model/EnemyModel.mv1";
	// 各攻撃パーツの部位パス
	const char* const kRightShoulder = "mixamorig:RightShoulder";	// 右肩
	const char* const kRightElbow = "mixamorig:RightForeArm";		// 右肘
	const char* const kRightHand = "mixamorig:RightHandMiddle4";	// 右手指
	const char* const kLeftShoulder = "mixamorig:LeftShoulder";		// 左肩
	const char* const kLeftElbow = "mixamorig:LeftForeArm";			// 左肘
	const char* const kLeftHand = "mixamorig:LeftHandMiddle4";		// 左手指
}

Enemy::Enemy(const std::shared_ptr<Map> pMap, const std::shared_ptr<Player> pPlayer) :
	m_attackKind(0),
	m_attackTimeCount(kNextAttackTime),
	m_targetDistance(0.0f),
	m_targetMoveDistance(0.0f),
	m_colPos(kInitVec),
	m_targetPos(kInitVec),
	m_rightShoulderPos(kInitVec),
	m_rightElbowPos(kInitVec),
	m_rightHandPos(kInitVec),
	m_leftShoulderPos(kInitVec),
	m_leftElbowPos(kInitVec),
	m_leftHandPos(kInitVec),
	m_vecToPlayer(kInitVec),
	m_isMove(false),
	m_isAttack(false),
	m_isColAttack(false),
	m_isSearchPlayer(false),
	m_isNextTargetPosSearch(true),
	m_pMap(pMap),
	m_pPlayer(pPlayer)
{
}

void Enemy::Init()
{
	// 素材・データの読み込み
	Load();

	ModelBase::Init();
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	MV1SetScale(m_model, VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize));
	m_angle = kInitAngle;
	m_status.situation.isMoving = false;

	m_hp = m_chara.maxHp;		// HPに最大値を入れる
	m_attack = kAttackHandArm;	// 攻撃力を入れる

	// アニメーションの設定
	SetAnimation(static_cast<int>(EnemyAnim::Idle), m_animSpeed.Idle, true, false);
}

void Enemy::Update()
{
	Death();
	PlaySE();
	ModelBase::Update();

	if (m_status.situation.isDeath) return;

	Attack();
	ColUpdate();
	SearchNearPosition();
	Move();
	Angle();

	// 攻撃が当たっていたらプレイヤーへ攻撃値を渡す
	if (m_isAttack && m_isColAttack)
	{
		Effect::GetInstance().AddEffect(EffectKind::kEffectKind::kHit, m_col.m_colEnemy.m_rightArm->m_pos);
		m_pPlayer->OnDamage(m_attack);
		m_isAttack = false;
	}

	ChangeAnimIdle();
}

void Enemy::Draw()
{
	ModelBase::Draw();

#ifdef _DEBUG
	// 体当たり判定
	m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_body, 0x0000ff, false);
	// 索敵範囲当たり判定
	m_col.TypeChangeSphereDraw(m_col.m_colEnemy.m_search, 0x0000ff, false);

	// 攻撃しているときのみ、腕の当たり判定
	if (m_status.situation.isAttack) {
		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_rightArm[0], 0x00ff00, false);
		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_rightArm[1], 0x00ff00, false);

		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_leftArm[0], 0x00ff00, false);
		m_col.TypeChangeCapsuleDraw(m_col.m_colEnemy.m_leftArm[1], 0x00ff00, false);
	}

	//DrawFormatString(0, 140, 0xffffff, "Enemy:HP=%d", m_hp);
	//DrawFormatString(0, 700, 0xffffff, "Enemy:m_isCheckPlayer=%d", m_isSearchPlayer);
	//DrawFormatString(0, 780, 0xffffff, "Enemy:m_pos.x=%.2f:z=%.2f", m_pos.x, m_pos.z);
	//DrawFormatString(0, 800, 0xffffff, "Enemy:m_targetMoveDistance=%.2f", m_targetMoveDistance);
	//DrawFormatString(0, 820, 0xffffff, "Enemy:m_targetDistance=%.2f", m_targetDistance);
	//DrawFormatString(0, 840, 0xffffff, "Enemy:m_isNextTargetPosSearch=%d", m_isNextTargetPosSearch);
	//DrawFormatString(0, 880, 0xffffff, "Enemy:m_targetPos.x=%.2f:z=%.2f", m_targetPos.x, m_targetPos.z);
	//DrawFormatString(0, 900, 0xffffff, "Enemy:m_move.x=%.2f:z=%.2f", m_move.x, m_move.z);
	//DrawFormatString(0, 920, 0xffffff, "Enemy:m_animNext.animNo=%d", m_animNext.animNo);
	//DrawFormatString(0, 940, 0xffffff, "Enemy:m_isAttackToPlayer=%d", m_isAttackToPlayer);
	//DrawFormatString(0, 960, 0xffffff, "Enemy:m_isAttack=%d", m_isAttack);
	//DrawFormatString(0, 900, 0xffffff, "Enemy:m_angle=%.2f", m_angle);
	//DrawFormatString(0, 920, 0xffffff, "target1.x=%.2f:.z=%.2f", target1.x, target1.z);
	//DrawFormatString(0, 940, 0xffffff, "target2.x=%.2f:.z=%.2f", target2.x, target2.z);
	//DrawFormatString(0, 960, 0xffffff, "target3.x=%.2f:.z=%.2f", target3.x, target3.z);
	//DrawFormatString(0, 980, 0xffffff, "target4.x=%.2f:.z=%.2f", target4.x, target4.z);
#endif // DEBUG
}

void Enemy::Load()
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadCommonFileData(m_chara, "enemy");
	// モデルの読み込み
	m_model = MV1LoadModel(kModelFilePath);
	assert(m_model != -1);

	// SEの初期化・ロード
	m_pSound->InitSE();
	m_pSound->LoadSE(SoundManager::SE_Type::kPunchSE2);
	m_pSound->LoadSE(SoundManager::SE_Type::kDeathrattle);
}

void Enemy::End()
{
	ModelBase::End();
	// サウンドの解放
	m_pSound->ReleaseSound();
}

void Enemy::ColUpdate()
{
	// 死んだら処理しない
	if (m_status.situation.isDeath)return;

	// 当たり判定用座標調整
	m_colPos = VAdd(m_pos, kBodyColUpPos);
	// 体当たり判定更新
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_body, m_pos, m_colPos, m_chara.bodyColRad);

	// 索敵範囲当たり判定更新
	m_col.TypeChangeSphereUpdate(m_col.m_colEnemy.m_search, m_pos, m_chara.searchRad);

	// 接触範囲当たり判定更新
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_hitting, m_pos, m_colPos, 30.0f);

	// プレイヤーの当たり判定獲得
	Collision playerCol = m_pPlayer->GetCol();

	// 索敵範囲とプレイヤーが当たったかどうかの判定
	m_isSearchPlayer = m_col.IsTypeChageSphereToCapsuleCollision(playerCol.m_colPlayer.m_body, m_col.m_colEnemy.m_search);

	// プレイヤーに敵の攻撃が当たったかどうかの判定
	if (m_attackKind == 1)
	{
		m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
	}
	else if (m_attackKind == 2)
	{
		if (m_nextAnimTime <= 28)	return;

		m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_leftArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
		if (!m_isColAttack) {
			m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_leftArm[0], m_pPlayer->GetCol().m_colPlayer.m_body);
		}
	}
	else if (m_attackKind == 3)
	{
		m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
	}
	else if (m_attackKind == 4)
	{
		m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
	}
}

void Enemy::Move()
{
	// 攻撃中もしくは死亡時は処理を通さない
	if (m_status.situation.isAttack || m_status.situation.isDeath) return;

	m_move = kInitVec;

	bool toPlayer = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_hitting, m_pPlayer->GetCol().m_colPlayer.m_body);
	
	if (!toPlayer) {
		if (m_isSearchPlayer)
		{
			m_isNextTargetPosSearch = true;

			m_targetPos = m_pPlayer->GetPos();
			m_vecToPlayer = VSub(m_targetPos, m_pos);

			m_targetDir = VNorm(m_vecToPlayer);
			m_move.x = m_targetDir.x * m_chara.walkSpeed;
			m_move.z = m_targetDir.z * m_chara.walkSpeed;

			// atan2 を使用して角度を取得						// 方向用
			m_angle = atan2(m_targetDir.x, m_targetDir.z);

			// atan2 で取得した角度に３Ｄモデルを正面に向かせるための補正値( DX_PI_F )を
			// 足した値を３Ｄモデルの Y軸回転値として設定
			MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + kInitAngle, 0.0f));
			MV1SetPosition(m_model, m_pos);
		}
		else {

			SearchNearPosition();

			m_pos.x = std::ceil(m_pos.x);
			m_pos.z = std::ceil(m_pos.z);

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

			// 正規化と移動速度の適用
			if (VSize(m_move) > 0.0f)
			{
				m_move = VNorm(m_move); // 正規化
				m_targetDir = m_move;  // 目標方向を保存
				m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用

				// 移動方向に向くように角度を設定
				float targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));
				m_angle = targetAngle;
				MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
			}
		}
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

void Enemy::SearchNearPosition()
{
	// 死亡していたら処理しない
	if (m_status.situation.isDeath)return;

	// エネミーが現在いる位置から、一番近いポイントを探しその地点へ向かう	
	if (m_isNextTargetPosSearch)
	{
		m_isNextTargetPosSearch = false;

		VECTOR target1, target2, target3, target4;

		target1 = VSub(m_pMap->GetPointPos().point1, m_pos);
		target2 = VSub(m_pMap->GetPointPos().point2, m_pos);
		target3 = VSub(m_pMap->GetPointPos().point3, m_pos);
		target4 = VSub(m_pMap->GetPointPos().point4, m_pos);

		float target1_1 = abs(target1.x) + abs(target1.z);
		float target2_1 = abs(target2.x) + abs(target2.z);
		float target3_1 = abs(target3.x) + abs(target3.z);
		float target4_1 = abs(target4.x) + abs(target4.z);

		std::vector<float> values = { target1_1,target2_1,target3_1,target4_1 };

		std::vector<float> positiveValues;
		for (float v : values) {
			if (v > 0.0f)positiveValues.push_back(v);
		}

		float result = positiveValues.empty() ? 0 : *std::min_element(positiveValues.begin(), positiveValues.end());

		 // 最小値を持つターゲットポイントをリストに追加
		std::vector<VECTOR> minTargets;
		if (result == target1_1) {
			minTargets.push_back(m_pMap->GetPointPos().point1);
		}
		if (result == target2_1) {
			minTargets.push_back(m_pMap->GetPointPos().point2);
		}
		if (result == target3_1) {
			minTargets.push_back(m_pMap->GetPointPos().point3);
		}
		if (result == target4_1) {
			minTargets.push_back(m_pMap->GetPointPos().point4);
		}

		// ランダムで1つ選択
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<> rand(0, minTargets.size() - 1);
		m_targetPos = minTargets[rand(mt)];
	}

	m_targetMoveDistance = abs(m_targetPos.x + m_targetPos.z) - abs(m_pos.x + m_pos.z);

	if (m_targetMoveDistance == 0.0f)
	{
		m_isNextTargetPosSearch = true;
	}
}

void Enemy::Angle()
{
	// プレイヤーの移動方向にモデルの方向を近づける
	float targetAngle;		// 目標角度
	float difference;		// 目標角度と現在の角度の差

	// 目標の方向ベクトルから角度値を算出する
	targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));

	// 目標の角度と現在の角度との差を割り出す
	difference = targetAngle - m_angle;

	// 差の角度が180度以上になっていたら修正する
	if (difference < -DX_PI_F)
	{
		difference += DX_TWO_PI_F;
	}
	else if (difference > DX_PI_F)
	{
		difference -= DX_TWO_PI_F;
	}

	// 角度の差が0に近づける
	if (difference > 0.0f)
	{
		// 差がプラスの場合は引く
		difference -= m_chara.rotaSpeed;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// 差がマイナスの場合は足す
		difference += m_chara.rotaSpeed;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// 攻撃アニメーションを再生していないときは角度を変える
	if (!m_status.situation.isAttack)
	{
		// モデルの角度を更新
		m_angle = targetAngle - difference;
		MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
	}
}

void Enemy::Attack()
{
	// 死んだら処理しない
	if (m_status.situation.isDeath) return;

	// 攻撃に使用する腕の場所の獲得
	int rightShoulder = MV1SearchFrame(m_model, kRightShoulder);
	int rightElbow = MV1SearchFrame(m_model, kRightElbow);
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

	if (m_isSearchPlayer)
	{
		// 攻撃してくるまでの間隔
		m_attackTimeCount--;
	}

	// 攻撃までのカウントが0になったら攻撃をする
	if (m_attackTimeCount == 0)
	{
		// ランダムで値を獲得する
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> rand(1, 4 + 1);
		m_attackKind = static_cast<int>(rand(mt));

		m_status.situation.isAttack = true;

		if (m_attackKind == 1)
		{
			ChangeAnimNo(EnemyAnim::AttackRightArm1, m_animSpeed.AttackRightArm1, false, m_animChangeTime.AttackRightArm1);
			// プレイヤーに敵の攻撃が当たったかどうかの判定
			m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
		}
		else if (m_attackKind == 2)
		{
			ChangeAnimNo(EnemyAnim::AttackLeftArm1, m_animSpeed.AttackLeftArm1, false, m_animChangeTime.AttackLeftArm1);
			m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_leftArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
		}
		else if (m_attackKind == 3)
		{
			ChangeAnimNo(EnemyAnim::AttackRightArm2, m_animSpeed.AttackRightArm2, false, m_animChangeTime.AttackRightArm2);
			m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
		}
		else if (m_attackKind == 4)
		{
			ChangeAnimNo(EnemyAnim::AttackRightArm3, m_animSpeed.AttackRightArm3, false, m_animChangeTime.AttackRightArm3);
			m_isColAttack = m_col.IsTypeChageCupsuleCollision(m_col.m_colEnemy.m_rightArm[1], m_pPlayer->GetCol().m_colPlayer.m_body);
		}

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
	// HPが0以上の時は処理しない
	if (m_hp > 0)return;

	// 死亡時処理
	m_status.situation.isDeath = true;
	ChangeAnimNo(EnemyAnim::Death, m_animSpeed.Death, false, m_animChangeTime.Death);
	// 体当たり判定更新
	m_col.TypeChangeCapsuleUpdate(m_col.m_colEnemy.m_body, kInitVec, kInitVec, 0.0f);

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
	if (!m_status.situation.isMoving && !m_status.situation.isDeath && !m_status.situation.isAttack)
	{
		ChangeAnimNo(EnemyAnim::Idle, m_animSpeed.Idle, true, m_animChangeTime.Idle);
	}
}

void Enemy::PlaySE()
{
	// プレイヤーへ攻撃が当たったら殴りSEを鳴らす
	if (m_isAttack && m_isColAttack)
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kPunchSE2, DX_PLAYTYPE_BACK);
	}

	// エネミーが死亡したら断末魔SEを鳴らす
	if (m_status.situation.isDeath)
	{
		m_pSound->PlaySE(SoundManager::SE_Type::kDeathrattle, DX_PLAYTYPE_BACK);
	}
}
