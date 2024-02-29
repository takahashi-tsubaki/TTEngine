#pragma once
#include "Character.h"
#include "EnemyActionManager.h"

class PlayerCharacter;
class EnemyCharacter : public Character
{
public:
	// 初期化
	void Initialize(TTEngine::DirectXCommon* dxCommon, Vector3 position, PlayerCharacter* player);

	// 更新
	void Update();

	// 描画
	void Draw() override;

		// Fbxオブジェクトの取得
	FbxObject3d* GetFbxObject3d() { return fbxObject_.get(); }

private:
	std::unique_ptr<EnemyActionManager> ActManager_;
	PlayerCharacter* player_ = nullptr;
	TTEngine::DirectXCommon* dxCommon_ = nullptr;


	// プレイヤーのFBXモデル
	std::unique_ptr<FbxObject3d> fbxObject_;
	std::unique_ptr<FbxModel> fbxModel_;
	// fbxの大きさ
	float fbxScale_;

	size_t SPHERE_COLISSION_NUM;             // コライダー（スフィア）の数
	std::vector<Matrix4>* collisionBonesMat; // 当たり判定用のボーンのワールド行列
	std::vector<SphereCollider*> sphere;
	std::vector<Vector3> spherePos = {};
};
