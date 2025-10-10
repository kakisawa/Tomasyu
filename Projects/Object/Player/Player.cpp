#include "Player.h"
#include "../Enemy.h"
#include "../Camera/Camera.h"
#include "../Shot.h"
#include "../Util/Score.h"
#include "../Util/Input.h"
#include "../Util/LoadCsv.h"
#include "../Manager/Effect.h"
#include "../Manager/SoundManager.h"
#include <cmath>
#include <random>
#include <cassert>

using namespace MyInputInfo;

namespace
{
	const char* kAmingHandlePath = "Data/Image/SceneGame/Aiming.png";	// 照準画像パス

	const char* kPlayerModelFilePath = "Data/Model/PlayerModel.mv1";	// プレイヤーモデルパス
	const char* kModelRightHandMiddle = "mixamorig:RightHandMiddle4";	// ハンドガン用右手パス
	const char* kModelRightHandRing3 = "mixamorig:RightHandRing3";		// マシンガン用右手パス
	const char* kModelRightHandRing4 = "mixamorig:RightHandRing4";		// ナイフ用右手パス
	const char* kKnifeModelTip = "Knife_Tip";							// ナイフ先端パス

	const char* const kWeaponPath[3] =
	{
		"Data/Model/Weapon/HandGun.mv1",	// ハンドガン用パス
		"Data/Model/Weapon/MachineGun.mv1",	// マシンガン用パス
		"Data/Model/Weapon/new_Knife.mv1",		// ナイフ用パス
	};

	constexpr int kSecondAttackTime = 25;		// 2コンボ目の攻撃の入力受付時間
	constexpr int kThirdAttackTime = 40;		// 3コンボ目の攻撃の入力受付時間

	// 銃関係
	/*外部データ化するか悩んだけど、少量の為保留*/
	constexpr int kMaxBulletHandGun = 50;		// ハンドガン最大弾数
	constexpr int kMaxBulletMachineGun = 100;	// マシンガン最大弾数
	constexpr int kAttackHandGun = 10;			// ハンドガン攻撃力
	constexpr int kAttackMachineGun = 3;		// マシンガン攻撃力
	constexpr int kRecoveryHandGun = 15;		// ハンドガン残弾回復量
	constexpr int kRecoveryMachineGun = 40;		// マシンガン残弾回復量
	constexpr int kScoreHandGun = 30;			// ハンドガンスコア
	constexpr int kScoreMachineGun = 10;		// マシンガンスコア

	constexpr int kScoreKnife = 500;			// ナイフスコア
	constexpr int kMaxItemNum = 3;				// 所持できるアイテムの最大数
	int useItem = 0;

	constexpr int kMedicRecoveryAmount = 10;	// 回復量
	constexpr float kMaxStamina = 100.0f;		// スタミナ最大値
	constexpr float kStaminaRecovery = 0.1f;	// スタミナ回復量
	
	constexpr float kStaminaConsumption = 25.0f;// 回避時スタミナ消費量
	constexpr float kInvincibleTime = 15.0f;	// 回避時の無敵時間

	constexpr float kAimingSize = 100.0f;		// 照準画像のサイズ

	VECTOR kAimingPos = VGet(960.0f, 480.0f, 0.0f);	// 照準座標

	constexpr float kInitFloat = 0.0f;				// float値初期化
	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// Vector値初期化値
}

Player::Player(std::shared_ptr<Camera> pCamera, std::shared_ptr<Enemy> pEnemy, std::shared_ptr<Item> pItem) :
	m_aimingHandle(-1),
	m_useItem(0),
	m_getItem(0),
	m_machineGunCount(0),
	m_inputX(0),
	m_inputY(0),
	m_is3Combo(false),
	m_isItem(false),
	m_isLookOn(false),
	m_isEnemy(false),
	m_isAttackToEnemy(false),
	m_isInvincibleTime(false),
	m_isHandGunAnim(false),
	m_roll(kInitVec),
	m_targetLockPos(kInitVec),
	m_rightHandPos(kInitVec),
	m_KnifeTipPos(kInitVec),
	m_setItem(Item::ItemKind::NoItem),
	m_useWeapon(WeaponKind::HandGun),
	m_SetComboknife(Knife::Attack1),
	m_pCamera(pCamera),
	m_pEnemy(pEnemy)
{
	// データの読み込みを行う
	LoadData();

	// プレイヤーモデルの座標初期値
	m_pos = VGet(m_chara.initPosX, m_chara.initPosY, m_chara.initPosZ);
	// プレイヤーモデルのサイズ初期化
	m_size = VGet(m_chara.modelSize, m_chara.modelSize, m_chara.modelSize);


	// プレイヤーの状態初期化
	m_status.situation.isMoving = false;		// 動いていない
	m_status.situation.isInstallation = false;	// 罠を仕掛けていない

	WeaponInfoSet();
}

Player::~Player()
{
	m_pShotHandGun.reset();
	m_pShotMachineGun.reset();
}

