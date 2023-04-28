#pragma once

#include <string>
#include <DirectXMath.h>
#include <vector>

#include<DirectXTex.h>

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ϊ��s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ϊ��s��
	DirectX::XMMATRIX worldTransform;
	//�e�m�[�h
	Node* parent = nullptr;

};

class FbxModel
{
public:

	//�t�����h�N���X
	friend class FbxLoader;

public://�T�u�N���X
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;//xyz���W
		DirectX::XMFLOAT3 normal;//�@���x�N�g��
		DirectX::XMFLOAT3 uv;//uv���W
	};

private:

	//���f����
	std::string name;

	//�m�[�h�z��
	std::vector<Node> nodes;

	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	//�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = {1,1,1};
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};

};
