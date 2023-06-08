#pragma once
#include "WinApp.h"

class Framework
{
public:

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	virtual void SetRequest(bool request) { endRequest_ = request; }

	//終了チェック
	virtual bool IsEndRequest() { return endRequest_; }

	//デストラクタ
	virtual ~Framework() = default;

	//実行
	void Run();

private:

	bool endRequest_ = false;
};