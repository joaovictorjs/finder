#include "finder.hpp"
#include <iostream>

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

    bool Filter::filter(const std::string& a_path){
        return !this->is_in_blacklist(a_path) && this->is_in_whitelist(a_path);
    }

    Filter& Filter::search(str_vec& a_found, const error_callback& a_error_callback){
        fs::directory_iterator iter;
        std::error_code ec;

        try{
            iter = fs::directory_iterator(this->m_root_dir);
        }catch(std::system_error& error){
            a_error_callback(this->m_root_dir, error);
            return *this;
        }

        while(iter != fs::directory_iterator()){
            fs::path filepath {(*iter).path()};
            iter.increment(ec);

            if(!fs::is_symlink(filepath) && !fs::is_directory(filepath) && this->filter(filepath) && !ec) {
                a_found.push_back(filepath);
            }

            if(ec){
                if(!a_error_callback(filepath, ec)) break;
            }
        }

        return *this;
    }


    Filter& Filter::search_recursive(str_vec& a_found, const error_callback& a_error_callback){
        fs::recursive_directory_iterator iter, restore;
        std::error_code ec;

        try{
            iter = fs::recursive_directory_iterator(this->m_root_dir);
        }catch(std::system_error& error){
            a_error_callback(this->m_root_dir, error);
            return *this;
        }

        while(iter != fs::recursive_directory_iterator()){
            restore = iter;
            fs::path filepath {(*iter).path()};
            iter.increment(ec);

            if(!fs::is_symlink(filepath) && !fs::is_directory(filepath) && this->filter(filepath) && !ec) {
                a_found.push_back(filepath);
            }

            if(ec){ 
                if(!a_error_callback(filepath, ec)) break;
                iter = restore;
                iter.disable_recursion_pending();
                iter.increment(ec);

                if(ec){
                    if(!a_error_callback(filepath, ec)) break;
                    iter = restore;
                    iter.pop(ec);

                    if(ec) break;
                }
            }
        }

        return *this;
    }
}
