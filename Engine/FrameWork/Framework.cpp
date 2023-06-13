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
	//初期化
	Initialize();

	while (true)
	{
		//更新
		Update();
		if (IsEndRequest())
		{
			break;
		}
		//描画
		Draw();
	}
	//解放処理
	Finalize();
}
