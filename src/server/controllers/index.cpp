#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>

class HelloViewController : public drogon::HttpSimpleController<HelloViewController>
{
  public:
    PATH_LIST_BEGIN
    PATH_ADD("/view");
    PATH_LIST_END

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpViewData data;
        data["name"] = req->getParameter("name");
        auto resp = drogon::HttpResponse::newHttpViewResponse("hello", data);
        callback(resp);
    }
};