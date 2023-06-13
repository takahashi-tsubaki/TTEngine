#pragma once
#include "WinApp.h"

class Framework
{
public:

	//������
	virtual void Initialize();

	//�I��
	virtual void Finalize();

	//���t���[���X�V
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	virtual void SetRequest(bool request) { endRequest_ = request; }

	//�I���`�F�b�N
	virtual bool IsEndRequest() { return endRequest_; }

	//�f�X�g���N�^
	virtual ~Framework() = default;

	//���s
	void Run();

private:

	bool endRequest_ = false;
};