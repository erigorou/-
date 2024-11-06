#pragma once
#include "Interface/IState.h"
#include "Game/Scene/PlayScene.h"

// オブジェクトの基底クラス　============================================
#include "Interface/IObject.h"
#include "Interface/IObserver.h"

// プレイヤーに付与されるもの ===========================================
#include "Game/Weapon/Sword/Sword.h"		// 武器
#include "Game/Player/PlayerHP.h"			// プレイヤーのHP

// プレイヤーの状態 =====================================================
#include "Game/Player/State/Header/Player_Idling.h"			// 待機状態
#include "Game/Player/State/Header/Player_Dodging.h"		// 回避状態
#include "Game/Player/State/Header/Player_Attacking_1.h"	// 攻撃状態１
#include "Game/Player/State/Header/Player_Attacking_2.h"	// 攻撃状態２
#include "Game/Player/State/Header/Player_Attacking_3.h"	// 攻撃状態３
#include "Game/Player/State/Header/Player_Attacking_4.h"	// 攻撃状態４

class Wall;


class Player :  public IObserver
{

// 固定値
public:
	// 初期座標
	static const DirectX::SimpleMath::Vector3 HOME_POSITION;
	// 速さ
	static const float PLAYER_SPEED;
	// 大きさ
	static const float PLAYER_SCALE;

	static constexpr float APPLIED_ATTACK_TIME	= 1.4f;	// ２段目以降の攻撃が許されるようになる時間
	static constexpr float APPLIED_DODGE_TIME	= 0.7f;	// 回避ができるようになる時間
	static constexpr float COOL_TIME			= 1.0f; // 攻撃のクールタイム
	static constexpr float X_COOL_TIME			= 0.8f; // 次の攻撃ができるようになる時間

	static constexpr float NORMAL_ATTACK_TIME	= 0.5f; // 通常攻撃のアニメーション時間

// アクセサ
public:
	// /////////////////プレイヤーの基礎情報を渡す関数//////////////////////////////////////////////////
	DirectX::SimpleMath::Vector3	GetPosition		()override	{ return m_position;			}
	DirectX::SimpleMath::Vector3	GetVelocity		()	const	{ return m_velocity;			}
	DirectX::SimpleMath::Vector3	GetDirection	()	const	{ return m_direction;			}
	float							GetAngle		()	const	{ return m_angle;				}
	PlayerHP*						GetPlayerHP		()	const	{ return m_hp.get();			}
	DirectX::BoundingSphere*		GetBodyCollision()			{ return m_bodyCollision.get(); }


	////////////////////プレイヤー基本情報を設定する関数/////////////////////////////////////////////////
	void SetPosition(DirectX::SimpleMath::Vector3 position)			{ m_position = position; }


	////////////////////プレイヤーのステートを渡す関数//////////////////////////////////////////////////
	PlayerIdling*		GetPlayerIdlingState	()	const { return m_playerIdling.		get(); }
	PlayerDodging*		GetPlayerDodgingState	()	const { return m_playerDodging.		get(); }
	PlayerAttacking_1*	GetPlayerAttackingState1()	const { return m_playerAttacking_1.	get(); }
	PlayerAttacking_2*	GetPlayerAttackingState2()	const { return m_playerAttacking_2.	get(); }
	PlayerAttacking_3*	GetPlayerAttackingState3()	const { return m_playerAttacking_3.	get(); }
	PlayerAttacking_4*	GetPlayerAttackingState4()	const { return m_playerAttacking_4.	get(); }

	////////////プレイヤーの移動に関するステートを設定する関数///////////////////////////////////////////
	void SetSpeed		(DirectX::SimpleMath::Vector3 velocity)		{ m_velocity = velocity;			}
	void SetAcceleration(DirectX::SimpleMath::Vector3 acceleration)	{ m_acceleration = acceleration;	}
	void SetAngle		(const float angle)							{ m_angle = angle;					}

	////////////////////プレイシーンに干渉するのに使用すr関数/////////////////////////////////////////////
	PlayScene* GetPlayScene()const { return m_playScene; }

	//////////////////////////////////衝突判定に使用//////////////////////////////////////////////////////
	void CanHit(bool flag) { m_canHit = flag; }

	//////////////////////////////////アニメーションに使用////////////////////////////////////////////////
	void SetAnimationRotate(DirectX::SimpleMath::Vector3 rotate) { m_animationRotate = rotate; }


// 公開関数
public:
	// コンストラクタ
	Player(PlayScene* playScene);
	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();
	// 当たり判定の生成関数
	void CreateCollision();
	// ステートの作成関数
	void CreateState();
	// 新しい状態に遷移する
	void ChangeState(IPlayer* newState);
	// 時間計測を行う
	void TimeComparison(float& nowTime, const float totalTime, IPlayer* newState, const float elapsedTime);
	// 更新処理
	void Update(
		const DirectX::SimpleMath::Vector3	enemyPos,
		const float							elapsedTime);

	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix&	view,
		const DirectX::SimpleMath::Matrix&	projection);

	// 終了処理
	void Finalize();
	// 回転角の計算
	void CalculationAngle(DirectX::SimpleMath::Vector3 const enemyPos);
	// Matrixの計算
	void CalculationMatrix();
	// 移動の管理
	void MovePlayer();

	// 当たったときの処理
	void HitAction(InterSectData data) override;

	// キーボードの入力を取得する
	void OnKeyPressed(const DirectX::Keyboard::Keys& key) override;

	// キーボードの入力を取得する
	void OnKeyDown(const DirectX::Keyboard::Keys& key) override;


// 内部変数
private:
	DirectX::SimpleMath::Vector3	m_position;			// 位置
	DirectX::SimpleMath::Vector3	m_velocity;			// 速度
	DirectX::SimpleMath::Vector3	m_inputVelocity;	// 入力保持用変数
	DirectX::SimpleMath::Vector3	m_direction;		// 向き
	DirectX::SimpleMath::Vector3	m_acceleration;		// 加速度
	float							m_angle;			// 回転	
	DirectX::SimpleMath::Vector3	m_pushBackValue;	// プッシュバック値


	// プレイヤー用のワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;

	// ステート関連 =================================================
	IPlayer* m_currentState;									// 現在のステート
	std::unique_ptr<PlayerIdling> m_playerIdling;				// 待機状態
	std::unique_ptr<PlayerDodging> m_playerDodging;				// 回避状態

	std::unique_ptr<PlayerAttacking_1> m_playerAttacking_1;		// 攻撃状態１
	std::unique_ptr<PlayerAttacking_2> m_playerAttacking_2;		// 攻撃状態２
	std::unique_ptr<PlayerAttacking_3> m_playerAttacking_3;		// 攻撃状態３
	std::unique_ptr<PlayerAttacking_4> m_playerAttacking_4;		// 攻撃状態４


	// プレイヤーに付与されるもの ============
	std::unique_ptr<Sword> m_sword;
	std::unique_ptr<PlayerHP> m_hp;

	// シェーダーに使用するもの ==============
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_particleTime;
	float m_elapsedTime;

// 内部関数
private:
	// プレイシーン(他のオブジェクトの情報の取得などに使う)
	PlayScene* m_playScene;

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// 体の当たり判定 ////////
	std::unique_ptr<DirectX::BoundingSphere> m_bodyCollision;	// 衝突判定
	bool m_isHit;		// 衝突したか
	float m_coolTime;	// クールタイム
	bool m_canHit;		// 衝突可能か

	// アニメーション用変数
	DirectX::SimpleMath::Vector3 m_animationRotate;
};