void Player::Init(std::shared_ptr<Score> score)
{
	ModelBase::Init();

	/*銃生成*/
	// ハンドガン
	m_pShotHandGun = std::make_shared<Shot>(shared_from_this(), m_pEnemy,
		kAttackHandGun, kMaxBulletHandGun, kScoreHandGun);

	// マシンガン
	m_pShotMachineGun = std::make_shared<Shot>(shared_from_this(), m_pEnemy,
		kAttackMachineGun, kMaxBulletMachineGun, kScoreMachineGun);

	m_pScore = score;			// スコアを入れる

	m_hp = m_chara.maxHp;		// HPに最大値を入れる
	m_stamina = kMaxStamina;		// スタミナに最大値を入れる
	m_attack = m_playerData["knife"].attack;	// 攻撃力にナイフの攻撃力を入れる


	m_pShotHandGun->Init(m_pScore);
	m_pShotMachineGun->Init(m_pScore);

	// 初期アニメーションの設定(待機状態)
	SetAnimation(static_cast<int>(PlayerAnim::Idle), m_animSpeed.Idle, true, false);

	// 仮設定
	m_item[0] = Item::ItemKind::NoItem;
	m_item[1] = Item::ItemKind::NoItem;
	m_item[2] = Item::ItemKind::NoItem;

#ifdef _DEBUG
#endif
}

void Player::Update(Input& input)
{
	m_isEnemy = false;
	InitTutorialActionFlag();	// チュートリアル用行動フラグを初期化する

	// スタミナ回復処理
	if (m_stamina < kMaxStamina) 
	{
		m_stamina += kStaminaRecovery;
	}

	// 銃の残弾セット
	SetRemainingBulletsHandgun();
	SetRemainingBulletsMachinegun();

	// 当たり判定の更新
	ColUpdate();
	// アイテムの獲得
	GetItem();

	// 更新処理
	Gravity();
	Move();
	UseItem(input);
	Angle();
	LockOn(input);
	Roll(input);
	Hit();
	Death();

	SetModelFramePosition(m_model, kModelRightHandMiddle, m_weapon[0], m_weaponSize[0], m_weaponRota[0]);
	SetModelFramePosition(m_model, kModelRightHandRing3, m_weapon[1], m_weaponSize[1], m_weaponRota[1]);
	SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], m_weaponRota[2]);

	// 攻撃
	AttackKnife(input);
	AttackGun(input);
	ChangeWeapon(input);

	m_pShotHandGun->Update();
	m_pShotMachineGun->Update();

	// SEを鳴らす
	PlaySE();

	// 攻撃が当たっていたらエネミーへ攻撃値を渡す
	if (m_isAttack && m_isAttackToEnemy)
	{
		Effect::GetInstance().AddEffect(EffectKind::kEffectKind::kKnife, m_KnifeTipPos);

		m_pScore->AddScore(kScoreKnife);
		m_pEnemy->OnDamage(m_attack);
		m_isAttack = false;
	}

	SetTutorialActionFlag();

	// アニメーションの更新
	ChangeAnimIdle();
	ModelBase::Update();
}

