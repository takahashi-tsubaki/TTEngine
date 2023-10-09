#pragma once

#include "Defineder.h"
#include "Pading.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include <vector>
#include <string>
ALICE_SUPPRESS_WARNINGS_END
#include "Vector3.h"

struct LevelEditer
{
	struct ObjectData
	{
// ファイル名
		std::string filename;
		// 平行移動
		Vector3 translation;
		// 回転角
		Vector3 rotation;
		// スケーリング
		Vector3 scaling;
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
	// 度数法 -> ラジアン
	static float RadConvert(float value);
};

