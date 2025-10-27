#include "NewCamera.h"
#include "../Player/Player.h"
#include "../Enemy.h"
#include <cmath>
#include <algorithm>
#include <memory>

namespace {
	constexpr float kCameraHeight = 65.0f;		// �J�����̍���
	constexpr float kCameraNear = 1.0f;			// �J������O�N���b�v����
	constexpr float kCameraFar = 10000.0f;		// �J�����ŉ��N���b�v����
	constexpr float kDist = 70.0f;				// �J��������v���C���[�܂ł̋���
	constexpr float kRightStickAngle = 0.08f;	// �E�X�e�B�b�N�ŃJ�����𓮂����p�x
	constexpr float kLeftStickAngle = 0.01f;	// ���X�e�B�b�N�ŃJ�����𓮂����p�x
	constexpr float kInitAngleH = DX_PI_F;		// �J�����̏������s�p�x
	constexpr float kInitAngleV = 0.3f;			// �J�����̏��������p�x
	constexpr float kMinAngleV = -DX_PI_F * 0.5f + 0.5f;	// �ŏ��̐����p�x
	constexpr float kMaxAngleV = DX_PI_F * 0.5f - 1.0f;		// �ő�̐����p�x

	constexpr float kCameraFollowSpeed = 0.2f;	// �J�������t���Ă��鑬�x
	constexpr float kPrevCameraFollowSpeed = 1.0f - kCameraFollowSpeed;	// �J�����̏����x
	constexpr float kCameraTargetFollowSpeed = 0.2f;	// �J�����������_��ǂ������鑬�x
	constexpr float kPrevCameraTargetFollowSpeed = 1.0f-kCameraTargetFollowSpeed;	// �J�����������_��ǂ������鏉���x

	/*���b�N�I����*/
	constexpr float kLockOnAngleH = -DX_PI_F * 0.5f + 0.1f;										// ���b�N�I�����ɃJ�����������X���邽�߂̒l
	constexpr float kLockOnAngleV = -DX_PI_F / 2.0f + 1.3f;										// ���b�N�I�����ɃJ�����������X���邽�߂̒l
	constexpr float kCameraTargetLockOnFollowSpeed = 0.5f;										// ���b�N�I�����ɃJ�������t���Ă��鑬�x
	constexpr float kPrevCameraTargetLockOnFollowSpeed = 1.0f - kCameraTargetLockOnFollowSpeed;	// ���b�N�I�����ɃJ�������t���Ă��鏉���x

	const VECTOR kInitVec = VGet(0.0f, 0.0f, 0.0f);	// �x�N�g���̏������l
	const VECTOR kLightDirection = VGet(20.0f, -50.0f, 0.0f);	// ���C�g�̎w����
}

NewCamera::NewCamera(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy) :
	m_angleH(kInitAngleH),
	m_angleV(kInitAngleV),
	m_cameraToTargetLength(0.0f),
	m_isLookOn(false),
	m_pos(kInitVec),
	m_targetPos(kInitVec),
	m_prevPos(kInitVec),
	m_nextPos(kInitVec),
	m_pPlayer(pPlayer),
	m_pEnemy(pEnemy)
{
}

