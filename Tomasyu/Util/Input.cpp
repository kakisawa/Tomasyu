#include "Input.h"
#include "DxLib.h"
#include <string>
#include <algorithm>

using namespace MyInputInfo;

Input::Input()
{
	m_commandTable[InputInfo::OK] = { {InputType::key,KEY_INPUT_A},
									  {InputType::pad,PAD_INPUT_A} };	// XBoxのAボタン

	m_commandTable[InputInfo::Back] = { {InputType::key,KEY_INPUT_B},
										{InputType::pad,PAD_INPUT_B} };	// XBoxのBボタン

	m_commandTable[InputInfo::Up] = { {InputType::key,KEY_INPUT_UP},
									  {InputType::pad,PAD_INPUT_UP} };

	m_commandTable[InputInfo::Down] = { {InputType::key,KEY_INPUT_DOWN},
										  {InputType::pad,PAD_INPUT_DOWN} };

	m_commandTable[InputInfo::Right] = { {InputType::key,KEY_INPUT_RIGHT},
									  {InputType::pad,PAD_INPUT_RIGHT} };

	m_commandTable[InputInfo::Left] = { {InputType::key,KEY_INPUT_LEFT},
									  {InputType::pad,PAD_INPUT_LEFT} };



	m_commandTable[InputInfo::DebugStart] = { {InputType::key,KEY_INPUT_R},
											  {InputType::pad,PAD_INPUT_R} };	// XBoxのstartボタン

	// プレイヤー操作関係
	m_commandTable[InputInfo::UseItem] = { {InputType::key,KEY_INPUT_SPACE},
												{InputType::pad,PAD_INPUT_X} }; // XBoxのYボタン

	m_commandTable[InputInfo::UseItemChange] = { {InputType::key,KEY_INPUT_Z},
												{InputType::pad,PAD_INPUT_C} };	// XBoxのXボタン

	m_commandTable[InputInfo::Attack] = { {InputType::key,KEY_INPUT_A},
											{InputType::pad,PAD_INPUT_B} };		// XBoxのBボタン

	m_commandTable[InputInfo::ChangeWeapon] = { {InputType::key,KEY_INPUT_Z},
												{InputType::pad,PAD_INPUT_Y} };	// XBoxのLBボタン

	m_commandTable[InputInfo::TargetLockOn] = { {InputType::key,KEY_INPUT_B},
												{InputType::pad,PAD_INPUT_Z} };	// XBoxのRBボタン

	m_commandTable[InputInfo::Roll] = { {InputType::key,KEY_INPUT_B},
											{InputType::pad,PAD_INPUT_A} };		// XBoxのLBボタン


	// デバッグ用
	m_commandTable[InputInfo::DebugDamageReceived] = { {InputType::key,KEY_INPUT_B},
											{InputType::pad,PAD_INPUT_L} };		// XBoxのBackボタン

}

void Input::Update()
{
	// 直前の入力を入れる
	m_lastInput = m_nowInput;

	// パッドとキーボードの情報を取得
	int inputState = GetJoypadInputState(DX_INPUT_PAD1);

	// 現在のキーボード入力を取得
	char keystate[256];
	GetHitKeyStateAll(keystate);	//全キー情報取得

	// m_nowInputの内容を更新
	for (const auto& cmd : m_commandTable)
	{
		auto& input = m_nowInput[cmd.first];
		for (const auto& hardI0 : cmd.second) 
		{
			// キーボードのチェック
			input = false;
			if (hardI0.first == InputType::key)
			{
				if (keystate[hardI0.second])
				{
					input = true;
					break;
				}
			}	// パッドのチェック
			else if (hardI0.first == InputType::pad)
			{
				if (inputState & hardI0.second)
				{
					input = true;
					break;
				}
			}
		}
	}
}

bool Input::IsPress(const InputInfo info) const
{
	auto it = m_nowInput.find(info);

	// 押されていない場合
	if (it == m_nowInput.end())
	{
		return false;
	}

	return m_nowInput.at(info);
}

bool Input::IsTrigger(const InputInfo info) const
{
	auto it = m_nowInput.find(info);

	// 押されていない場合
	if (it == m_nowInput.end()) {
		return false;
	}

	return (m_nowInput.at(info) && !m_lastInput.at(info));
}

bool Input::IsRelease(const InputInfo info) const
{
	auto it = m_nowInput.find(info);

	// 押されていない場合
	if (it == m_nowInput.end()) {
		return false;
	}

	return (!m_nowInput.at(info) && m_lastInput.at(info));
}