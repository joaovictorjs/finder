#include <gtest/gtest.h>

#include "finder.hpp"

namespace fs = std::filesystem;
using str_vec = std::vector<std::string>;

class FilterHelper : public Finder::Filter {
    public:
        FilterHelper& set_root_dir(const std::string& a_dir){
            Finder::Filter::set_root_dir(a_dir);
            return *this;
        }

        FilterHelper& set_whitelist(const str_vec& a_whitelist){
            Finder::Filter::set_whitelist(a_whitelist);
            return *this;
        }

        FilterHelper& set_blacklist(const str_vec& a_blacklist){
            Finder::Filter::set_blacklist(a_blacklist);
            return *this;
        }

        bool is_in_whitelist(const std::string& a_path){
            return Finder::Filter::is_in_whitelist(a_path);
        }

        bool is_in_blacklist(const std::string& a_path){
            return Finder::Filter::is_in_blacklist(a_path);
        }
};

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
