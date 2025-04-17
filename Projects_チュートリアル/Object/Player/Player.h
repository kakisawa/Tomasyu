#pragma once
#include "../Item.h"
#include "../ModelBase.h"
#include "DxLib.h"
#include <string>
#include <array>
#include <map>

class Enemy;
class Shot;
class Camera;
class Input;
class Score;
class Player : 
	public ModelBase, public std::enable_shared_from_this<Player>
{
public:
	struct Situation {
		bool isMoving = false;			// 移動中
		bool isUseItem = false;			// アイテム使用中
		bool isInstallation = false;	// 罠使用中
		bool isDrink = false;			// 回復アイテム使用中
		bool isReload = false;			// 弾再装填アイテム使用中
		bool isGunAttack = false;		// 銃攻撃中
		bool isKnifeAttack = false;		// ナイフ攻撃中
		bool isRoll = false;			// 回避中
		bool isDamageReceived = false;	// 被ダメージ
		bool isDeath = false;			// 死亡時
	};

	// プレイヤーの状態
	struct PlayerStatus {
		int animNo = -1;				// アニメーション番号
		float animSpeed = 0.0f;			// アニメーション再生スピード
		Situation situation;			// 状態
		bool isLoop = false;			// ループ再生
	}m_status;

	// アニメーション
	enum class PlayerAnim {
		Idle,			// 待機
		Run,			// 走る
		Run2,			// 走る2
		HandGun,		// 撃つ(ハンドガン)
		HandGun2,		// 撃つ(ハンドガン)
		MachineGun,		// 撃つ(マシンガン)
		MachineGun2,	// 撃つ(マシンガン)
		Knife1,			// ナイフ攻撃1
		Knife2,			// ナイフ攻撃2
		Knife3,			// ナイフ攻撃3
		Installation,	// 罠設置
		Drink,			// 飲む
		Summon,			// 召喚
		Reload,			// 弾再装填
		DamageReceived,	// 被ダメージ
		Roll,			// 回避
		Dying,			// 死亡
		Win,			// 勝利
		Lose,			// 敗北
		MAX,
	};

	// アニメーション切り替えにかかる時間
	struct AnimChangeTime
	{
		int Idle = 7;			// 待機
		int Run = 5;			// 走る
		int HandGun = 2;		// 撃つ(ハンドガン)
		int MachineGun = 5;		// 撃つ(マシンガン)
		int Knife1 = 5;			// ナイフ攻撃1
		int Knife2 = 5;			// ナイフ攻撃2
		int Knife3 = 5;			// ナイフ攻撃3
		int Installation = 5;	// 罠設置
		int Drink = 1;			// 飲む
		int Reload = 5;			// 弾再装填
		int Roll = 5;			// 回避
		// ここから下未定
		int DamageReceived = 5;	// 被ダメージ
		int Dying = 5;			// 死亡
		int Win = 5;			// 勝利
		int Lose = 5;			// 敗北
	}m_animChangeTime;

	// アニメーションの再生速度
	struct AnimSpeed {
		float Default = 0.5f;		// 基本
		float Idle = 0.5f;			// 待機
		float Run = 0.4f;			// 走る
		float HandGun = 0.7f;		// 撃つ(ハンドガン)
		float MachineGun = 0.5f;	// 撃つ(マシンガン)
		float Knife1 = 0.8f;		// ナイフ攻撃1
		float Knife2 = 0.8f;		// ナイフ攻撃2
		float Knife3 = 0.8f;		// ナイフ攻撃3
		float Installation = 1.3f;	// 罠設置
		float Drink = 1.3f;			// 飲む
		float Reload = 0.6f;		// 弾再装填
		float DamageReceived = 1.0f;// 被ダメージ
		float Roll = 0.65f;			// 回避
		float Dying = 0.5f;			// 死亡
		float Win = 1.0f;			// 勝利
		float Lose = 1.0f;			// 敗北
	}m_animSpeed;

	enum class WeaponKind {
		HandGun,	// 銃1
		MachineGun,	// 銃2
		Knife,		// ナイフ
	};

	enum class Knife {
		Attack1,	// 1コンボ目
		Attack2,	// 2コンボ目
		Attack3,	// 3コンボ目
	};

	// 所持しているアイテム
	std::array<Item::ItemKind, 3>  m_item;

	/// <summary>
	/// プレイヤーデータ
	/// </summary>
	struct PlayerOnlyData
	{
		int attack;			// 基礎攻撃力
		float sizeX;		// 初期拡大座標X
		float sizeY;		// 初期拡大座標Y
		float sizeZ;		// 初期拡大座標Z
		float RotaX;		// 初期回転角度X
		float RotaY;		// 初期回転角度Y
		float RotaZ;		// 初期回転角度Z
		float HitRad;		// 当たり判定半径
	};

	std::map<std::string, PlayerOnlyData> m_playerData;




	struct Tutorial{
		bool isTMove = false;

	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pCamera">カメラ情報</param>
	/// <param name="pEnemy">敵情報</param>
	/// <param name="pItem">アイテム情報</param>
	Player(std::shared_ptr<Camera> pCamera ,std::shared_ptr<Enemy> pEnemy,std::shared_ptr<Item> pItem);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::shared_ptr<Score> score);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// データを読み込む
	/// </summary>
	void LoadData();

	/// <summary>
	/// データの削除関係
	/// </summary>
	void End() override;

	/// <summary>
	/// モデルに物を持たせる処理
	/// </summary>
	/// <param name="ModelHandle">物を持たせるモデル</param>
	/// <param name="FrameName">モデルを持たせる場所</param>
	/// <param name="SetModelHandle">持たせるモデル</param>
	/// <param name="ModelSize">持たせるモデルのサイズ</param>
	void SetModelFramePosition(int ModelHandle, const char* FrameName, int SetModelHandle, VECTOR ModelSize, VECTOR ModelRota);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 移動処理更新
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// 重力処理
	/// </summary>
	void Gravity();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Angle();

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void ColUpdate();

	/// <summary>
	/// アイテム獲得
	/// </summary>
	void GetItem();

	/// <summary>
	/// 獲得したアイテムを入れ替える
	/// </summary>
	void ItemChange();

	/// <summary>
	/// アイテム使用
	/// </summary>
	void UseItem(Input& input);

	/// <summary>
	/// 対象を追跡する
	/// </summary>
	/// <param name="input">入力</param>
	void LockOn(Input& input);

	/// <summary>
	/// 武器切り替え
	/// </summary>
	/// <param name="input">入力</param>
	void ChangeWeapon(Input& input);

	/// <summary>
	/// ハンドガン攻撃
	/// </summary>
	/// <param name="input">入力</param>
	void AttackGun(Input& input);

	/// <summary>
	/// ナイフ攻撃
	/// </summary>
	/// <param name="input">入力</param>
	void AttackKnife(Input& input);

	/// <summary>
	/// 回避処理
	/// </summary>
	/// <param name="input">入力</param>
	void Roll(Input& input);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	void Hit();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	void OnDamage(int damage);

	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="anim">選択するアニメーション</param>
	/// <param name="animSpeed">アニメーションの再生速度</param>
	/// <param name="isAnimLoop">ループさせるか</param>
	/// <param name="changeTime">切り替えにかかる時間</param>
	void ChangeAnimNo(const PlayerAnim anim, const float animSpeed, const bool isAnimLoop, const int changeTime);

	/// <summary>
	/// アニメーションを待機状態に変更する
	/// </summary>
	void ChangeAnimIdle();

	/// <summary>
	/// 武器の角度セット
	/// </summary>
	void WeaponInfoSet();

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Death();

	/// <summary>
	/// SEを鳴らす
	/// </summary>
	void PlaySE();

	/// <summary>
	/// ハンドガンの残弾数をセットする
	/// </summary>
	void SetRemainingBulletsHandgun();

	/// <summary>
	/// マシンガンの残弾数をセットする
	/// </summary>
	void SetRemainingBulletsMachinegun();

	/// <summary>
	/// アイテムへ当たったかフラグのセット
	/// </summary>
	/// <param name="flag">当たったか入力</param>
	void SetGetItemFlag(int flag) { m_isItem = flag; }

	/// <summary>
	///	Enemyポインタを設定する
	/// </summary>
	/// <param name="pEnemy">敵情報</param>
	void SetEnemyPointer(std::shared_ptr<Enemy> pEnemy) { m_pEnemy = pEnemy; }

	/// <summary>
	/// Cameraポインタを設定する
	/// </summary>
	/// <param name="pCamera">カメラ情報</param>
	void SetCameraPointer(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }

	/// <summary>
	/// スタミナの値を渡す
	/// </summary>
	/// <returns>スタミナ</returns>
	float GetStamina()const { return m_stamina; }

	/// <summary>
	/// 選択中のアイテムを渡す関数
	/// </summary>
	/// <returns>選択中のアイテム</returns>
	int GetItemFrame() const { return m_useItem; }

	/// <summary>
	/// 残りのハンドガンの弾数を渡す関数
	/// </summary>
	/// <returns>ハンドガンの残弾数</returns>
	int GetRemainingBulletsHandgun() const { return m_remainingBulletsHandgun; }

	/// <summary>
	/// 残りのマシンガンの弾数を渡す関数
	/// </summary>
	/// <returns>マシンガンの残弾数</returns>
	int GetRemainingBulletsMachinegun() const { return m_remainingBulletsMachinegun; }

	/// <summary>
	/// ロックオンフラグ渡し
	/// </summary>
	/// <returns>ロックオンフラグ</returns>
	bool GetLockOn() const { return m_isLookOn; }

	/// <summary>
	/// 無敵時間中かどうかのフラグ渡し
	/// </summary>
	/// <returns>被ダメ中の無敵時間</returns>
	bool GetInvincibleTime() const { return m_isInvincibleTime; }

	/// <summary>
	/// 照準座標渡し
	/// </summary>
	/// <returns>ロックオン時の照準座標渡し</returns>
	VECTOR GetTargetPos() const { return m_targetLockPos; }

	/// <summary>
	/// 選択中の武器を渡す
	/// </summary>
	/// <returns>選択中の武器の値</returns>
	WeaponKind GetWeaponKind() const { return m_useWeapon; }

	/// <summary>
	/// 選択中のアイテムの種類を渡す
	/// </summary>
	/// <returns>選択中のアイテムの種類</returns>
	Item::ItemKind item() const { return m_item[m_useItem]; }

private:
	int m_aimingHandle;			// 照準画像
	int m_remainingBulletsHandgun;		// ハンドガンの残弾数
	int m_remainingBulletsMachinegun;	// マシンガンの残弾数
	int m_useItem;				// 所持している3つのうち、使用するアイテム
	int m_getItem;				// アイテムをランダムで獲得する際に使用する
	int m_machineGunCount;		// マシンガンの弾が発射されるまでのインターバル用
	float m_stamina;			// スタミナ
	bool m_isItem;				// アイテムと当たったか
	bool m_isLookOn;			// ロックオンフラグ
	bool m_isInvincibleTime;	// 被ダメ中の無敵時間
	bool m_isEnemy;				// 敵と当たったか
	bool m_isAttackToEnemy;		// 敵に攻撃が当たったかの判定
	bool m_is3Combo;			// ナイフ攻撃の3コンボ目かどうか
	bool m_isHandGunAnim;		// ハンドガンのアニメーションが終了したかどうか

	VECTOR m_roll;				// 回転時の移動量
	VECTOR m_targetLockPos;		// ロックオン時の照準座標
	VECTOR m_rightHandPos;		// 右手当たり判定用座標
	VECTOR m_KnifeTipPos;		// 武器先端当たり判定用座標

	std::array<int, 3> m_weapon{};	// 武器

	std::array<VECTOR, 3> m_weaponSize{};
	std::array<VECTOR, 3> m_weaponRota{};

	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Shot> m_pShotHandGun;
	std::shared_ptr<Shot> m_pShotMachineGun;
	std::shared_ptr<Camera> m_pCamera;

	std::shared_ptr<Score> m_pScore;

	Item::ItemKind m_setItem;	// 獲得したアイテムをセットするための値
	WeaponKind m_useWeapon;		// 使用中の武器
	Knife m_SetComboknife;		// ナイフ攻撃のコンボ数

	// 左パッド入力用
	int m_inputX, m_inputY;
	DINPUT_JOYSTATE m_input;
};