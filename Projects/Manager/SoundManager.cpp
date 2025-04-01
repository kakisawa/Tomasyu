#include "SoundManager.h"
#include "../Util/Input.h"
#include "DxLib.h"
#include <cassert>

namespace {
	constexpr int kMaxVolume = 255;						// 音量の最大値
	constexpr int kChangeVolumeMaxNum = 7;				// 音量増減の最大数
	constexpr int kVolumeStepSize = kMaxVolume / kChangeVolumeMaxNum;	// 一回当たりの音量増減値

	// 音量の初期値
	constexpr int kInitBgmVolume = kVolumeStepSize * 7;	// BGM初期音量
	constexpr int kInitSeVolume = kVolumeStepSize * 5;	// SE初期音量

	// 変更後音量保存
	int kChangeBgm = kInitBgmVolume;
	int kChangeSe = kInitSeVolume;
}

SoundManager::~SoundManager()
{
	ReleaseSound();
}

void SoundManager::Init()
{
	// 音量を調整
	InitSound();
	LoadSE(SoundManager::SE_Type::kSelectSE);
}

void SoundManager::ChangeSEVolume(Input& input)
{
	// 右ボタンで音量を上げる
	if (input.IsTrigger(InputInfo::Right))
	{
		m_seVolume += kVolumeStepSize;
		SetSeVolume();
		PlaySE(SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		if (m_seVolume >= kMaxVolume - kVolumeStepSize)
		{
			m_seVolume = kMaxVolume;
		}
		kChangeSe = m_seVolume;
	}

	// 左ボタンで音量を下げる
	if (input.IsTrigger(InputInfo::Left))
	{
		m_seVolume -= kVolumeStepSize;
		SetSeVolume();
		PlaySE(SE_Type::kSelectSE, DX_PLAYTYPE_BACK);
		if (m_seVolume <= 0)
		{
			m_seVolume = 0;
		}
		kChangeSe = m_seVolume;
	}
}

void SoundManager::ChangeBGMVolume(Input& input)
{
	// 右ボタンで音量を上げる
	if (input.IsTrigger(InputInfo::Right))
	{
		m_bgmVolume += kVolumeStepSize;
		SetBgmVolume();
		if (m_bgmVolume >= kMaxVolume- kVolumeStepSize)
		{
			m_bgmVolume = kMaxVolume;
		}
		kChangeBgm = m_bgmVolume;
	}

	// 左ボタンで音量を下げる
	if (input.IsTrigger(InputInfo::Left))
	{
		m_bgmVolume -= kVolumeStepSize;
		SetBgmVolume();
		if (m_bgmVolume <= 0)
		{
			m_bgmVolume = 0;
		}
		kChangeBgm = m_bgmVolume;
	}
}

void SoundManager::InitSound(void)
{
	InitBGM();
	InitSE();
}

void SoundManager::InitBGM(void)
{
	// 音量を設定する
	m_bgmVolume = kChangeBgm;

	// BGMのパスをつなげる
	m_bgmPass[BGM_Type::kTitleBGM] = "titleBgm.mp3";
	m_bgmPass[BGM_Type::kSelectBGM] = "selectBgm.mp3";
	m_bgmPass[BGM_Type::kRankingBGM] = "Ranking.mp3";
	m_bgmPass[BGM_Type::kOptionBGM] = "";
	m_bgmPass[BGM_Type::kGameBGM] = "Game.mp3";
	m_bgmPass[BGM_Type::kGameClearBGM] = "GameClear.mp3";
	m_bgmPass[BGM_Type::kGameOverBGM] = "";
}

void SoundManager::LoadBGM(BGM_Type bgm)
{
	// BGMを読み込む
	m_bgm[bgm] = LoadSoundMem(("Data/Sound/BGM/" + m_bgmPass[bgm]).c_str());
	assert(m_bgm[bgm] != -1);
}

void SoundManager::InitSE(void)
{
	// 音量を設定する
	m_seVolume = kChangeSe;

	// SEのパスをつなげる
	m_sePass[SE_Type::kSelectSE] = "Select.mp3";
	m_sePass[SE_Type::kButtonSE] = "Button.mp3";
	m_sePass[SE_Type::kBackSE] = "";
	m_sePass[SE_Type::kHandGunSE] = "HandGun.mp3";
	m_sePass[SE_Type::kMachineGunSE] = "";
	m_sePass[SE_Type::kKnifeSE] = "Knife.mp3";
	m_sePass[SE_Type::kInstallationSE] = "";
	m_sePass[SE_Type::kDrinkSE] = "";
	m_sePass[SE_Type::kSummonSE] = "";
	m_sePass[SE_Type::kDamageReceivedSE] = "";
	m_sePass[SE_Type::kPunchSE1] = "Punch1.mp3";
	m_sePass[SE_Type::kPunchSE2] = "Punch2.mp3";
	m_sePass[SE_Type::kDeathrattle] = "Deathrattle.mp3";
}

void SoundManager::LoadSE(SE_Type se)
{
	// SEを読み込む
	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	m_se[se] = LoadSoundMem(("Data/Sound/SE/" + m_sePass[se]).c_str(), 6);
	assert(m_se[se] != -1);
}

void SoundManager::PlayBGM(BGM_Type bgm, int playType, int volumePar, bool topPositionFlag)
{
	ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), m_bgm[bgm]);
	PlaySoundMem(m_bgm[bgm], playType, topPositionFlag);
}

void SoundManager::PlaySE(SE_Type se, int playType, int volumePar, bool topPositionFlag)
{
	ChangeVolumeSoundMem(static_cast<int>(m_seVolume), m_se[se]);
	PlaySoundMem(m_se[se], playType, topPositionFlag);
}

bool SoundManager::CheckPlayBGM(BGM_Type bgm)
{
	return CheckSoundMem(m_bgm[bgm]);
}

bool SoundManager::CheckPlaySE(SE_Type se)
{
	return CheckSoundMem(m_se[se]);
}

void SoundManager::StopBGM(BGM_Type bgm)
{
	StopSoundMem(m_bgm[bgm]);
}

void SoundManager::StopSE(SE_Type se)
{
	StopSoundMem(m_se[se]);
}

void SoundManager::ReleaseSound(void)
{
	// BGMの解放
	for (int b = 0; b < static_cast<int>(BGM_Type::MAX); b++)
	{
		DeleteSoundMem(m_bgm[static_cast<BGM_Type>(b)]);
	}

	// SEの解放
	for (int s = 0; s < static_cast<int>(SE_Type::MAX); s++)
	{
		DeleteSoundMem(m_se[static_cast<SE_Type>(s)]);
	}
}

void SoundManager::SetBgmVolume()
{
	for (const auto& entry : m_bgm)
	{
		ChangeVolumeSoundMem(static_cast<int>(m_bgmVolume), entry.second);
	}
}

void SoundManager::SetSeVolume()
{
	for (const auto& entry : m_se)
	{
		ChangeVolumeSoundMem(static_cast<int>(m_seVolume), entry.second);
	}
}