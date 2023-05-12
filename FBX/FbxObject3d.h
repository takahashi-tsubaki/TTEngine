#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<DirectXMath.h>
#include <string>

class FbxObject3d
{
protected://�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransfrom
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

public://��I�����o�֐�
	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

	static void CreateGraphicsPipeline();

private://��I�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;

	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12PipelineState> pipelineState;

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="fbxModel"></param>
	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	void SetPosition(XMFLOAT3 position) { this->position = position; }

protected:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;

	FbxModel* fbxModel = nullptr;

};