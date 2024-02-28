/// \file encodingcheck.cpp
/// \brief File encoding check

// 白酱让我用 C++ 写一个，替换掉原来的 Rust 版本

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <is_utf8.h>
#include <format>
#include <unistd.h>
#include <xdgmime.h>

#include <filesystem>
#include <iostream>
#include <algorithm>
#include <ranges>

template<class... Args>
constexpr void print(std::format_string<Args...> fmt, Args&&... args)
{
    std::fputs(std::format(fmt, std::forward<decltype(args)>(args)...).c_str(), stdout);
}

template<class... Args>
constexpr void printerr(std::format_string<Args...> fmt, Args&&... args)
{
    std::fputs(std::format(fmt, std::forward<decltype(args)>(args)...).c_str(), stderr);
}

namespace fs = std::filesystem;

bool checkutf8(fs::path file)
{
    auto fd = open(file.c_str(), O_RDONLY);
    if (fd < 0) {
        throw fs::filesystem_error("open()", std::error_code{errno, std::system_category()});
    }
    struct stat statbuf { };
    if (fstat(fd, &statbuf) != 0) {
        close(fd);
        throw fs::filesystem_error("fstat()", std::error_code{errno, std::system_category()});
    }
    auto ptr = static_cast<char*>(mmap(nullptr, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (ptr == MAP_FAILED) {
        throw fs::filesystem_error("mmap()", std::error_code{errno, std::system_category()});
    }
    bool result = is_utf8(ptr, statbuf.st_size);
    munmap(ptr, statbuf.st_size);
    close(fd);
    return result;
}

int main()
{
    auto workdir = fs::current_path();
    print("Working under {}\n", workdir.c_str());

    size_t success {};
    size_t failure {};

    for (auto it = fs::recursive_directory_iterator { workdir }; it != fs::recursive_directory_iterator{}; ++it) {
        const auto& entry = *it;
        if (std::ranges::find(entry.path(), fs::path { ".git" }) != entry.path().end()) {
            // print("Skipping {}\n", entry.path().c_str());
            it.disable_recursion_pending();
            continue;
        }
        if (!entry.is_regular_file()) {
            continue;
        }
        struct stat statbuf { };
        if (stat(entry.path().c_str(), &statbuf) != 0) {
            throw fs::filesystem_error("stat()", std::error_code{errno, std::system_category()});
        }
        auto mime = xdg_mime_get_mime_type_for_file(entry.path().c_str(), &statbuf);
        if (std::string_view { mime }.rfind("text", 0) != 0) {
            // print("Skipping {} ({})\n", entry.path().c_str(), mime);
            continue;
        }

        try {
            bool result = checkutf8(entry.path());
            if (result) {
                print("CHECKED {} ({})\n", entry.path().c_str(), mime);
                ++success;
            } else {
                printerr("FAILURE {} ({})\n", entry.path().c_str(), mime);
                ++failure;
            }
        }
        catch(const std::exception& err){
            printerr("ERROR {} ({})\n", entry.path().c_str(), mime);
            printerr("{}", err.what());
        }
    }

    print("Checked {} files, {} failures.\n", success + failure, failure);
    if (failure > 0) {
        return 1;
    }
    return 0;
}
