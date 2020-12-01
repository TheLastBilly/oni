#pragma once

#include <iostream>
#include <vector>

#include "json11.hpp"

namespace oni
{
    class fs
    {
    public:
        class path_t
        {
        public:
            path_t(const char * path): path_t(std::string(path)){}
            path_t(std::string path):
                updated(false)
            {change_path(path);}
            path_t(const path_t &path);

            bool is_directory() {return is_dir;}
            std::string get_path() {return path;}
            std::string get_extension() {return extension;}
            std::string get_relative_name() {return name;}

            void change_path(std::string path);

            void update();
            void update(std::string path)
            {change_path(path); update();}

            bool is_updated() {return updated;}

            json11::Json to_json() const {return json11::Json::object{{"size", static_cast<double>(size)}, {"path", path}};}
        private:
            std::string path;
            std::string name;
            std::string extension;
            size_t size;
            
            bool updated = false;
            bool is_dir = false;
        };
        
        typedef std::vector<path_t> paths_t;
        
        static paths_t get_subpaths(std::string path);
        static paths_t get_subpaths_recursively(std::string path);

        static std::string get_extension(std::string path);
        static std::string get_relative_name(std::string path);
        static std::string add_missing_directory_divider(std::string path);

    private:
        static const char path_div_character;
        static const char ext_character;
    };
}