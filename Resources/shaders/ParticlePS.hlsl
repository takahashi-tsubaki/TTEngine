#include "ParticleShader.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//float4 main(VSOutput input) : SV_TARGET
//{
//	float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
//	float light_diffuse = saturate(dot(-light, input.normal));
//	float3 shade_color;
//	shade_color = m_ambient; // �A���r�G���g��
//	shade_color += m_diffuse * light_diffuse;	// �f�B�t���[�Y��
//	float4 texcolor = tex.Sample(smp, input.uv);
//	return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	//�V�F�[�f�B���O�̂��F
	float4 shadecolor;
	//����x
	const float shininess = 4.0f;
	//���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	float3 dotlightnormal = dot(lightv,input.normal);
	//���ˌ��x�N�g��
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//�����ˌ�
	float3 ambient = m_ambient;
	//�g�U���ˌ�
	float3 diffuse = dotlightnormal * m_diffuse;
	//���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect,eyedir)),shininess) * m_specular;
	//�S�ĉ��Z����
	shadecolor.rgb = (ambient + diffuse + specular) * lightColor;
	shadecolor.a = m_alpha;
	return shadecolor * texcolor * color;
}