#pragma once

UENUM(BlueprintType)
enum class ESurvivorMoveState : uint8
{
	ESMS_Walking UMETA(DisplayName = "Walking"),
	ESMS_NONE UMETA(DisplayName = "NONE"),
};

UENUM(BlueprintType)
enum class ESurvivorWorkState : uint8
{
	ESWS_Walking UMETA(DisplayName = "Mining"),
	ESWS_NONE UMETA(DisplayName = "NONE"),
};

