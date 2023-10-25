#include <system_error>
#include <iostream>
#include <vector>
#include  <string>

#include <finder.hpp>

bool on_error_callback(const std::string& a_filepath, const std::system_error& a_error){
    std::cout<<"[ ERROR ] "<<a_filepath<<' '<<a_error.what()<<'\n';
    return true;
}

int main() {
    Finder::Filter filter;
    std::vector<std::string> files_found;

    filter
        .set_root_dir("/home")
        .set_whitelist({"*.conf"})
        .search_recursive(files_found, on_error_callback);

    for(const auto& f : files_found){
        std::cout<<f<<'\n';
    }

    std::cout<<files_found.size()<<std::endl;

    return 0;
}
