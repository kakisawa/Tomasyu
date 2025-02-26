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
	/// <param name="pos">�����_���W</param>
	void Init(VECTOR pos);

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

	VECTOR m_pos;				// �J�������W
	VECTOR m_targetPos;			// �����_���W
};