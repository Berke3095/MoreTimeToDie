#include "MyView.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

#include "MyGameManager.h"
#include "MyPlayerController.h"

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
	
	GameManager = AMyGameManager::GetInstance();
	if (GameManager)
	{
		PlayerController = GameManager->GetMyPlayerController();
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
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::BeginPlay - GameManager is null.")); }
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
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &AMyView::MoveCamera);

		EnhancedInputComponent->BindAction(MidMouseAction, ETriggerEvent::Started, this, &AMyView::MidMouseStart);
		EnhancedInputComponent->BindAction(MidMouseAction, ETriggerEvent::Completed, this, &AMyView::MidMouseEnd);

		EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &AMyView::RotateCamera);

		EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &AMyView::ZoomCamera);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - EnhancedInputComponent is null.")); }
}

void AMyView::MoveCamera(const FInputActionValue& InputValue1)
{
	const FVector2D MoveValue = InputValue1.Get<FVector2D>();

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

	if (PlayerController)
	{
		float MinArm = MinTargetArmLength;
		float MaxArm = MaxTargetArmLength;

		float NewArmLength = SpringArm->TargetArmLength + (ZoomValue * CameraZoomSpeed);

		CurrentTargetArmLength = FMath::Clamp(NewArmLength, MinArm, MaxArm);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::ZoomCamera - PlayerController is null.")); }
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

