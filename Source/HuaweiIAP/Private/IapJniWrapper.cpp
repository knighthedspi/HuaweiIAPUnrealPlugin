#include "IapJniWrapper.h"
#include "HuaweiIapModule.h"
#include "json98.h"
#include "Async/AsyncWork.h"

using namespace std;
using namespace json98;

DEFINE_LOG_CATEGORY(HuaweiIap_Native);

#if PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

// Initialize JNI context
#define INIT_JAVA_METHOD(name, signature)
if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
{
    name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false);
    check(name != NULL);
}
else
{
    check(0);
}

#define DECLARE_JAVA_METHOD(name)
static jmethodID name = NULL;

string jstring2string(JNIEnv *env, jstring jstr)
{
    if (jstr == nullptr || !env)
    {
        return "";
    }

    const char *chars = env->GetStringUTFChars(jstr, nullptr);
    string ret(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

FString jstring2FString(JNIEnv *env, jstring jstr)
{
    if (jstr == nullptr || !env)
    {
        return "";
    }

    const char *chars = env->GetStringUTFChars(jstr, nullptr);
    FString ret = FString(UTF8_TO_TCHAR(chars));
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnCheckEnvironmentSuccess(JNIEnv *env, jobject thiz)
{
    AsyncTask(ENamedThreads::GameThread, [=]()
              { huawei::IapJniWrapper::getInstance()->onCheckEnvironmentSuccess(); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnException(JNIEnv *env, jobject thiz, jstring action_, jstring message_)
{
    FString action = jstring2FString(env, action_);
    FString message = jstring2FString(env, message_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { huawei::IapJniWrapper::getInstance()->onException(action, message); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnObtainProductList(JNIEnv *env, jobject thiz, jstring data_, int type)
{
    string data = jstring2string(env, data_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { huawei::IapJniWrapper::getInstance()->onObtainProductList(data, type); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnPurchaseSuccess(JNIEnv *env, jobject thiz, jstring data_, int type)
{
    string data = jstring2string(env, data_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { huawei::IapJniWrapper::getInstance()->onPurchaseSuccess(data, type); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnObtainPurchases(JNIEnv *env, jobject thiz, jstring purchasedData_, jstring nonPurchasedData_, int type)
{
    string purchasedData = jstring2string(env, purchasedData_);
    string nonPurchasedData = jstring2string(env, nonPurchasedData_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { huawei::IapJniWrapper::getInstance()->onObtainPurchases(purchasedData, nonPurchasedData, type); });
}

extern "C" void Java_com_epicgames_ue4_GameActivity_nativeOnObtainPurchasedRecords(JNIEnv *env, jobject thiz, jstring data_, int type)
{
    string data = jstring2string(env, data_);
    AsyncTask(ENamedThreads::GameThread, [=]()
              { huawei::IapJniWrapper::getInstance()->onObtainPurchasedRecords(data, type); });
}

namespace huawei
{

    DECLARE_JAVA_METHOD(HuaweiIap_Check_Environment);
    DECLARE_JAVA_METHOD(HuaweiIap_Query_Products);
    DECLARE_JAVA_METHOD(HuaweiIap_Query_Purchases);
    DECLARE_JAVA_METHOD(HuaweiIap_Buy_Product);
    DECLARE_JAVA_METHOD(HuaweiIAP_Get_PurchaseRecords);

    static IapJniWrapper *s_instance = nullptr;

    IapJniWrapper *IapJniWrapper::getInstance()
    {
        if (!s_instance)
        {
            s_instance = new IapJniWrapper();
        }

        return s_instance;
    }

    IapJniWrapper::IapJniWrapper()
    {
        _listener = nullptr;

        INIT_JAVA_METHOD(HuaweiIap_Check_Environment, "()V");
        INIT_JAVA_METHOD(HuaweiIap_Query_Products, "([Ljava/lang/String;I)V");
        INIT_JAVA_METHOD(HuaweiIap_Query_Purchases, "(I)V");
        INIT_JAVA_METHOD(HuaweiIap_Buy_Product, "(Ljava/lang/String;I)V");
        INIT_JAVA_METHOD(HuaweiIAP_Get_PurchaseRecords, "(I)V");
    }

    IapJniWrapper::~IapJniWrapper()
    {
    }

    void IapJniWrapper::checkEnvironment()
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
        {
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiIap_Check_Environment);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiIap_Check_Environment\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

    void IapJniWrapper::queryProducts(const std::vector<string> &productIds, int type)
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
        {
            int size = productIds.size();
            jstring[] ids = new jstring[size];
            for (int i = 0; i < length; i++)
            {
                ids[i] = Env->NewStringUTF(*productIds[i].c_str());
            }
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiIap_Query_Products, ids, type);
            Env->DeleteLocalRef(ids);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiIap_Query_Products\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

    void IapJniWrapper::queryPurchases(int type)
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
        {
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiIap_Query_Purchases, type);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiIap_Query_Purchases\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

    void IapJniWrapper::buyProduct(const std::string productId, int type)
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
        {
            jstring id = Env->NewStringUTF(productId.c_str());
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiIap_Buy_Product, id, type);
            Env->DeleteLocalRef(id);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiIap_Buy_Product\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

    void IapJniWrapper::getPurchasedRecords(int type)
    {
        if (JNIEnv *Env = FAndroidApplication::GetJavaEnv(true))
        {
            FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HuaweiIAP_Get_PurchaseRecords, type);
            UE_LOG(LogAndroid, Warning, TEXT("I found the java method HuaweiIAP_Get_PurchaseRecords\n"));
        }
        else
        {
            UE_LOG(LogAndroid, Warning, TEXT("ERROR Could note get Java ENV\n"));
        }
    }

    /********************************************
     * Listener
     ********************************************/

    void IapJniWrapper::setListener(IapListener *listener)
    {
        _listener = listener;
    }

    void IapJniWrapper::removeListener()
    {
        _listener = nullptr;
    }

    IapListener *IapJniWrapper::getListener()
    {
        return _listener;
    }

    /********************************************
     * Callbacks
     ********************************************/

    void IapJniWrapper::onCheckEnvironmentSuccess()
    {
        UE_LOG(HuaweiIap_Native, Log, TEXT("Check environment success"));
        if (_listener != nullptr)
        {
            _listener->onCheckEnvironmentSuccess();
        }
    }

    void IapJniWrapper::onException(const FString &action, const FString &message)
    {
        if (_listener != nullptr)
        {
            _listener->onException(action, message);
        }
    }

    void IapJniWrapper::onObtainProductList(const std::string &products, int type)
    {
        if (_listener != nullptr)
        {
            auto json_data = Json::parse(data);
            vector<ProductInfo> products;

            for (auto &d : json_data.array_items())
            {
                ProductInfo p(d);
                products.push_back(p);
            }

            _listener->onObtainProductList(products, type);
        }
    }

    void IapJniWrapper::onPurchaseSuccess(const FString &productId, int type)
    {
        if (_listener != nullptr)
        {
            _listener->onPurchaseSuccess(productId, type);
        }
    }

    void IapJniWrapper::onObtainPurchases(const std::string &purchasedProductIds, const std::string &nonPurchasedProductIds, int type)
    {
        if (_listener != nullptr)
        {
            vector<InAppPurchaseData> purchasedProductDatas = huawei::IapJniWrapper::getInstance()->convertFromJsonString(purchasedProductIds);
            vector<InAppPurchaseData> nonPurchasedProductDatas = huawei::IapJniWrapper::getInstance()->convertFromJsonString(nonPurchasedProductIds);
            _listener->onObtainPurchases(purchasedProductDatas, nonPurchasedProductDatas, type);
        }
    }

    void IapJniWrapper::onObtainPurchasedRecords(const std::string &purchasedProductIds, int type)
    {
        if (_listener != nullptr)
        {
            vector<InAppPurchaseData> purchasedProductDatas = huawei::IapJniWrapper::getInstance()->convertFromJsonString(purchasedProductIds);
            _listener->onObtainPurchasedRecords(purchasedProductDatas, type);
        }
    }

    vector<InAppPurchaseData> convertFromJsonString(const std::string &data)
    {
        auto jsonData = Json::parse(data);
        vector<InAppPurchaseData> result;

        for (auto$d : jsonData.array_items())
        {
            InAppPurchaseData p(d);
            result.push_back(p);
        }

        return result;
    }

}

#else

// Stub function for other platforms
namespace huawei
{

    static IapJniWrapper *s_instance = nullptr;

    IapJniWrapper *IapJniWrapper::getInstance()
    {
        if (!s_instance)
        {
            s_instance = new IapJniWrapper();
        }

        return s_instance;
    }

    IapJniWrapper::IapJniWrapper()
    {
        _listener = nullptr;
    }

    IapJniWrapper::~IapJniWrapper()
    {
    }

    void IapJniWrapper::checkEnvironment()
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::queryProducts(const std::vector<string> &productIds, int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::queryPurchases(int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::buyProduct(const std::string productId, int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::getPurchasedRecords(int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    /********************************************
     * Listener
     ********************************************/

    void IapJniWrapper::setListener(IapListener *listener)
    {
        _listener = listener;
    }

    void IapJniWrapper::removeListener()
    {
        _listener = nullptr;
    }

    IapListener *IapJniWrapper::getListener()
    {
        return _listener;
    }

    /********************************************
     * Callbacks
     ********************************************/

    void IapJniWrapper::onCheckEnvironmentSuccess()
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::onException(const FString &action, const FString &message)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::onObtainProductList(const std::string &products, int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::onPurchaseSuccess(const FString &productId, int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::onObtainPurchases(const std::string &purchasedProductIds, const std::string &nonPurchasedProductIds, int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

    void IapJniWrapper::onObtainPurchasedRecords(const std::string &purchasedProductIds, int type)
    {
        UE_LOG(HuaweiIap_Native, Warning, TEXT("Huawei IAP is not supported on this platform\n"));
    }

}

#endif
