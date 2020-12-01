#include "fs.hpp"

#include <sys/stat.h>
#include <dirent.h>

using namespace oni;

const char fs::path_div_character = '/';
const char fs::ext_character = '.';

fs::path_t::path_t(const fs::path_t &path)
{
    this->path = path.path;
    this->name = path.name;
    this->extension = path.extension;
    this->size = path.size;
    this->updated = path.updated;
    this->is_dir = path.is_dir;
}

void fs::path_t::change_path(std::string path)
{
    updated = false;
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
    
    is_dir = S_ISDIR(file_stat.st_mode);

    if(is_dir)
        path += path_div_character;

    size = file_stat.st_size;
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
    if(path.size() > 0 && path[path.size()-1] == path_div_character)
        path.pop_back();

    size_t div_index;
    if((div_index = path.rfind(path_div_character)) == std::string::npos)
        return path;
    return path.substr(div_index+1);
}

std::string fs::add_missing_directory_divider(std::string path)
{
    if(path.size() > 0 && path[path.size()-1] != path_div_character)
        path += path_div_character;
    return path;
}

fs::paths_t fs::get_subpaths(std::string path)
{
    fs::path_t p = path;
    p.update();
    if(!p.is_directory())
        throw std::runtime_error("\"" + path + "\" is not a directory");

    DIR *path_dir = opendir(path.c_str());
    if(path_dir == NULL)
        throw std::runtime_error("cannot read \"" + path + "\"");

    struct dirent *entry;
    paths_t paths;
    
    path = add_missing_directory_divider(path);

    while(entry = readdir(path_dir))
    {
        path_t p = path + std::string(entry->d_name);
        try
        {
            p.update();
        }
        catch(std::runtime_error e)
        {
            continue;
        }
        paths.push_back(p);
    }

    closedir(path_dir);
    return paths;
}

fs::paths_t fs::get_subpaths_recursively(std::string path)
{   
    fs::path_t p = path;
    p.update();
    if(!p.is_directory())
        throw std::runtime_error("\"" + path + "\" is not a directory");

    if(p.get_relative_name() == ".." || p.get_relative_name() == ".")
        return {};

    fs::paths_t paths = get_subpaths(path);
    fs::paths_t new_paths = {};

    for(path_t cp : paths)
    {
        if(!cp.is_directory() || cp.get_relative_name() == "..")
            continue;
        fs::paths_t np = get_subpaths_recursively(cp.get_path());

        for(path_t &c : np)
            new_paths.push_back(c);
    }

    for(path_t &c : new_paths)
            paths.push_back(c);

    return paths;
}