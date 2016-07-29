#!python2
import os
import sys
import re
import argparse

RAM_MEM_END =   0x20010000

# The value of the GATTS Attribute Table size for which the app_ram_base_size.h was generated
BLE_GATTS_ATTR_TAB_SIZE_DEFAULT = 0x580
BLE_GATTS_ATTR_TAB_SIZE_QUANT   = 4

# The alignment of RAM used by the SoftDevice
SOFTDEVICE_RAM_ALIGNMENT = 8

# Additional amount of ram needed for vendor specific 128-bit uuid table.
UUID_VENDOR_TABLE_ADDITION = (3 * 16)


LD_SCRIPT = "/* Linker script to configure memory regions. */\n" +\
            "\n" +\
            "\n" +\
            "MEMORY\n" +\
            "{{\n" +\
            "  FLASH (rx) : ORIGIN = 0x1C000, LENGTH = 0x64000\n" +\
            "  RAM (rwx) :  ORIGIN = {0:#x}, LENGTH = {1:#x}\n" +\
            "}}\n" +\
            "\n" +\
            "INCLUDE \"NRF52832.ld\"\n"


def get_ram_property(in_memo_offset, in_mem_end = RAM_MEM_END, in_attr_table_size = None, ld_script = None):
    if (in_attr_table_size != None):
        delta = in_attr_table_size - BLE_GATTS_ATTR_TAB_SIZE_DEFAULT

        if (delta != 0):
            if ((delta % BLE_GATTS_ATTR_TAB_SIZE_QUANT) != 0):
                delta = delta + (delta % BLE_GATTS_ATTR_TAB_SIZE_QUANT) #force the use of quantum of ATTR_TABLE_SIZE
                
            # Customization add SOFTDEVICE_RAM_ALIGNMENT for safety. Equation is simplified - this additional part of RAM may be unused.
            in_memo_offset = in_memo_offset + SOFTDEVICE_RAM_ALIGNMENT + delta
        
    in_memo_offset = in_memo_offset + UUID_VENDOR_TABLE_ADDITION
    mem_remain = in_mem_end - in_memo_offset
    
    if (ld_script != None):
        return ld_script.format (in_memo_offset,mem_remain, in_mem_end - 1)
    else:
        return LD_SCRIPT.format (in_memo_offset,mem_remain)
    

def create_a_linker_file(in_ld_item, dest_path, in_attr_table_size = None, ld_script = None, r = None):
    mem_start = int(in_ld_item['mem_start'], 16)
    ld_file_str = get_ram_property(mem_start, in_mem_end = r, in_attr_table_size = in_attr_table_size, ld_script = ld_script)
    myfile_name = dest_path
    
    
    with open(myfile_name , "w") as myfile:
        myfile.write(ld_file_str)
        print("Output scatter file: " + myfile_name)


def create_a_linker_list(file_name):
    with open(file_name , "r") as myfile:
        file_str = myfile.read()
        pat_reg  = r"#define APP_RAM_BASE_CENTRAL_LINKS_([0-9])_PERIPH_LINKS_([0-9])_SEC_COUNT_([0-9])_MID_BW 0x([0-9a-fA-F]{8})"
        list_of_link_defines = re.findall(pat_reg, file_str)
        return list_of_link_defines
    
    
def create_linker_files(file_name, dest_path = ".", c = 0, p = 1, in_attr_table_size = None, file_script = None, r = None):
    ld_script = None
    if (file_script != None):
        with open(file_script , "r") as myfile:
            ld_script = myfile.read()
        
    ld_list = create_a_linker_list(file_name)
    for ld in ld_list:
        if (c != None) and (int(ld[0]) != c):
            continue
            
        if (p != None) and (int(ld[1]) != p):
            continue

        if (ld[2] == '1') or ((ld[2] == '0') and (ld[0] == '0')):
            ld_item = {'central_links' : ld[0],
               'peripheral_links' : ld[1],
               'mem_start' : ld[3]}
            create_a_linker_file(ld_item, dest_path, in_attr_table_size, ld_script, r)
            break
    
    
if __name__ == '__main__':
    argument_parser = argparse.ArgumentParser()
    argument_parser.add_argument('-f', '--file', help='Specify a C header file name to input.', required=True)
    argument_parser.add_argument('-d', '--destination', help='Specify the destination file path. The destination directory must exist.', default="scater_file.txt")
    argument_parser.add_argument('-c', '--central', help='Number of supported BLE central connections. If empty, 0 will be used.', type=int, default=None)
    argument_parser.add_argument('-p', '--peripheral', help='Number of supported BLE peripheral connections. If empty, 1 will be used.', type=int, default=None)
    argument_parser.add_argument('-a', '--attr_table_size', help='The GATTS attribute table size. If empty, the default value will be used.', type=str, default=None)
    argument_parser.add_argument('-s', '--scater_file_pattern', help='Specify a file name of a pattern of scater file format.', default=None)
    argument_parser.add_argument('-r', '--ram_end', help='End of RAM addresses, e.g. for nRF52 0x20010000, ', type=str, default=None)

    parser_args = vars(argument_parser.parse_args())
    
    #parser_args['file'] is a required argument
    path = parser_args['file']
    print("Input C header: {0}".format(path))
    
    dest_path = parser_args['destination']

    c = parser_args['central']

    p = parser_args['peripheral']
    
    
    try:
        a = int(parser_args['attr_table_size'],0)
    except TypeError:
        a = None
        
    try:
        r = int(parser_args['ram_end'],0)
    except TypeError:
        r = RAM_MEM_END
        

    path_pattern = parser_args['scater_file_pattern']

    create_linker_files(path,dest_path, c, p, a, path_pattern, r)
    
    
    
