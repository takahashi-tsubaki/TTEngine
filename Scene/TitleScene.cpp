#include "TitleScene.h"
#include "levelLoader.h"
#include <cassert>
#include <sstream>
#include <iomanip>
TitleScene::TitleScene(SceneManager* controller, SceneObjects* sceneObj)
{
	controller_ = controller;
	sceneObj_ = sceneObj;

	targetPos.translation_ = { 0,0,50 };

}

TitleScene::~TitleScene()
{


	sceneObj_->Reset();
	//delete sceneObj_->skydomeO_;
	/*sceneObj_->Delete();*/
}

void TitleScene::Initialize()
{
	controller_->camera_->SetFollowerPos(&followPos);

	controller_->camera_->SetTargetPos(&targetPos);
	Sprite::LoadTexture(6, L"Resources/sprite/title.png");
	sprite_ = Sprite::Create(6, { 100,100 });

	levelEditer = LevelLoader::LoadFile("untitled");

	modelBume_ = Model::CreateFromOBJ("bume");

	models.insert(std::make_pair("bume", modelBume_));

	//// レベルデータからオブジェクトを生成、配置
	//for (auto& objectData : levelEditer->objects) {
	//	// ファイル名から登録済みモデルを検索
	//	Model* model = nullptr;
	//	decltype(models)::iterator it = models.find(objectData.filename);
	//	if (it != models.end())
	//	{
	//		model = it->second;
	//	}

	//	// モデルを指定して3Dオブジェクトを生成
	//	Object3d* newObject = Object3d::Create();
	//	newObject->SetModel(model);

	//	// 座標
	//	Vector3 pos;
	//	pos = objectData.translation;
	//	newObject->worldTransform.translation_= pos ;
	//	/*newObject->SetPosition(pos);*/

	//	// 回転角
	//	Vector3 rot;
	//	rot = objectData.rotation;
	//	newObject->worldTransform.rotation_ = rot;
	//	/*newObject->SetRotation(rot);*/

	//	// 座標
	//	Vector3 scale;
	//	scale = objectData.scaling;
	//	newObject->worldTransform.scale_=scale; 
	//	/*newObject->SetScale(scale);*/

	//	// 配列に登録
	//	objects.push_back(newObject);
	//}

}

void TitleScene::Update(Input* input, GamePad* gamePad)
{
	sceneObj_->skydomeO_->Update();
	controller_->camera_->Update();
	/*for (auto& object : objects) {
		object->Update();
	}*/
	gamePad->Update();
	if (input->TriggerKey(DIK_RETURN) || gamePad->ButtonTrigger(X))
	{
		controller_->ChangeSceneNum(S_PLAY);
	}
}

void TitleScene::Draw()
{
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());
	// 背景スプライト描画


	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

#pragma region 3Dオブジェクト描画
	//// 3Dオブジェクト描画前処理
	Object3d::PreDraw(controller_->dxCommon_->GetCommandList());

	//// 3Dオブジェクトの描画

	/*fbxObject->Draw(dxCommon_->GetCommandList());*/

	sceneObj_->skydomeO_->Draw();
	/*for (auto& object : objects) {
		object->Draw();
	}*/

	//skydomeO_->Draw();

	///// <summary>
	///// ここに3Dオブジェクトの描画処理を追加できる
	///// </summary>

	//// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion


	//#pragma region ぺらポリゴン描画
	//	postEffect->PreDrawScene(dxCommon_->GetCommandList());
	//
	//	//// ぺらポリゴンの描画
	//	postEffect->Draw(dxCommon_->GetCommandList());
	//	///// <summary>
	//	///// ここにぺらポリゴンの描画処理を追加できる
	//	///// </summary>
	//
	//	
	//
	//	postEffect->PostDrawScene(dxCommon_->GetCommandList());
	//
	//
	//
	//#pragma endregion
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(controller_->dxCommon_->GetCommandList());

	//sprite_->Draw();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
