#include "Characters/SurvivorAnimInstance.h"

#include "Characters/Survivor.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/KismetMathLibrary.h" 
#include "KismetAnimationLibrary.h" 

#include "NavigationSystem.h"

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

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeUpdateAnimation - Navigation system is null.")); }

		Speed = UKismetMathLibrary::VSizeXY(SurvivorMovement->Velocity);
		if (Survivor)
		{
			if (Speed == 0)
			{
				if (Survivor->GetSurvivorMoveState() != ESurvivorMoveState::ESMS_NONE)
				{
					Survivor->SetSurvivorMoveState(ESurvivorMoveState::ESMS_NONE);
				}
				
				if (!Survivor->GetCapsuleComponent()->CanEverAffectNavigation()) { Survivor->GetCapsuleComponent()->SetCanEverAffectNavigation(true); }
			}
			else if(Speed > 0)
			{
				if (Survivor->GetCapsuleComponent()->CanEverAffectNavigation()) { Survivor->GetCapsuleComponent()->SetCanEverAffectNavigation(false); }
			}
		}
		else { UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeUpdateAnimation - Survivor is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeInitializeAnimation - SurvivorMovement is null.")); }
}