void Player::Draw()
{
	ModelBase::Draw();
	MV1DrawModel(m_weapon[0]);
	MV1DrawModel(m_weapon[1]);
	MV1DrawModel(m_weapon[2]);

	m_pShotHandGun->Draw();
	m_pShotMachineGun->Draw();


	// 照準の描画
	if (m_isLookOn) 
	{
		DrawExtendGraphF(kAimingPos.x - kAimingSize / 2 , kAimingPos.y - kAimingSize / 2,
			kAimingPos.x + kAimingSize / 2, kAimingPos.y + kAimingSize / 2,
			m_aimingHandle, true);
	}
	

#ifdef _DEBUG
	//DrawFormatString(0, 60, 0xffffff, "Playe:HP=%d", m_hp);
	//DrawFormatString(0, 200, 0xffffff, "Playe:m_stamina=%.2f", m_stamina);
	//DrawFormatString(0, 300, 0xffffff, "Player:m_pos.x=%.2f:z=%.2f", m_pos.x,m_pos.z);
	//DrawFormatString(0, 220, 0xffffff, "Player:m_attack=%d", m_attack);
	//DrawFormatString(0, 240, 0xffffff, "Player:m_remainingBullets_handgun=%d", m_remainingBullets_handgun);
	//DrawFormatString(0, 260, 0xffffff, "Player:m_remainingBullets_machinegun=%d", m_remainingBullets_machinegun);
	DrawFormatString(0, 280, 0xffffff, "Player:m_angle=%.2f", m_angle);
	//DrawFormatString(0, 300, 0xffffff, "Player:m_move.x/y/z=%.2f/%.2f/%.2f", m_move.x, m_move.y, m_move.z);
	//DrawFormatString(0, 340, 0xffffff, "Player:m_targetDir.x=%.2f,y=%.2f,z=%.2f,", m_targetDir.x, m_targetDir.y, m_targetDir.z);
	//DrawFormatString(0, 360, 0xffffff, "Player:inputX=%d", m_inputX);
	//DrawFormatString(0, 380, 0xffffff, "Player:inputY=%d", m_inputY);
	//DrawFormatString(0, 400, 0xffffff, "Player:item=%d", m_useItem);
	//DrawFormatString(0, 420, 0xffffff, "Player:m_isItem=%d", m_isItem);
	//DrawFormatString(0, 440, 0xffffff, "Player:m_getItem=%d", m_getItem);
	//DrawFormatString(0, 460, 0xffffff, "Player:itemCount=%d", m_getitemCount);
	//DrawFormatString(0, 480, 0xffffff, "Player:m_useItem[0]=%d", m_item[0]);
	//DrawFormatString(0, 500, 0xffffff, "Player:m_useItem[1]=%d", m_item[1]);
	//DrawFormatString(0, 520, 0xffffff, "Player:m_useItem[2]=%d", m_item[2]);
	//DrawFormatString(0, 540, 0xffffff, "Player:m_useWeapon=%d", m_useWeapon);
	/*DrawFormatString(0, 560, 0xffffff, "Player:m_animNext.totalTime=%.2f", m_animNext.totalTime);
	DrawFormatString(0, 580, 0xffffff, "Player:m_nextAnimTime=%.2f", m_nextAnimTime);*/
	//DrawFormatString(0, 640, 0xffffff, "Player:m_status.situation.isKnifeAttack=%d", m_status.situation.isKnifeAttack);
	//DrawFormatString(0, 660, 0xffffff, "Player:m_isEnemy=%d", m_isEnemy);
	//DrawFormatString(0, 680, 0xffffff, "Player:m_isAttackToEnemy=%d", m_isAttackToEnemy);
	//DrawFormatString(0, 700, 0xffffff, "Player:m_isAttack=%d", m_isAttack);
	//DrawFormatString(0, 720, 0xffffff, "Player:m_attackTheEnemy=%d", m_attackTheEnemy);
	//DrawFormatString(0, 640, 0xffffff, "Player:m_status.situation.isRoll=%d", m_status.situation.isRoll);
	//DrawFormatString(0, 660, 0xffffff, "Player:m_status.situation.isDamageReceived=%d", m_status.situation.isDamageReceived);

	// 体の当たり判定描画
	m_col.TypeChangeCapsuleDraw(m_col.m_colPlayer.m_body, 0xffff00, false);

	// ナイフ当たり判定の描画
	if (m_status.situation.isKnifeAttack) {
		m_col.TypeChangeCapsuleDraw(m_col.m_colPlayer.m_weapon, 0xff0000, true);
	}
#endif // DEBUG
}

void Player::LoadData()
{
	// プレイヤー外部データ読み込み
	LoadCsv::GetInstance().LoadCommonFileData(m_chara, "player");
	LoadCsv::GetInstance().LoadPlayerOnlyFileData(m_playerData);
	// プレイヤーモデルの読み込み
	m_model = MV1LoadModel(kPlayerModelFilePath);
	assert(m_model != -1);

	// 武器モデルの読み込み
	for (int i = 0; i < m_weapon.size(); i++) 
	{
		m_weapon[i] = MV1LoadModel(kWeaponPath[i]);
		assert(m_weapon[i] != -1);
	}

	m_aimingHandle = LoadGraph(kAmingHandlePath);
	assert(m_aimingHandle != -1);

	// SEの初期化・読み込み
	m_pSound->InitSE();
	m_pSound->LoadSE(SoundManager::SE_Type::kKnifeSE);
	m_pSound->LoadSE(SoundManager::SE_Type::kHandGunSE);
}

void Player::End()
{
	ModelBase::End();
	m_pShotHandGun->End();
	m_pShotMachineGun->End();

	// 武器モデルの削除
	for (int i = 0; i < m_weapon.size(); i++) 
	{
		MV1DeleteModel(m_weapon[i]);
	}

	DeleteGraph(m_aimingHandle);

	// サウンドの解放
	m_pSound->ReleaseSound();
}

void Player::SetModelFramePosition(int ModelHandle, const char* FrameName, int SetModelHandle, VECTOR ModelSize, VECTOR ModelRota)
{
	MATRIX FrameMatrix;
	int FrameIndex;
	MATRIX RotateMatrix;

	// フレーム名からフレーム番号を取得する
	FrameIndex = MV1SearchFrame(ModelHandle, FrameName);

	// フレームの現在のワールドでの状態を示す行列を取得する
	FrameMatrix = MV1GetFrameLocalWorldMatrix(ModelHandle, FrameIndex);

	// 角度を回転させる為
	CreateRotationXYZMatrix(&RotateMatrix, ModelRota.x, ModelRota.y, ModelRota.z);
	MV1SetMatrix(SetModelHandle, MMult(MMult(RotateMatrix, MGetScale(ModelSize)), FrameMatrix));
}

