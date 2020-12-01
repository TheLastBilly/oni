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

    if(path.size() < 1)
        return;
    
    struct stat file_stat = {};
    int err = stat(path.c_str(), &file_stat);

    if(err != 0)
        throw std::runtime_error("cannot read \"" + path + "\"");
    
    is_dir = S_ISDIR(file_stat.st_mode);

    size = file_stat.st_size * !is_dir;

    if(path.size() > 0 && is_dir && path[path.size() -1] != path_div_character)
        path += "/";
}

void fs::directory_t::update()
{
    if(path.size() < 1)
        return;
    
    this->path_t::update();
    fs::directory_t dir = scan_directory(path);
    files = dir.files;
    directories = dir.directories;
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
    while(path.size() > 0 && path[path.size()-1] == path_div_character)
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

fs::paths_t fs::get_subpaths(path_t path)
{
    path.update();
    if(!path.is_directory())
        throw std::runtime_error("\"" + path.get_path() + "\" is not a directory");

    DIR *path_dir = opendir(path.get_path().c_str());
    if(path_dir == NULL)
        throw std::runtime_error("cannot read \"" + path.get_path() + "\"");

    struct dirent *entry;
    paths_t paths;

    while(entry = readdir(path_dir))
    {
        path_t p = path.get_path() + std::string(entry->d_name);
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

fs::directory_t fs::scan_directory(path_t path)
{
    directory_t dir(path);

    if(dir.get_relative_name() == ".." || dir.get_relative_name() == ".")
        return dir;

    fs::paths_t paths = fs::get_subpaths(path);

    for(fs::path_t cp : paths)
    {
        if(!cp.is_directory())
        {
            dir.append_file(cp);
            continue;
        }

        dir.append_directory(scan_directory(cp.get_path()));
    }

    return dir;
}