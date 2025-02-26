#include "TitleCamera.h"
#include <cmath>
#include <algorithm>

namespace {
	constexpr float kCameraNear = 0.1f;			// �J������O�N���b�v����
	constexpr float kCameraFar = 5000.0f;		// �J�����ŉ��N���b�v����

	constexpr float kDist = -80.0f;				// �J��������v���C���[�܂ł̋���

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏������l
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ���C�g�̎w����
}

TitleCamera::TitleCamera():
	m_pos(kInitVec),
	m_targetPos(kInitVec)
{
	// 	�J�����̎�O�N���b�v�����Ɖ��N���b�v������ݒ肷��
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void TitleCamera::Init(VECTOR pos)
{
	m_targetPos = pos;
	m_targetPos = VAdd(m_targetPos, VGet(10.0f, 50.0f, 0.0f));
	m_pos = VGet(50.0f, 60.0f, 230.0f);
}

void TitleCamera::Update()
{
	// �J�����̏������C�u�����̃J�����ɔ��f������
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);

	// 	�W�����C�g�̃^�C�v���f�B���N�V���i�����C�g�ɂ���
	ChangeLightTypeDir(kLightDirection);
}