#pragma once

// -------------------------------------------------------
// イベントリスト
// -------------------------------------------------------
enum class EventList
{
	// ボスがプレイヤーに攻撃しているか
	PlayerCanDamageBoss,
	// 金棒がプレイヤーに攻撃しているか
	PlayerCanDamageCudgel,
	// ゴブリンがプレイヤーに攻撃しているか
	PlayerCanDamageGoblin,
	// 敵に攻撃ができるか
	EnemyCanHit,
	// 刀のステートを変更する
	ChangeSwordState,
	// 金棒のステートを変更する
	ChangeCudgelState,
	// ボスのステートを変更する
	ChangeBossState,
	// ボスの顔を変更する
	ChangeBossFace,
	// 敵を１体削除
	DeleteEnemy,
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
	// カメラのステートを変更する
	ChangeCamera,
	// プレイシーン終了
	EndPlayScene
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
	// 刀のポインタを取得する
	GetSword,
	// 金棒のポインタを取得する
	GetCudgel,
};