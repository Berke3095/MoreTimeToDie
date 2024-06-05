#pragma once

UENUM(BlueprintType)
enum class ESurvivorMoveState : uint8
{
	ESMS_Walking UMETA(DisplayName = "Walking"),
	ESMS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class ESurvivorWorkState : uint8
{
	ESWS_Mining UMETA(DisplayName = "Mining"),
	ESWS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class ESurvivorGeneralState : uint8
{
	ESGS_Tasking UMETA(DisplayName = "Tasking"),
	ESGS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class ESurvivorTaskState : uint8
{
	ESTS_Performing UMETA(DisplayName = "Performing"),
	ESTS_Preparing UMETA(DisplayName = "Preparing"),
	ESTS_NONE UMETA(DisplayName = "NONE")
};

