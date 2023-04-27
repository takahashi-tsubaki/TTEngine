﻿#include "FbxLoader.h"

#include <cassert>

using namespace DirectX;


const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
    //再初期化チェック
    assert(fbxManager == nullptr);

    //引数からメンバ変数に代入
    device_ = device;

    //FBXマネージャーの生成
    fbxManager = FbxManager::Create();

    //FBXマネージャーの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager,IOSROOT);
    fbxManager->SetIOSettings(ios);

    //FBXインポータの生成
    fbxImpoter = FbxImporter::Create(fbxManager, "");

}

void FbxLoader::Finalize()
{
    fbxManager->Destroy();
    fbxImpoter->Destroy();
}

void FbxLoader::LoadModelFromFile(const string& modelname)
{
    //モデルと同じ名前のフォルダから読み込む1
    const string directoryPath = baseDirectory + modelname + "/";

    //拡張子.fbxを付加
    const string filename = modelname + ".fbx";

    //連結してフルパスを得る
    const string fullpath = directoryPath + filename;

    //フォルダ名を指定してFBXファイルを読み込む
    if (!fbxImpoter->Initialize(fullpath.c_str(),-1,fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    //シーンの生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBXの情報をシーンにインポート
    fbxImpoter->Import(fbxScene);

    //Model生成
    FbxModel* fbxModel = new FbxModel();
    fbxModel->name = modelname;

    //FBXのノード数を取得
    int nodeCount = fbxScene->GetNodeCount();

    //あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
    fbxModel->nodes.reserve(nodeCount);

    //ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive(fbxModel,fbxScene->GetRootNode());

    //FBXシーン開放
    fbxScene->Destroy();

}

void FbxLoader::ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent)
{
    //ノード名を取得
    string name = fbxNode->GetName();

    //モデルにノードを追加
    fbxModel->nodes.emplace_back();
    Node& node = fbxModel->nodes.back();
    //ノード名を取得
    node.name = fbxNode->GetName();

    //FBXノードのローカル移動情報
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //形式変換して代入
    node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
    node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
    node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

    //回転角をDegreeからラジアンに変換
    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    //スケール、回転　平行移動行列の計算
    XMMATRIX matscaling, matrotation, matTranslation;
    matscaling = XMMatrixScalingFromVector(node.scaling);
    matrotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //ローカル変換行列の計算
    node.transform = XMMatrixIdentity();
    node.transform *= matscaling;
    node.transform *= matrotation;
    node.transform *= matTranslation;

    //グローバル変換行列計算
    node.worldTransform = node.transform;
    if (parent) {
        node.parent = parent;
        //親の変形を乗算
        node.worldTransform *= parent->worldTransform;
    }

    //FBXノードの情報を解析してノードに記録(Todo)

    //FBXノードのメッシュ情報を解析(Todo)

    //子ノードに対して再帰呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(fbxModel, fbxNode->GetChild(i),&node);
    }
}