void NewCamera::Init()
{
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void NewCamera::Update()
{
	// �O�̍��W��ۑ�����
	m_prevPos = m_nextPos;

	// �v���C���[�ʒu�擾
	bool isLockOnNow = m_pPlayer->GetLockOn();
	VECTOR playerPos = m_pPlayer->GetPos();

	// �����_���W��ݒ肷��(�����͕ʓr����)
	VECTOR viewPointPos = playerPos;
	viewPointPos.y += kCameraHeight;

	// �p�x�X�V
	LeftStickCameraUpdate();
	UpdateAngle();

	if (isLockOnNow) {	// ���b�N�I����


		//�v�C��
		
		//VECTOR enemyPos=VAdd()
		// �J�������b�N�I�����̍X�V
		LookOnUpdate(m_pEnemy->GetPos());
	}
	else {	// �ʏ펞
		// �ʏ펞�X�V
		NormalUpdate(viewPointPos);
	}

	// ���W�̊m��
	UpdatePos();

	// �J�����̏��𔽉f������
	m_pos = VAdd(VScale(m_prevPos, kPrevCameraFollowSpeed), VScale(m_nextPos, kCameraFollowSpeed));
	// ���W�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	// �W�����C�g�̃^�C�v���f�B���N�V���i�����C�g�ɂ���
	ChangeLightTypeDir(kLightDirection);
}

void NewCamera::Draw()
{
	DrawFormatString(0, 500, 0xffffff, "Camera:m_pos.x/y/z=%.2f/%.2f/%.2f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 520, 0xffffff, "Camera:m_targetPos.x/y/z=%.2f/%.2f/%.2f",
		m_targetPos.x, m_targetPos.y, m_targetPos.z);

	DrawSphere3D(m_targetPos, 3.0f, 32, 0xff00ff, 0xff00ff, true);
}

void NewCamera::NormalUpdate(VECTOR target)
{
	// �^�[�Q�b�g���W�X�V
	m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (target.x * kCameraTargetFollowSpeed);
	m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (target.y * kCameraTargetFollowSpeed);
	m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (target.z * kCameraTargetFollowSpeed);
}

void NewCamera::LookOnUpdate(VECTOR target)
{
	// �^�[�Q�b�g���W�X�V
	m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (target.x * kCameraTargetFollowSpeed);
	m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (target.y * kCameraTargetFollowSpeed);
	m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (target.z * kCameraTargetFollowSpeed);

	// �v���C���[����J�����܂ł̕����x�N�g�����o��
	VECTOR playerToCameraVec = VNorm(VSub(m_nextPos, m_targetPos));
	// �v���C���[����G�܂ł̕����x�N�g�����o��
	VECTOR playerToEnemyVec = VNorm(VSub(m_pEnemy->GetPos(), target));

	// ���b�N�I�����̃J�����̕����x�N�g�����o��
	VECTOR lookOnCameraVec = VAdd(VScale(playerToCameraVec, kPrevCameraTargetLockOnFollowSpeed), 
		VScale(playerToEnemyVec, kCameraTargetLockOnFollowSpeed));
	// �J�����������X����
	m_angleH = static_cast<float>(atan2(-lookOnCameraVec.z, lookOnCameraVec.x) + kLockOnAngleH);

	// �v���C���[����G�܂ł̊p�x(��������)�����߂�
	float targetAngleV = static_cast<float>(atan2(playerToEnemyVec.y,
		sqrt(playerToEnemyVec.x * playerToEnemyVec.x + playerToEnemyVec.z * playerToEnemyVec.z)));
	// �㉺�����̊p�x����
	const float minAngleV = -DX_PI_F * 0.2f;
	const float maxAngleV = DX_PI_F * 0.1f;
	// m_angleV�̕��
	m_angleV = (m_angleV * kPrevCameraFollowSpeed) + (targetAngleV * kCameraFollowSpeed);
	// �p�x�����̓K�p
	m_angleV = std::max(std::min(m_angleV, maxAngleV), minAngleV);

	// �J�����̍��W���o��
	VECTOR cameraPos = VAdd(VScale(m_prevPos, kPrevCameraTargetLockOnFollowSpeed),
		VScale(m_nextPos, kCameraTargetLockOnFollowSpeed));
	// �J�����̍��W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY(cameraPos, m_targetPos);
}

void NewCamera::UpdateAngle()
{
	// ���b�N�I�����͉������Ȃ�
	if (m_pPlayer->GetLockOn()) return;

	// ���͏�ԏ�����
	rightInput.X = 0;
	rightInput.Y = 0;
	// ���͏�ԏ�����
	leftInput.X = 0;
	leftInput.Y = 0;

	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &rightInput);

	// ��]��1�`1000����0.001�`1�͈̔͂ɂ���
	// ��ŃJ�������x���p�ӂ���(��U3�ɂ��Ă���)
	float rotX = 0.00020f * rightInput.Rx * 3;
	float rotY = 0.00017f * rightInput.Ry * 3;

	// �J�����̉�]�X�s�[�h��������
	rotX *= kRightStickAngle;
	rotY *= kRightStickAngle;

	m_angleH += rotX;
	if (rightInput.Rx < 0.0f)
	{
		// -180���ȉ��ɂȂ�����p�x�n���傫���Ȃ肷���Ȃ��悤��360���𑫂�
		if (m_angleH < -DX_PI_F)
		{
			m_angleH += DX_TWO_PI_F;
		}
	}
	if (rightInput.Rx > 0.0f)
	{
		// 180���ȏ�ɂȂ�����p�x�n���傫���Ȃ肷���Ȃ��悤��360��������
		if (m_angleH > DX_PI_F)
		{
			m_angleH -= DX_TWO_PI_F;
		}
	}

	m_angleV -= rotY;
	// ���p�x�ȉ��ɂɂ͂Ȃ�Ȃ��悤�ɂ���
	if (m_angleV < kMinAngleV)
	{
		m_angleV = kMinAngleV;
	}
	// ���p�x�ȏ�ɂ͂Ȃ�Ȃ��悤�ɂ���
	if (m_angleV > kMaxAngleV)
	{
		m_angleV = kMaxAngleV;
	}
}

void NewCamera::UpdatePos()
{
	constexpr float kCameraYOffset = 50.0f;

	// ���������̉�](X��)
	MATRIX RotX = MGetRotX(m_angleV);
	// ���������̉�](Y��)
	MATRIX RotY = MGetRotY(m_angleH);

	/*�J�����̍��W�̌v�Z
	(X���ɃJ�����ƃv���C���[�Ƃ̋����������L�т��x�N�g����
	����������]��������ɐ���������]����
	����ɒ����_�̍��W�𑫂�)*/
	m_cameraToTargetLength = std::min(m_cameraToTargetLength + 5.0f, kDist);

	m_nextPos = VAdd(VTransform(VTransform(VGet(0.0f, kCameraYOffset, m_cameraToTargetLength), RotX), RotY), m_targetPos);
}

void NewCamera::LeftStickCameraUpdate()
{
	// ���͏�ԏ�����
	leftInput.X = 0;
	leftInput.Y = 0;

	GetJoypadDirectInputState(DX_INPUT_PAD1, &leftInput);

	if (leftInput.X < 0.0f)
	{
		m_angleH -= kLeftStickAngle;	// ���X�e�B�b�N���E�ɓ|������E��]����
	}
	if (leftInput.X > 0.0f)
	{
		m_angleH += kLeftStickAngle;	// ���X�e�B�b�N�����ɓ|�����獶��]����
	}
}