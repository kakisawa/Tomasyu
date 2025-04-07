#include "UISceneGame.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Shot.h"
#include "DxLib.h"
#include <cassert>
#include <algorithm>
#include <functional>

namespace {
	const float kHpMaxWidth = 1205.0f;		// 最大HPUIの横幅
	const float kStaminaMaxWidth = 1172.0f;	// 最大スタミナUIの横幅
	const float kBarUIHeight = 37.0f;		// HPスタミナUIの縦幅
	const float kHpStepSize = 0.5f;			// HPの赤色を緑色の場所へ減らす移動量

	const VECTOR kWeaponSelectPos[3]{	// 武器セレクトカーソルUI座標
		VGet(1741.0f, 108.0f,0.0f),		// 一つ目
		VGet(1741.0f,244.0f,0.0f),		// 二つ目
		VGet(1741.0f,380.0f,0.0f),		// 三つ目
	};

	const VECTOR kItemSelectPos[3]{		// アイテムセレクトカーソルUI座標
		VGet(1741.0f, 660.0f,0.0f),		// 一つ目
		VGet(1741.0f, 796.0f,0.0f),		// 二つ目
		VGet(1741.0f, 932.0f,0.0f),		// 三つ目
	};

	const VECTOR kItemPos[3]{			// アイテムUI座標
		 VGet(1768.0f, 690, 0.0f),		// 一つ目
		 VGet(1768.0f, 826, 0.0f),		// 二つ目
		 VGet(1768.0f, 962, 0.0f),		// 三つ目
	};

	VECTOR kDisplayBgPos[2]{			// 選択中のアイテム・武器文字背景UI座標
		VGet(1490.0f, 0.0f,0.0f),		// 武器
		VGet(1540.0f, 535.0f,0.0f),		// アイテム
	};

	VECTOR kChangeButtonPos[2]{			// 武器・アイテム切り替えボタンUI座標
		VGet(1457.0f,21.0f,0.0f),		// 武器
		VGet(1482.0f,555.0f,0.0f),		// アイテム
	};

	const VECTOR kBarPos[6]{			// バーUI座標
		VGet(220.0f,21.0f,0.0f),		// HP背景バー
		VGet(224.0f,28.0f,0.0f),		// HPバー(緑/赤)
		VGet(220.0f,84.0f,0.0f),		// スタミナ背景バー
		VGet(224.0f,89.0f,0.0f),		// スタミナバー
		VGet(343.0f,1022.0f,0.0f),		// 敵HP背景バー
		VGet(347.0f,1026.0f,0.0f),		// 敵HPバー
	};

	const VECTOR kGunUIPos[6] = {		// 残弾数UI座標
		VGet(1817.0f, 205.0f,0.0f),
		VGet(1840.0f, 205.0f,0.0f),
		VGet(1868.0f, 205.0f,0.0f),
		VGet(1817.0f, 347.0f,0.0f),
		VGet(1840.0f, 347.0f,0.0f),
		VGet(1868.0f, 347.0f,0.0f),
	};

	VECTOR kDisplayItemPos = VGet(1650, 580, 0.0f);			// 選択中のアイテム文字UI座標
	VECTOR kDisplayWeaponPos = VGet(1610, 33, 0.0f);		// 選択中の武器文字UI座標
	VECTOR kBaseUI = VGet(1740.0f, 91.0f, 0.0f);			// アイテム・武器大本UI座標

	const char* const kItemCharaUI[9]{						// 文字UI画像
		"Data/Image/SceneGame/CharUI/Tentative.png",
		"Data/Image/SceneGame/CharUI/landmine.png",
		"Data/Image/SceneGame/CharUI/SurpriseBox.png",
		"Data/Image/SceneGame/CharUI/IceFloor.png",
		"Data/Image/SceneGame/CharUI/RecoveryMedic.png",
		"Data/Image/SceneGame/CharUI/Ammunition.png",
		"Data/Image/SceneGame/CharUI/HandGun.png",
		"Data/Image/SceneGame/CharUI/MachineGun.png",
		"Data/Image/SceneGame/CharUI/Knife.png",
	};

