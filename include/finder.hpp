#pragma once 

#include <filesystem>
#include <string>
#include <vector>

namespace Finder {
    namespace fs = std::filesystem;
    using str_vec = std::vector<std::string>;

    class Filter {
        private:
            std::string m_root_dir;
            str_vec m_whitelist, m_blacklist;

        protected:

        public:
            Filter& set_root_dir(const std::string& a_dir);
            Filter& set_whitelist(const str_vec& a_whitelist);
            Filter& set_blacklist(const str_vec& a_blacklist);
    };
}
