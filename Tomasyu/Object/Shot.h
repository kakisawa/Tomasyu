#pragma once
#include "../Util/Collision.h"
#include <memory>
#include <vector>
#include "DxLib.h"

class Player;
class Enemy;
class IsTime;
class Score;
class Shot
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤー情報</param>
	/// <param name="pEnemy"></param>
	/// <param name="attack">攻撃力</param>
	/// <param name="num">弾の最大数</param>
	/// <param name="score">スコア</param>
	Shot(Player* pPlayer, std::shared_ptr<Enemy> pEnemy,int attack,int num,int score);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Shot();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(std::shared_ptr<Score> score);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	/// <summary>
	/// 弾生成
	/// </summary>
	void CreateBullet();

	/// <summary>
	/// 弾更新
	/// </summary>
	void UpdateBullet();

	void Attack();

	/// <summary>
	/// 残弾数を渡す関数
	/// </summary>
	/// <returns>残弾数</returns>
	int GetBulletNum() { return m_bulletNum; }

private:
	int m_baseModel;	// 弾の基本モデル
	int m_attack;		// 攻撃力
	int m_maxBullet;	// 弾の最大上限数
	int m_bulletNum;	// 使用できる残り弾数
	int m_score;		// 弾を当てた際に獲得できるスコア

	struct Bullet
	{
		int m_model;		// モデル
		VECTOR m_pos;		// 座標
		VECTOR m_move;		// 移動
		VECTOR m_colPos;	// 当たり判定座標
		VECTOR m_direction;	// 進む方向
		Collision m_col;	// 当たり判定
		std::shared_ptr<IsTime> m_existTime;	// 消えるまでの時間
		bool m_isExist;		// 存在しているかのフラグ
	};

	std::vector<Bullet> m_bullet;			// 弾(配列)

	std::shared_ptr<Player> m_pPlayer;		// プレイヤーポインタ
	std::shared_ptr<Enemy> m_pEnemy;		// 敵ポインタ
	std::shared_ptr<Score> m_pScore;		// スコア
};