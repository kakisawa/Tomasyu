#pragma once
#include <map>
#include <array>
#include <unordered_map>
#include <string>

namespace MyInputInfo{
	enum class InputInfo {
		OK,			// 進む
		Back,		// 戻る
		Up,			// 上
		Down,		// 下
		Right,		// 右
		Left,		// 左
		DebugStart,	// デバッグ用シーン遷移ボタン

		// プレイヤー操作用
		UseItem,		// アイテム使用
		UseItemChange,	// アイテム切り替え
		Attack,			// 攻撃
		ChangeWeapon,	// 武器切り替え
		TargetLockOn,	// ターゲットの照準固定
		Roll,			// 回避
		DebugDamageReceived,		// デバッグ用被ダメージ

		MAX,			// 最大入力数
	};

}
// 入力機器の種類
enum class InputType {
	key,	// キーボード
	pad,	// パッド
	mouse,	// マウス
};

// 入力情報

using namespace MyInputInfo;

class Input
{
public:
	Input();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 押されているかを取得
	/// </summary>
	bool IsPress(const InputInfo info) const;

	/// <summary>
	/// 押された瞬間を取得
	/// </summary>
	bool IsTrigger(const InputInfo info) const;

	/// <summary>
	/// 離した瞬間を取得
	/// </summary>
	bool IsRelease(const InputInfo info) const;


private:

	// コマンドの種類
	using InputTable_t = std::unordered_map<InputInfo, std::map<InputType, int> >;

	// コマンド名と入力のセット
	InputTable_t m_commandTable;

	std::map<InputInfo, bool> m_nowInput;	// 現在の入力
	std::map<InputInfo, bool> m_lastInput;	// 直前の入力
};