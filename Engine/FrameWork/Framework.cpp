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
	//‰Šú‰»
	Initialize();

	while (true)
	{
		//XV
		Update();
		if (IsEndRequest())
		{
			break;
		}
		//•`‰æ
		Draw();
	}
	//‰ğ•úˆ—
	Finalize();
}
