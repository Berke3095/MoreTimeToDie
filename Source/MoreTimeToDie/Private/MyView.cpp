#include "MyView.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

#include "NavigationSystem.h"

#include "MyGameManager.h"
#include "MyPlayerController.h"
#include "MyHUD.h"
#include "Characters/Survivor.h"
#include "Harvestables/Harvestable.h"

AMyView::AMyView()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	SetupSpringArm();
	SetupCamera();
}

void AMyView::BeginPlay()
{
	Super::BeginPlay();

	GetReferences();
	
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ViewMappingContext, 0);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::BeginPlay - Enh.InputSubsystem is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::BeginPlay - PlayerController is null.")); }
}

void AMyView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CurrentTargetArmLength, DeltaTime, 3.0f);
}

/*
	INPUTS
*/
void AMyView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(MoveCameraAction){ EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &AMyView::MoveCamera); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - CameraMoveAction is null.")); }

		if (MidMouseAction)
		{
			EnhancedInputComponent->BindAction(MidMouseAction, ETriggerEvent::Started, this, &AMyView::MidMouseStart);
			EnhancedInputComponent->BindAction(MidMouseAction, ETriggerEvent::Completed, this, &AMyView::MidMouseEnd);
		}
		else{ UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - MidMouseAction is null.")); }

		if (RotateCameraAction) { EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &AMyView::RotateCamera); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - RotateCameraAction is null.")); }

		if(ZoomCameraAction){ EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &AMyView::ZoomCamera); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - ZoomCameraAction is null.")); }

		if (LeftClickAction)
		{
			EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AMyView::LeftClickStart);
			EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AMyView::LeftClickTrigger);
			EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AMyView::LeftClickEnd);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - LeftClickAction is null.")); }

		if (ShiftAction)
		{
			EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AMyView::ShiftStart);
			EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AMyView::ShiftEnd);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - ShiftAction is null.")); }

		if (CtrlAction)
		{
			EnhancedInputComponent->BindAction(CtrlAction, ETriggerEvent::Started, this, &AMyView::CtrlStart);
			EnhancedInputComponent->BindAction(CtrlAction, ETriggerEvent::Completed, this, &AMyView::CtrlEnd);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - CtrlAction is null.")); }

		if (RightClickAction)
		{
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AMyView::RightClickStart);
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AMyView::RightClickEnd);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - RightClickAction is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - EnhancedInputComponent is null.")); }
}

void AMyView::GetReferences()
{
	GameManager = AMyGameManager::GetInstance();
	if (GameManager)
	{
		PlayerController = GameManager->GetMyPlayerController();

		MyHUD = GameManager->GetMyHUD();
		if (!MyHUD) { UE_LOG(LogTemp, Warning, TEXT("AMyView::GetReferences - MyHUD is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::GetReferences - GameManager is null.")); }
}

void AMyView::MoveCamera(const FInputActionValue& InputValue1)
{
	const FVector2D MoveValue = InputValue1.Get<FVector2D>();

	if (GameManager)
	{
		GameManager->DestroyHarvestWidgets();
	}

	if (PlayerController)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X) * MoveValue.Y
			+ FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y) * MoveValue.X;

		SpringArm->AddRelativeLocation(Direction * CameraSpeed * GetWorld()->GetDeltaSeconds());
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::MoveCamera - PlayerController is null.")); }
}

