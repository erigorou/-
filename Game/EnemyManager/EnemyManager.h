// ---------------------------------------------
// 「内容」全ての敵の管理を行うクラス
// 「日付」12/18
// 「作成者」池田
// ---------------------------------------------
#pragma once
#include "pch.h"

// 前方宣言
class Enemy;
class Goblin;
class PlayScene;
class IEnemy;

class EnemyManager
{
// ** 固定値 ** 
public:
	// 敵の種類
	enum class EnemyType
	{ Goblin,Boss };

	// 敵のデータ
	struct EnemyData
	{ EnemyType type; IEnemy* enemy; };


	static const wchar_t* GOBLIN_MODEL_PATH;
	static const wchar_t* BOSS_MODEL_PATH;

	// ゴブリンの最大沸き数
	static constexpr int MAX_GOBLIN = 3;


// ** 公開関数 **
public:
	// コンストラクタ
	EnemyManager();
	// デストラクタ
	~EnemyManager();
	// 初期化処理
	void Initialize(PlayScene* playScene);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& projection
	);

	void Finalize();

	void CreateGoblin(
		const DirectX::SimpleMath::Vector3 position
	);

	DirectX::Model* CreateModel(const wchar_t* filePath);

// ** 非公開関数 **
private:
	// 生存処理


// ** メンバ変数 **
private:
	std::unique_ptr<EnemyData> m_enemies;

	DirectX::Model* m_goblinModel;
	DirectX::Model* m_bossModel;
};