//--------------------------------------------------------------------------------------
// File: ArcBall.h
// Ken Shoemake, "Arcball Rotation Control", Graphics Gems IV, pg 176 - 192  
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ARC_BALL_DEFINED
#define ARC_BALL_DEFINED

// ArcBall�N���X
class ArcBall
{
public:
    // �R���X�g���N�^
    ArcBall()
        :
        m_width(1280.f),
        m_height(720.f),
        m_radius(1.0f),
        m_drag(false) 
    {
        Reset();  
    }

    // ���Z�b�g����
    void Reset() { m_qdown = m_qnow = DirectX::SimpleMath::Quaternion::Identity; }
    // �J�n���ɌĂяo�����
    void OnBegin( int x, int y )
    {
        m_drag = true;
        m_qdown = m_qnow;
        m_downPoint = ScreenToVector(float(x), float(y));
    }

    // �ړ����ɌĂяo�����
    void OnMove(int x, int y)
    {
        using namespace DirectX;
		  // �}�E�X���h���b�O���Ă���ꍇ
        if (m_drag)
        {
			  // �X�N���[�����W���x�N�g���ɕϊ�����
            XMVECTOR currentVector = ScreenToVector(float(x), float(y));
				// ���݂̃N�H�[�^�j�I�����v�Z����
            m_qnow = XMQuaternionMultiply(m_qdown, QuatFromBallPoints(m_downPoint, currentVector));
            // ���K������
            m_qnow.Normalize();
        }
    }

    // �I�����ɌĂяo�����
    void OnEnd() { m_drag = false; }
    // �E�B���h�E��ݒ肷��
    void SetWindow(const int& width, const int& height) { m_width = float(width); m_height = float(height); }
    // ���a��ݒ肷��
    void SetRadius(const float& radius) { m_radius = radius; }
    // �N�H�[�^�j�I�����擾����
    DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_qnow; }
    // �h���b�O���Ă��邩�ǂ����𒲂ׂ�
    bool IsDragging() const { return m_drag; }

private:
	// �X�N���[�����x�N�g���ɕϊ�����
    DirectX::XMVECTOR ScreenToVector(const float& screenx, const float& screeny)
    {
        float x = -( screenx - m_width / 2.f ) / ( m_radius * m_width / 2.f );
        float y = ( screeny - m_height / 2.f ) / ( m_radius * m_height / 2.f );

        float z = 0.0f;
        float magitude = x * x + y * y;

        if( magitude > 1.0f )
        {
            float scale = 1.0f / sqrtf( magitude );
            x *= scale;
            y *= scale;
        }
        else
            z = sqrtf( 1.0f - magitude );

        return DirectX::XMVectorSet( x, y, z, 0 );
    }

    static DirectX::XMVECTOR QuatFromBallPoints(const DirectX::FXMVECTOR& vFrom, const DirectX::FXMVECTOR& vTo )
    {
        using namespace DirectX;
        XMVECTOR dot = XMVector3Dot( vFrom, vTo );
        XMVECTOR vPart = XMVector3Cross( vFrom, vTo );
        return XMVectorSelect( dot, vPart, g_XMSelect1110 );
    }

private:
    float  m_width;
    float  m_height;
    float  m_radius;
    DirectX::SimpleMath::Quaternion m_qdown;
    DirectX::SimpleMath::Quaternion m_qnow;
    DirectX::SimpleMath::Vector3 m_downPoint;
    bool  m_drag;
};

#endif      // ARC_BALL_DEFINED
