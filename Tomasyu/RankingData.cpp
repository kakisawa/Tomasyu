#include "RankingData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>

namespace {
	const char* kTimeFilePath = "Data/csv/ScoreRankingData.csv";	// �X�R�A�����L���O�f�[�^��ۑ�����t�@�C����
	const char* kScoreFilePath = "Data/csv/TimeRankingData.csv";	// �^�C�������L���O�f�[�^��ۑ�����t�@�C����
	constexpr int kRankingNum = 5;	// �ۑ����鏇�ʐ�
}

RankingData::RankingData()
{
	Load(kTimeFilePath);
	Load(kScoreFilePath);
}

void RankingData::Load(const char* path)
{
	std::ifstream file(path);


	if (!file.is_open()) // �t�@�C���ǂݍ��ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���ǂݍ��ݎ��s\n");
#endif
	}
	else // �t�@�C���ǂݍ��ݐ���
	{
		std::string line;
		m_ranking.clear(); // ���łɗv�f������ꍇ�̓N���A����
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			int time = 0, score = 0, year = 0, month = 0, day = 0, hour = 0, min = 0;
			char comma;
			ss >> time >> comma >> score >> comma >> year >> comma >> month >> comma >> day >> comma >> hour >> comma >> min;
			if (ss.fail()) {
#ifdef _DEBUG
				printfDx("�f�[�^�̓ǂݍ��݂Ɏ��s���܂���: %s\n", line.c_str());
#endif
				continue; // �ǂݍ��݂Ɏ��s�����ꍇ�͎��̍s��
			}
			m_ranking.emplace_back(time, score, year, month, day, hour, min);
		}
		file.close();
	}
}

void RankingData::Save(int time, int score, int year, int month, int day, int hour, int min)
{
//
//	// ���̒l������ꍇ�͉��������I��
//	if (time <= 0 || score <= 0 || year <= 0 || month <= 0 || day <= 0 || hour <= 0 || min <= 0) {
//#ifdef _DEBUG
//		printfDx("���̒l���܂܂�Ă��邽�߁A�ۑ����X�L�b�v���܂���\n");
//#endif
//		return; // �����𒆒f
//	}
//
//	m_ranking.emplace_back(time, score, year, month, day, hour, min);
//	// �����L���O���\�[�g���ď�� kRankingNum ����ۑ�
//	std::sort(m_ranking.begin(), m_ranking.end(), [](const std::tuple<int, int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int, int>& rhs) {
//		return std::get<0>(lhs) > std::get<0>(rhs); // �N���A���ԂŃ\�[�g
//		});
//	if (m_ranking.size() > kRankingNum)
//	{
//		m_ranking.resize(kRankingNum);
//	}
//
//	std::ofstream file(fileName, std::ios::out | std::ios::trunc);
//
//
//	if (!file.is_open()) // �t�@�C���������ݎ��s
//	{
//#ifdef _DEBUG
//		printfDx("�t�@�C���������ݎ��s\n");
//#endif
//	}
//	else // �t�@�C���������ݐ���
//	{
//		for (const auto& entry : m_ranking)
//		{
//			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
//				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
//				<< std::get<4>(entry) << "," << std::get<5>(entry) << ","
//				<< std::get<6>(entry) << "\n";
//		}
//		file.close();
//	}
}

void RankingData::ScoreSave(int score, int year, int month, int day, int hour, int min)
{

	// ���̒l������ꍇ�͉��������I��
	if ( score <= 0 || year <= 0 || month <= 0 || day <= 0 || hour <= 0 || min <= 0) {
#ifdef _DEBUG
		printfDx("���̒l���܂܂�Ă��邽�߁A�ۑ����X�L�b�v���܂���\n");
#endif
		return; // �����𒆒f
	}

	m_ranking.emplace_back(score, year, month, day, hour, min);
	// �����L���O���\�[�g���ď�� kRankingNum ����ۑ�
	std::sort(m_ranking.begin(), m_ranking.end(), [](const std::tuple<int, int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int, int>& rhs) {
		return std::get<0>(lhs) > std::get<0>(rhs); // �N���A���ԂŃ\�[�g
		});
	if (m_ranking.size() > kRankingNum)
	{
		m_ranking.resize(kRankingNum);
	}

	std::ofstream file(kScoreFilePath, std::ios::out | std::ios::trunc);


	if (!file.is_open()) // �t�@�C���������ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���������ݎ��s\n");
#endif
	}
	else // �t�@�C���������ݐ���
	{
		for (const auto& entry : m_ranking)
		{
			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
				<< std::get<4>(entry) << "," << std::get<5>(entry) << ","
				<< std::get<6>(entry) << "\n";
		}
		file.close();
	}
}

void RankingData::TimeSave(int time, int year, int month, int day, int hour, int min)
{
	// ���̒l������ꍇ�͉��������I��
	if (time <= 0 ||year <= 0 || month <= 0 || day <= 0 || hour <= 0 || min <= 0) {
#ifdef _DEBUG
		printfDx("���̒l���܂܂�Ă��邽�߁A�ۑ����X�L�b�v���܂���\n");
#endif
		return; // �����𒆒f
	}

	m_ranking.emplace_back(time, year, month, day, hour, min);
	// �����L���O���\�[�g���ď�� kRankingNum ����ۑ�
	std::sort(m_ranking.begin(), m_ranking.end(), [](const std::tuple<int, int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int, int>& rhs) {
		return std::get<0>(lhs) > std::get<0>(rhs); // �N���A���ԂŃ\�[�g
		});
	if (m_ranking.size() > kRankingNum)
	{
		m_ranking.resize(kRankingNum);
	}

	std::ofstream file(kTimeFilePath, std::ios::out | std::ios::trunc);


	if (!file.is_open()) // �t�@�C���������ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���������ݎ��s\n");
#endif
	}
	else // �t�@�C���������ݐ���
	{
		for (const auto& entry : m_ranking)
		{
			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
				<< std::get<4>(entry) << "," << std::get<5>(entry) << ","
				<< std::get<6>(entry) << "\n";
		}
		file.close();
	}
}