void Player::Move()
{
	// 移動できる条件の時
	bool isItem = m_status.situation.isUseItem && !m_status.situation.isReload;
	bool isNotMove = isItem || m_status.situation.isKnifeAttack || m_status.situation.isRoll || m_status.situation.isDamageReceived;

	if (isNotMove)	return;

	// カメラの向きベクトルを取得
	VECTOR cameraForwardVec = VSub(m_pCamera->GetTarget(), m_pCamera->GetPosition());
	cameraForwardVec.y = 0.0f; // 水平成分のみ考慮する
	cameraForwardVec = VNorm(cameraForwardVec); // 正規化

	// カメラの右方向ベクトルを取得（forwardベクトルとY軸上ベクトルの外積で算出）
	VECTOR cameraRightVec = VCross(cameraForwardVec, VGet(0.0f, 1.0f, 0.0f));

	// 入力の初期化
	m_move = kInitVec;
	m_inputX = m_inputY = 0;

	// 入力取得
	GetJoypadAnalogInput(&m_inputX, &m_inputY, DX_INPUT_KEY_PAD1);

	// カメラ基準でプレイヤーの移動ベクトルを設定
	m_move = VScale(cameraForwardVec, static_cast<float>(-m_inputY));  // 前後移動
	m_move = VAdd(m_move, VScale(cameraRightVec, static_cast<float>(-m_inputX)));  // 左右移動

	// 正規化と移動速度の適用
	if (VSize(m_move) > 0.0f)
	{
		m_move = VNorm(m_move); // 正規化
		m_targetDir = m_move;  // 目標方向を保存
		m_move = VScale(m_move, m_chara.walkSpeed); // 移動速度を適用
	}
	// 設置アニメーションを再生していないときは移動する
	m_pos = VAdd(m_pos, m_move);

	m_pos.y = m_pos.y + m_gravity;

	// 移動処理の更新
	MoveUpdate();
}

void Player::MoveUpdate()
{
	// 移動値を入れる
	float movingSpeed = std::max(m_move.x, m_move.z);

	// プレイヤーの移動状態を初期化する
	m_status.situation.isMoving = false;

	// 移動値があった場合
	if (movingSpeed != 0.0f)
	{
		// プレイヤーの移動状態をtrueにする
		m_status.situation.isMoving = true;

		// プレイヤーが罠設置状態でないとき、走るアニメーションを入れる
		if (!m_status.situation.isUseItem && !m_status.situation.isGunAttack && !m_status.situation.isKnifeAttack && !m_status.situation.isRoll
			&& !m_status.situation.isDamageReceived)
		{
			ChangeAnimNo(PlayerAnim::Run, m_animSpeed.Run, true, m_animChangeTime.Idle);
		}
	}
}

void Player::Gravity()
{
	// 地面に埋まったり宙に浮かないようにする
	if (m_pos.y > 0.0f|| m_pos.y < 0.0f) 
	{
		m_gravity = 0.0f;
	}
}

void Player::Angle()
{
	// ロックオン中は向きを変更しない
	//if (m_isLookOn)	return;

	// プレイヤーの移動方向にモデルの方向を近づける
	float targetAngle;		// 目標角度
	float difference;		// 目標角度と現在の角度の差

	if (m_isLookOn) {

		VECTOR playerPos = GetPos();
		VECTOR targetPos = m_pEnemy->GetPos();
		VECTOR dirToEnemy = VSub(targetPos, playerPos);

		targetAngle = static_cast<float>(atan2(dirToEnemy.x, dirToEnemy.z));
	}
	else {
		targetAngle = static_cast<float>(atan2(m_targetDir.x, m_targetDir.z));
	}

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

	// 設置アニメーションを再生していないときは角度を変える
	if (!m_status.situation.isUseItem || m_status.situation.isReload)
	{
		// モデルの角度を更新
		m_angle = targetAngle - difference;
		MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
	}
}

void Player::ColUpdate()
{
	if (m_status.situation.isDeath)return;

	// プレイヤーの当たり判定更新
	m_colPos = VAdd(m_pos, VGet(0.0f, m_chara.bodyColUpY, 0.0f));
	m_col.TypeChangeCapsuleUpdate(m_col.m_colPlayer.m_body, m_pos, m_colPos, m_chara.bodyColRad);

	// 敵・アイテムの当たり判定獲得
	Collision enemyCol = m_pEnemy->GetCol();

	// 敵とプレイヤーが当たったかどうかの判定
	if (m_col.IsTypeChageCupsuleCollision(m_col.m_colPlayer.m_body, enemyCol.m_colEnemy.m_body))
	{
		// 当たっていたらその分押し返す
		m_isEnemy = true;
		VECTOR colNormal = VNorm(VSub(m_pos, m_pEnemy->GetPos()));
		m_pos = VAdd(m_pos, VScale(colNormal, m_chara.walkSpeed));
	}

	// 敵にプレイヤーのナイフ攻撃が当たったかどうかの判定
	m_isAttackToEnemy = m_col.IsTypeChageCupsuleCollision(m_col.m_colPlayer.m_weapon, enemyCol.m_colEnemy.m_body);
}

