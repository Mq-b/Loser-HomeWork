cmake_minimum_required(VERSION 3.17)

project(LHWLatex)

message(CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH})
find_package(Latexmk REQUIRED)
set(Latexmk_FLAGS ${Latexmk_FLAGS} -shell-escape -synctex=1)
add_latex_document(现代C++题目.tex ALL ENGINE XELATEX)
add_latex_document(现代C++题目（答案与解析）.tex ALL ENGINE XELATEX)
