// All rights reserved Daniil Grigoriev


#include "UBDAIController.h"
#include "UBDBaseEnemy.h"

void AUBDAIController::BeginPlay()
{
    Super::BeginPlay();
    AUBDBaseEnemy* ControlledMonster = Cast<AUBDBaseEnemy>(GetPawn());

    if (ControlledMonster)
    {
        Agent = ControlledMonster;
        TeamId = FGenericTeamId(Agent->ID);
    }
}

void AUBDAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AUBDBaseEnemy* ControlledMonster = Cast<AUBDBaseEnemy>(InPawn);

    if (ControlledMonster)
    {
        Agent = ControlledMonster;
        TeamId = FGenericTeamId(Agent->ID);
    }
}

ETeamAttitude::Type AUBDAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    const APawn* OtherPawn = Cast<APawn>(&Other);
    if (OtherPawn == nullptr)
    {
        return ETeamAttitude::Neutral;
    }

    auto PlayerTI = Cast<IGenericTeamAgentInterface>(&Other);
    class IGenericTeamAgentInterface* BotTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
    if (BotTI == nullptr && PlayerTI == nullptr)
    {
        return ETeamAttitude::Neutral;
    }
    FGenericTeamId OtherActorTeamId = NULL;
    if (BotTI)
    {
        OtherActorTeamId = BotTI->GetGenericTeamId();
    }
    else if (PlayerTI)
    {
        OtherActorTeamId = PlayerTI->GetGenericTeamId();
    }

    FGenericTeamId ThisId = GetGenericTeamId();
    if (OtherActorTeamId == 8)
    {
        return ETeamAttitude::Neutral;
    }
    else if (OtherActorTeamId == ThisId)
    {
        return ETeamAttitude::Friendly;
    }
    return ETeamAttitude::Hostile;
}
