#include "GameplayTags/DedicatedServersGameplayTags.h"


namespace DedicatedServersGameplayTags
{
	namespace GameSessionsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleetsResource, "DedicatedServersGameplayTags.GameSessionsAPI.ListFleetsResource", "List Fleets Resource on the Game Sessions API");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSessionResource, "DedicatedServersGameplayTags.GameSessionsAPI.FindOrCreateGameSessionsResource", "Retrieves an ACTIVE game session, creating one if one doesn't exist, on the Game Sessions API");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSessionResource, "DedicatedServersGameplayTags.GameSessionsAPI.CreatePlayerSessionResource", "Creates a new player session on the Game Sessions API");
	}

	namespace PortalAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignUpResource, "DedicatedServersGameplayTags.PortalAPI.SignUpResource", "Creates a new user in the Portal API.");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConfirmSignUpResource, "DedicatedServersGameplayTags.PortalAPI.ConfirmSignUpResource", "Confirms the creation of a new user in the Portal API.");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignInResource, "DedicatedServersGameplayTags.PortalAPI.SignInResource", "Signs a new user in the Portal API into their account.");
	}
}