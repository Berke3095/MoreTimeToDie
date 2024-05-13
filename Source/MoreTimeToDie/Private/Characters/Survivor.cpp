#include "Characters/Survivor.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"

ASurvivor::ASurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetCapsuleComponent();
	SetSkeletalMeshComponent();
	SetAvoidanceSphere();
}

void ASurvivor::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
	COMPONENTS
*/
void ASurvivor::SetCapsuleComponent()
{
	CapsuleComponent = GetCapsuleComponent();
	if (CapsuleComponent)
	{
		CapsuleComponent->SetGenerateOverlapEvents(false);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block); // Block mouse
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetCapsuleComponent - CapsuleComponent is null.")); }
}

void ASurvivor::SetSkeletalMeshComponent()
{
	MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetGenerateOverlapEvents(false);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetSkeletalMeshComponent - MeshComponent is null.")); }
}

void ASurvivor::SetCharacterMovement()
{
	CharMovement = GetCharacterMovement();
	if (CharMovement)
	{
		CharMovement->MaxWalkSpeed = 600.0f;
		bUseControllerRotationYaw = false;
		CharMovement->bOrientRotationToMovement = true;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetCharacterMovement - CharMovement is null")); }
}

void ASurvivor::SetAvoidanceSphere()
{
	AvoidanceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AvoidanceSphere"));
	if (AvoidanceSphere)
	{
		AvoidanceSphere->SetupAttachment(RootComponent);
		AvoidanceSphere->bUseAttachParentBound = true;
		AvoidanceSphere->SetGenerateOverlapEvents(false);
		AvoidanceSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AvoidanceSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // AvoidanceSphere
		AvoidanceSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AvoidanceSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Pathtrace
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetAvoidanceSphere - AvoidanceSphere is null")); }
}

