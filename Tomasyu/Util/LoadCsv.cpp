#include "LoadCsv.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

LoadCsv* LoadCsv::m_instance = nullptr;

namespace
{
	const char* const kCommonCharaInfoFileName = "Data/csv/CharaInfo.csv";
	const char* const kPlayerOnlyDataFileName = "Data/csv/PlayerOnlyData.csv";


	/// <summary>
	/// 文字列を分割する
	/// </summary>
	/// <param name="input">文字列</param>
	/// <param name="delimiter">区切る文字(,)</param>
	/// <returns>分割した文字列</returns>
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

void LoadCsv::LoadCommonFileData(ModelBase::CharaCommonData& charaData, std::string charaName)
{
	std::ifstream ifs(kCommonCharaInfoFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();
		if (str == charaName) {
			try
			{
				// プレイヤーの詳細
				charaData.maxHp = std::stoi(strvec[1]);		// 最大HP
				charaData.initPosX = std::stof(strvec[2]);	// 初期座標X
				charaData.initPosY = std::stof(strvec[3]);	// 初期座標Y
				charaData.initPosZ = std::stof(strvec[4]);	// 初期座標Z
				charaData.modelSize = std::stof(strvec[5]);	// モデルサイズ
				charaData.walkSpeed = std::stof(strvec[6]);	// 歩く速度
				charaData.rotaSpeed = std::stof(strvec[7]);	// 回転速度
				charaData.bodyColRad = std::stof(strvec[8]);	// 体当たり判定半径
				charaData.bodyColUpY = std::stof(strvec[9]);// 体当たり判定Y座標
			}
			catch (const std::exception&)
			{

			}
		}
	}


}

void LoadCsv::LoadPlayerOnlyFileData(std::map<std::string, Player::PlayerOnlyData>& playerData)
{
	std::ifstream ifs(kPlayerOnlyDataFileName);
	std::string line;
	std::vector<std::string> strvec;
	m_data.clear();

	while (std::getline(ifs, line))
	{
		strvec = split(line, ',');
		const char* str = strvec[0].c_str();

		try
		{
			// プレイヤーの詳細
			playerData[str].attack = std::stoi(strvec[1]);
			playerData[str].sizeX = std::stof(strvec[2]);
			playerData[str].sizeY = std::stof(strvec[3]);
			playerData[str].sizeZ = std::stof(strvec[4]);
			playerData[str].RotaX = std::stof(strvec[5]);
			playerData[str].RotaY = std::stof(strvec[6]);
			playerData[str].RotaZ = std::stof(strvec[7]);
			playerData[str].HitRad = std::stof(strvec[8]);
		}
		catch (const std::exception&)
		{

		}

	}
}