void AMyView::MidMouseStart()
{
	bMidMouseHeld = true;
	
	if (PlayerController)
	{
		PlayerController->GetMousePosition(MouseX, MouseY);
		PlayerController->bShowMouseCursor = false;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::MidMouseStart - PlayerController is null.")); }
}

void AMyView::MidMouseEnd()
{
	if (bMidMouseHeld) { bMidMouseHeld = false; }

	if (PlayerController && !PlayerController->bShowMouseCursor)
	{
		PlayerController->bShowMouseCursor = true;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::MidMouseEnd - PlayerController is null or bShowMouseCursor is already true.")); }
}

void AMyView::RotateCamera(const FInputActionValue& InputValue1)
{
	const FVector2D RotateValue = InputValue1.Get<FVector2D>();

	if (GameManager)
	{
		GameManager->DestroyHarvestWidgets();
	}

	FVector2D YawValue = RotateValue / 8;
	FVector2D PitchValue = RotateValue / 3;

	if (PlayerController && bMidMouseHeld)
	{
		AddControllerYawInput(YawValue.X);

		float MinPitch = -15.0f;
		float MaxPitch = 30.0f;

		float NewPitch = FMath::Clamp(Camera->GetRelativeRotation().Pitch + PitchValue.Y, MinPitch, MaxPitch); // Clamp the pitch of camera

		Camera->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f)); // Set the new pitch

		PlayerController->SetMouseLocation(MouseX, MouseY); // Set the mouse position to the last capture positions 
	}
	else if(!PlayerController){ UE_LOG(LogTemp, Warning, TEXT("AMyView::RotateCamera - PlayerController is null.")); }
}

void AMyView::ZoomCamera(const FInputActionValue& InputValue1)
{
	const float ZoomValue = InputValue1.Get<float>();

	if (GameManager)
	{
		GameManager->DestroyHarvestWidgets();
	}

	if (PlayerController)
	{
		float MinArm = MinTargetArmLength;
		float MaxArm = MaxTargetArmLength;

		float NewArmLength = SpringArm->TargetArmLength + (ZoomValue * CameraZoomSpeed);

		CurrentTargetArmLength = FMath::Clamp(NewArmLength, MinArm, MaxArm);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::ZoomCamera - PlayerController is null.")); }
}

void AMyView::LeftClickStart()
{
	if (GameManager)
	{
		GameManager->DestroyHarvestWidgets();
	}

	if (PlayerController)
	{
		if (MyHUD)
		{
			AActor* HoveredActor = PlayerController->GetHoveredActor();
			if (HoveredActor && HoveredActor->ActorHasTag("Selectable"))
			{
				MyHUD->DeselectAll();
				MyHUD->Select(HoveredActor);
			}
			else
			{
				MyHUD->DeselectAll();
			}
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyView::LeftClickStart - MyHUD is null.")); }

		if (!bMidMouseHeld)
		{
			PlayerController->GetMousePosition(MouseX, MouseY);
			StartingRectanglePosition = FVector2D(MouseX, MouseY);
			bCanDraw = true;
		}
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("AMyView::LeftClickStart - PlayerController is null.")); }
}

void AMyView::LeftClickTrigger()
{
	if (PlayerController)
	{
		PlayerController->GetMousePosition(MouseX, MouseY);
		EndingRectanglePosition = FVector2D(MouseX, MouseY);
	}
	else{ UE_LOG(LogTemp, Warning, TEXT("AMyView::LeftClickTrigger - PlayerController is null.")); }
}

void AMyView::LeftClickEnd()
{
	if (PlayerController && bCanDraw)
	{
		bCanDraw = false;
	}
	else if (!PlayerController){ UE_LOG(LogTemp, Warning, TEXT("AMyView::LeftClickEnd - PlayerController is null.")); }
}

void AMyView::ShiftStart()
{
	bShiftHeld = true;
}

void AMyView::ShiftEnd()
{
	if (bShiftHeld)
	{
		bShiftHeld = false;
	}
}

void AMyView::CtrlStart()
{
	bCtrlHeld = true;
}

void AMyView::CtrlEnd()
{
	if (bCtrlHeld)
	{
		bCtrlHeld = false;
	}
}

void AMyView::RightClickStart()
{
	OrderMove();
}

void AMyView::RightClickEnd()
{
	HandleHarvestWidget();
}

void AMyView::SetDestinations(FVector& CenterPoint)
{
	TArray<ASurvivor*> MoveableSurvivors{};
	TArray <FVector> FoundDestinations{};

	if (MyHUD)
	{
		for (ASurvivor* Survivor : MyHUD->GetSelectedSurvivors())
		{
			if (Survivor->GetbIsDrafted())
			{
				MoveableSurvivors.AddUnique(Survivor);
			}
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetDestinations - MyHUD is null.")); }

	const TArray<FVector> FormationOffsets = {
		FVector(0.0f, 0.0f, 0.0f),
		FVector(100.0f, -100.0f, 0.0f),
		FVector(-100.0f, -100.0f, 0.0f),
		FVector(-100.0f, 100.0f, 0.0f),
		FVector(100.0f, 100.0f, 0.0f),
		FVector(150.0f, 0.0f, 0.0f),
		FVector(-150.0f, 0.0f, 0.0f)
	};

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("AMyAIController::SetDestinations - Navigation system is null.")); }

	for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
	{
		FVector OffsetVector = FormationOffsets[i];
		FVector LeaderDestination = CenterPoint + OffsetVector;

		FNavLocation ProjectedLocation{};
		if (NavSys && NavSys->ProjectPointToNavigation(LeaderDestination, ProjectedLocation, FVector(100.0f, 100.0f, 100.0f)))
		{
			FoundDestinations.AddUnique(ProjectedLocation.Location);
		}
	}

	for (int32 i = 0; i < MoveableSurvivors.Num(); ++i)
	{
		if (MoveableSurvivors.IsValidIndex(i) && FoundDestinations.IsValidIndex(i))
		{
			MoveableSurvivors[i]->SetDestination(FoundDestinations[i]);
		}
		else { break; }
	}

	MoveableSurvivors.Empty();
	FoundDestinations.Empty();
}

void AMyView::OrderMove()
{
	if (MyHUD && MyHUD->GetSelectedSurvivors().Num() > 0 &&
		PlayerController && PlayerController->GetHoveredActor())
	{
		Destination = PlayerController->GetHitResult().ImpactPoint;

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSys) { UE_LOG(LogTemp, Warning, TEXT("AMyView::OrderMove - Navigation system is null.")); }

		FNavLocation ProjectedLocation{};
		if (!NavSys->ProjectPointToNavigation(Destination, ProjectedLocation)) { return; }

		SetDestinations(Destination);
	}
	else if (!MyHUD || !PlayerController) { UE_LOG(LogTemp, Warning, TEXT("AMyView::OrderMove - MyHUD or PlayerController is null.")); }
}

