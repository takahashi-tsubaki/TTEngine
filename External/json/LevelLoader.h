#pragma once
#include <vector>
#include <string>
#include "Vector3.h"

struct LevelEditer
{
	struct ObjectData {
		// �t�@�C����
		std::string filename;
		// ���s�ړ�
		Vector3 translation;
		// ��]�p
		Vector3 rotation;
		// �X�P�[�����O
		Vector3 scaling;
	};
	//�I�u�W�F�N�g
	std::vector<ObjectData> objects;
};

class LevelLoader
{
public://�����o�֐�

	//�f�B���N�g���[
	static const std::string directry;

	//�t�@�C���g���q
	static const std::string kakutyousi;

public://�����o�ϐ�

	static LevelEditer* LoadFile(const std::string& filename);
	// �x���@ -> ���W�A��
	static float RadConvert(float value);
};

