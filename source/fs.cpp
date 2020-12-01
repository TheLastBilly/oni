#include "fs.hpp"

#include <sys/stat.h>

using namespace oni;

const char fs::path_div_character = '/';
const char fs::ext_character = '.';

fs::path_t::path_t(const fs::path_t &path)
{
    this->path = path.path;
    this->name = path.name;
    this->extension = path.extension;
    this->size = path.size;
    this->type = path.type;
    this->updated = path.updated;
}

void fs::path_t::change_path(std::string path)
{
    this->path = path;
    name = fs::get_relative_name(path);
    extension = fs::get_extension(path);
}

void fs::path_t::update()
{
    updated = false;

    struct stat file_stat = {};
    int err = stat(path.c_str(), &file_stat);

    if(err != 0)
        throw std::runtime_error("cannot read \"" + path + "\"");
    
    if(S_ISDIR(file_stat.st_mode))
        type = types::directory;
    else
        type = types::file;
}

std::string fs::get_extension(std::string path)
{
    size_t div_index;
    if((div_index = path.rfind(path_div_character)) == std::string::npos)
        div_index = 0;
    
    size_t ext_index;
    if((ext_index = path.rfind(ext_character)) == std::string::npos)
        return "";
    return path.substr(ext_index+1);
}

std::string fs::get_relative_name(std::string path)
{
    size_t div_index;
    if((div_index = path.rfind(path_div_character)) == std::string::npos)
        return path;
    return path.substr(div_index+1);
}