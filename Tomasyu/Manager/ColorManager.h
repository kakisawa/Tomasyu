#pragma once
class ColorManager
{
public:
	ColorManager();
	virtual ~ColorManager() {}

public:
	int GetColorBlack() { return m_colorBlack; }	// 黒
	int GetColorWhite() { return m_colorWhite; }	// 白
	int GetColorRed() { return m_colorRed; }		// 赤
	int GetColorBlue() { return m_colorBlue; }		// 青

private:
	int m_colorBlack;
	int m_colorWhite;
	int m_colorRed;
	int m_colorBlue;
};