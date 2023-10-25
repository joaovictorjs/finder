#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include "finder.hpp"

namespace fs = std::filesystem;
using str_vec = std::vector<std::string>;
using error_callback = std::function<bool(const std::string&, const std::system_error&)>;

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

class SearchTest : public testing::Test {
    public:
        void SetUp() override {
            str_vec folders {"Downloads", "Documents", "Pictures", "Images", "Videos"};
            str_vec files {
                "anime.mp4", 
                    "anime.mp3", 
                    "malware.exe", 
                    "sheet.xls",
                    "sheet.xlsx", 
                    "document.doc",
                    "document.docx", 
                    "document.pdf",
                    "nf.xml", 
                    "archlinux.iso"
            };

            fs::path root {"home"};
            fs::create_directory(root);

            for(const auto& folder : folders){
                fs::path dir {root / folder};
                fs::create_directory(dir);

                for(const auto& file : files){
                    fs::path filename {dir / file};
                    std::ofstream ofs {filename};
                    ofs << "dummy";
                    ofs.close();
                }
            }
        }

        void TearDown() override {
            fs::remove_all("home");
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

TEST_F(SearchTest, search){
    Finder::Filter filter;
    str_vec found {};
    int err_counter {0};
    filter.set_root_dir("./home/Downloads");

    error_callback err_cb =[&err_counter](const std::string& a_entry, const std::system_error& a_error){
        err_counter++;
        return true;
    }; 

    filter.search(found, err_cb);
    ASSERT_EQ(found.size(), 10);
    found.clear();

    filter.set_whitelist({"*.mp?", "*doc*.*", "*.exe"}).set_blacklist({"*malware*"});
    filter.search(found, err_cb);
    ASSERT_EQ(found.size(), 5);
    found.clear();

    filter.set_root_dir("/not_found");
    filter.search(found, err_cb);
    ASSERT_EQ(err_counter, 1);
}

TEST_F(SearchTest, search_recursive){
    Finder::Filter filter;
    str_vec found {};
    int err_counter {0};
    filter.set_root_dir("./home");

    error_callback err_cb =[&err_counter](const std::string& a_entry, const std::system_error& a_error){
        err_counter++;
        return true;
    }; 

    filter.search_recursive(found, err_cb);
    ASSERT_EQ(found.size(), 50);
    found.clear();

    filter.set_whitelist({"*/Do*/*.mp?", "*/Do*/*doc*.*", "*/Do*/*.exe"}).set_blacklist({"*malware*"});
    filter.search_recursive(found, err_cb);
    ASSERT_EQ(found.size(), 10);
    found.clear();

    filter.set_root_dir("/not_found");
    filter.search_recursive(found, err_cb);
    ASSERT_EQ(err_counter, 1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
