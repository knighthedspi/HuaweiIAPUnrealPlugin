#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include <string>
#include "json98.h"

#define IN_APP_CONSUMABLE 0
#define IN_APP_NONCONSUMABLE 1
#define IN_APP_SUBSCRIPTION 2

namespace huawei
{
    struct ProductInfo
    {
        ProductInfo(const json98::Json &json);

        FString currency;
        long microsPrice;
        int offerUsedStatus;
        FString originalLocalPrice;
        long originalMicroPrice;
        FString price;
        int priceType;
        FString productDesc;
        FString productId;
        FString productName;
        int status;
        FString subFreeTrialPeriod;
        FString subGroupId;
        FString subGroupTitle;
        FString subPeriod;
        int subProductLevel;
        FString subSpecialPeriod;
        int subSpecialPeriodCycles;
        FString subSpecialPrice;
        long subSpecialPriceMicros;
    };

    struct InAppPurchaseData
    {
        InAppPurchaseData(const json98::Json &json);

        int accountFlag;
        FString appInfo;
        FString applicationId;
        int cancelledSubKeepDays;
        long cancellationTime;
        int cancelReason;
        long cancelTime;
        int cancelWay;
        int consumptionState;
        FString country;
        FString currency;
        long daysLasted;
        int deferFlag;
        FString developerChallenge;
        FString developerPayload;
        long expirationDate;
        int expirationIntent;
        long graceExpirationTime;
        int introductoryFlag;
        int kind;
        FString lastOrderId;
        int notifyClosed;
        long numOfDiscount;
        long numOfPeriods;
        FString orderID;
        long oriPurchaseTime;
        FString oriSubscriptionId;
        FString packageName;
        FString payOrderId;
        FString payType;
        long price;
        int priceConsentStatus;
        FString productGroup;
        FString productId;
        FString productName;
        int purchaseState;
        long purchaseTime;
        FString purchaseToken;
        int purchaseType;
        int quantity;
        long renewPrice;
        int renewStatus;
        long resumeTime;
        int retryFlag;
        FString subscriptionId;
        int trialFlag;
        bool isAutoRenewing;
        bool isSubValid;
    };

    class IapListener
    {
        virtual void onCheckEnvironmentSuccess();
        virtual void onException(const FString &action, const FString &message);
        virtual void onObtainProductList(const std::vector<ProductInfo> &products, int type);
        virtual void onPurchaseSuccess(const FString &productId, int type);
        virtual void onObtainPurchases(const std::vector<InAppPurchaseData> &purchasedProductIds, const std::vector<InAppPurchaseData> &nonPurchasedProductIds, int type);
        virtual void onObtainPurchasedRecords(const std::vector<InAppPurchaseData> &purchasedProductIds, int type);
    };

    class Iap
    {
    public:
        static void checkEnvironment();
        static void queryProducts(const std::vector<FString> &productIds, int type);
        static void queryPurchases(int type);
        static void buyProduct(FString &productId, int type);
        static void getPurchasedRecords(int type);

        static void setListener(IapListener *listener);
        static void removeListener();
        static IapListener *getListener();
    };
}