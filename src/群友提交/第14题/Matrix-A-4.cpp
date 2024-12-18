//!msvc
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <link.h>
#include <string.h>

namespace ss {
    int a = 0;
}

uintptr_t base_address = 0;

const char* symbol_name="_ZN2ss1aE";

uintptr_t get_self_base_address() {
    dl_iterate_phdr([](struct dl_phdr_info *info, size_t size, void *data) {
        base_address = info->dlpi_addr;
        return 1; 
    }, NULL);
    return base_address;
}

// 解析 ELF 文件，查找符号内存地址
void find_symbol_in_self(const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("fopen");
        return;
    }

    // 读取 ELF Header
    Elf64_Ehdr ehdr;
    fseek(file, 0, SEEK_SET);
    fread(&ehdr, sizeof(Elf64_Ehdr), 1, file);

    // 跳转到节头表
    fseek(file, ehdr.e_shoff, SEEK_SET);

    // 读取所有节头表
    Elf64_Shdr *sh_table = (Elf64_Shdr*)malloc(ehdr.e_shentsize * ehdr.e_shnum);
    fread(sh_table, ehdr.e_shentsize, ehdr.e_shnum, file);

    // 读取节头字符串表
    Elf64_Shdr shstrtab = sh_table[ehdr.e_shstrndx];
    char *shstrtab_data = (char*)malloc(shstrtab.sh_size);
    fseek(file, shstrtab.sh_offset, SEEK_SET);
    fread(shstrtab_data, 1, shstrtab.sh_size, file);

    // 查找 .symtab 和 .strtab
    Elf64_Shdr symtab, strtab;
    int found_symtab = 0, found_strtab = 0;
    for (int i = 0; i < ehdr.e_shnum; i++) {
        const char *name = &shstrtab_data[sh_table[i].sh_name];
        if (strcmp(name, ".symtab") == 0) {
            symtab = sh_table[i];
            found_symtab = 1;
        } else if (strcmp(name, ".strtab") == 0) {
            strtab = sh_table[i];
            found_strtab = 1;
        }
    }

    if (!found_symtab || !found_strtab) {
        fprintf(stderr, "Failed to find .symtab or .strtab\n");
        free(sh_table);
        free(shstrtab_data);
        fclose(file);
        return;
    }

    // 读取符号表
    Elf64_Sym *symtab_data = (Elf64_Sym*)malloc(symtab.sh_size);
    fseek(file, symtab.sh_offset, SEEK_SET);
    fread(symtab_data, symtab.sh_entsize, symtab.sh_size / symtab.sh_entsize, file);

    // 读取字符串表
    char *strtab_data = (char*)malloc(strtab.sh_size);
    fseek(file, strtab.sh_offset, SEEK_SET);
    fread(strtab_data, 1, strtab.sh_size, file);

    // 查找目标符号
    uintptr_t target_address = 0;
    for (size_t i = 0; i < symtab.sh_size / symtab.sh_entsize; i++) {
        const char *sym_name = &strtab_data[symtab_data[i].st_name];
        if (strcmp(sym_name, symbol_name) == 0) {
            target_address = symtab_data[i].st_value;
            break;
        }
    }

    if (target_address == 0) {
        fprintf(stderr, "Symbol %s not found\n", symbol_name);
        free(sh_table);
        free(shstrtab_data);
        free(symtab_data);
        free(strtab_data);
        fclose(file);
        return;
    }

    // 计算实际内存地址
    uintptr_t base_address = get_self_base_address();
    uintptr_t actual_address = base_address + target_address;

    // 修改
    *(int*)actual_address = 100;

    // 清理资源
    free(sh_table);
    free(shstrtab_data);
    free(symtab_data);
    free(strtab_data);
    fclose(file);
}

int main(int argc, char **argv) {
    find_symbol_in_self(argv[0]);
    std::cout << ss::a << '\n'; 
}
