#include "packages.hpp"

using namespace oni;

package::directory_t package::scan_directory(std::string path)
{
    directory_t dir(path);

    dir.path.update();
    if(!dir.path.is_directory())
        throw std::runtime_error("\"" + path + "\" is not a directory");

    if(dir.path.get_relative_name() == ".." || dir.path.get_relative_name() == ".")
        return dir;

    fs::paths_t paths = fs::get_subpaths(path);

    for(fs::path_t cp : paths)
    {
        if(!cp.is_directory())
        {
            dir.files.push_back(cp);
            continue;
        }

        dir.directories.push_back(scan_directory(cp.get_path()));
    }

    return dir;
}