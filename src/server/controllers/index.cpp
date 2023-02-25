#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>

class IndexViewController : public drogon::HttpSimpleController<IndexViewController>
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/");
    PATH_LIST_END

    void asyncHandleHttpRequest(const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpViewData data;

        std::string projectName{PROJECT_NAME};
        projectName.front() = std::toupper(projectName.front());
        data.insert("project_name", std::move(projectName));

        // ISSUE: drogon doesn't allow to name csp files as index
        auto resp = drogon::HttpResponse::newHttpViewResponse("index_", data);
        callback(resp);
    }
};