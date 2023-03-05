#include "tools.hpp"
#include "database.h"

#include <drogon/HttpController.h>

class Panel : public drogon::HttpController<Panel>
{
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(::Panel::index, "");
    METHOD_LIST_END

protected:
    void index(const drogon::HttpRequestPtr& pReq,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback)
    {
        drogon::HttpResponsePtr pResp;
        
        auto user{pReq->session()->getOptional<tsrpp::Database::User>("user")};
        if (!user)
        {
            pResp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/login"));
            callback(pResp);
            return;
        }

        drogon::HttpViewData data;
        std::cout << user->id << std::endl;
        pResp = drogon::HttpResponse::newHttpViewResponse("panel_patient_personal_informations.csp", data);
        callback(pResp);
    }
};