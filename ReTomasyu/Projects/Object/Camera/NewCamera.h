#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy;
class NewCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pPlayer">�v���C���[���</param>
	/// <param name="pEnemy">�G���</param>
	NewCamera(std::shared_ptr<Player> pPlayer, std::shared_ptr<Enemy> pEnemy);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~NewCamera(){}

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ʏ�J�����X�V
	/// </summary>
	/// <param name="target">�����_���W</param>
	void NormalUpdate(VECTOR target);

	/// <summary>
	/// ���b�N�I�����J�����X�V
	/// </summary>
	/// <param name="target">�����_���W</param>
	void LookOnUpdate(VECTOR target);
	
	/// <summary>
	/// �p�x�X�V
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// ���W�̍X�V
	/// </summary>
	void UpdatePos();

	/// <summary>
	/// ���X�e�B�b�N���g�p�����J�����̍X�V
	/// </summary>
	void LeftStickCameraUpdate();

	/// <summary>
	/// �^�[�Q�b�g���W���Z�b�g����
	/// </summary>
	/// <param name="target"></param>
	void SetTargetPos(const VECTOR& target) { m_targetPos = target; }

	/// <summary>
	/// ���W�n��
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	const VECTOR& GetPos() const { return m_pos; }

	/// <summary>
	/// �����_���W�n��
	/// </summary>
	/// <returns>�����_���W</returns>
	const VECTOR& GetTargetPos()	const { return m_targetPos; }

private:

	float m_angleH;	 // �����p�x
	float m_angleV;	 // �����p�x
	float m_cameraToTargetLength;	// �J��������^�[�Q�b�g�܂ł̍ŏ�����

	bool m_isLookOn;	// ���O�܂Ń��b�N�I�������Ă�����

	VECTOR m_pos;		// �J�����̍��W
	VECTOR m_targetPos;	// �����_���W

	VECTOR m_prevPos;	// �O�̍��W
	VECTOR m_nextPos;	// ���̍��W

	DINPUT_JOYSTATE rightInput;	// �E�W���C�p�b�g�̓���
	DINPUT_JOYSTATE leftInput;	// ���W���C�p�b�g�̓���

	std::shared_ptr<Player> m_pPlayer;	// �v���C���[�̃|�C���^
	std::shared_ptr<Enemy> m_pEnemy;	// �G�̃|�C���^
};