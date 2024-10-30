#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SIDEWAYS "Sideways ROM"
#define LANGUAGE "Language ROM"
#define LANGUAGE_RELOC "Language ROM relocation address"

#define SIDEWAYS_LOAD_ADDRESS 0xFFFF8000
#define LANGUAGE_LOAD_ADDRESS 0x00008000

int read_rom_header(char *filename)   {
    FILE *fd = fopen(filename, "rb");
    if(!fd)
        return -1;
    uint8_t jump_instr;
    fread((void *) &jump_instr, 1, 1, fd);
    uint16_t jump_entry;
    fread((void *) &jump_entry, 1, 2, fd);
    char instruction[20];
    if(jump_instr == 0x6C)
        printf("Code entry point: indirect \"JMP (&%04X)\"\n", jump_entry);
    else if(jump_instr == 0x4C)
        printf("Code entry point: absolute \"JMP &%04X\"\n", jump_entry);
    else
        printf("Code entry point: 0x%04X%02X\n", jump_entry, jump_instr);
    fread((void *) &jump_instr, 1, 1, fd);
    uint16_t jump_service;
    fread((void *) &jump_service, 1, 2, fd);
    if(jump_instr == 0x6C)
        printf("Service entry point: indirect \"JMP (&%04X)\"\n", jump_service);
    else if(jump_instr == 0x4C)
        printf("Service entry point: absolute \"JMP &%04X\"\n", jump_service);
    else
        printf("Code entry point: 0x%04X%02X\n", jump_service, jump_instr);
    uint8_t rom_type;
    fread((void *) &rom_type, 1, 1, fd);
    char type[50] = SIDEWAYS;
    uint32_t load_address = SIDEWAYS_LOAD_ADDRESS;
    if(rom_type & 0x40) {
        memcpy(type, LANGUAGE, sizeof(LANGUAGE));
        load_address = LANGUAGE_LOAD_ADDRESS;
    } else if(rom_type & 0x20)    {
        memcpy(type, LANGUAGE_RELOC, sizeof(LANGUAGE_RELOC));
        load_address = 0;
    }
    printf("Type: %s\n", type);
    if(load_address)
        printf("Load address: 0x%08X\n", load_address);
    uint8_t offset;
    fread((void *) &offset, 1, 1, fd);
    offset++;
    uint8_t bin_ver;
    fread((void *)&bin_ver, 1, 1, fd);
    printf("Binary software version: %02X\n", bin_ver);
    char str_buff[100];
    fgets(str_buff, 90, fd);
    printf("Title: \"%s\"\n", str_buff);
    long delta = ftell(fd) - (90 - strlen(str_buff));
    if(offset - delta > 2)  {
        fseek(fd, delta + 2, SEEK_SET);
        char str_software_version[100];
        fgets(str_software_version, 90, fd);
        printf("Software version: \"%s\"\n", str_software_version);
    }
    fseek(fd, offset, SEEK_SET);
    char str_buff_copyright[100];
    fgets(str_buff_copyright, 90, fd);
    printf("Copyright: \"%s\"\n", str_buff_copyright);
    if(!load_address)    {
        delta = strlen(str_buff_copyright) + offset;
        fseek(fd, delta + 1, SEEK_SET);
        uint32_t address;
        fread((void *) &address, 1, 4, fd);
        printf("Relocation load address: 0x%08X\n", address);
    }
    fclose(fd);
    return 0;
}

int main(int argc, char *argv[])  {
    if(argc < 2)    {
        fprintf(stderr, "ERROR: argument list empty.\n");
        return -1;
    }

    int pret = 0;
    int i;
    for(i = 1; i < argc; i++)   {
        printf("------------------------------------------------\n");
        printf("File: %s\n", argv[i]);
        int ret = read_rom_header(argv[i]);
        if(ret) {
            fprintf(stderr, "ERROR: coundn't open the file.\n");
            pret = -1;
        }
    }
    printf("------------------------------------------------\n");
    return pret;
}