void Player::GetItem()
{
	// アイテムと当たっていなかったら処理しない
	if (!m_isItem) return;

	// ランダムで値を獲得する
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> rand(1, kItemKind + 1);
	m_getItem = static_cast<int>(rand(mt));

	ItemChange();

	m_tutorial.isTGetItem = true;	// チュートリアル用アイテム獲得フラグをtrueにする
}

void Player::ItemChange()
{
	// 獲得したアイテムの値をセット用の値の中に入れる
	m_setItem = static_cast<Item::ItemKind>(m_getItem);

	// 空のアイテムに獲得したアイテムを入れる
	for (int i = 0; i < kMaxItemNum; i++)
	{
		if (m_item[i] == Item::ItemKind::NoItem)
		{
			m_item[i] = m_setItem;
			break;
		}
	}
}

void Player::UseItem(Input& input)
{
	// Xボタンでアイテムを切り替える
	if (input.IsTrigger(InputInfo::UseItemChange))
	{
		m_useItem++;
		if (m_useItem >= kMaxItemNum)
		{
			m_useItem = 0;
		}
		m_tutorial.isTChangeItem = true;	// チュートリアル用アイテム切り替えフラグをtrueにする
	}

	// Yボタンでアイテムを使う
	if (input.IsTrigger(InputInfo::UseItem))
	{
		// アイテムを持っていなかったら処理をしない
		if (m_item[m_useItem] == Item::ItemKind::NoItem)	return;

		useItem = m_useItem;

		// 状態をアイテム使用中にする
		m_status.situation.isUseItem = true;

		// 使用アイテムが罠系だった場合
		{
			// 使用するアイテムが氷床だった場合
			if (m_item[m_useItem] == Item::ItemKind::IceFloor)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}

			// 使用するアイテムが地雷だった場合
			if (m_item[m_useItem] == Item::ItemKind::landmine)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}

			// 使用するアイテムがびっくり箱だった場合
			if (m_item[m_useItem] == Item::ItemKind::SurpriseBox)
			{
				//プレイヤーの罠設置状態をtrueにする
				m_status.situation.isInstallation = true;
				// 罠設置アニメーションを入れる
				ChangeAnimNo(PlayerAnim::Installation, m_animSpeed.Installation, false, m_animChangeTime.Installation);
			}
		}


		// 使用するアイテムが体力回復だった場合
		if (m_item[m_useItem] == Item::ItemKind::RecoveryMedic)
		{
			Effect::GetInstance().AddEffect(EffectKind::kEffectKind::kRecovery, m_pos);

			// プレイヤーの飲む状態をtrueにする
			m_status.situation.isDrink = true;
			// 飲むアニメーションを入れる
			ChangeAnimNo(PlayerAnim::Drink, m_animSpeed.Drink, false, m_animChangeTime.Drink);
			if (m_hp < m_chara.maxHp)
			{
				m_hp += std::min(m_chara.maxHp - m_hp, kMedicRecoveryAmount);
			}
		}

		// 使用するアイテムが弾再装填アイテムだった場合
		if (m_item[m_useItem] == Item::ItemKind::Ammunition)
		{
			// プレイヤーの弾再装填状態をtrueにする
			m_status.situation.isReload = true;
			// 弾再装填のアニメーションを入れる
			ChangeAnimNo(PlayerAnim::Reload, m_animSpeed.Reload, false, m_animChangeTime.Reload);

			if (m_remainingBulletsMachinegun < kMaxBulletMachineGun) {
				m_pShotMachineGun->SetAddBullet(std::min(kMaxBulletMachineGun - m_remainingBulletsMachinegun, 50));
			}
			else {
				m_pShotHandGun->SetAddBullet(std::min(kMaxBulletHandGun - m_remainingBulletsHandgun, 20));
			}
		}
	}

	// プレイヤーがアイテム使用状態かつ、アイテム使用アニメーションが終了したら
	if (m_status.situation.isUseItem && IsAnimEnd())
	{
		// プレイヤーのアイテム使用状態を初期化する
		m_status.situation.isInstallation = false;
		m_status.situation.isDrink = false;
		m_status.situation.isReload = false;
		m_status.situation.isUseItem = false;

		m_item[useItem] = Item::ItemKind::NoItem;
	}
}

void Player::LockOn(Input& input)
{
	m_isLookOn = false;
	if (input.IsPress(InputInfo::TargetLockOn))
	{
		m_targetLockPos = m_pEnemy->GetPos();
		m_isLookOn = true;

		// プレイヤーの向きを敵の方向に向ける
		VECTOR playerPos = GetPos();
		VECTOR enemyPos = m_pEnemy->GetPos();

		// プレイヤーから敵へのベクトルを計算
		VECTOR directionToEnemy = VSub(enemyPos, playerPos);

		// ベクトルから角度を計算
		//float targetAngle = static_cast<float>(atan2(directionToEnemy.x, directionToEnemy.z));

		//// プレイヤーの向きを更新
		//m_angle = targetAngle;
		//MV1SetRotationXYZ(m_model, VGet(0.0f, m_angle + DX_PI_F, 0.0f));

		// カメラの注視点を敵の位置に設定
		m_pCamera->SetTarget(enemyPos);
	}
	else if (input.IsRelease(InputInfo::TargetLockOn))
	{
		m_isLookOn = false;
	}
}

