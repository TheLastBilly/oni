#pragma once

#include "fs.hpp"
#include "json11.hpp"

namespace oni
{
    class package
    {
    public:
        struct directory_t
        {
            directory_t(std::string path):path(path){}
            fs::path_t path;

            std::vector<fs::path_t> files;
            std::vector<directory_t> directories;

            json11::Json to_json() const {return json11::Json::object{{"directories", directories}, {"files", files}, {"directory", path}};}
        };

        static directory_t scan_directory(std::string path);
        static json11::Json build_json_from_directory(directory_t directory);
    };
}