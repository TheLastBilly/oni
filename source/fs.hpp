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
            path_t(): path_t(""){}
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

            virtual void update();
            void update(std::string path)
            {change_path(path); update();}

            bool is_updated() {return updated;}

            json11::Json to_json() const {return json11::Json::object{{"size", static_cast<double>(size)}, {"path", path}};}

        protected:
            std::string path = "";
            std::string name = "";
            std::string extension = "";
            size_t size = 0;
            
            bool updated = false;
            bool is_dir = false;
        };

        class directory_t : public path_t
        {
        public:
            directory_t(){}
            directory_t(std::string path):path_t(path){}
            directory_t(fs::path_t path):path_t(path){}
            directory_t(const char * path):directory_t(std::string(path)){}

            std::string build_json()
            {return json11::Json(*this).dump();}

            std::vector<path_t> get_files() {return files;}
            void append_file(path_t path) {files.push_back(path);}

            std::vector<directory_t> get_directory() {return directories;}
            void append_directory(directory_t directory) {directories.push_back(directory);}

            void update() override;

        private:
            json11::Json to_json() const {return json11::Json::object{{"directories", directories}, {"files", files}, {"directory", path}};}
            friend json11::Json;

            std::vector<path_t> files;
            std::vector<directory_t> directories;
        };

        typedef std::vector<path_t> paths_t;
        
        static paths_t get_subpaths(path_t path);

        static std::string get_extension(std::string path);
        static std::string get_relative_name(std::string path);
        static std::string add_missing_directory_divider(std::string path);
        
        static directory_t scan_directory(path_t path);

        static void mkdir(path_t path);
        static void remove(path_t path);
        static void cp(path_t source, path_t target);

    private:
        static const char path_div_character;
        static const char ext_character;
    };
}