void Player::ChangeWeapon(Input& input)
{
	// 武器を使用中は処理を行わない
	if (m_status.situation.isGunAttack || m_status.situation.isKnifeAttack)	return;

	if (input.IsTrigger(InputInfo::ChangeWeapon))
	{
		// 現在の武器がハンドガンなら、マシンガンへ変更する
		if (m_useWeapon == WeaponKind::HandGun)
		{
			m_useWeapon = WeaponKind::MachineGun;
		}// 現在の武器がマシンガンなら、ナイフへ変更する
		else if (m_useWeapon == WeaponKind::MachineGun)
		{
			m_useWeapon = WeaponKind::Knife;
		}// 現在の武器がナイフなら、ハンドガンへ変更する
		else if (m_useWeapon == WeaponKind::Knife)
		{
			m_useWeapon = WeaponKind::HandGun;
		}

		m_tutorial.isTChangeWeapon = true;	// チュートリアル用武器切り替えフラグをtrueにする
	}
}

void Player::AttackGun(Input& input)
{
	bool cannotAttackGun = m_status.situation.isRoll || m_status.situation.isDamageReceived || m_status.situation.isUseItem;	// true=攻撃できない false=攻撃できる
	
	// 銃攻撃できない場合または使用武器がナイフの場合処理しない
	if (cannotAttackGun || m_useWeapon == WeaponKind::Knife) return;

	m_status.situation.isGunAttack = false;

	if (input.IsRelease(InputInfo::Attack))
	{
		m_isHandGunAnim = false;
	}

	if (input.IsPress(InputInfo::Attack) && (m_useWeapon == WeaponKind::HandGun || m_useWeapon == WeaponKind::MachineGun))
	{
		m_status.situation.isGunAttack = true;

		if (m_useWeapon == WeaponKind::HandGun)
		{
			if (m_pShotHandGun->GetBulletNum() > 0 )
			{
				StartJoypadVibration(input.IsPress(InputInfo::Attack), 30, 5);
			}

			SetModelFramePosition(m_model, kModelRightHandRing3, m_weapon[0], m_weaponSize[0], m_weaponRota[0]);
			MV1SetVisible(m_weapon[0], true);

			ChangeAnimNo(PlayerAnim::HandGun2, m_animSpeed.HandGun, true, m_animChangeTime.HandGun);

			if (m_isLoopFinish)
			{
				m_isHandGunAnim = false;
			}

			if (m_nextAnimTime < 1.0f && m_isHandGunAnim == false)
			{
				if (m_pShotHandGun->GetBulletNum() > 0)
				{
					m_pSound->PlaySE(SoundManager::SE_Type::kHandGunSE, DX_PLAYTYPE_BACK);
					m_isHandGunAnim = true;

					// ロックオン状態の場合、弾の発射方向を敵の方向に設定
					VECTOR shotDirection = m_targetDir;
					if (m_isLookOn)
					{
						VECTOR enemyPos = m_pEnemy->GetPos();
						VECTOR playerPos = GetPos();
						shotDirection = VNorm(VSub(enemyPos, playerPos));
					}

					m_pShotHandGun->CreateBullet(shotDirection);
				}
			}
			m_tutorial.isTAttackHandGun = true;	// チュートリアル用ハンドガン攻撃フラグをtrueにする

		}
		else if (m_useWeapon == WeaponKind::MachineGun)
		{
			if (m_pShotMachineGun->GetBulletNum() > 0)
			{
				StartJoypadVibration(input.IsPress(InputInfo::Attack), 100, 1);
			}

			SetModelFramePosition(m_model, kModelRightHandRing3, m_weapon[1], m_weaponSize[1], m_weaponRota[1]);
			MV1SetVisible(m_weapon[1], true);

			ChangeAnimNo(PlayerAnim::MachineGun2, m_animSpeed.MachineGun, true, m_animChangeTime.MachineGun);

			m_machineGunCount++;
			if (m_machineGunCount >= 5)
			{
				if (m_pShotMachineGun->GetBulletNum() > 0)
				{
					m_pSound->PlaySE(SoundManager::SE_Type::kHandGunSE, DX_PLAYTYPE_BACK);
					m_machineGunCount = 0;

					// ロックオン状態の場合、弾の発射方向を敵の方向に設定
					VECTOR shotDirection = m_targetDir;
					if (m_isLookOn)
					{
						VECTOR enemyPos = m_pEnemy->GetPos();
						VECTOR playerPos = GetPos();
						shotDirection = VNorm(VSub(enemyPos, playerPos));
					}

					m_pShotMachineGun->CreateBullet(shotDirection);
				}
			}
			m_tutorial.isTAttackMachineGun = true;	// チュートリアル用マシンガン攻撃フラグをtrueにする
		}
	}
}

