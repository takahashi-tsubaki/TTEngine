#include "ParticleObj.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "MyMath.h"
//#include "BaseCollider.h"
//#include "CollisionManager.h"
using namespace std;


#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//Object3d::Material Object3d::material;
const float ParticleObj::radius = 5.0f;		//��ʂ̔��a
const float ParticleObj::prizmHeight = 8.0f;		//���̍���
ID3D12Device* ParticleObj::device = nullptr;
ID3D12GraphicsCommandList* ParticleObj::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleObj::rootsignature;
ComPtr<ID3D12PipelineState> ParticleObj::pipelinestate;
ComPtr<ID3D12RootSignature> ParticleObj::rimRootsignature;
ComPtr<ID3D12PipelineState> ParticleObj::rimPipelinestate;


Camera* ParticleObj::camera_ = nullptr;

//
Matrix4 ParticleObj::matView{};
Matrix4 ParticleObj::matProjection{};
//


//XMFLOAT3 Object3d::eye = { 0.0f, 0.0f, -5.0f };
//XMFLOAT3 Object3d::up = { 0, 1, 0 };
//XMFLOAT3 Object3d::target = { 0, 0, 300.0f };

//Object3d::Object3d()
//{
//}

ParticleObj::~ParticleObj()
{
	//if (collider)
	//{
	//	CollisionManager::GetInstance()->RemoveCollider(collider);
	//	delete collider;
	//}
}

void ParticleObj::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	ParticleObj::device = device;

	ParticleModel::SetDevice(device);

	// �J����������
	//InitializeCamera(window_width, window_height);

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

}

void ParticleObj::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(ParticleObj::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	ParticleObj::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleObj::PostDraw()
{
	// �R�}���h���X�g������
	ParticleObj::cmdList = nullptr;
}

std::unique_ptr<ParticleObj> ParticleObj::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	unique_ptr<ParticleObj> object3d = std::make_unique<ParticleObj>();
	if (object3d == nullptr) {
		return nullptr;
	}

	// ������
	if (!object3d->Initialize(true)) {
		//delete object3d;
		assert(0);
		return nullptr;
	}
	//�X�P�[�����Z�b�g
	float scale_val = 1;
	object3d->worldTransform.scale_ = { scale_val,scale_val ,scale_val };

	return object3d;
}

//void Object3d::SetEye(XMFLOAT3 eye)
//{
//	Object3d::eye = eye;
//
//	UpdateViewMatrix();
//}
//
//void Object3d::SetTarget(XMFLOAT3 target)
//{
//	Object3d::target = target;
//
//	UpdateViewMatrix();
//}






void ParticleObj::InitializeCamera(int window_width, int window_height)
{
	// �r���[�s��̐���
	matView.identity();


	// �������e�ɂ��ˉe�s��̐���
	matProjection.identity();



}

void ParticleObj::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	{
		ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

		// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		result = D3DCompileFromFile(
			L"Resources/shaders/ObjVS.hlsl",	// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&vsBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlob����G���[���e��string�^�ɃR�s�[
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// �G���[���e���o�̓E�B���h�E�ɕ\��
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}

		// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		result = D3DCompileFromFile(
			L"Resources/shaders/ObjPS.hlsl",	// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&psBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlob����G���[���e��string�^�ɃR�s�[
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// �G���[���e���o�̓E�B���h�E�ɕ\��
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}

		// ���_���C�A�E�g
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // xy���W(1�s�ŏ������ق������₷��)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv���W(1�s�ŏ������ق������₷��)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

		// �T���v���}�X�N
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
		// ���X�^���C�U�X�e�[�g
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		// �f�v�X�X�e���V���X�e�[�g
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// �u�����h�X�e�[�g�̐ݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;

		// �[�x�o�b�t�@�̃t�H�[�}�b�g
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// ���_���C�A�E�g�̐ݒ�
		gpipeline.InputLayout.pInputElementDescs = inputLayout;
		gpipeline.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�i�O�p�`�j
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
		gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

		// �f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

		// ���[�g�p�����[�^
		/*CD3DX12_ROOT_PARAMETER rootparams[2];
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);*/

		CD3DX12_ROOT_PARAMETER rootparams[3];
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		// ���[�g�V�O�l�`���̐ݒ�
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> rootSigBlob;
		// �o�[�W������������̃V���A���C�Y
		result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		// ���[�g�V�O�l�`���̐���
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
		assert(SUCCEEDED(result));

		gpipeline.pRootSignature = rootsignature.Get();

		// �O���t�B�b�N�X�p�C�v���C���̐���
		result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
		assert(SUCCEEDED(result));
	}

	{
		ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
		ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

		// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		result = D3DCompileFromFile(
			L"Resources/Shaders/RimLightVS.hlsl",	// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&vsBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlob����G���[���e��string�^�ɃR�s�[
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// �G���[���e���o�̓E�B���h�E�ɕ\��
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}

		// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		result = D3DCompileFromFile(
			L"Resources/Shaders/RimLightPS.hlsl",	// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&psBlob, &errorBlob);
		if (FAILED(result)) {
			// errorBlob����G���[���e��string�^�ɃR�s�[
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			// �G���[���e���o�̓E�B���h�E�ɕ\��
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}

		// ���_���C�A�E�g
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // xy���W(1�s�ŏ������ق������₷��)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv���W(1�s�ŏ������ق������₷��)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

		// �T���v���}�X�N
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
		// ���X�^���C�U�X�e�[�g
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		// �f�v�X�X�e���V���X�e�[�g
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// �u�����h�X�e�[�g�̐ݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;

		// �[�x�o�b�t�@�̃t�H�[�}�b�g
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// ���_���C�A�E�g�̐ݒ�
		gpipeline.InputLayout.pInputElementDescs = inputLayout;
		gpipeline.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�i�O�p�`�j
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
		gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

		// �f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

		//// ���[�g�p�����[�^
		//CD3DX12_ROOT_PARAMETER rootparams[2];
		//rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// ���[�g�p�����[�^
		CD3DX12_ROOT_PARAMETER rootparams[4];
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
		rootparams[3].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		// ���[�g�V�O�l�`���̐ݒ�
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> rootSigBlob;
		// �o�[�W������������̃V���A���C�Y
		result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		// ���[�g�V�O�l�`���̐���
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rimRootsignature));
		assert(SUCCEEDED(result));

		gpipeline.pRootSignature = rimRootsignature.Get();

		// �O���t�B�b�N�X�p�C�v���C���̐���
		result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&rimPipelinestate));
		assert(SUCCEEDED(result));
	}

}


