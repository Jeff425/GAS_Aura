// Copyright KhalorlStudios


#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AAuraAIController::AAuraAIController()
{

	this->Blackboard = this->CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(this->Blackboard);
	this->BehaviorTreeComponent = this->CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(this->BehaviorTreeComponent);
}