void Player::AttackKnife(Input& input)
{
	bool cannotAttackKnife = m_status.situation.isRoll || m_status.situation.isDamageReceived || m_status.situation.isUseItem;	// true=攻撃できない false=攻撃できる
	
	// ナイフ攻撃ができない場合または使用武器が銃の場合処理しない
	if (m_useWeapon == WeaponKind::HandGun || m_useWeapon == WeaponKind::MachineGun || cannotAttackKnife)	return;

	// 武器を持たせる場所の獲得
	int handle = MV1SearchFrame(m_model, kModelRightHandRing4);
	int knifeTip = MV1SearchFrame(m_weapon[2], kKnifeModelTip);

	// 武器を持たせる場所のセット
	m_rightHandPos = MV1GetFramePosition(m_model, handle);  // ボーンの位置を取得
	m_KnifeTipPos = MV1GetFramePosition(m_weapon[2], knifeTip);

	// ナイフで攻撃をしている時のみ、当たり判定も移動させる
	if (m_status.situation.isKnifeAttack)
	{
		m_col.TypeChangeCapsuleUpdate(m_col.m_colPlayer.m_weapon, m_rightHandPos, m_KnifeTipPos, m_playerData["knife"].HitRad);
	}

	// 攻撃アニメーション系処理
	if (input.IsTrigger(InputInfo::Attack))
	{
		// 1コンボ目攻撃
		if (m_SetComboknife == Knife::Attack1)
		{
			m_status.situation.isKnifeAttack = true;
			m_SetComboknife = Knife::Attack2;
			ChangeAnimNo(PlayerAnim::Knife1, m_animSpeed.Knife1, false, m_animChangeTime.Knife1);
			m_isAttack = true;

			SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], kInitVec);
			MV1SetVisible(m_weapon[2], true);
		}

		// 2コンボ目
		else if (m_SetComboknife == Knife::Attack2 && (m_nextAnimTime >= kSecondAttackTime))
		{
			m_status.situation.isKnifeAttack = true;
			m_SetComboknife = Knife::Attack3;
			ChangeAnimNo(PlayerAnim::Knife2, m_animSpeed.Knife2, false, m_animChangeTime.Knife2);
			m_isAttack = true;

			SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], kInitVec);
			MV1SetVisible(m_weapon[2], true);
		}

		// 3コンボ目
		else if (m_SetComboknife == Knife::Attack3 && (m_nextAnimTime >= kThirdAttackTime))
		{
			m_status.situation.isKnifeAttack = true;
			ChangeAnimNo(PlayerAnim::Knife3, m_animSpeed.Knife3, false, m_animChangeTime.Knife3);
			if (!m_is3Combo)
			{
				m_isAttack = true;
			}

			SetModelFramePosition(m_model, kModelRightHandRing4, m_weapon[2], m_weaponSize[2], m_weaponRota[2]);
			MV1SetVisible(m_weapon[2], true);
			m_is3Combo = true;
		}
	}

	// アニメーションが終わったらコンボ攻撃を初期化する
	if (m_status.situation.isKnifeAttack && IsAnimEnd())
	{
		m_status.situation.isKnifeAttack = false;
		m_SetComboknife = Knife::Attack1;
		m_isAttack = false;
		m_is3Combo = false;
	}
}

/// <summary>
/// 回避処理
/// </summary>
void Player::Roll(Input& input)
{
	// ダメージを受けている間は処理しない
	if (m_status.situation.isDamageReceived)return;
	m_isInvincibleTime = false;

	// 回避開始から15フレームの間はダメージを受けないようにする
	if (m_status.situation.isRoll && (m_nextAnimTime <= kInvincibleTime)) {
		m_isInvincibleTime = true;
	}

	// 回避ボタンを押したら処理を行う
	if (!m_status.situation.isRoll && input.IsTrigger(InputInfo::Roll) && m_stamina >= kStaminaConsumption) {
		m_stamina -= kStaminaConsumption;
		m_stamina = std::max(m_stamina, 0.0f);

		// 回避のアニメーションを再生
		ChangeAnimNo(PlayerAnim::Roll, m_animSpeed.Roll, false, m_animChangeTime.Roll);
		m_status.situation.isRoll = true;

		// 回避の移動量を設定
		if (VSize(m_move) <= 0.0f) {
			m_roll = m_targetDir;
			m_roll = VScale(m_roll, m_chara.walkSpeed);
		}
		else {
			m_roll = m_move;
		}
	}

	if (!m_status.situation.isRoll) return;	// 回避をしていない場合は以下の処理をしない

	// 座標の設定
	m_pos = VAdd(m_pos, m_roll);

	// 回避のアニメーションが終了したら
	if (m_status.situation.isRoll && IsAnimEnd()){
		// 状態・回避の移動量を初期化する
		m_status.situation.isRoll = false;
		m_roll = kInitVec;
	}
}

