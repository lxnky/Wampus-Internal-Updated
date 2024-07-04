#pragma once

namespace Settings
{
	namespace Aimbot
	{
		bool Triggerbot = false;
		bool Interpolate = true;
		bool Constant = false;
		double Speed = 10.0;
		bool MagicBullet = false;
		bool PredictProjectiles = false;
		double FOV = 50.0;
		bool TargetLine = false;
		bool DrawFOV = true;
		double MaxDistance = 100.0;
		int AimType = 2;
		int BoneType = 1;
		int KeyType = 0;
		bool VisableOnly = true;
		bool Backtrack = false;
		bool SkipKnocked = true;

		namespace StickySilent
		{
			bool Spinbot = false;
			bool StickyCameraSpin = false;
			bool SwitchPitch = false;
			double SpinSpeed = 5.0;
		}

		namespace TriggerBot
		{
			bool OnlyOnAimKey = true;
			bool FireOnlyOnAimIn = false;
			bool NonProjectileWeapons = false;
			double FireRate = 5.0;
		}
	}

	namespace Player
	{
		int BoxType = 1;
		bool Skeleton = false;
		bool Lines = false;
		bool Name = false;
		bool Weapon = false;
		bool Distance = false;
		bool ViewAngle = false;
		bool Wireframe = false;
		bool Minimap = false;
		bool Target = false;

		// Player 2
		bool ShowBots = false;
		bool ShowTeam = false;

		double MaxDistance = 350.0;
	}

	namespace Environment
	{
		bool Weakspot = false;
		bool Projectile = false;
		bool Pickup = false;
		bool Container = false;
		bool Trap = false;
		bool Vehicle = false;
		bool SupplyDrop = false;
		int WeakspotAim = 0;
	}

	namespace Misc
	{
		int CrosshairMode = 0;
		bool BulletTraces = false;
	}

	namespace Exploits
	{
		bool Hook = false;
		bool PickaxeRange = false;
		bool BulletNoClip = false;
		bool InstantReload = false;
		bool NoSpread = false;
		bool NoRecoil = false;
		bool UnlockFireRate = false;
		bool FastProjectileShoot = false;
		bool ProjectileTeleport = false;
		int TeleportMethod = 0;
		bool PerfectBHop = false;
		bool InstaDMAClip = false;
		bool InstaOpen = false;
		bool CameraFOVChanger = false;
		double CameraFOV = 100.0;
		bool VehicleFlight = false;
		double VehicleFlightSpeed = 25.0;
		bool CheatFly = false;
	}

	namespace Colors
	{
		FLinearColor TargetLine = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor FieldOfView = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor BulletColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor TeammateColor = FLinearColor(0.2f, 0.8f, 0.2f, 1.f);
		FLinearColor BoxVisible = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor BoxInVisible = FLinearColor(0.f, 1.f, 1.f, 1.f);
		FLinearColor SkeletonVisible = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor SkeletonInVisible = FLinearColor(0.f, 1.f, 1.f, 1.f);
		FLinearColor SnaplineVisible = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor SnaplineInVisible = FLinearColor(0.f, 1.f, 1.f, 1.f);
		FLinearColor SkinColor = FLinearColor(1.f, 1.f, 0.f, 1.f);
	}
}