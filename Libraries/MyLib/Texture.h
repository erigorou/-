#pragma once


namespace mylib
{
	class Texture
	{
	public:

		// ------------------------------------------------
		// �e�N�X�`�������[�h����
		// ------------------------------------------------
		static void LoadTexture(
			ID3D11Device* device										,	// �f�o�C�X
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	,	// �e�N�X�`��
			const wchar_t* fileName											// �t�@�C����
		);

		// ------------------------------------------------
		// �摜�̑傫�������߁A�e�N�X�`���̒��S���W���v�Z����
		// ------------------------------------------------
		static void CalculateTextureCenter(
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture	,	// �e�N�X�`��
			DirectX::SimpleMath::Vector2& texSize						,	// �e�N�X�`���̑傫��
			DirectX::SimpleMath::Vector2& texCenter							// �e�N�X�`���̒��S���W
		);
	};
};