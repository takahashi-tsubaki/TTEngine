#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include "json.hpp"

struct LevelEditer
{
	struct ObjectData {
		// ファイル名
		std::string filename;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};
	//オブジェクト
	std::vector<ObjectData> objects;
};

class LevelLoader
{
public://メンバ関数

	//ディレクトリー
	static const std::string directry;

	//ファイル拡張子
	static const std::string kakutyousi;

public://メンバ変数

	static LevelEditer* LoadFile(const std::string& filename);

};

