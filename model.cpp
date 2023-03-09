#include "model.h"

#include <stdexcept>

namespace model {
using namespace std::literals;

void Map::AddOffice(Office office) {
    if (warehouse_id_to_index_.contains(office.GetId())) {
        throw std::invalid_argument("Duplicate warehouse");
    }

    const size_t index = offices_.size();
    Office& o = offices_.emplace_back(std::move(office));
    try {
        warehouse_id_to_index_.emplace(o.GetId(), index);
    } catch (...) {
        // Удаляем офис из вектора, если не удалось вставить в unordered_map
        offices_.pop_back();
        throw;
    }
}

void Game::AddMap(Map map) {
    const size_t index = maps_.size();
    if (auto [it, inserted] = 
    map_id_to_index_.emplace(map.GetId(), index); !inserted) {
        throw std::invalid_argument("Map with id "s + *map.GetId() + " already exists"s);
    } else {
        try {
            maps_.emplace_back(std::move(map));
        } catch (...) {
            map_id_to_index_.erase(it);
            throw;
        }
    }
}

std::string Map::GetDescr() const
{
   std::string res ("{\n\"id\":\"" + *id_ + 
   + "\",\n\"name\":\"" + GetName() + "\",\n" 
   + "\"roads\": " 
   +GetDescrOfRoads() +",\n \"buildings\":" 
   +GetDescrOfBuildings() +",\n\"offices\":"
   +GetDescrOfOffices() + ""
   ) ;
   

   return res + "\n}" ;
}

//TODO:: wanted template for 3 function
std::string Map::GetDescrOfOffices() const
{
   std::string res ;
   bool fst = true ;
   for ( auto& of : GetOffices() )
   {
     res += "{ \"id\":\"" + *of.GetId() + "\","
     + "\"x\":"+ std::to_string(of.GetPosition().x) + ","
     + "\"y\":"+ std::to_string(of.GetPosition().y) + ","
     + "\"offsetX\":"+ std::to_string(of.GetOffset().dx) + ","
     + "\"offsetY\":"+ std::to_string(of.GetOffset().dy) 
     + "}" ;
   }
   
   return "[\n" + res + "\n]\n" ;
}

std::string Map::GetDescrOfBuildings() const
{
   std::string res ;
   bool fst = true ;
   for ( auto& bl : GetBuildings() )
   {
     res += "{ \"x\":" + std::to_string(bl.GetBounds().position.x) +","
     + "\"y\":"+ std::to_string(bl.GetBounds().position.y) +","
     + "\"w\":"+ std::to_string(bl.GetBounds().size.width) +","
     + "\"h\":"+ std::to_string(bl.GetBounds().size.height) 
     + "}" ;
   }
   
   return "[\n" + res + "\n]\n" ;
}

std::string Map::GetDescrOfRoads() const
{
   std::string res ;
   bool fst = true ;
   for ( auto& rd : GetRoads() )
   {
      if ( !fst )
        res += ",\n" ;
      res += rd.GetDescr() ;
      fst = false ;
   }
   return "[\n" + res + "\n]\n" ;
}

std::string Road::GetDescr() const
{
  return "{ \"x0\" :" + std::to_string(GetStart().x) 
  + ",\"y0\":" + std::to_string(GetStart().y) 
  + GetEndDescr()   + "}" ;
}

std::string Road::GetEndDescr() const
{
   return IsHorizontal()
   ? 
   ", \"x1\":" + std::to_string(GetEnd().x) 
   : 
   ", \"y1\":" + std::to_string(GetEnd().y) ;
} 

}  // namespace model
