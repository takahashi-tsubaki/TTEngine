#include "ParticleShader.hlsli"
////Lambart����
//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//	//�E�A���A���̕��������������C�g
//	float3 lightdir = float3(1,-1,1);
//	lightdir = normalize(lightdir);
//	//���C�g�̐F
//	float3 lightColor = float3(1, 1, 1);
//	// �s�N�Z���V�F�[�_�[�ɓn���l
//	VSOutput output; 
//	output.svpos = mul(mat, pos);
//	//Lambart���˂̌v�Z
//	output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightColor;
//	output.color.a = m_alpha;
//	output.uv = uv;
//	return output;
//}

//Phong����
VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);


	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);
	//Lambart���˂̌v�Z
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;

}