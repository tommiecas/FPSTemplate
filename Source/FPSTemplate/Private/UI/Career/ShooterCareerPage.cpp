// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Career/ShooterCareerPage.h"

#include "Components/ScrollBox.h"
#include "Data/SpecialElimData.h"
#include "ShooterTypes/ShooterTypes.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "UI/Portal/Dashboard/CareerAchievement.h"



void UShooterCareerPage::OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse)
{
	Super::OnRetrieveMatchStats(RetrieveMatchStatsResponse);

	ScrollBox_Achievements->ClearChildren();

	TMap<ESpecialElimType, int32> AchievementData;

	if (RetrieveMatchStatsResponse.hits > 0) AchievementData.Emplace(ESpecialElimType::Hits, RetrieveMatchStatsResponse.hits);
	if (RetrieveMatchStatsResponse.misses > 0) AchievementData.Emplace(ESpecialElimType::Misses, RetrieveMatchStatsResponse.misses);
	if (RetrieveMatchStatsResponse.scoredEliminations > 0) AchievementData.Emplace(ESpecialElimType::ScoredEliminations, RetrieveMatchStatsResponse.scoredEliminations);
	if (RetrieveMatchStatsResponse.defeats > 0) AchievementData.Emplace(ESpecialElimType::Defeats, RetrieveMatchStatsResponse.defeats);
	if (RetrieveMatchStatsResponse.longestStreak > 0) AchievementData.Emplace(ESpecialElimType::Streak, RetrieveMatchStatsResponse.longestStreak);
	if (RetrieveMatchStatsResponse.dethroneEliminations > 0) AchievementData.Emplace(ESpecialElimType::Dethrone, RetrieveMatchStatsResponse.dethroneEliminations);
	if (RetrieveMatchStatsResponse.gotFirstBlood > 0) AchievementData.Emplace(ESpecialElimType::FirstBlood, RetrieveMatchStatsResponse.gotFirstBlood);
	if (RetrieveMatchStatsResponse.revengeEliminations > 0) AchievementData.Emplace(ESpecialElimType::Revenge, RetrieveMatchStatsResponse.revengeEliminations);
	if (RetrieveMatchStatsResponse.showstopperEliminations > 0) AchievementData.Emplace(ESpecialElimType::Showstopper, RetrieveMatchStatsResponse.showstopperEliminations);
	if (RetrieveMatchStatsResponse.headShotEliminations > 0) AchievementData.Emplace(ESpecialElimType::Headshot, RetrieveMatchStatsResponse.headShotEliminations);
	
	check(SpecialElimData);

	for (const TPair<ESpecialElimType, int32>& Pair : AchievementData)
	{
		const FString& CareerAchievementName = SpecialElimData->SpecialElimInfo.FindChecked(Pair.Key).CareerPageAchievementName;
		UTexture2D* Icon = SpecialElimData->SpecialElimInfo.FindChecked(Pair.Key).ElimIcon;

		UCareerAchievement* CareerAchievement = CreateWidget<UCareerAchievement>(this, CareerAchievementClass);
		if (IsValid(CareerAchievement))
		{
			CareerAchievement->SetAchievementText(CareerAchievementName, Pair.Value);
			if (Icon)
			{
				CareerAchievement->SetAchievementIcon(Icon);
			}
			ScrollBox_Achievements->AddChild(CareerAchievement);
		}
	}
}
