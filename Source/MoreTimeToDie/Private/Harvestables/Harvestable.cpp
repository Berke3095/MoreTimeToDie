#include "Harvestables/Harvestable.h"

AHarvestable::AHarvestable()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Selectable");

	HarvestableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvestableMesh"));
	RootComponent = HarvestableMesh;

	HarvestableMesh->SetMobility(EComponentMobility::Static);
	HarvestableMesh->SetCanEverAffectNavigation(false);
}

void AHarvestable::BeginPlay()
{
	Super::BeginPlay();
}

void AHarvestable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*UE_LOG(LogTemp, Warning, TEXT("HarvestHealth: %d"), HarvestHealth);*/
}