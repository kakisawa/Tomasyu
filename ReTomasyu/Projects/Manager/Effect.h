#pragma once
#include "DxLib.h"
#include <vector>
#include <map>

class ModelBase;

namespace EffectKind{
	// エフェクトの種類
	enum class kEffectKind {
		kKnife,		// ナイフ
		kShot,		// 銃
		kRecovery,	// 回復薬
		kItemBox,	// アイテムBox
		kHit,		// 殴り
	};
}

class Effect
{
public:
	struct EffectData {
		int m_handle = -1;						// エフェクト素材
		int m_playingHandle = -1;				// 再生中のハンドル
		VECTOR m_pos = VGet(0.0f, 0.0f, 0.0f);	// 座標
		VECTOR m_rota = VGet(0.0f, 0.0f, 0.0f);	// 回転率
		int m_time = 0;							// 現在の再生時間
		int m_totalTime = 0;					// 総再生時間
		float m_scale = 0;						// 拡大率
		int m_speed = 0;						// 再生速度
		bool m_isPlaying = false;				// エフェクトを再生中かどうか
		bool m_isLoop = false;					// ループ再生するかどうか
	};

public:
	// コピーコンストラクタの禁止
	Effect(const Effect&) = delete;
	// ムーブコンストラクタの禁止
	Effect(Effect&&) = delete;
	// 代入演算子の禁止
	Effect& operator=(const Effect&) = delete;
	Effect& operator=(Effect&&) = delete;

	static Effect& GetInstance() {
		if (!m_instance) {
			m_instance = new Effect;
		}
		return *m_instance;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトの読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 画面上のエフェクト削除
	/// </summary>
	/// <param name="kind">エフェクトの種類</param>
	void ClearEffect(const EffectKind::kEffectKind kind);

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="kind">エフェクトの種類</param>
	/// <param name="pos">表示座標</param>
	void AddEffect(const EffectKind::kEffectKind kind, VECTOR pos);

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	/// <param name="kind">エフェクトの種類</param>
	/// <param name="pos">表示座標</param>
	void StopEffect(const EffectKind::kEffectKind kind, const VECTOR pos);

	/// <summary>
	/// エフェクトが再生中かどうか取得する
	/// </summary>
	/// <param name="kind">エフェクトの種類</param>
	/// <returns>再生中かどうか</returns>
	bool GetIsPlaying(const EffectKind::kEffectKind kind) { return m_effectData[kind].m_isPlaying; }

private:
	Effect() = default;
	virtual ~Effect() = default;
	static Effect* m_instance;		// インスタンス

private:
	std::map<EffectKind::kEffectKind, EffectData> m_effectData;	// エフェクトのデータ
	std::vector<EffectData> m_effects;			// エフェクトの複数管理用
};