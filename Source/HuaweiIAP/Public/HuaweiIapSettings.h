// Copyright 2020-2021 Nikolay Prudnikov. All Rights Reserved.

#pragma once

#include "HuaweiIapSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class UPrHuaweiIapSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/* Path to agconnect-services.json */
	UPROPERTY(Config, EditAnywhere, Category = "HuaweiIAP")
	FString HuaweiAGCConfig;

	UPROPERTY(Config, EditAnywhere, Category = "HuaweiIAP")
	FString HuaweiIAPPublicKey;
};
