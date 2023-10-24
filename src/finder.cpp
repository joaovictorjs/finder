#include "finder.hpp"

namespace Finder {
    Filter& Filter::set_root_dir(const std::string& a_dir){
        this->m_root_dir = a_dir;
        return *this;
    }

    Filter& Filter::set_whitelist(const str_vec& a_whitelist){
        this->m_whitelist = a_whitelist;
        return *this;
    }

    Filter& Filter::set_blacklist(const str_vec& a_blacklist){
        this->m_blacklist = a_blacklist;
        return *this;
    }

}
