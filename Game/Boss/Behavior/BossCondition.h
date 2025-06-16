// ----------------------------------------------------------------------------------
//
// 名前:	BossCondition.h
// 説明:    ビヘイビアツリーで条件ノードの実装を行うクラス
// 作成:	池田桜輔
// 日付:    2025/06/02
//
// ----------------------------------------------------------------------------------
#pragma once

// 前方宣言
class IObject;


class BossCondition final
{
	//------------------------
	// 固定値
	// -----------------------
public:
	// 攻撃範囲
	static constexpr float ATTACK_RANGE = 20.0f;

	//------------------------
	// アクセサ
	// -----------------------
public:
	// 攻撃範囲内かどうか
	bool IsInCloseRange();
	// 二分の一の確率で成功する
	bool IsHalfSuccess();
	// HPが半分以上か
	bool IsHpOverHalf();

	//------------------------
	// メンバ関数(公開)
	// -----------------------
public:
	// コンストラクタ
	BossCondition();
	// デストラクタ
	~BossCondition();

	// -----------------------
	// メンバ関数(非公開)
	// -----------------------
private:
	// プレイヤー等の必要な情報を取得する
	void GetNecessaryInfo();

	// -----------------------
	// メンバ変数
	// -----------------------
private:
	// プレイヤーオブジェクトのポインタ
	IObject* m_player;
	// ボスオブジェクトのポインタ
	IObject* m_boss;

	// プレイヤーの座標
	DirectX::SimpleMath::Vector3 m_playerPos;
	// ボスの座標
	DirectX::SimpleMath::Vector3 m_bossPos;
};