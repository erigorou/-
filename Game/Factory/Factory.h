#pragma once
#ifndef FACTORY
#define FACTORY

#include "pch.h"

class PlayScene;
class IObserver;

class CollisionManager;
class Sound;
class Camera;
class SkySphere;
class Particle;
class Floor;
class Wall;
class Player;
class Enemy;
class Sword;
class Cudgel;
class PlaySceneUIManager;
class Warning;

class Factory
{
public:
	static std::unique_ptr<CollisionManager>	CreateCollisionManager	();
	static std::unique_ptr<Camera>				CreateCamera			();
	static std::unique_ptr<SkySphere>			CreateSkySphere			(ID3D11Device1* device);
	static std::unique_ptr<Particle>			CreateParticle			();
	static std::unique_ptr<Floor>				CreateFloor				(ID3D11Device1* device);
	static std::unique_ptr<Wall>				CreateWall				(PlayScene* playScene);
	static std::unique_ptr<Player>				CreatePlayer			(PlayScene* playScene);
	static std::unique_ptr<Enemy>				CreateEnemy				(PlayScene* playScene);
	static std::unique_ptr<Sword>				CreateSword				(PlayScene* playScene);
	static std::unique_ptr<Cudgel>				CreateCudgel			(PlayScene* playScene);
	static std::unique_ptr<PlaySceneUIManager>	CreateUIManager			(PlayScene* playScene);
};

#endif // !FACTORY

