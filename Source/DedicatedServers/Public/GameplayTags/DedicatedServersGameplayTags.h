#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace DedicatedServersGameplayTags
{
	namespace GameSessionsAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ListFleetsResource);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FindOrCreateGameSessionResource);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(CreatePlayerSessionResource);

	}

	namespace PortalAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignUpResource);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConfirmSignUpResource);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignInResource);
	}
}