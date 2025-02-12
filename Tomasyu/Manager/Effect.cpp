#include "Effect.h"
#include "EffekseerForDXLib.h"
#include "../Util/Input.h"

Effect* Effect::m_instance = nullptr;

namespace {
	// エフェクトパス
	const char* const kEffectShot = "Data/Effect/Shot.efk";			// 銃
	const char* const kEffectKnife = "Data/Effect/Knife.efk";		// ナイフ
	const char* const kEffectRecovery = "Data/Effect/Drink.efk";	// 回復
	const char* const kEffectItemBox = "Data/Effect/Item.efk";		// アイテム
	const char* const kEffectHit = "Data/Effect/EnemyAttack.efk";	// 殴る

	// サイズ
	const float kScale30 = 30.0f;
	const float kScale15 = 15.0f;
	const float kScale10 = 10.0f;

	// 総合時間
	const int kTotalTime60 = 60;
	const int kTotalTime600 = 600;
}

void Effect::Init()
{
	Effekseer_InitDistortion();	// エフェクトの歪みを適用する
}

void Effect::Update()
{
	Effekseer_Sync3DSetting();	// 3Dの情報をDxLibとEffekseerで合わせる
	UpdateEffekseer3D();

	for (auto it = m_effects.begin(); it != m_effects.end();)
	{
		EffectData& data = *it;
		if (!data.m_isPlaying)
		{
			it = m_effects.erase(it);	// エフェクトを削除する
			continue;
		}

		data.m_time++;

		// 再生時間を超えた場合
		if (data.m_time >= data.m_totalTime)
		{
			// ループ再生の場合
			if (data.m_isLoop)
			{
				data.m_time = 0;
				data.m_playingHandle = PlayEffekseer3DEffect(data.m_handle);
			}
			else
			{
				StopEffekseer3DEffect(data.m_playingHandle);
				data.m_isPlaying = false;
			}
		}
		else
		{
			SetPosPlayingEffekseer3DEffect(data.m_playingHandle, data.m_pos.x, data.m_pos.y, data.m_pos.z);
			SetScalePlayingEffekseer3DEffect(data.m_playingHandle, data.m_scale, data.m_scale, data.m_scale);
			SetRotationPlayingEffekseer3DEffect(data.m_playingHandle, data.m_rota.x, data.m_rota.y, data.m_rota.z);
		}

		it++;
	}
}

void Effect::Draw()
{
	DrawEffekseer3D();
}

void Effect::Load()
{
	// エフェクト読み込み
	m_effectData[EffectKind::kEffectKind::kShot].m_handle = LoadEffekseerEffect(kEffectShot);
	m_effectData[EffectKind::kEffectKind::kKnife].m_handle = LoadEffekseerEffect(kEffectKnife);
	m_effectData[EffectKind::kEffectKind::kRecovery].m_handle = LoadEffekseerEffect(kEffectRecovery);
	m_effectData[EffectKind::kEffectKind::kItemBox].m_handle = LoadEffekseerEffect(kEffectItemBox);
	m_effectData[EffectKind::kEffectKind::kHit].m_handle = LoadEffekseerEffect(kEffectHit);

	// サイズセット
	m_effectData[EffectKind::kEffectKind::kKnife].m_scale = kScale30;
	m_effectData[EffectKind::kEffectKind::kShot].m_scale = kScale30;
	m_effectData[EffectKind::kEffectKind::kRecovery].m_scale = kScale15;
	m_effectData[EffectKind::kEffectKind::kItemBox].m_scale = kScale10;
	m_effectData[EffectKind::kEffectKind::kHit].m_scale = kScale10;

	// 総合時間セット
	m_effectData[EffectKind::kEffectKind::kShot].m_totalTime = kTotalTime60;
	m_effectData[EffectKind::kEffectKind::kKnife].m_totalTime = kTotalTime60;
	m_effectData[EffectKind::kEffectKind::kRecovery].m_totalTime = kTotalTime60;
	m_effectData[EffectKind::kEffectKind::kItemBox].m_totalTime = kTotalTime600;
	m_effectData[EffectKind::kEffectKind::kHit].m_totalTime = kTotalTime600;
}

void Effect::ClearEffect(const EffectKind::kEffectKind kind)
{
	auto it = m_effectData.find(kind);
	if (it != m_effectData.end())
	{
		m_effectData.erase(it);
	}
}

void Effect::AddEffect(EffectKind::kEffectKind kind, VECTOR pos)
{
	auto it = m_effectData.find(kind);
	if (it != m_effectData.end())
	{
		EffectData& data = it->second;
		data.m_pos = pos;	// エフェクトの微調整
		data.m_time = 0;
		data.m_isPlaying = true;
		data.m_playingHandle = PlayEffekseer3DEffect(data.m_handle);

		// 　エフェクトがアイテムボックスだったらループ再生させる
		if (kind == EffectKind::kEffectKind::kItemBox)
		{
			data.m_isLoop = true;
		}

		m_effects.push_back(data);
	}
}

void Effect::StopEffect(const EffectKind::kEffectKind kind, const VECTOR pos)
{
	for (auto it = m_effects.begin(); it != m_effects.end();)
	{
		EffectData& data = *it;

		// 位置が一致するかどうか
		if (VSize(pos) == VSize(data.m_pos))
		{
			StopEffekseer3DEffect(data.m_playingHandle);
			data.m_isPlaying = false;
			it = m_effects.erase(it);
		}
		else
		{
			it++;
		}
	}
}