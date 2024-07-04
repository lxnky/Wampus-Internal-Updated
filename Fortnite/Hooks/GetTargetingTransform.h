#pragma once

//double* (*GetTargetingTransformOriginal)(APlayerPawn_Athena_C* AcknowledgedPawn, double*, char, uintptr_t);
//double* GetTargetingTransform(APlayerPawn_Athena_C* AcknowledgedPawn, double* Transform, char a3, uintptr_t a4)
//{
//	double* ReturnValue = reinterpret_cast<double* (__cdecl*)(APlayerPawn_Athena_C*, double*, char, uintptr_t, uintptr_t, void*)>(DoSpoofCall)(AcknowledgedPawn, Transform, a3, a4, SpoofCode, GetTargetingTransformOriginal);
//
//	if (Variables::GetTargetingTransform::bPickaxeRangeActive)
//	{
//		FVector CameraForward = UKismetMathLibrary::GetForwardVector(Variables::CameraRotation);
//
//		FVector MaxLocation = Variables::CameraLocation + (CameraForward * 975.0); //975.0 < Ghost hit
//
//		FHitResult OutHit;
//		TArray<UObject*> ActorsToIgnore;
//
//		if (UWorld* GWorld = UWorld::GetWorld())
//		{
//			if (UKismetSystemLibrary::LineTraceSingle(GWorld, Variables::CameraLocation, MaxLocation, ActorsToIgnore, &OutHit) && OutHit.ImpactPoint)
//			{
//				ReturnValue[4] = OutHit.ImpactPoint.X;
//				ReturnValue[5] = OutHit.ImpactPoint.Y;
//				ReturnValue[6] = OutHit.ImpactPoint.Z;
//			}
//		}
//	}
//
//	if (Variables::GetTargetingTransform::bShouldStartAtLocationTarget)
//	{
//		ReturnValue[4] = Variables::GetTargetingTransform::LocationTarget.X;
//		ReturnValue[5] = Variables::GetTargetingTransform::LocationTarget.Y;
//		ReturnValue[6] = Variables::GetTargetingTransform::LocationTarget.Z;
//
//		if (Settings::Exploits::MagicBullet)
//		{
//			FVector LocalPawnRelativeLocation = Variables::AcknowledgedPawn->K2_GetActorLocation();
//
//			auto y_tmp = atan2f(Variables::GetTargetingTransform::LocationTarget.Y - LocalPawnRelativeLocation.Y, Variables::GetTargetingTransform::LocationTarget.X - LocalPawnRelativeLocation.X);
//
//			LocalPawnRelativeLocation.X = LocalPawnRelativeLocation.X + cosf(y_tmp + 1.5708f) * 32.0f;
//			LocalPawnRelativeLocation.Y = LocalPawnRelativeLocation.Y + sinf(y_tmp + 1.5708f) * 32.0f;
//			LocalPawnRelativeLocation.Z = LocalPawnRelativeLocation.Z + 45.0f;
//
//			auto length = sqrtf(powf(Variables::GetTargetingTransform::LocationTarget.X - LocalPawnRelativeLocation.X, 2.0f) + powf(Variables::GetTargetingTransform::LocationTarget.Y - LocalPawnRelativeLocation.Y, 2.0f));
//			FVector NewPos;
//
//			NewPos.X = -atan2f(Variables::GetTargetingTransform::LocationTarget.Z - LocalPawnRelativeLocation.Z, length) / 2.0f;
//			NewPos.Y = atan2f(Variables::GetTargetingTransform::LocationTarget.Y - LocalPawnRelativeLocation.Y, Variables::GetTargetingTransform::LocationTarget.X - LocalPawnRelativeLocation.X) / 2.0f;
//
//			ReturnValue[0] = -(sinf(NewPos.X) * sinf(NewPos.Y));
//			ReturnValue[1] = sinf(NewPos.X) * cosf(NewPos.Y);
//			ReturnValue[2] = cosf(NewPos.X) * sinf(NewPos.Y);
//			ReturnValue[3] = cosf(NewPos.X) * cosf(NewPos.Y);
//		}
//	}
//
//	return ReturnValue;
//}

float* (*CalculateShotOriginal)(uintptr_t, uintptr_t, uintptr_t) = nullptr;
float* CalculateShot(uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
	float* ReturnValue = CalculateShotOriginal(arg0, arg1, arg2);

	if (Variables::GetTargetingTransform::bPickaxeRangeActive)
	{
		FVector CameraForward = UKismetMathLibrary::GetForwardVector(Variables::CameraRotation);

		FVector MaxLocation = Variables::CameraLocation + (CameraForward * 975.0);

		FHitResult OutHit;
		TArray<AActor*> ActorsToIgnore;

		if (UWorld* GWorld = UWorld::GetWorld())
		{
			if (UKismetSystemLibrary::LineTraceSingle(GWorld, Variables::CameraLocation, MaxLocation, ETraceTypeQuery::TraceTypeQuery5, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, false))
			{
				ReturnValue[4] = OutHit.ImpactPoint.X;
				ReturnValue[5] = OutHit.ImpactPoint.Y;
				ReturnValue[6] = OutHit.ImpactPoint.Z;
			}
		}
	}

	if (Variables::GetTargetingTransform::bShouldStartAtLocationTarget)
	{
		FVector ActorLocation = Variables::AcknowledgedPawn->K2_GetActorLocation();

		if (Settings::Aimbot::MagicBullet)
		{
			double YAxis = UKismetMathLibrary::Atan2(Variables::GetTargetingTransform::LocationTarget.Y - ActorLocation.Y, Variables::GetTargetingTransform::LocationTarget.X - ActorLocation.X);

			ActorLocation.X = ActorLocation.X + UKismetMathLibrary::cos(YAxis + 1.5708) * 32.0;
			ActorLocation.Y = ActorLocation.Y + UKismetMathLibrary::sin(YAxis + 1.5708) * 32.0;
			ActorLocation.Z = ActorLocation.Z + 45.0;

			double DeltaX = Variables::GetTargetingTransform::LocationTarget.X - ActorLocation.X;
			double DeltaY = Variables::GetTargetingTransform::LocationTarget.Y - ActorLocation.Y;
			double NewLength = UKismetMathLibrary::sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
			
			FVector Position;
			Position.X = -UKismetMathLibrary::Atan2(Variables::GetTargetingTransform::LocationTarget.Z - ActorLocation.Z, NewLength) / 2.0;
			Position.Y = UKismetMathLibrary::Atan2(Variables::GetTargetingTransform::LocationTarget.Y - ActorLocation.Y, Variables::GetTargetingTransform::LocationTarget.X - ActorLocation.X) / 2.0;

			ReturnValue[0] = -(UKismetMathLibrary::sin(Position.X) * UKismetMathLibrary::sin(Position.Y));
			ReturnValue[1] = UKismetMathLibrary::sin(Position.X) * UKismetMathLibrary::cos(Position.Y);
			ReturnValue[2] = UKismetMathLibrary::cos(Position.X) * UKismetMathLibrary::sin(Position.Y);
			ReturnValue[3] = UKismetMathLibrary::cos(Position.X) * UKismetMathLibrary::cos(Position.Y);
		}
		else
		{
			ReturnValue[4] = Variables::GetTargetingTransform::LocationTarget.X;
			ReturnValue[5] = Variables::GetTargetingTransform::LocationTarget.Y;
			ReturnValue[6] = Variables::GetTargetingTransform::LocationTarget.Z;
		}
	}

	return ReturnValue;
}