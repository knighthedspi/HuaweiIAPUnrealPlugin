#pragma once
#include "Iap.h"
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(HuaweiIap_Native, Log, All);

namespace huawei
{

    class IapJniWrapper
    {

    public:
        static IapJniWrapper *getInstance();

        IapJniWrapper();
        ~IapJniWrapper();

        void checkEnvironment();
        void queryProducts(const std::vector<std::string> &productIds, int type);
        void queryPurchases(int type);
        void buyProduct(const std::string productId, int type);
        void getPurchasedRecords(int type);

        // Listener
        void setListener(IapListener *listener);
        void removeListener();
        IapListener *getListener();

        // Callbacks
        void onCheckEnvironmentSuccess();
        void onException(const FString &action, const FString &message);
        void onObtainProductList(const std::string &products, int type);
        void onPurchaseSuccess(const FString &productId, int type);
        void onObtainPurchases(const std::string &purchasedProductIds, const std::string &nonPurchasedProductIds, int type);
        void onObtainPurchasedRecords(const std::string &purchasedProductIds, int type);

    protected:
        IapListener *_listener;
    };
}
