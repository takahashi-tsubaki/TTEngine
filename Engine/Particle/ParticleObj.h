#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Vector3.h"
#include "Matrix4.h"
#include "Camera.h"
#include "imguiManager.h"

#include "ParticleModel.h"
#include "ParticleWtf.h"

class ParticleObj
{
public:
	ParticleObj() = default;
	virtual ~ParticleObj();

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// �F (RGBA)
		Matrix4 mat;	// �R�c�ϊ��s��
	};


	//�}�e���A��
	struct Material
	{
		std::string name;	//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material()
		{
			ambient = { 0.3f , 0.3f , 0.3f };
			diffuse = { 0.0f , 0.0f , 0.0f };
			specular = { 0.0f , 0.0f , 0.0f };
			alpha = 1.0f;
		}
	};

	struct Rim
	{
		XMFLOAT4 color;
		float pow;
		float Emission;

		Rim()
		{
			pow = 3;
			color = { 1.0f , 1.0f , 1.0f , 1.0f };
			Emission = 1.0f;
		}
	};


private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = planeCount * 3;		// ���_��

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �ʏ�`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<ParticleObj> Create();






private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
#pragma region �ʏ탂�f���i���C�g�����j
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

#pragma endregion
#pragma region �������C�g
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rimRootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> rimPipelinestate;
#pragma endregion
	//// �r���[�s��
	static Matrix4 matView;
	//// �ˉe�s��
	static Matrix4 matProjection;
	//// ���_���W
	//static XMFLOAT3 eye;
	//// �����_���W
	//static XMFLOAT3 target;
	//// ������x�N�g��
	//static XMFLOAT3 up;

private:// �ÓI�����o�֐�
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>


	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

public: // �����o�֐�
	virtual bool Initialize(bool Isrim);
	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(ID3D12GraphicsCommandList* cmdList);

public:
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const { return worldTransform.translation_; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position) { this->worldTransform.translation_ = position; }

	/// <summary>
	/// �{���̎擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetScale() const { return worldTransform.scale_; }

	/// <summary>
	/// �{���̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetScale(const Vector3& scale) { this->worldTransform.scale_ = scale; }

	void SetRotate(const Vector3& rotate) { this->worldTransform.rotation_ = rotate; }

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const Matrix4& GetMatWorld() { return this->worldTransform.matWorld_; };

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetRimColor(const XMFLOAT4& rimColor) { this->rim.color = rimColor; }

	/// <summary>
	/// �J�����̃Z�b�^�[
	/// </summary>
	/// <param name="camera">�J����</param>
	static void SetCamera(Camera* camera);
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetRimPow(const float& rimPow) { this->rim.pow = rimPow; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetRimEmission(const float& rimEmission) { this->rim.Emission = rimEmission; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	//void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	//virtual void OnCollision(const CollisionInfo& info) {};

	//�A�N�Z�b�T
	void SetModel(ParticleModel* model) { this->model = model; }

	inline ParticleModel* GetModel() { return model; };

#pragma region ���ʂ̒萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
#pragma endregion
#pragma region �������C�g�̒萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffCameraPosition; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffRim; // �萔�o�b�t�@
#pragma endregion

public: // �����o�ϐ�
	//���[���h�s��
	ParticleWtf worldTransform;

	Rim rim;
	// �F
	XMFLOAT4 color = { 1 , 1 , 1 , 1 };
	//// ���[�J���X�P�[��
	//Vector3 scale = { 1,1,1 };
	//// X,Y,Z�����̃��[�J����]�p
	//Vector3 rotation = { 0,0,0 };
	//// ���[�J�����W
	//Vector3 position = { 0,0,0 };
	//// ���[�J�����[���h�ϊ��s��
	//Matrix4 matWorld;
	//// �e�I�u�W�F�N�g
	//Object3d* parent = nullptr;

	//���f��
	ParticleModel* model = nullptr;

	//�N���X��(�f�o�b�O�p)
	const char* name = nullptr;
	////�R���C�_�[
	//BaseCollider* collider = nullptr;

	bool isRim = false;

	//Camera
	static Camera* camera_;
};