void AMyView::HandleHarvestWidget()
{
	if (PlayerController && PlayerController->GetHoveredActor() && PlayerController->GetHoveredActor()->IsA<AHarvestable>())
	{
		HarvestableActor = Cast<AHarvestable>(PlayerController->GetHoveredActor());
		if (HarvestableActor && GameManager)
		{
			GameManager->CreateWidgetAtHarvest(HarvestableActor);
		}
		else if (!GameManager) { UE_LOG(LogTemp, Warning, TEXT("AMyView::RightClick - GameManager is null.")); }
	}
	else if (!PlayerController) { UE_LOG(LogTemp, Warning, TEXT("AMyView::RightClick - PlayerController is null.")); }
	else
	{
		GameManager->DestroyHarvestWidgets();
	}
}

/*
	VIEW
*/
void AMyView::SetupSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArm)
	{
		RootComponent = SpringArm;

		SpringArm->bEnableCameraLag = true;
		SpringArm->CameraLagSpeed = 5.0f;
		SpringArm->CameraRotationLagSpeed = 5.0f;

		SpringArm->bDoCollisionTest = false;

		SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->TargetArmLength = MaxTargetArmLength;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupSpringArm - SpringArm is null.")); }
}

void AMyView::SetupCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera)
	{
		Camera->SetupAttachment(RootComponent);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupCamera - Camera is null.")); }
}

