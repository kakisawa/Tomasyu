#include "Shot.h"
#include "Player/Player.h"
#include "Enemy.h"
#include "../Manager/Effect.h"
#include "../Util/Collision.h"
#include "../Time/IsTime.h"
#include "../Util/Score.h"
#include <cassert>
#include <iostream>

namespace {
	const char* kPlayerModelFilePath = "Data/Model/Weapon/bullet.mv1";	// 弾のモデルパス

	constexpr int kBulletExistTime = 150;	// 弾が消えるまでの時間
	constexpr float kBulletSpeed = 30.0f; // 弾の速度
	constexpr float kBulletRad = 2.0f; // 弾の半径

	const VECTOR kBulletScale = VGet(1.0f, 1.0f, 1.0f);	// 弾のサイズ
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);		// ベクトル初期化用
}

Shot::Shot(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy, int attack, int num,int score) :
	m_baseModel(-1),
	m_attack(attack),
	m_maxBullet(num),
	m_bulletNum(num),
	m_score(score),
	m_pPlayer(pPlayer),
	m_pEnemy(pEnemy)
{
	// ベースモデルの読み込み
	m_baseModel = MV1LoadModel(kPlayerModelFilePath);

	// 弾の最大数配列を指定
	m_bullet.resize(m_maxBullet);
	// 弾情報の初期化
	for (auto& bullet : m_bullet)
	{
		// モデルの読み込み
		bullet.m_model = MV1DuplicateModel(m_baseModel);
		assert(bullet.m_model != -1);

		// 座標の初期化
		bullet.m_pos = kInitVec;
		// 移動量の初期化
		bullet.m_move = kInitVec;
		// 角度の初期化
		bullet.m_direction = kInitVec;
		// 消えるまでの時間
		bullet.m_existTime = std::make_shared<IsTime>(kBulletExistTime);
		// 存在しているかどうか
		bullet.m_isExist = false;

		// 当たり判定
		bullet.m_col.TypeChangeCapsuleUpdate(bullet.m_col.m_colBullet,
			bullet.m_pos, bullet.m_colPos, kBulletRad);
		// サイズ設定
		MV1SetScale(bullet.m_model, kBulletScale);
	}
}

Shot::~Shot()
{
	End();
}

void Shot::Init(std::shared_ptr<Score> score)
{
	m_pScore = score;
}

void Shot::Update()
{
	UpdateBullet();

	Attack();
}

void Shot::Draw()
{
	// 弾の描画
	for (auto& bullet : m_bullet)
	{
		// 存在していたら描画する
		if (bullet.m_isExist)
		{
			MV1DrawModel(bullet.m_model);

#ifdef _DEBUG
			bullet.m_col.TypeChangeCapsuleDraw(bullet.m_col.m_colBullet, 0xff0000, false);
#endif // DEBUG
		}
	}
}

void Shot::End()
{
	// モデルの削除
	if (m_baseModel != -1)
	{
		MV1DeleteModel(m_baseModel);
		m_baseModel = -1;
	}
	for (auto& bullet : m_bullet)
	{
		if (bullet.m_model != -1)
		{
			MV1DeleteModel(bullet.m_model);
			bullet.m_model = -1;
		}
	}
}

void Shot::CreateBullet(const VECTOR& direction)
{
	for (auto& bullet : m_bullet)
	{
		// 残弾数が0になったら処理をしない
		if (m_bulletNum <= 0)	return;

		// 弾が存在していなかったら
		if (!bullet.m_isExist)
		{
			// 存在させる
			bullet.m_isExist = true;
			// 座標のセット
			bullet.m_pos = m_pPlayer->GetPos();
			bullet.m_pos.y += 50.0f;
			// 発射する向きの設定
			if (m_pPlayer->GetLockOn()) {
				bullet.m_direction = direction;
			}
			else {
				bullet.m_direction = m_pPlayer->GetTargetDir();
			}
			bullet.m_move = VScale(VNorm(bullet.m_direction), kBulletSpeed);

			// モデルの向きを発射する方向へセットする
			MV1SetRotationXYZ(bullet.m_model, VGet(0.0f, m_pPlayer->GetAngle() + DX_PI_F, 0.0f));

			// 残弾数を減らす
			m_bulletNum--;

			

			break;
		}
	}
}

void Shot::UpdateBullet()
{
	// 位置設定
	for (auto& bullet : m_bullet)
	{
		// 弾が存在していなかったら処理をしない
		if (!bullet.m_isExist) continue;

		// 弾を生成して一定時間経過したら
		if (bullet.m_existTime->Update())
		{
			// 存在を消す
			bullet.m_isExist = false;
			// 時間のカウントを0に戻す
			bullet.m_existTime->Reset();
		}
		else
		{
			/*プレイヤーが初期位置から移動していないと、弾が変な方向へ行く*/
			/*要修正*/

			// 弾の移動
			bullet.m_pos = VAdd(bullet.m_pos, bullet.m_move);
			// 当たり判定
			bullet.m_colPos = VAdd(bullet.m_pos, VScale(bullet.m_move, 2.0f));

			// 弾の位置を設定する
			MV1SetPosition(bullet.m_model, bullet.m_pos);

			// 弾の当たり判定を更新する
			bullet.m_col.TypeChangeCapsuleUpdate(bullet.m_col.m_colBullet,
				bullet.m_pos, bullet.m_colPos, kBulletRad);
		}
	}
}

void Shot::Attack()
{
	// 敵の当たり判定を獲得する
	Collision enemyCol = m_pEnemy->GetCol();

	for (auto& bullet : m_bullet)
	{
		// 弾が存在していなかったら処理をしない
		if (!bullet.m_isExist) continue;

		// 弾が敵と当たっていたら
		if (bullet.m_col.IsTypeChageCupsuleCollision
		(bullet.m_col.m_colBullet, enemyCol.m_colEnemy.m_body))
		{
			VECTOR bulletPos = VAdd(m_pEnemy->GetPos(), VGet(0.0f, 40.0f, 0.0f));

			Effect::GetInstance().AddEffect(EffectKind::kEffectKind::kShot, bulletPos);

			// 敵にダメージを与える
			m_pEnemy->OnDamage(m_attack);
			// スコアを加算する
			m_pScore->AddScore(m_score);

			// 存在を消す
			bullet.m_isExist = false;
		}
		else
		{
			// もう一度処理を行う
			continue;
		}
	}
}
