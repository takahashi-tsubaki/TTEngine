#include "FBXObject3d.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "Affin.h"

using namespace Microsoft::WRL;
using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* FbxObject3d::device_ = nullptr;
Camera* FbxObject3d::camera_ = nullptr;
ComPtr<ID3D12RootSignature> FbxObject3d::rootsignature;
ComPtr<ID3D12PipelineState> FbxObject3d::pipelinestate;

void FbxObject3d::CreateGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device_);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
	    L"Resources/shaders/FBXVS.hlsl", // シェーダファイル名
	    nullptr,
	    D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
	    "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	    0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
		    (char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
	    L"Resources/shaders/FBXPS.hlsl", // シェーダファイル名
	    nullptr,
	    D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
	    "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	    0, &psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(
		    (char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	    {// xy座標(1行で書いたほうが見やすい)
	     "POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0,                      D3D12_APPEND_ALIGNED_ELEMENT,
	     D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	                                                                                    0	                                                     },
	    {// 法線ベクトル(1行で書いたほうが見やすい)
	     "NORMAL",   0,  DXGI_FORMAT_R32G32B32_FLOAT, 0,                      D3D12_APPEND_ALIGNED_ELEMENT,
	     D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	                                                                                    0	                                                     },
	    {// uv座標(1行で書いたほうが見やすい)
	     "TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT,    0,                      D3D12_APPEND_ALIGNED_ELEMENT,
	     D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	                                                                                    0	                                                     },
	    {// 影響を受けるボーン番号
	     "BONEINDICES",                    0, DXGI_FORMAT_R32G32B32A32_UINT,        0, D3D12_APPEND_ALIGNED_ELEMENT,
	     D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,              0	                                                                                                                                               },
	    {// ボーンのスキンウェイト(4つ)
	     "BONEWEIGHTS",                   0,                      DXGI_FORMAT_R32G32B32A32_FLOAT,                            0,                               D3D12_APPEND_ALIGNED_ELEMENT,
	     D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;                            // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// CBV(スキニング用)
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
	    _countof(rootparams), rootparams, 1, &samplerDesc,
	    D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(
	    &rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device_->CreateRootSignature(
	    0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
	    IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(
	    &gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0);
	}
}

bool FbxObject3d::Initialize() {

	HRESULT result;
	CD3DX12_HEAP_PROPERTIES unt6 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC unt7 =
	    CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
	    &unt6, D3D12_HEAP_FLAG_NONE, &unt7, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	    IID_PPV_ARGS(&constBuffTransform));

	CD3DX12_HEAP_PROPERTIES unt8 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC unt9 =
	    CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	result = device_->CreateCommittedResource(
	    &unt8, D3D12_HEAP_FLAG_NONE, &unt9, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	    IID_PPV_ARGS(&constBuffSkin));

	// 1フレーム分の時間を60FPSで設定
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	return true;
}

void FbxObject3d::Update() {
	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = Affin::matScale(worldTransform.scale_);
	matRot = Affin::matRotation(worldTransform.rotation_);
	matTrans = Affin::matTrans(worldTransform.translation_);

	// ワールド行列の合成
	worldTransform.matWorld_ = Affin::matUnit(); // 変形をリセット
	worldTransform.matWorld_ *= matScale;         // ワールド行列にスケーリングを反映
	worldTransform.matWorld_ *= matRot;           // ワールド行列に回転を反映
	worldTransform.matWorld_ *= matTrans;          // ワールド行列に平行移動を反映

	// ビュープロジェクション行列
	const Matrix4& matViewProjection = camera_->GetViewProjectionMatrix();
	// モデルのメッシュトランスフォーム
	const Matrix4& modelTransform = fbxmodel_->GetModelTransform();
	// カメラ座標
	const Vector3& cameraPos = camera_->GetEye();

	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->viewproj = MyMath::ConvertMat4toXMMat(matViewProjection);
		constMap->world = MyMath::ConvertMat4toXMMat(modelTransform * worldTransform.matWorld_);
		constMap->cameraPos = {cameraPos.x, cameraPos.y, cameraPos.z};
		constBuffTransform->Unmap(0, nullptr);
	}

	std::vector<FbxModel::Bone>& bones = fbxmodel_->GetBones();

	// アニメーション
	if (isPlay) {
		// 1フレーム進める
		currentTime += frameTime;
		// 最後まで進めたら先頭に戻る
		if (currentTime > endTime) {
			
			if (animRot == false) {
				currentTime = endTime;
			}
			else
			{
				currentTime = startTime;
			}
		}
	}

	// 定数バッファへのデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	if (bones.size() != bonesMat.size()) {
		ResizeBonesMat(bones);
	}
	for (uint32_t i = 0; i < bones.size(); i++) {
		// 今の姿勢行列
		XMMATRIX matCurrentPose;
		// 今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose =
		    bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		// XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		// 合成してスキニング行列に
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
		// if (isBonesWorldMatCalc == true) {

		bonesMat[i] = MyMath::ConvertXMMATtoMat4(matCurrentPose) * worldTransform.matWorld_;

		//}
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList* cmdList) {
	// モデルの割り当てがなければ描画しない
	if (fbxmodel_ == nullptr) {
		return;
	}

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
	// モデル描画
	fbxmodel_->Draw(cmdList);
}

std::unique_ptr<FbxObject3d> FbxObject3d::Create() {
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<FbxObject3d> fbxObject3d = std::make_unique<FbxObject3d>();
	if (fbxObject3d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!fbxObject3d->Initialize()) {
		// delete object3d;
		assert(0);
		return nullptr;
	}
	// スケールをセット
	float scale_val = 1;
	fbxObject3d->worldTransform.scale_ = {scale_val, scale_val, scale_val};

	return fbxObject3d;
}

void FbxObject3d::SetFlame(uint32_t flame) {
	isChangeFlame = true;
	currentTime = frameTime * flame;
}

void FbxObject3d::AnimPlay() { isAnim = true; }

void FbxObject3d::AnimStop() { isAnim = false; }

void FbxObject3d::AnimIsRotateChange() {
	if (animRot == true) {
		animRot = false;
	} else if (animRot == false) {
		animRot = true;
	}
}

FbxTime FbxObject3d::GetCurrentTimer() { return currentTime; }

FbxTime FbxObject3d::GetEndTime() { return endTime; }

bool FbxObject3d::GetIsAnimRot() { return animRot; }

uint32_t FbxObject3d::ConvertFbxTimeToInt(FbxTime time) {
	FbxLongLong frameCount = time.GetFrameCount();
	return static_cast<uint32_t>(frameCount);
}

WorldTransform FbxObject3d::GetWorldTransform() { return worldTransform; }

WorldTransform* FbxObject3d::GetWorldTransformPtr() { return &worldTransform; }

void FbxObject3d::ResizeBonesMat(std::vector<FbxModel::Bone> bones_) {
	bonesMat.resize(bones_.size());
}

std::vector<Matrix4>* FbxObject3d::GetBonesMatPtr() { return &bonesMat; }

void FbxObject3d::SetIsBonesWorldMatCalc(bool isCalc) { isBonesWorldMatCalc = isCalc; }

void FbxObject3d::PlayAnimation(uint32_t animationNum,bool isAnimeRot) {

	animRot = true;
	

	FbxScene* fbxScene = fbxmodel_->GetFbxScene();
	// 0番のアニメーションを取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(animationNum);
	// 取得したアニメーションに変更
	fbxScene->SetCurrentAnimationStack(animstack);
	// アニメーションの名前取得
	const char* animstackname = animstack->GetName();
	// アニメーションの時間情報
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	// 開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	// 終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//// 開始時間に合わせる


	//アニメーションを繰り返すかどうか
	animRot = isAnimeRot;

	// 再生中にする
	isPlay = true;

}

void FbxObject3d::AnimFlameInter(FbxTime nowCount, FbxTime maxCount) {
	FbxTime pDiv = endTime * 1 / maxCount;
	currentTime = pDiv * nowCount;
}
