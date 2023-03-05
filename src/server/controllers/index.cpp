#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>

class IndexViewController : public drogon::HttpSimpleController<IndexViewController>
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/");
    PATH_LIST_END

    void asyncHandleHttpRequest(const drogon::HttpRequestPtr& pReq,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        // ISSUE: drogon doesn't allow to name csp files as index
        auto pResp = drogon::HttpResponse::newHttpViewResponse("index_");
        callback(pResp);
    }
};