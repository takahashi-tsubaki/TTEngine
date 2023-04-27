#include "FbxLoader.h"

#include <cassert>

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

}
