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

    bool Filter::is_in_whitelist(const std::string& a_path){
        if(this->m_whitelist.empty()) return true;

        for(const auto& value : this->m_whitelist){
            if(wildcard(value.c_str(), a_path.c_str())) return true;
        }

        return false;
    }

    bool Filter::is_in_blacklist(const std::string& a_path){
        if(this->m_blacklist.empty()) return false;

        for(const auto& value : this->m_blacklist){
            if(wildcard(value.c_str(), a_path.c_str())) return true;
        }

        return false;
    }

}
