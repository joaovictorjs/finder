# 🔍 Finder
An easy-to-use C++ library for finding files in the system

## 📖 Summary
- [Example](#-example)
- [Installation](#%EF%B8%8F-installation)
- [Testing](#-testing)
- [Dependencies](#-dependencies)

## 💡 Example
__Purpose:__ write a software that finds all `.conf` files under `/home` directory

- File: main.cpp

```cpp
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
```

- File: meson.build

```cpp
executable(
  'example', 
  sources: ['main.cpp'], 
  link_with: static,
  include_directories: include,
  dependencies: dependency('wildest-card')
)
```

## 🛠️ Installation

- Getting the library from source:

```bash
git clone https://github.com/joaovictorjs/finder
```

- Setup and compile (meson required):

```bash
cd finder &&
meson setup --prefix=/usr --buildtype=release --wipe build &&
meson compile -C build
```

- Finally, install the library:

``` bash
meson install -C build
```

## 🧪 Testing
```bash
meson test -C build
```

## 📚 Dependencies

- [wildest-card](https://github.com/QtRoS/wildest-card)
