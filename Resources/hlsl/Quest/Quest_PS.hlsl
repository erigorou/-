#include "Quest.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);

SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // �w�肳�ꂽ�摜�̕\��
    float4 quest = tex.Sample(samLinear, input.Tex);
    float4 pattern = tex2.Sample(samLinear, input.Tex / 2);

    // �f�B�]���u�� 0 �̏ꍇ�͂��̂܂� quest ���o��
    float dissolveActive = step(0.01f, dissolve);
    
    // �����鋫�E���C�� (���ԂɊ�Â�)
    float boundary_min = dissolve; // �t�F�[�h�A�E�g�J�n���_
    float boundary_max = dissolve + (0.3f * dissolve * 2); // �t�F�[�h�A�E�g�I�����_

    // rgb�̕��ϒl���擾
    float average = (pattern.r + pattern.g + pattern.b) / 3.0f;

    // �t�F�[�h�A�E�g�͈͔���
    float inFadeOutRange = step(boundary_min, average) * (1.0 - step(boundary_max, average));

    // �O���f�[�V�����p�̕�Ԓl���v�Z
    float t = smoothstep(boundary_min, boundary_max, average);

    // �ʏ�̃O���f�[�V����
    float4 gradientColor = lerp(
        lerp(float4(0.0f, 0.0f, 0.0f, quest.a), float4(1.0f, 0.5f, 0.0f, quest.a), t),
        quest, t * 2);

    // �t�F�[�h�A�E�g�͈͂Ȃ�^�����ɂ���i���������̓����x���ێ��j
    float4 fadeToBlack = float4(0, 0, 0, quest.a);
    gradientColor = lerp(gradientColor, fadeToBlack, inFadeOutRange);
    
    // �f�B�]���u�K�p�idissolve �� 0 �̏ꍇ�͓K�p���Ȃ��j
    float dissolveFactor = lerp(1.0f, t, dissolveActive);
    
    // �F���Ȃ����A�N�G�X�g��\�����邩
    float4 gradColor = lerp(gradientColor, quest, dissolveFactor);
    
    // **�ŏI�I�ȓ����x��ݒ�**�i���� quest.a ���l���j
    gradColor.a = quest.a * dissolveFactor;

    // �����x��������
    gradColor *= alpha;
    
    return gradColor;
}
