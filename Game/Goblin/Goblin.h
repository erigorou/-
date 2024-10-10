// -------------------------------------------------------
// 小鬼の親クラス
// -------------------------------------------------------


#pragma once

#ifndef GOBLIN_OBJECT
#define GOBLIN_OBJECT

#include "pch.h"
#include "Interface/IObject.h"

class Player;
class PlayScene;

class Goblin : public IObject
{
public:
	static const float GOBLIN_SPEED;
	static const float GOBLIN_SCALE;

	DirectX::SimpleMath::Vector3	GetPosition()	override{ return m_position;	}	// 鬼の座標を取得する
	DirectX::SimpleMath::Vector3	GetVelocity()	const	{ return m_velocity;	}	// 速度の取得
	DirectX::SimpleMath::Vector3	GetAngle()		const	{ return m_angle;		}	// 回転角の取得
	DirectX::SimpleMath::Matrix		GetWorldMatrix()const	{ return m_worldMatrix; }	// ワールド座標の取得

	// ステートパターン


	PlayScene* GetPlayScen() const { return m_playScene; }	// PlaySceneの取得

	Goblin(PlayScene* playScene);
	~Goblin();

	void Initialize();									// 初期化関数
	void Update(const float elapsedTime);				// 更新処理
	
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection);	// 描画関数
	
	void Finalize();									// 終了関数
	void HitAction(InterSectData data) override;		// 当たったときの処理

private:

	void CreateState();		// ステートの作成
	void CreateCollision();	// 当たり判定の生成
	
	DirectX::SimpleMath::Vector3 m_position;	// 座標
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	DirectX::SimpleMath::Vector3 m_angle;		// 回転角
	DirectX::SimpleMath::Matrix m_worldMatrix;	// ワールド座標

	std::unique_ptr<DirectX::Model> m_model;	// モデル


	// ステートパターン



	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;	// エフェクト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// プレイシーン(当たり判定の処理に使用)
	PlayScene* m_playScene;

	// 体の当たり判定
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;
};


#endif // !GOBLIN_OBJECT