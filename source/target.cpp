#include "target.hpp"

namespace oni
{
    std::string target_t::build_json()
    {
        return json11::Json(*this).dump();
    }

    void target_t::update()
    {
        this->root.update();
        this->data_path.update();
        this->config_path.update();
        this->saves_path.update();
    }

    json11::Json target_t::to_json() const
    {
        return json11::Json::object{
            {"root", root},
            {"data_path", data_path},
            {"config_path", config_path},
            {"saves_path", saves_path}
        };
    }
}