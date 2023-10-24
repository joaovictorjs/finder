#pragma once 

#include <filesystem>
#include <string>
#include <vector>
#include <system_error>
#include <functional>

#include <wildcard.h>

namespace Finder {
    namespace fs = std::filesystem;
    using str_vec = std::vector<std::string>;
    using error_callback = std::function<bool(const std::string&, const std::system_error&)>;

    class Filter {
        private:
            std::string m_root_dir;
            str_vec m_whitelist, m_blacklist;

        protected:
            bool is_in_whitelist(const std::string& a_path);
            bool is_in_blacklist(const std::string& a_path);
            bool filter(const std::string& a_path);

        public:
            Filter& set_root_dir(const std::string& a_dir);
            Filter& set_whitelist(const str_vec& a_whitelist);
            Filter& set_blacklist(const str_vec& a_blacklist);
    };
}