	const char* const kPlayerToolUI[12]{
		"Data/Image/SceneGame/Item/ItemWeaponBg.png",
		"Data/Image/SceneGame/WeaponCursor.png",
		"Data/Image/SceneGame/ItemCursor.png",
		"Data/Image/SceneGame/ItemWeaponUIBg.png",
		"Data/Image/SceneGame/Item/Tentative.png",
		"Data/Image/SceneGame/Item/landmine.png",
		"Data/Image/SceneGame/Item/SurpriseBox.png",
		"Data/Image/SceneGame/Item/IceFloor.png",
		"Data/Image/SceneGame/Item/RecoveryMedic.png",
		"Data/Image/SceneGame/Item/Ammunition.png",
		"Data/Image/SceneGame/ChangeWeapon.png",		// 武器切り替えUI
		"Data/Image/SceneGame/ChangeItem.png",			// アイテム切り替えUI
	};

	const char* const kBarUI[7]{
		"Data/Image/SceneGame/Bar/HpBar.png",
		"Data/Image/SceneGame/Bar/RedHp.png",
		"Data/Image/SceneGame/Bar/GreenHp.png",
		"Data/Image/SceneGame/Bar/StaminaBg.png",
		"Data/Image/SceneGame/Bar/Stamina.png",
		"Data/Image/SceneGame/Bar/EnemyHpBg.png",
		"Data/Image/SceneGame/Bar/EnemyHp.png",
	};

	const char* const kNumberUI[10]{
		"Data/Image/SceneGame/BulletNumber/0.png",
		"Data/Image/SceneGame/BulletNumber/1.png",
		"Data/Image/SceneGame/BulletNumber/2.png",
		"Data/Image/SceneGame/BulletNumber/3.png",
		"Data/Image/SceneGame/BulletNumber/4.png",
		"Data/Image/SceneGame/BulletNumber/5.png",
		"Data/Image/SceneGame/BulletNumber/6.png",
		"Data/Image/SceneGame/BulletNumber/7.png",
		"Data/Image/SceneGame/BulletNumber/8.png",
		"Data/Image/SceneGame/BulletNumber/9.png",
	};
}

UISceneGame::UISceneGame(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy>pEnemy) :
	m_playerRemainingBullets_handgun(0),
	m_playerRemainingBullets_machinegun(0),
	m_useWeaponChara(0),
	m_useItemChara(0),
	m_playerHp_Green(kHpMaxWidth),
	m_playerHp_Red(kHpMaxWidth),
	m_playerStamina(kStaminaMaxWidth),
	m_enemyHp(kHpMaxWidth),
	m_cursorUI1Pos(kWeaponSelectPos[0]),
	m_cursorUI2Pos(kItemSelectPos[0]),
	m_pPlayer(pPlayer),
	m_pEnemy(pEnemy)
{
	// 素材の読み込み
	Load();
}

UISceneGame::~UISceneGame()
{
}

void UISceneGame::Init()
{
	// HP,スタミナ値の初期化
	m_playerHp_Red = static_cast<float>(m_pPlayer->GetHp());
	m_enemyHp = static_cast<float>(m_pPlayer->GetHp());
	m_playerStamina = static_cast<float>(m_pPlayer->GetStamina());
}

void UISceneGame::Update()
{
	// 各種更新
	UpdateBarUI();
	UpdateItemUI();
	UpdateWeaponUI();

	// 残弾UIの設定
	SetUI_RemainingBullets();
}

