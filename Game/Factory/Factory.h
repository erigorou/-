#pragma once
#ifndef FACTORY
#define FACTORY

#include "pch.h"

// ëOï˚êÈåæ
class PlayScene;
class IObserver;

class CollisionManager;
class Sound;
class Camera;
class SkySphere;
class Particle;
class Floor;
class Sea;
class Wall;
class Player;
class Boss;
class Goblin;
class Sword;
class Cudgel;
class Warning;


class PlaySceneUIManager;
class EnemyManager;
class QuestManager;

class Factory
{
public:
	static std::unique_ptr<Camera>				CreateCamera			();
	static std::unique_ptr<Particle>			CreateParticle			();

	static std::unique_ptr<SkySphere>			CreateSkySphere			();
	static std::unique_ptr<Floor>				CreateFloor				();
	static std::unique_ptr<Sea>					CreateSea				();
	static std::unique_ptr<Wall>				CreateWall				();

	static std::unique_ptr<Player>				CreatePlayer			(PlayScene* playScene);
	static std::unique_ptr<Boss>				CreateBoss				();
	static std::unique_ptr<Goblin>				CreateGoblin			(PlayScene* playScene);
	static std::unique_ptr<Sword>				CreateSword				(Player* player);
	static std::unique_ptr<Cudgel>				CreateCudgel			(Boss* boss);
	
	
	static std::unique_ptr<CollisionManager>	CreateCollisionManager();
	static std::unique_ptr<PlaySceneUIManager>	CreateUIManager			(PlayScene* playScene);
	static std::unique_ptr<EnemyManager>		CreateEnemyManager		(PlayScene* playScene);
	static std::unique_ptr<QuestManager>		CreateQuestManager		(PlayScene* playScene);
};

#endif // !FACTORY

