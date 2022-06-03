#include "HuaweiIapSettings.h"

UPrHuaweiIapSettings::UPrHuaweiIapSettings(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer), HuaweiAGCConfig(TEXT("Config/AGC/agconnect-services.json")), HuaweiIAPPublicKey(TEXT(""))
{
}
