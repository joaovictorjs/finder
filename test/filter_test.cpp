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

        bool filter(const std::string& a_path){
            return Finder::Filter::filter(a_path);
        }
};

TEST(FilterHelper, is_in_whitelist){
    FilterHelper filter;

    ASSERT_TRUE(filter.is_in_whitelist("/home"));

    filter.set_whitelist({"/home/Do*"});
    ASSERT_TRUE(filter.is_in_whitelist("/home/Downloads"));
    ASSERT_TRUE(filter.is_in_whitelist("/home/Documents"));
    ASSERT_FALSE(filter.is_in_whitelist("/home/Dump"));

    filter.set_whitelist({"/home/Do*/*.mp?"});
    ASSERT_TRUE(filter.is_in_whitelist("/home/Downloads/anime.mp4"));
    ASSERT_TRUE(filter.is_in_whitelist("/home/Downloads/anime.mp3"));
    ASSERT_TRUE(filter.is_in_whitelist("/home/Documents/anime.mp4"));
    ASSERT_TRUE(filter.is_in_whitelist("/home/Documents/anime.mp3"));
    ASSERT_FALSE(filter.is_in_whitelist("/home/Dump/anime.mp4"));
    ASSERT_FALSE(filter.is_in_whitelist("/home/Dump/anime.mp3"));
}

TEST(FilterHelper, is_in_blacklist){
    FilterHelper filter;

    ASSERT_FALSE(filter.is_in_blacklist("/home"));

    filter.set_blacklist({"/home/Do*"});
    ASSERT_TRUE(filter.is_in_blacklist("/home/Downloads"));
    ASSERT_TRUE(filter.is_in_blacklist("/home/Documents"));
    ASSERT_FALSE(filter.is_in_blacklist("/home/Dump"));

    filter.set_blacklist({"/home/Do*/*.mp?"});
    ASSERT_TRUE(filter.is_in_blacklist("/home/Downloads/anime.mp4"));
    ASSERT_TRUE(filter.is_in_blacklist("/home/Downloads/anime.mp3"));
    ASSERT_TRUE(filter.is_in_blacklist("/home/Documents/anime.mp4"));
    ASSERT_TRUE(filter.is_in_blacklist("/home/Documents/anime.mp3"));
    ASSERT_FALSE(filter.is_in_blacklist("/home/Dump/anime.mp4"));
    ASSERT_FALSE(filter.is_in_blacklist("/home/Dump/anime.mp3"));
}

TEST(FilterHelper, filter){
    FilterHelper fh;

    ASSERT_TRUE(fh.filter("/home"));

    fh.set_whitelist({"/home/Do*/*.mp?", "*.exe"}).set_blacklist({"*.exe", "*windows*"});
    ASSERT_TRUE(fh.filter("/home/Downloads/anime.mp4"));
    ASSERT_TRUE(fh.filter("/home/Downloads/anime.mp3"));
    ASSERT_TRUE(fh.filter("/home/Documents/anime.mp4"));
    ASSERT_TRUE(fh.filter("/home/Documents/anime.mp3"));
    ASSERT_FALSE(fh.filter("/home/Downloads/malware.exe"));
    ASSERT_FALSE(fh.filter("/home/Documents/malware.exe"));
    ASSERT_FALSE(fh.filter("/home/Downloads/windows.mp3"));
    ASSERT_FALSE(fh.filter("/home/Documents/windows.mp4"));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
