#pragma once
#include "../Object/Player.h"
#include <map>
#include <string>

class Player;

/// <summary>
/// シングルトンを使用してCSVファイル読み込みを行う
/// </summary>
class LoadCsv
{
public:
	// コピーコンストラクタの禁止
	LoadCsv(const LoadCsv&) = delete;
	// ムーブコンストラクタの禁止
	LoadCsv(LoadCsv&&) = delete;
	// 代入演算子の禁止
	LoadCsv& operator= (const LoadCsv&) = delete;
	LoadCsv& operator= (LoadCsv&&) = delete;

	// インスタンスを取得する
	static LoadCsv& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCsv;
		}
		return *m_instance;
	}

	/// <summary>
	/// 共通csvファイルのロード
	/// </summary>
	void LoadCommonFileData(ModelBase::CharaCommonData& charaData, std::string charaName);

	/// <summary>
	/// プレイヤーのみcsvファイルのロード
	/// </summary>
	/// <param name="charaData"></param>
	/// <param name="charaName"></param>
	void LoadPlayerOnlyFileData(std::map<std::string, Player::PlayerOnlyData>& playerData);


	void LoadRankingData(std::map<std::string, int>& rankingData);

private:
	LoadCsv() = default;
	virtual ~LoadCsv() = default;

private:
	static LoadCsv* m_instance;				   // メッセージのインスタンス
	std::map<std::string, std::string> m_data; // メッセージのデータ
};