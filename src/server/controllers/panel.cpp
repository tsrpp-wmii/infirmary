#include <drogon/HttpController.h>

#include "tools.hpp"

class Panel : public drogon::HttpController<Panel>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(::Panel::index, "");
    METHOD_LIST_END

protected:
    void index(const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        bool loggedIn = req->session()->getOptional<bool>("loggedIn").value_or(false);
        drogon::HttpResponsePtr resp;
        if (loggedIn == false)
        {
            resp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/login"));
            callback(resp);
            return;
        }

        drogon::HttpViewData data;
        resp = drogon::HttpResponse::newHttpViewResponse("panel_patient_personal_informations.csp", data);
        callback(resp);
    }
};