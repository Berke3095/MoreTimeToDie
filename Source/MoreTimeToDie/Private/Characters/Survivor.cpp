#include "Characters/Survivor.h"

#include "MyGameManager.h" 

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Button.h"
#include "Components/Image.h" 
#include "MyAIController.h"

#include "Widgets/PortraitWidget.h"
#include "MyHUD.h"

ASurvivor::ASurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Selectable");

	SetCapsuleComponent();
	SetSkeletalMeshComponent();
	SetCharacterMovement();
}

void ASurvivor::BeginPlay()
{
	Super::BeginPlay();

	AMyGameManager* GameManager = AMyGameManager::GetInstance();
	if (GameManager)
	{
		MyHUD = GameManager->GetMyHUD();
		if(!MyHUD){ UE_LOG(LogTemp, Warning, TEXT("ASurvivor::BeginPlay - MyHUD is null.")); }

		PortraitWidget = GameManager->GetPortraitWidget();
		if (PortraitWidget)
		{
			PortraitWidget->SetSurvivorHud(Portrait, Name, this);
			PortraitButton->OnClicked.AddDynamic(this, &ASurvivor::OnPortraitClicked);
		}
		else{ UE_LOG(LogTemp, Warning, TEXT("ASurvivor::BeginPlay - PortraitWidget is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::BeginPlay - GameManager is null.")); }

	MyAIController = Cast<AMyAIController>(GetController()); // For some reason newly spawned survivors spawn without AI controllers
	if (!MyAIController)
	{
		MyAIController = GetWorld()->SpawnActor<AMyAIController>(AMyAIController::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
		if (MyAIController)
		{
			MyAIController->Possess(this);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::BeginPlay - MyAIController is null")); }
	}
}

void ASurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("bIsSelected has been set to: %d"), bIsSelected);
	//UE_LOG(LogTemp, Warning, TEXT("bIsDrafted has been set to: %d"), bIsDrafted);

	/*if (PortraitWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("DraftedSurvivors: %d"), PortraitWidget->GetDraftedSurvivors().Num());
	}*/
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
		CharMovement->MaxWalkSpeed = 250.0f;
		bUseControllerRotationYaw = false;
		CharMovement->bOrientRotationToMovement = true;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("ASurvivor::SetCharacterMovement - CharMovement is null")); }
}

void ASurvivor::OnPortraitClicked()
{
	if (MyHUD)
	{
		MyHUD->DeselectAll();
		MyHUD->Select(this);
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("ASurvivor::OnPortraitClicked - MyHUD is null")); }
}

void ASurvivor::SetbIsDrafted(bool bIsDrafted1)
{
	bIsDrafted = bIsDrafted1;
	if (bIsDrafted){ DraftedImage->SetVisibility(ESlateVisibility::Visible); }
	else { DraftedImage->SetVisibility(ESlateVisibility::Hidden); }
}

void ASurvivor::MoveToDestination()
{
	if (!bIsDrafted || !bIsSelected) { return; }

	if (MyAIController)
	{
		MyAIController->MoveToLocation(Destination, Acceptance);
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("ASurvivor::MoveTo - MyAIController is null")); }
}

