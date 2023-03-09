// Этот файл служит для подключения реализации библиотеки Boost.Json
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <iterator>
#include <fstream>
#include <typeinfo>
#include <deque>

#include "model.h"

namespace json = boost::json;

namespace model
{
class RoadJson{
public:    
    int X0;
    int Y0;
    int X1=-1;
    int Y1=-1;
    RoadJson(boost::json::value js)  {
        const auto& obj = js.as_object();
        X0 = (int)js.at("x0").as_int64() ;
        Y0 = (int)js.at("y0").as_int64() ;
        if (obj.contains("x1"))
         X1 = (int)obj.at("x1").as_int64() ;
        if (obj.contains("y1"))
         Y1 = (int)obj.at("y1").as_int64() ;
    }
} ;

struct BuildingJson
{
     int X;int Y;int W;int H;
     BuildingJson(boost::json::value js)  {
         X = (int)js.at("x").as_int64() ;
         Y = (int)js.at("y").as_int64() ;
         W = (int)js.at("w").as_int64() ;
         H = (int)js.at("h").as_int64() ;
     }
} ;


struct OfficesJson
{
     std::string id ;int x;int y;int offsetX; int offsetY;
     OfficesJson(boost::json::value js)  {
         id = value_to<std::string>( js.at("id") ) ;
         x = (int)js.at("x").as_int64() ;
         y = (int)js.at("y").as_int64() ;
         offsetX = (int)js.at("offsetX").as_int64() ;
         offsetY = (int)js.at("offsetY").as_int64() ;
     }
} ;

class MapJson {
public:
    typedef std::deque<RoadJson> Roads;
    typedef std::deque<BuildingJson> Buildings;
    typedef std::deque<OfficesJson> Offices;
    std::string id;
    std::string name;
    Roads roads ;
    Buildings buildings ;
    Offices offices;
    friend MapJson tag_invoke(json::value_to_tag<MapJson>, json::value const& v) {
        auto& o = v.as_object();
        auto attt = o.at("roads").as_array() ;
        Roads rds ;
        for (auto &js : attt )
        {
            RoadJson rd ( js ) ;
            rds.push_back(rd) ;
        }
        auto buildingsJson = o.at("buildings").as_array();
        Buildings blds ;
        for ( auto &js : buildingsJson)
        {
           BuildingJson bld (js) ;
           blds.push_back(bld) ;
        }
        Offices ofs ;
        auto officesJson = o.at("offices").as_array();
        for ( auto &js : officesJson)
        {
           OfficesJson of (js) ;
           ofs.push_back(of) ;
        }
        return {value_to<std::string>(o.at("id")),
            value_to<std::string>(o.at("name")),
            rds,
            blds,
            ofs
        };
    }
};

    void model::Game::fill(std::string   input) 
    {
       auto mapsJson = json::parse(input).at("maps");
       using MapJsons = std::deque<MapJson>;   
       MapJsons recs  = value_to<std::deque<MapJson>>( mapsJson );
       for (MapJson & mj : recs )
       {
         Map::Id id {mj.id} ;
         Map m ( id , mj.name ) ;
         for (auto& rdjson : mj.roads)
         {
           Point pnt ={ rdjson.X0, rdjson.Y0 } ;
           if ( rdjson.Y1 >=0 ) 
           {
              Road rd ( Road::VERTICAL , pnt , rdjson.Y1 );
              m.AddRoad(rd) ;
           }
           else if ( rdjson.X1 >=0 ) 
           {
              Road rd ( Road::HORIZONTAL , pnt , rdjson.X1 );
              m.AddRoad(rd) ;
           }
         }
         for (auto& bldjson : mj.buildings)
         {
             Rectangle rc = { {bldjson.X,bldjson.Y} , {bldjson.W,bldjson.H}} ;
             Building bl (rc) ;
             m.AddBuilding( bl ) ;
         }
         
         for (auto& ofjson : mj.offices)
         {
             Offset offs = { ofjson.offsetX,ofjson.offsetY } ;
             Office of ( Office::Id{ofjson.id} , {ofjson.x,ofjson.y} , offs  ) ;
             m.AddOffice( of ) ;
         }
         AddMap (m);
       }
   }
}
