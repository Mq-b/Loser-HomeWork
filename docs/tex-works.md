# Work with TeX

Loser-Homework 用 TeX 格式编写了部分文档。

学会与 TeX 共事是很重要的技能，TeX 文档是 Loser-Homework 的重要组成部分，学会使用 TeX 并参与贡献能够为建设卢瑟帝国做出伟大的贡献。

## Standards

Loser-Homework 的 TeX 采用 LaTeX 变体，ctex 宏包提供中文排版支持。

使用 xelatex 编译器前端是最标准的做法，但理论上也可以使用 pdflatex 等前端编译文档。

文档使用了一些额外的宏包，这些包在 TeXLive 中的名称存储在 `/.github/tl_packages` 中。

Loser-Homework 建议使用 latexmk 自动化构建流程。

此外，要编译整个 Loser-Homework 的文档，可以很方便地使用 CMake 来完成。

## CMake & TeX

Loser-Homework 的 CMake 首要为 CI 服务，其次也为贡献者和用户提供便利。

为了提供对 TeX 的 CI，Loser-Homework 采用 CMake 以方便地编译 TeX 文档并导出结果。

对 TeX 提供支持的 CMake 文件使用 [FindLatexmk](https://github.com/kprussing/FindLatexmk)。

要启用 Loser-Homework 的 CMake 对 TeX 的支持，请在 configure 时添加 `-DBUILD_DOCS=true` 命令行参数。

CMake 会要求 Latexmk 和 XeTeX。请确保它们能被 CMake 找到。

## Getting started

见 [#270](https://github.com/Mq-b/Loser-HomeWork/discussions/270)。
