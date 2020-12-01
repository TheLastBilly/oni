#pragma once

#include "fs.hpp"

namespace oni
{
    class target_t
    {
    public:
        target_t(fs::directory_t root, fs::directory_t data_path = "", fs::directory_t config_path = "", fs::directory_t saves_path = ""):
            root(root),
            data_path(data_path),
            config_path(config_path),
            saves_path(saves_path)
        {}

        std::string build_json();

        void set_root_path(fs::directory_t path) {root = path;}
        void set_config_path(fs::directory_t path) {config_path = path;}
        void set_saves_path(fs::directory_t path) {saves_path = path;}
        void set_data_path(fs::directory_t path) {data_path = path;}

        fs::directory_t get_root_path() {return root;}
        fs::directory_t get_config_path() {return config_path;}
        fs::directory_t get_saves_path() {return saves_path;}
        fs::directory_t get_data_path() {return data_path;}

        void update();

    private:
        fs::directory_t root;
        fs::path_t data_path;
        fs::path_t config_path;
        fs::path_t saves_path;

        json11::Json to_json() const;
        friend json11::Json;

    };
}