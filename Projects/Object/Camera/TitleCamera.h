#pragma once
#include "DxLib.h"

class TitleCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleCamera();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TitleCamera(){}

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �J�����ʒu�␳
	/// </summary>
	void FixCameraPos();

	/// <summary>
	/// �J�����̍��W���Z�b�g����
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const VECTOR& pos) { m_pos = pos; }

private:
	float m_angleH;	 // �����p�x
	float m_angleV;	 // �����p�x

	VECTOR m_pos;				// �J�������W
	VECTOR m_targetPos;			// �����_���W
};