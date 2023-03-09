#include "json_loader.h"
#include <iostream>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <typeinfo>
#include <deque>

//namespace json = boost::json;
using namespace std::literals;

namespace json_loader {




model::Game LoadGame(const std::filesystem::path& json_path) {
    // Загрузить содержимое файла json_path, например, в виде строки
    // Распарсить строку как JSON, используя boost::json::parse
    // Загрузить модель игры из файла
    model::Game game;
    std::cout << "Begin 2\n" ;
    std::ifstream ifs(json_path);
    std::string   input(std::istreambuf_iterator<char>(ifs), {});
    game.fill(input) ; 

    return game;
}

}  // namespace json_loader
