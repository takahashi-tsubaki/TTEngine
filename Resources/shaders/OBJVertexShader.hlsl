#include "OBJShaderHeader.hlsli"
////Lambart反射
//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//	//右、下、奥の方向を向いたライト
//	float3 lightdir = float3(1,-1,1);
//	lightdir = normalize(lightdir);
//	//ライトの色
//	float3 lightColor = float3(1, 1, 1);
//	// ピクセルシェーダーに渡す値
//	VSOutput output; 
//	output.svpos = mul(mat, pos);
//	//Lambart反射の計算
//	output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightColor;
//	output.color.a = m_alpha;
//	output.uv = uv;
//	return output;
//}

//Phong反射
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;	//ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;

}