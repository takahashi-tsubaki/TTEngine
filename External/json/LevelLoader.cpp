#include "LevelLoader.h"
#include <cassert>
#include <fstream>

#include<json.hpp>

const std::string LevelLoader::directry = "Resources/levels/";

//ファイル拡張子
const std::string LevelLoader::kakutyousi = ".json";

LevelEditer* LevelLoader::LoadFile(const std::string& filename)
{
	//連結してフルパスを得る
	const std::string fullpath = directry + filename + kakutyousi;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name" を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelEditer* levelEditer = new LevelEditer();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別の取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelEditer->objects.emplace_back(LevelEditer::ObjectData{});
			//今追加した要素の参照を得る
			LevelEditer::ObjectData& objectData = levelEditer->objects.back();

			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.filename = object["file_name"];
				
			}

			//トランスフォームパラメータの読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation.x = -(float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][1];
			objectData.translation.z = (float)transform["translation"][2]; // -
			//objectData.translation.z = - objectData.translation.z;
			// 回転角
			objectData.rotation.x = LevelLoader::RadConvert((float)transform["rotation"][2]); // -
			objectData.rotation.x = -objectData.rotation.x;
			objectData.rotation.y = LevelLoader::RadConvert((float)transform["rotation"][1]); // -
			objectData.rotation.y = -objectData.rotation.y;
			objectData.rotation.z = LevelLoader::RadConvert((float)transform["rotation"][0]);
			// スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][1];
			objectData.scaling.z = (float)transform["scaling"][2];

			// TODO: コライダーのパラメータ読み込み
			// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
			if (object.contains("children")) {

			}
		}
	}


	return levelEditer;
}
float LevelLoader::RadConvert(float value) {
	float radValue;

	radValue = value * (3.141592f / 180);

	return radValue;
}