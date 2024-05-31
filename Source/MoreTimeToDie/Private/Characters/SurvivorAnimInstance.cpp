#include "Characters/SurvivorAnimInstance.h"

#include "Characters/Survivor.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/KismetMathLibrary.h" 
#include "KismetAnimationLibrary.h" 

void USurvivorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Survivor = Cast<ASurvivor>(TryGetPawnOwner());
	if (Survivor)
	{
		SurvivorMovement = Survivor->GetCharacterMovement();
		if(!SurvivorMovement) { UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeInitializeAnimation - SurvivorMovement is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeInitializeAnimation - Survivor is null.")); }
}

void USurvivorAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SurvivorMovement)
	{
		SurvivorDirection = UKismetAnimationLibrary::CalculateDirection(SurvivorMovement->Velocity, Survivor->GetActorRotation());

		Speed = UKismetMathLibrary::VSizeXY(SurvivorMovement->Velocity);

		if (Survivor)
		{
			if (Speed == 0)
			{
				if (Survivor->GetSurvivorMoveState() != ESurvivorMoveState::ESMS_NONE) { Survivor->SetSurvivorMoveState(ESurvivorMoveState::ESMS_NONE); }
				if (!Survivor->GetCapsuleComponent()->CanEverAffectNavigation()) { Survivor->GetCapsuleComponent()->SetCanEverAffectNavigation(true); }
			}
			else
			{
				if (Survivor->GetSurvivorMoveState() != ESurvivorMoveState::ESMS_Walking) { Survivor->SetSurvivorMoveState(ESurvivorMoveState::ESMS_Walking); }
				if (Survivor->GetCapsuleComponent()->CanEverAffectNavigation()) { Survivor->GetCapsuleComponent()->SetCanEverAffectNavigation(false); }
			}
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeInitializeAnimation - SurvivorMovement is null.")); }
}

