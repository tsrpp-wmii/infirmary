#include <drogon/HttpController.h>

class Panel : public drogon::HttpController<Panel>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(::Panel::index, "", drogon::Get);
    METHOD_LIST_END

protected:
    void index(const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        bool loggedIn = req->session()->getOptional<bool>("loggedIn").value_or(false);
        drogon::HttpResponsePtr resp;
        if (loggedIn == false)
        {
            // TODO:: select of a appropriate proctol needs to be refined
            const auto redirectionUrl = "http://" + req->getHeader("host") + "/login";
            resp = drogon::HttpResponse::newRedirectionResponse(redirectionUrl);
        }
        else
        {
            resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("loggedIn true");
        }
        callback(resp);
    }
};