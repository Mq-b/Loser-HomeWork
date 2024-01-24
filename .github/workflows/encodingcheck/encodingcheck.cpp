/// \file encodingcheck.cpp
/// \brief File encoding check

// 白酱让我用 C++ 写一个，替换掉原来的 Rust 版本

#include <xdgmime.h>

#include <filesystem>
#include <iostream>
#include <algorithm>
#include <ranges>

namespace fs = std::filesystem;

int main()
{
    auto workdir = fs::current_path();

    for (auto it = fs::recursive_directory_iterator { workdir }; it != fs::recursive_directory_iterator{}; ++it) {
        const auto& entry = *it;
        if (std::ranges::find(entry.path(), fs::path { ".git" }) != entry.path().end()) {
            it.disable_recursion_pending();
            continue;
        }
        struct stat statbuf { };
        if (stat(entry.path().native(), &statbuf) != 0) {
            throw fs::filesystem_error("stat()", std::error_code{errno, std::system_category()});
        }
        auto mime = xdg_mime_get_mime_type_for_file(entry.path().native(), &statbuf);
        std::cout << mime << '\n';
    }
}
