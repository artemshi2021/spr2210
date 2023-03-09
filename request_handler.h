#pragma once
#include "http_server.h"
#include "model.h"

namespace http_handler {
namespace beast = boost::beast;
namespace http = beast::http;

class RequestHandler {
public:
    explicit RequestHandler(model::Game& game)
        : game_{game} {
    }
    
    // Запрос, тело которого представлено в виде строки
using StringRequest = http::request<http::string_body>;
// Ответ, тело которого представлено в виде строки
using StringResponse = http::response<http::string_body>;

struct ContentType {
    ContentType() = delete;
    constexpr static std::string_view TEXT_HTML = "text/html";
    constexpr static std::string_view TEXT_Json = "application/json";
    // При необходимости внутрь ContentType можно добавить и другие типы контента
};

// Создаёт StringResponse с заданными параметрами
StringResponse MakeStringResponse(http::status status, 
   std::string_view body, unsigned http_version,
   bool keep_alive,
   std::string_view content_type = ContentType::TEXT_HTML) {
    StringResponse response(status, http_version);
    response.set(http::field::content_type, content_type);
    response.body() = body;
    response.content_length(body.size());
    response.keep_alive(keep_alive);
    return response;
}
    
StringResponse MyHandleRequest(StringRequest&& req) {
    // Подставьте сюда код из синхронной версии HTTP-сервера

    std::string_view path2Map ("/api/v1/maps/");
    std::string mapIdStr = std::string( req.target().length() > path2Map.length() ? req.target().substr(path2Map.length()+1) : "" ) ;

    if ( req.method_string() == "GET" && mapIdStr.length() > 0 )
    { 
       model::Map::Id mapId {mapIdStr} ;
       const model::Map* mp = game_.FindMap(mapId);
       std::cout << mp ;
       if ( mp!= nullptr )
       {
           std::string body ( mp->GetDescr() );
           return MakeStringResponse(http::status::ok, 
	     body , 
	     req.version(), 
	     req.keep_alive(),
	     ContentType::TEXT_Json
	     );
       }
       return MakeStringResponse(http::status::ok, 
	    std::string("Error. No map [") + mapIdStr +"]", 
	    req.version(), 
	    req.keep_alive()
	    );
    }    
    return MakeStringResponse(http::status::ok, 
	    std::string("Error") , 
	    req.version(), 
	    req.keep_alive()
	    );
}

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send) {
        // Обработать запрос request и отправить ответ, используя send
        std::cout << "new Req\n" ;
        send( MyHandleRequest( std::forward<decltype(req)>(req) ) ) ;
    }

private:
    model::Game& game_;
};

}  // namespace http_handler
