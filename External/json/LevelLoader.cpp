#include "LevelLoader.h"
#include <cassert>
#include <fstream>

#include<json.hpp>

const std::string LevelLoader::directry = "Resources/levels/";

//�t�@�C���g���q
const std::string LevelLoader::kakutyousi = ".json";

LevelEditer* LevelLoader::LoadFile(const std::string& filename)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = directry + filename + kakutyousi;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name" �𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelEditer* levelEditer = new LevelEditer();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//��ʂ̎擾
		std::string type = object["type"].get<std::string>();

		//��ނ��Ƃ̏���
		//MESH
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelEditer->objects.emplace_back(LevelEditer::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelEditer::ObjectData& objectData = levelEditer->objects.back();

			if (object.contains("file_name"))
			{
				//�t�@�C����
				objectData.filename = object["file_name"];
				
			}

			//�g�����X�t�H�[���p�����[�^�̓ǂݍ���
			nlohmann::json& transform = object["transform"];
			// ���s�ړ�
			objectData.translation.x = -(float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][1];
			objectData.translation.z = (float)transform["translation"][2]; // -
			//objectData.translation.z = - objectData.translation.z;
			// ��]�p
			objectData.rotation.x = LevelLoader::RadConvert((float)transform["rotation"][2]); // -
			objectData.rotation.x = -objectData.rotation.x;
			objectData.rotation.y = LevelLoader::RadConvert((float)transform["rotation"][1]); // -
			objectData.rotation.y = -objectData.rotation.y;
			objectData.rotation.z = LevelLoader::RadConvert((float)transform["rotation"][0]);
			// �X�P�[�����O
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][1];
			objectData.scaling.z = (float)transform["scaling"][2];

			// TODO: �R���C�_�[�̃p�����[�^�ǂݍ���
			// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
			if (object.contains("children")) {

			}
		}
	}


	return levelEditer;
}
float LevelLoader::RadConvert(float value) {
	float radValue;

	radValue = value * (3.141592f / 180);

	return radValue;
}