#include "TitleCamera.h"
#include <cmath>
#include <algorithm>

namespace {
	constexpr float kCameraNear = 1.0f;			// �J������O�N���b�v����
	constexpr float kCameraFar = 10000.0f;		// �J�����ŉ��N���b�v����
	constexpr float kDist = -80.0f;				// �J��������v���C���[�܂ł̋���
	constexpr float kRightStickAngle = 0.03f;	// �J�����𓮂����p�x
	constexpr float kLeftStickAngle = 0.01f;	// �J�����𓮂����p�x

	constexpr float kInitAngleH = 1.7f;		// �J�����̏������s�p�x
	constexpr float kInitAngleV = 0.3f;		// �J�����̏��������p�x
	constexpr float kMinAngleV = DX_PI_F * 0.5f - 0.5f;	// �ŏ��̐����p�x
	constexpr float kMaxAngleV = -DX_PI_F * 0.5f + 1.0f;	// �ő�̐����p�x


	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏������l
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ���C�g�̎w����
}

TitleCamera::TitleCamera():
	m_pos(VGet(0.0f,0.0f,0.0f)),
	m_targetPos(VGet(0.0f, 70.0f, 1000.0f)),
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV)
{
}

void TitleCamera::Init()
{
	// 	�J�����̎�O�N���b�v�����Ɖ��N���b�v������ݒ肷��
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void TitleCamera::Update()
{
	// �J�����̏������C�u�����̃J�����ɔ��f������
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);

	// 	�W�����C�g�̃^�C�v���f�B���N�V���i�����C�g�ɂ���
	ChangeLightTypeDir(kLightDirection);

	m_pos = VAdd(m_pos, m_targetPos);
}