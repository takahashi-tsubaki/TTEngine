#include "Framework.h"

void Framework::Initialize()
{
}

void Framework::Finalize()
{
	
}

void Framework::Update()
{

}

void Framework::Run()
{
	//������
	Initialize();

	while (true)
	{
		//�X�V
		Update();
		if (IsEndRequest())
		{
			break;
		}
		//�`��
		Draw();
	}
	//�������
	Finalize();
}
