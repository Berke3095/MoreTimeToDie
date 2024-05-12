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

}

void AMyView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &AMyView::MoveCamera);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupPlayerInputComponent - EnhancedInputComponent is null.")); }
}

void AMyView::MoveCamera(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();
	if (PlayerController)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X) * Value.Y
			+ FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y) * Value.X;

		SpringArm->AddRelativeLocation(Direction * CameraSpeed * GetWorld()->GetDeltaSeconds());
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::MoveCamera - PlayerController is null.")); }
}

void AMyView::SetupSpringArm() // SpringArm
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

void AMyView::SetupCamera() // Camera
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera)
	{
		Camera->SetupAttachment(RootComponent);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupCamera - Camera is null.")); }
}