//void Object3d::UpdateViewMatrix()
//{
//	// �r���[�s��̍X�V
//	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
//}

bool ParticleObj::Initialize(bool isRim)
{
	// nullptr�`�F�b�N
	assert(device);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));

	resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(Vector3) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffCameraPosition));
	assert(SUCCEEDED(result));

	resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(Rim) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffRim));
	assert(SUCCEEDED(result));

	this->isRim = isRim;

	return true;

	name = typeid(*this).name();
}

void ParticleObj::Update()
{
	HRESULT result;
	//Matrix4 matScale, matRot, matTrans;

	//matScale.identity();
	//matRot.identity();
	//matTrans.identity();

	//matScale = MathFunc::Scale(scale);
	//matRot = MathFunc::Rotation(rotation, 6);
	//matTrans = MathFunc::Move(position);

	//// ���[���h�s��̍���
	//matWorld.identity(); // �ό`�����Z�b�g
	//matWorld *= matScale;// ���[���h�s��ɃX�P�[�����O�𔽉f
	//matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	//matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	//// �e�I�u�W�F�N�g�������
	//if (parent != nullptr) {
	//	// �e�I�u�W�F�N�g�̃��[���h�s����|����
	//	matWorld *= parent->matWorld;
	//}

	worldTransform.UpdateMatWorld();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	Matrix4 myMat = worldTransform.matWorld_;
	myMat *= camera_->GetViewProjectionMatrix();	// �s��̍��� 
	constMap->mat = myMat;
	constBuffB0->Unmap(0, nullptr);


	XMVECTOR CameraVec = { 10000.0f,10000.0f,10000.0f/*camera_->GetEye().x - worldTransform.translation_.x + 2.0f,camera_->GetEye().x - worldTransform.translation_.y + 2.0f,camera_->GetEye().x - worldTransform.translation_.z*/ };

	CameraVec = XMVector3Normalize(CameraVec);
	// �萔�o�b�t�@B2�փf�[�^�]��
	XMFLOAT3* constMap2 = nullptr;
	result = constBuffCameraPosition->Map(0, nullptr, (void**)&constMap2);
	constMap2->x = CameraVec.m128_f32[0];
	constMap2->y = CameraVec.m128_f32[1];
	constMap2->z = CameraVec.m128_f32[2];
	constBuffCameraPosition->Unmap(0, nullptr);

	Rim* constMap3 = nullptr;
	result = constBuffRim->Map(0, nullptr, (void**)&constMap3);
	constMap3->color = rim.color;
	constMap3->pow = rim.pow;
	constMap3->Emission = rim.Emission;
	constBuffRim->Unmap(0, nullptr);

	//if (collider)
	//{
	//	collider->Update();
	//}

}

void ParticleObj::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptr�`�F�b�N
	assert(device);
	assert(cmdList);

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���f�����Z�b�g����Ă��Ȃ���Ε`����X�L�b�v
	if (model == nullptr) {
		return;
	}
	if (isRim == false)
	{
		// �萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

		//���f����`��
		model->Draw(cmdList, 1);
	}
	else
	{

		// �p�C�v���C���X�e�[�g�̐ݒ�
		cmdList->SetPipelineState(rimPipelinestate.Get());
		// ���[�g�V�O�l�`���̐ݒ�
		cmdList->SetGraphicsRootSignature(rimRootsignature.Get());
		// �萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
		// �萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(1, constBuffCameraPosition->GetGPUVirtualAddress());
		// �萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(2, constBuffRim->GetGPUVirtualAddress());

		//���f����`��
		model->RimDraw(cmdList);

		// �p�C�v���C���X�e�[�g�̐ݒ�
		cmdList->SetPipelineState(pipelinestate.Get());
		// ���[�g�V�O�l�`���̐ݒ�
		cmdList->SetGraphicsRootSignature(rootsignature.Get());
	}
}

void ParticleObj::SetCamera(Camera* camera)
{
	ParticleObj::camera_ = camera;
}

//void Object3d::SetCollider(BaseCollider* collider)
//{
//	collider->SetObject(this);
//	this->collider = collider;
//	CollisionManager::GetInstance()->AddCollider(this->collider);
//	this->collider->Update();
//}
