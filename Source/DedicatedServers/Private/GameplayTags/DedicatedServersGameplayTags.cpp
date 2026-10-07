#include "GameplayTags/DedicatedServersGameplayTags.h"


namespace DedicatedServersGameplayTags
{
	namespace GameSessionsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleetsResource, "DedicatedServersGameplayTags.GameSessionsAPI.ListFleetsResource", "List Fleets Resource on the Game Sessions API");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSessionResource, "DedicatedServersGameplayTags.GameSessionsAPI.FindOrCreateGameSessionsResource", "Retrieves an ACTIVE game session, creating one if one doesn't exist, on the Game Sessions API");
	}
}