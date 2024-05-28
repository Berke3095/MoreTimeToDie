#include "Harvestables/Harvestable.h"

#include "Components/WidgetComponent.h"

AHarvestable::AHarvestable()
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add("Selectable");

	HarvestableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvestableMesh"));
	RootComponent = HarvestableMesh;

	HarvestableMesh->SetMobility(EComponentMobility::Static);
}

void AHarvestable::BeginPlay()
{
	Super::BeginPlay();
}

void AHarvestable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHarvestable::CreateWidgetComponent(TSubclassOf<UUserWidget> AssignedWidgetClass1)
{
	if (AssignedWidgetClass1 && !WidgetComponent)
	{
		WidgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("WidgetComponent"));
		if (WidgetComponent)
		{
			WidgetComponent->SetupAttachment(RootComponent);
			WidgetComponent->SetWidgetClass(AssignedWidgetClass1);
			WidgetComponent->SetWorldLocation(GetActorLocation());
			WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AHarvestable::CreateWidgetComponent - WidgetComponent is null.")); }
	}
	else if (!AssignedWidgetClass1) { UE_LOG(LogTemp, Warning, TEXT("AHarvestable::CreateWidgetComponent - AssignedWidgetClass1 is null.")); }
}