void UISceneGame::Draw()
{
	// アイテム・武器大本UI
	DrawGraphF(kBaseUI.x, kBaseUI.y, m_playerToolUIHandle[0], true);

	// HPバー・スタミナ背景バーUI
	DrawGraphF(kBarPos[0].x, kBarPos[0].y, m_barUIHandle[0], true);
	DrawGraphF(kBarPos[2].x, kBarPos[2].y, m_barUIHandle[3], true);

	// プレイヤーHPバー・スタミナバーUI
	float stamina = kStaminaMaxWidth * (m_playerStamina * 0.01f);
	float hp_red = kHpMaxWidth * (m_playerHp_Red * 0.01f);
	float hp_green = kHpMaxWidth * (m_playerHp_Green * 0.01f);

	DrawRectGraphF2(kBarPos[3].x, kBarPos[3].y, 0.0f, 0.0f,
		std::max(0.0f, stamina), kBarUIHeight, m_barUIHandle[4], true);
	DrawRectGraphF2(kBarPos[1].x, kBarPos[1].y, 0, 0,
		std::max(0.0f, hp_red), kBarUIHeight, m_barUIHandle[1], true);
	DrawRectGraphF2(kBarPos[1].x, kBarPos[1].y, 0.0f, 0.0f,
		std::max(0.0f, hp_green), kBarUIHeight, m_barUIHandle[2], true);

	// 敵HP背景バーUI
	DrawGraphF(kBarPos[4].x, kBarPos[4].y, m_barUIHandle[5], true);
	// 敵HPバー
	float hp = kHpMaxWidth * (m_enemyHp * 0.001f);

	DrawRectGraphF2(kBarPos[5].x, kBarPos[5].y, 0.0f, 0.0f,
		std::max(0.0f, hp), kBarUIHeight, m_barUIHandle[6], true);

	// 選択中のアイテム・武器名背景UI
	for (int i = 0; i < 2; i++)
	{
		DrawGraphF(kDisplayBgPos[i].x, kDisplayBgPos[i].y, m_playerToolUIHandle[3], true);
	}

	// 武器・アイテム切り替えUI
	DrawGraphF(kChangeButtonPos[0].x, kChangeButtonPos[0].y, m_playerToolUIHandle[10], true);
	DrawGraphF(kChangeButtonPos[1].x, kChangeButtonPos[1].y, m_playerToolUIHandle[11], true);


	// 選択中の武器文字UI
	DrawGraphF(kDisplayWeaponPos.x, kDisplayWeaponPos.y, m_useWeaponChara, true);
	// 選択中のアイテム文字UI
	DrawGraphF(kDisplayItemPos.x, kDisplayItemPos.y, m_useItemChara, true);


	// 獲得したアイテムUI
	for (int i = 0; i < m_itemUI.size(); i++)
	{
		DrawGraphF(kItemPos[i].x, kItemPos[i].y, m_itemUI[i], true);
	}

	// 選択中の武器カーソル
	DrawGraphF(m_cursorUI1Pos.x, m_cursorUI1Pos.y, m_playerToolUIHandle[1], true);
	// 選択中のアイテムカーソル
	DrawGraphF(m_cursorUI2Pos.x, m_cursorUI2Pos.y, m_playerToolUIHandle[2], true);

	// 残弾数UI
	DrawGraphF(kGunUIPos[0].x, kGunUIPos[0].y, balanceBullets[GunType::HandGun].m_playerHundredsHandle, true);
	DrawGraphF(kGunUIPos[1].x, kGunUIPos[1].y, balanceBullets[GunType::HandGun].m_playerTensHandle, true);
	DrawGraphF(kGunUIPos[2].x, kGunUIPos[2].y, balanceBullets[GunType::HandGun].m_playerOneHandle, true);
	DrawGraphF(kGunUIPos[3].x, kGunUIPos[3].y, balanceBullets[GunType::MachineGun].m_playerHundredsHandle, true);
	DrawGraphF(kGunUIPos[4].x, kGunUIPos[4].y, balanceBullets[GunType::MachineGun].m_playerTensHandle, true);
	DrawGraphF(kGunUIPos[5].x, kGunUIPos[5].y, balanceBullets[GunType::MachineGun].m_playerOneHandle, true);

#ifdef _DEBUG
	//DrawFormatString(0, 0, 0xffffff, "m_playerRemainingBullets_handgun=%d", m_playerRemainingBullets_handgun);
	//DrawFormatString(0, 20, 0xffffff, "m_playerRemainingBullets_machinegun=%d", m_playerRemainingBullets_machinegun);
#endif // DEBUG
}

