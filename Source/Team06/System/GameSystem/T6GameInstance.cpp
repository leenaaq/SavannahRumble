// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetData.h"

void UT6GameInstance::Init()
{
	Super::Init();

	LoadLevelsFromFolder();
}

FString UT6GameInstance::GetRandomLevel()
{
	if (LevelList.Num() == 0)
	{
		return TEXT(""); // 레벨이 없을 경우 빈 문자열 반환
	}

	int32 RandomIndex = FMath::RandRange(0, LevelList.Num() - 1);
	return LevelList[RandomIndex];
	// 중복 없는 것으로 수정필요
}

void UT6GameInstance::LoadLevelsFromFolder()
{
	LevelList.Empty();

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FString> PathsToScan = { "/Game/Team6/GameSystem/GS_Level/GameLevel" };
	AssetRegistryModule.Get().ScanPathsSynchronous(PathsToScan, true);
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByPath(FName("/Game/Team6/GameSystem/GS_Level/GameLevel"), AssetData, true);

	if (AssetData.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No Assets found in /Game/Team6/GameSystem/GS_Level/GameLevel!"));
	}
	for (const FAssetData& Asset : AssetData)
	{
		FName AssetClassPathString = Asset.AssetClassPath.GetAssetName();

		if (AssetClassPathString == FName("World"))
		{
			FString LevelPath = Asset.GetObjectPathString();
			UE_LOG(LogTemp, Warning, TEXT("Found Level: %s"), *LevelPath);

			LevelPath = LevelPath.LeftChop(LevelPath.Find(TEXT(".")) - LevelPath.Len());
			int32 DotIndex;
			if (LevelPath.FindLastChar(TEXT('.'), DotIndex))
			{
				LevelPath = LevelPath.Left(DotIndex);  // 마지막 '.' 이전까지만 남김
			}
			LevelList.Add(LevelPath);
		}
	}
	if (LevelList.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No levels found in /Game/Team6/GameSystem/GS_Level/GameLevel !!"));
	}
}
