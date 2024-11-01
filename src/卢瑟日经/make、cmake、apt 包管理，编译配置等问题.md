# `make`、`cmake`、`apt` 包管理，编译配置等问题

## `make install`

`make install` 通常将我们编译好的库安装到 `/usr/local/lib` 路径下，头文件则是：`/usr/local/include`。

当然，这个路径可以根据 makefile 的编写而自定义，我们说的是通常编译库的情况。

## `apt install`

`apt install` 通常会将库安装在 `/usr/lib/x86_64-linux-gnu` 中，头文件则是：`/usr/include`。

我们可以使用命令查看其安装位置：

```shell
dpkg -L [package]
```

## `cmake --install`

`cmake --install` 类似于 `make install`，不过它是基于 cmake，而后者是基于 makefile。

也就是通常我们会：

```cmake
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```

而使用 Makefile 的流程则是：

```shell
mkdir build
cd build
cmake ..
make
make install
```

- `cmake ..` 在 Linux 平台默认会生成 Makefile，make 是基于这些 Makefile 来进行构建的

这两种方法的最终目标都是构建和安装软件，`cmake --install` 提供了一种更通用的方法，因为它不依赖于具体的构建系统（Makefile、Ninja 等），而是直接通过 CMake 的安装机制来处理。


查找安装目录。

## `which`


如果是一些程序，比如 `gcc`、`cmake`、`git` 这种，我们可以使用 `which` 命令查看，通常都是在 `/usr/bin/`：

```shell
root@Mq-B:/usr/local/include# which cmake
/usr/bin/cmake
```

## 强调

这些都是 Linux 系统的查找路径，理论上来说只需要添加链接选项，库路径和 include 路径我们都可以无需设置，直接使用库。

## CMake 的 `PackageConfig.cmake` 文件

绝大多数的 C++ 第三方库会提供 `PackageConfig.cmake` 文件（有时也称为 `<PackageName>Config.cmake`），这个文件的主要目的是简化库的查找和链接过程。使用这种文件有以下几个优点：

1. **自动查找**：当你在 `CMakeLists.txt` 中使用 `find_package(<PackageName> REQUIRED)` 时，CMake 会自动在标准路径下查找相应的配置文件。找到后，CMake 会设置相应的变量和目标，使得链接库和包含头文件变得更加简洁。

2. **简化配置**：一旦找到包，配置文件中定义的变量会提供库的路径、头文件的路径以及必要的依赖关系。你只需在项目中添加两行代码：

   ```cmake
   find_package(<PackageName> REQUIRED)
   target_link_libraries(${PROJECT_NAME} PRIVATE <PackageName>::<PackageName>)
   ```
这样就能完成库的引入，无需手动设置路径。

比如 Qt 只需要两行 CMake 就能引入：

```cmake
find_package(Qt6 REQUIRED Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
```

> 当然了，前提是你当前的设置能找到 Qt 的那些 PackageConfig.cmake 文件。 vs 的话安装 Qt 插件设置路径，就能直接如此。

## 使用 CMake 编译构建安装引入 `spdlog`

这实际很简单：

```shell
git clone https://github.com/gabime/spdlog.git
cd spdlog
mkdir build
cd build
cmake ..
cmake --build . --parallel
cmake --install .
```

> 默认构建为 Debug 产物，如果需要 Release，则在 build 改成：
>```shell
> cmake --build . --config Release --parallel
>```
> 如果是 windows，执行 install 会要求你有 Release 和 Debug 两个版本，Linux 系统无此问题。

最后的 install 的输出会显示我们的库被安装在哪，其实和 make install 没什么区别。

- 头文件：安装在 /usr/local/include/spdlog 目录下。
- 库文件：libspdlog.a 安装在 /usr/local/lib 目录。
- CMake 配置文件：安装在 /usr/local/lib/cmake/spdlog，方便其他项目通过 CMake 使用 spdlog。
- 包配置文件：spdlog.pc 安装在 /usr/local/lib/pkgconfig，供 pkg-config 使用。

而我们的要想在我们的项目引入这个库也非常的简单，只需要两行：

```cmake
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
```

另外，尤其是 windows，可能希望指明安装路径，这很简单：

```cmake
cmake .. -DCMAKE_INSTALL_PREFIX="C:\YourCustomInstallPath"
```

但是这样的话，库也就没安装在 cmake 默认会查找的路径了，如果想要设置全局，我们需要设置  **CMAKE_PREFIX_PATH**  环境变量。

cmake 就会去查找这个路径。

我们可以简单一些，直接在 cmake 文件中定义这个变量：

```cmake
set(CMAKE_PREFIX_PATH "C:/YourCustomInstallPath" CACHE STRING "自定义查找包的安装路径" FORCE)
```

然后另一个 cmake 文件包含它，如：

```cmake
include(CMakeVariables.cmake)
```

---

测试代码：

```cpp
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

inline void setupLogging() {
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs.txt");
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%@] [%!] [%O] [%l] %v");

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S] [%@] [%!] [%O] [%l] %v%$");

    auto logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{file_sink, console_sink});
    spdlog::register_logger(logger);

    spdlog::set_default_logger(logger);
}

#define LOG_INFO(msg) SPDLOG_LOGGER_INFO(spdlog::get("multi_sink"), msg)
#define LOG_WARN(msg) SPDLOG_LOGGER_WARN(spdlog::get("multi_sink"), msg)
#define LOG_ERROR(msg) SPDLOG_LOGGER_ERROR(spdlog::get("multi_sink"), msg)

int main() {
    setupLogging();
    LOG_INFO("INFO");
    LOG_WARN("WANR");
    LOG_ERROR("ERROR");
}
```

编译构建：

```shell
cmake .
cmake --build.
```

![编译输出执行](../../image/卢瑟日经/cmake_test01.png)

### 建议 windows 配置

由于 `cmake --install .` 默认放到 `C:\Program Files (x86)` 目录下，所以我们建议通常指明安装位置，定义我们的库存放的地方。

所有库都可以安装在那，也就是说我们 windows 建议 `spdlog` 的编译构建使用大概是：

```powershell
cmake .. -DCMAKE_INSTALL_PREFIX="D:\lib\spdlog"
cmake --build . --config Debug --parallel
cmake --build . --config Release --parallel
cmake --install .
```

然后 cmake 中定义一个变量即可，也就是：

```cmake
set(CMAKE_PREFIX_PATH "D:/lib" CACHE STRING "自定义查找包的安装路径" FORCE)
```

那么所有 `D:/lib` 路径下使用 cmake 管理的库，我们就都可以简单的两行代码引入了。

```cmake
find_package(SFML 2.6.1 COMPONENTS system window graphics audio network REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
```

```powershell
PS D:\lib> ls


    Directory: D:\lib


Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
d-----         2024/9/28     12:02                fmt_x64-windows
d-----         2024/9/20     18:30                SFML-2.6.1-windows-vc17-64-bit
d-----         2024/11/1     18:30                spdlog
```