void UISceneGame::Load()
{
	// 文字UI画像読み込み
	for (int i = 0; i < m_itemCharaUIHnadle.size(); i++)
	{
		m_itemCharaUIHnadle[i] = LoadGraph(kItemCharaUI[i]);
		assert(m_itemCharaUIHnadle[i] != -1);
	}

	// プレイヤー使用ツール用UI画像読み込み
	for (int i = 0; i < m_playerToolUIHandle.size(); i++)
	{
		m_playerToolUIHandle[i] = LoadGraph(kPlayerToolUI[i]);
		assert(m_playerToolUIHandle[i] != -1);
	}

	// バーUI画像の読み込み
	for (int i = 0; i < m_barUIHandle.size(); i++)
	{
		m_barUIHandle[i] = LoadGraph(kBarUI[i]);
		assert(m_barUIHandle[i] != -1);
	}
	// 残弾用UI画像の読み込み
	for (int i = 0; i < m_numberUIHandle.size(); i++)
	{
		m_numberUIHandle[i] = LoadGraph(kNumberUI[i]);
		assert(m_numberUIHandle[i] != -1);
	}
}

void UISceneGame::End()
{
	// 画像データの削除
	// 文字UI画像の削除
	for (int i = 0; i < m_itemCharaUIHnadle.size(); i++)
	{
		DeleteGraph(m_itemCharaUIHnadle[i]);
	}

	// プレイヤー使用ツール用UI画像の削除
	for (int i = 0; i < m_playerToolUIHandle.size(); i++)
	{
		DeleteGraph(m_playerToolUIHandle[i]);
	}

	// バーUI画像の削除
	for (int i = 0; i < m_barUIHandle.size(); i++)
	{
		DeleteGraph(m_barUIHandle[i]);
	}

	// バーUI画像の削除
	for (int i = 0; i < m_numberUIHandle.size(); i++)
	{
		DeleteGraph(m_numberUIHandle[i]);
	}
}

void UISceneGame::UpdateBarUI()
{
	// プレイヤーのHPバー管理
	m_playerHp_Green = static_cast<float>(m_pPlayer->GetHp());

	if (m_playerHp_Green < m_playerHp_Red)
	{
		m_playerHp_Red -= kHpStepSize;
	}

	// プレイヤーのスタミナバー管理
	m_playerStamina = m_pPlayer->GetStamina();

	// 敵のHPバー管理
	m_enemyHp = static_cast<float>(m_pEnemy->GetHp());
}

void UISceneGame::UpdateItemUI()
{
	// 所持アイテムが何もなかったら、UIも表示しないようにする
	for (int i = 0; i < m_itemUI.size(); i++) {
		if (m_pPlayer->m_item[i] == Item::ItemKind::NoItem)
		{
			m_itemUI[i] = -1;
		}
	}

	// プレイヤーが獲得したアイテムのUIを設定する関数
	SetUI_GetItem();

	// プレイヤーのアイテム選択情報を獲得しての処理
	for (int i = 0; i < 3; i++)
	{
		if (m_pPlayer->GetItemFrame() == i)
		{
			m_cursorUI2Pos = kItemSelectPos[i];		// カーソルの位置移動
		}
	}

	// アイテムを所持していなかったらアイテム画像を消し、先には進まない
	if (m_pPlayer->item() == Item::ItemKind::NoItem) {
		m_useItemChara = -1;
		return;
	}

	SetUI_SelectItem();
}

