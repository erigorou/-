// ---------------------------------------------
// 「内容」全ての敵の管理を行うクラス
// 「日付」12/18
// 「作成者」池田
// ---------------------------------------------
#pragma once
#include "pch.h"
#include "Interface/IEnemy.h"

// 前方宣言
class Enemy;
class Goblin;
class PlayScene;
class IObject;
class PlayScene;



// ---------------------------
// 敵の管理クラス
// ---------------------------
class EnemyManager
{
// ** 構造体 ** 
public:
	// 敵の種類
	enum class EnemyType
	{ Goblin,Boss };

	// 敵のデータ
	struct EnemyData
	{
		EnemyType type;
		std::unique_ptr<IEnemy> data;
	};


// ---------------------------
// 固定値
// ---------------------------
public:
	// モデルのパス
	static const wchar_t* GOBLIN_MODEL_PATH;
	static const wchar_t* BOSS_MODEL_PATH;

	// ゴブリンの最大沸き数
	static constexpr int MAX_GOBLIN = 3;


// ---------------------------
// アクセサ
// ---------------------------
public:
	// ボスのポインタを取得
	Enemy* GetBossEnemy();
	// ボスの座標を取得
	DirectX::SimpleMath::Vector3 GetBossPosition();
	// ターゲットとしている敵の座標を取得
	DirectX::SimpleMath::Vector3 GetPicupEnemyPosition();


// ---------------------------
// 公開関数
// ---------------------------
public:
	// コンストラクタ
	EnemyManager(PlayScene* playScene);
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
	// 終了処理
	void Finalize();
	// 敵の生成処理
	void GenerateEnemy(const DirectX::SimpleMath::Vector3& position, EnemyType type);

	// ゴブリンの全削除
	void DeleteAllGoblin();

	// 全てのゴブリンのHPを0にする
	void AllGoblinHPZero();

	// 敵１体の削除
	void DeleteEnemy(IEnemy* enemy);

	// カメラのターゲットの設定
	void ChangeCameraTarget();

	// 敵が生きているかのフラグ
	bool IsEnemysAlive();


// ---------------------------
// 内部関数
// ---------------------------
private:

	// 敵生成関連 *****
	// クエストの選択
	void GenerateStartEnemy();
	// クエスト１の敵生成
	void GenerateEnemy0();
	// クエスト２の敵生成
	void GenerateEnemy1();
	// クエスト３の敵生成
	void GenerateEnemy2();


	// ゴブリンの生成処理
	void GenerateGoblin(const DirectX::SimpleMath::Vector3& position);
	// ボスの生成処理
	void GenerateBoss(const DirectX::SimpleMath::Vector3& position);

	// モデルの生成処理
	DirectX::Model* CreateModel(const wchar_t* filePath);


// ---------------------------
// メンバ変数
// ---------------------------
private:
	// 敵の配列
	std::vector<EnemyData> m_enemies;

	// プレイシーン
	PlayScene* m_playScene;

	// 現在ターゲットにしている敵の配列番号
	int m_targetEnemyIndex;

	DirectX::Model* m_goblinModel;
	DirectX::Model* m_bossModel;

	// インデックス数
	int m_selectQuestIndex;
};