#include "MyView.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

AMyView::AMyView()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupSpringArm();
	SetupCamera();
}

void AMyView::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyView::MoveCamera(const FInputActionValue& InputValue1)
{

}

void AMyView::SetupSpringArm() // SpringArm
{
	ViewSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (ViewSpringArm)
	{
		RootComponent = ViewSpringArm;

		ViewSpringArm->bEnableCameraLag = true;
		ViewSpringArm->CameraLagSpeed = 5.0f;
		ViewSpringArm->CameraRotationLagSpeed = 5.0f;

		ViewSpringArm->bDoCollisionTest = false;

		ViewSpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ViewSpringArm->TargetArmLength = MaxTargetArmLength;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupSpringArm - SpringArm is null.")); }
}

void AMyView::SetupCamera() // Camera
{
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (ViewCamera)
	{
		ViewCamera->SetupAttachment(RootComponent);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyView::SetupCamera - Camera is null.")); }
}

