#include "Harvestables/Harvestable.h"

AHarvestable::AHarvestable()
{
	PrimaryActorTick.bCanEverTick = false;

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

