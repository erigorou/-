
#pragma once

class Player
{
public:
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	static const float PLAYER_SPEED;

public:
	// プレイヤーの座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	// プレイヤーの回転角を取得する
	float GetAngle() { return m_angle; }

public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize(
		 ID3D11Device* device,
		const ID3D11DeviceContext* context,
		const DirectX::CommonStates* states
	);
	// 更新処理
	void Update(
		DirectX::SimpleMath::Vector3 enemyPos
	);

	// 描画処理
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection,
		const CommonResources* resources
	);

	// 終了処理
	void Finalize();
	// 回転角の計算
	void CaluclationAngle(DirectX::SimpleMath::Vector3 enemyPos);
	// Matrixの計算
	void CaluclationMatrix();
	// 移動の管理
	void MovePlayer();

private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 角度
	float m_angle;

	// プレイヤー用のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
};