void Player::Hit()
{
	// 死んでいる場合は処理しない
	if (m_status.situation.isDeath) return;

	/*回避開始から15フレームの間は以下の処理に進まないようにする*/
	if (m_status.situation.isRoll && (m_nextAnimTime <= 15.0f))return;

	if (m_status.situation.isDamageReceived && IsAnimEnd())
	{
		m_status.situation.isDamageReceived = false;
		m_isInvincibleTime = false;
	}
}

void Player::OnDamage(int damage)
{
	if (m_status.situation.isDeath)	return;

	// 被ダメ中以外は処理を行う
	if (!m_isInvincibleTime)
	{
		m_hp -= damage;

		ChangeAnimNo(PlayerAnim::DamageReceived, m_animSpeed.DamageReceived, false, m_animChangeTime.DamageReceived);
		m_status.situation.isDamageReceived = true;
		m_status.situation.isRoll = false;
	}
}

void Player::ChangeAnimNo(const PlayerAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime)
{
	m_status.animNo = static_cast<int>(anim);
	m_status.animSpeed = animSpeed;
	m_status.isLoop = isAnimLoop;
	ChangeAnimation(m_status.animNo, animSpeed, m_status.isLoop, false, changeTime);
}

void Player::ChangeAnimIdle()
{
	// 待機アニメーションに変更する
	if (!m_status.situation.isUseItem && !m_status.situation.isMoving && !m_status.situation.isGunAttack && !m_status.situation.isKnifeAttack
		&& !m_status.situation.isRoll && !m_status.situation.isDamageReceived && !m_status.situation.isDeath)
	{
		ChangeAnimNo(PlayerAnim::Idle, m_animSpeed.Idle, true, m_animChangeTime.Idle);
		MV1SetVisible(m_weapon[0], false);
		MV1SetVisible(m_weapon[1], false);
		MV1SetVisible(m_weapon[2], false);
	}
}

void Player::WeaponInfoSet()
{
	m_weaponSize[0] = VGet(m_playerData["handGun"].sizeX, m_playerData["handGun"].sizeY, m_playerData["handGun"].sizeZ);
	m_weaponSize[1] = VGet(m_playerData["machineGun"].sizeX, m_playerData["machineGun"].sizeY, m_playerData["machineGun"].sizeZ);
	m_weaponSize[2] = VGet(m_playerData["knife"].sizeX, m_playerData["knife"].sizeY, m_playerData["knife"].sizeZ);

	m_weaponRota[0] = VGet(m_playerData["handGun"].RotaX, m_playerData["handGun"].RotaY, m_playerData["handGun"].RotaZ);
	m_weaponRota[1] = VGet(m_playerData["machineGun"].RotaX, m_playerData["machineGun"].RotaY, m_playerData["machineGun"].RotaZ);
	m_weaponRota[2] = VGet(m_playerData["knife"].RotaX, m_playerData["knife"].RotaY, m_playerData["knife"].RotaZ);
}

void Player::Death()
{
	// HPが0になったら死亡アニメーションを流す
	if (m_hp <= 0)
	{
		m_status.situation.isDeath = true;
		ChangeAnimNo(PlayerAnim::Dying, m_animSpeed.Dying, false, m_animChangeTime.Dying);
	}

	// 死亡アニメーションが終わったらゲームオーバーシーンへ切り替わる
	if (m_status.situation.isDeath && IsAnimEnd())
	{
		m_deathFlag = true;
	}
}

void Player::PlaySE()
{
	// ナイフ攻撃時SEを鳴らす
	if (m_useWeapon == WeaponKind::Knife)
	{
		if (m_isAttack && m_isAttackToEnemy)
		{
			m_pSound->PlaySE(SoundManager::SE_Type::kKnifeSE, DX_PLAYTYPE_BACK);
		}
	}
}

void Player::SetRemainingBulletsHandgun()
{
	m_remainingBulletsHandgun = m_pShotHandGun->GetBulletNum();
}

void Player::SetRemainingBulletsMachinegun()
{
	m_remainingBulletsMachinegun = m_pShotMachineGun->GetBulletNum();
}

void Player::InitTutorialActionFlag()
{
	m_tutorial.isTMove = false;
	m_tutorial.isTRoll = false;
	m_tutorial.isTGetItem = false;
	m_tutorial.isTChangeWeapon = false;
	m_tutorial.isTRockOn = false;
	m_tutorial.isTAttackHandGun = false;
	m_tutorial.isTAttackMachineGun = false;
	m_tutorial.isTAttackKnife = false;
	m_tutorial.isTChangeItem = false;
	m_tutorial.isTUseItem = false;
}

void Player::SetTutorialActionFlag()
{
	m_tutorial.isTMove = m_status.situation.isMoving;				// 移動
	m_tutorial.isTRoll = m_status.situation.isRoll;					// 回避	
	m_tutorial.isTUseItem = m_status.situation.isUseItem;			// アイテム使用
	m_tutorial.isTRockOn = m_isLookOn;								// ロックオン
	m_tutorial.isTAttackKnife = m_status.situation.isKnifeAttack;	// ナイフ攻撃
}
