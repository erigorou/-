#pragma once


// -------------------------------------------------------
// イベントリスト
// -------------------------------------------------------
enum class EventList
{
	// プレイヤーに攻撃ができるか
	PlayerCanHit,
	// ボスに攻撃ができるか
	BossCanHit,
	// 刀のステートを変更する
	ChangeSwordState,
	// 金棒のステートを変更する
	ChangeCudgelState,
	// ボスのステートを変更する
	ChangeBossState,
	// ボスの顔を変更する
	ChangeBossFace,
	// ゴブリンを全て倒す
	DeleteAllGoblin,
	// OBB衝突判定を登録する
	AddOBBCollision,
	// 球衝突判定を登録する
	AddSphereCollision,
	// 衝突判定の登録を解除する
	DeleteCollision,
	// 移動パーティクルの作成
	CreateTrailDust,
	// 土煙パーティクルの作成
	CreateBashDust,
	// 武器の軌跡パーティクルの作成
	CreateWeaponTrail,
	// カメラを揺らす
	ShakeCamera,
};



// -------------------------------------------------------
// ゲッターリスト
// -------------------------------------------------------
enum class GetterList
{
	// プレイヤーのポインタを取得する
	GetPlayer,
	// ボスのポインタを取得する
	GetBoss,
};