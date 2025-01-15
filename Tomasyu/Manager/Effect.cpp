#include "Effect.h"
#include "EffekseerForDXLib.h"
#include "../Util/Input.h"

Effect* Effect::m_instance = nullptr;

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
	m_effectData[EffectKind::kEffectKind::kShot].m_handle = LoadEffekseerEffect("Data/Effect/Shot.efk");
	m_effectData[EffectKind::kEffectKind::kKnife].m_handle = LoadEffekseerEffect("Data/Effect/Knife.efk");

	m_effectData[EffectKind::kEffectKind::kKnife].m_scale = 30.0f;
	m_effectData[EffectKind::kEffectKind::kKnife].m_totalTime = 60;

	m_effectData[EffectKind::kEffectKind::kShot].m_scale = 30.0f;
	m_effectData[EffectKind::kEffectKind::kShot].m_totalTime = 60;
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

		//// 　エフェクトが〇〇だったらループ再生させる
		//if (kind == EffectKind::)
		//{
		//	data.m_isLoop = true;
		//}

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