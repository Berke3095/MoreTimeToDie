#include "MyView.h"

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

void AMyView::SetupSpringArm() // SpringArm
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	RootComponent = SpringArm;

	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->CameraRotationLagSpeed = 5.0f;

	SpringArm->bDoCollisionTest = false;

	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = MaxTargetArmLength;
}

void AMyView::SetupCamera() // Camera
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