void UISceneGame::UpdateWeaponUI()
{
	// プレイヤーの武器選択情報を獲得しての処理
	if (m_pPlayer->GetWeaponKind() == Player::WeaponKind::HandGun)
	{
		m_cursorUI1Pos = kWeaponSelectPos[0];	// カーソルの位置移動
		m_useWeaponChara = m_itemCharaUIHnadle[6];	// 選択中武器名UIの表示

	}
	if (m_pPlayer->GetWeaponKind() == Player::WeaponKind::MachineGun) {
		m_cursorUI1Pos = kWeaponSelectPos[1];	// カーソルの位置移動
		m_useWeaponChara = m_itemCharaUIHnadle[7];	// 選択中武器名UIの表示
	}

	if (m_pPlayer->GetWeaponKind() == Player::WeaponKind::Knife) {
		m_cursorUI1Pos = kWeaponSelectPos[2];	// カーソルの位置移動
		m_useWeaponChara = m_itemCharaUIHnadle[8];	// 選択中武器名UIの表示
	}
}

void UISceneGame::SetUI_SelectItem()
{
	// 選択中アイテム名UIの表示
	if (m_pPlayer->item() == Item::ItemKind::landmine)
	{
		m_useItemChara = m_itemCharaUIHnadle[1];
	}
	else if (m_pPlayer->item() == Item::ItemKind::SurpriseBox)
	{
		m_useItemChara = m_itemCharaUIHnadle[2];
	}
	else if (m_pPlayer->item() == Item::ItemKind::IceFloor)
	{
		m_useItemChara = m_itemCharaUIHnadle[3];
	}
	else if (m_pPlayer->item() == Item::ItemKind::RecoveryMedic)
	{
		m_useItemChara = m_itemCharaUIHnadle[4];
	}
	else if (m_pPlayer->item() == Item::ItemKind::Ammunition)
	{
		m_useItemChara = m_itemCharaUIHnadle[5];
	}
}

void UISceneGame::SetUI_GetItem()
{
	for (int i = 0; i < m_itemUI.size(); i++)
	{
		// アイテム枠
		if (m_pPlayer->m_item[i] == Item::ItemKind::landmine)
		{
			m_itemUI[i] = m_playerToolUIHandle[5];
		}
		else if (m_pPlayer->m_item[i] == Item::ItemKind::SurpriseBox)
		{
			m_itemUI[i] = m_playerToolUIHandle[6];
		}
		else if (m_pPlayer->m_item[i] == Item::ItemKind::IceFloor)
		{
			m_itemUI[i] = m_playerToolUIHandle[7];
		}
		else if (m_pPlayer->m_item[i] == Item::ItemKind::RecoveryMedic)
		{
			m_itemUI[i] = m_playerToolUIHandle[8];
		}
		else if (m_pPlayer->m_item[i] == Item::ItemKind::Ammunition)
		{
			m_itemUI[i] = m_playerToolUIHandle[9];
		}
	}
}

void UISceneGame::SetUI_RemainingBullets()
{
	m_playerRemainingBullets_handgun = m_pPlayer->GetRemainingBulletsHandgun();
	m_playerRemainingBullets_machinegun = m_pPlayer->GetRemainingBulletsMachinegun();

	SetUI_RemainingBulletsHandle(GunType::HandGun, m_playerRemainingBullets_handgun);
	SetUI_RemainingBulletsHandle(GunType::MachineGun, m_playerRemainingBullets_machinegun);
}

void UISceneGame::SetUI_RemainingBulletsHandle(GunType type, int num)
{
	int hundred = num / 100;		// 100の位
	int ten = (num % 100) / 10;		// 10の位
	int one = (num % 100) % 10;		// 1の位

	if (hundred == 1)
	{
		balanceBullets[type].m_playerHundredsHandle = m_numberUIHandle[1];
	}
	else if (hundred == 0)
	{
		balanceBullets[type].m_playerHundredsHandle = -1;
	}

	for (int i = 0; i < 10; i++)
	{
		if (ten == i)
		{
			balanceBullets[type].m_playerTensHandle = m_numberUIHandle[i];
		}

		if (one == i) {
			balanceBullets[type].m_playerOneHandle = m_numberUIHandle[i];
		}
	}
}
