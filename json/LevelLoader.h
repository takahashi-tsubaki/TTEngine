#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include "json.hpp"

struct LevelEditer
{
	struct ObjectData {
		// �t�@�C����
		std::string filename;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
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

};

