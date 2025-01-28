#include "RankingData.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>

namespace {
	const char* kTimeFilePath = "Data/csv/TimeRankingData.csv";	// �X�R�A�����L���O�f�[�^��ۑ�����t�@�C����
	const char* kScoreFilePath = "Data/csv/ScoreRankingData.csv";	// �^�C�������L���O�f�[�^��ۑ�����t�@�C����
	constexpr int kRankingNum = 5;	// �ۑ����鏇�ʐ�
}

RankingData::RankingData()
{
	ScoreLoad();
	TimeLoad();
}

void RankingData::ScoreLoad()
{
	std::ifstream file(kScoreFilePath);


	if (!file.is_open()) // �t�@�C���ǂݍ��ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���ǂݍ��ݎ��s\n");
#endif
	}
	else // �t�@�C���ǂݍ��ݐ���
	{
		std::string line;
		m_scoreRanking.clear(); // ���łɗv�f������ꍇ�̓N���A����
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			int score = 0, year = 0, month = 0, day = 0, hour = 0, min = 0;
			char comma;
			ss >>  score >> comma >> year >> comma >> month >> comma >> day >> comma >> hour >> comma >> min;
			if (ss.fail()) {
#ifdef _DEBUG
				printfDx("�f�[�^�̓ǂݍ��݂Ɏ��s���܂���: %s\n", line.c_str());
#endif
				continue; // �ǂݍ��݂Ɏ��s�����ꍇ�͎��̍s��
			}
			m_scoreRanking.emplace_back(score, year, month, day, hour, min);
		}
		file.close();
	}
}

void RankingData::TimeLoad()
{
	std::ifstream file(kTimeFilePath);


	if (!file.is_open()) // �t�@�C���ǂݍ��ݎ��s
	{
#ifdef _DEBUG
		printfDx("�t�@�C���ǂݍ��ݎ��s\n");
#endif
	}
	else // �t�@�C���ǂݍ��ݐ���
	{
		std::string line;
		m_timeRanking.clear(); // ���łɗv�f������ꍇ�̓N���A����
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			int time = 0, year = 0, month = 0, day = 0, hour = 0, min = 0;
			char comma;
			ss >> time >> comma >>  year >> comma >> month >> comma >> day >> comma >> hour >> comma >> min;
			if (ss.fail()) {
#ifdef _DEBUG
				printfDx("�f�[�^�̓ǂݍ��݂Ɏ��s���܂���: %s\n", line.c_str());
#endif
				continue; // �ǂݍ��݂Ɏ��s�����ꍇ�͎��̍s��
			}
			m_timeRanking.emplace_back(time, year, month, day, hour, min);
		}
		file.close();
	}
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

	m_scoreRanking.emplace_back(score, year, month, day, hour, min);
	// �����L���O���\�[�g���ď�� kRankingNum ����ۑ�
	std::sort(m_scoreRanking.begin(), m_scoreRanking.end(), [](const std::tuple<int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int>& rhs) {
		return std::get<0>(lhs) > std::get<0>(rhs); // �N���A���ԂŃ\�[�g
		});
	if (m_scoreRanking.size() > kRankingNum)
	{
		m_scoreRanking.resize(kRankingNum);
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
		for (const auto& entry : m_scoreRanking)
		{
			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
				<< std::get<4>(entry) << "," << std::get<5>(entry) << "\n";
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

	m_timeRanking.emplace_back(time, year, month, day, hour, min);
	// �����L���O���\�[�g���ď�� kRankingNum ����ۑ�
	std::sort(m_timeRanking.begin(), m_timeRanking.end(), [](const std::tuple<int, int, int, int, int, int>& lhs, const std::tuple<int, int, int, int, int, int>& rhs) {
		return std::get<0>(lhs) < std::get<0>(rhs); // �N���A���ԂŃ\�[�g
		});
	if (m_timeRanking.size() > kRankingNum)
	{
		m_timeRanking.resize(kRankingNum);
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
		for (const auto& entry : m_timeRanking)
		{
			file << std::get<0>(entry) << "," << std::get<1>(entry) << ","
				<< std::get<2>(entry) << "," << std::get<3>(entry) << ","
				<< std::get<4>(entry) << "," << std::get<5>(entry) <<"\n";
		}
		file.close();
	}
}
