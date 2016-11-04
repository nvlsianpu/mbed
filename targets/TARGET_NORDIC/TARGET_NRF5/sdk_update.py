import os, shutil, json, pprint, sys, string
from collections import OrderedDict



def rename_sdk_old_dirs(path, dry_run = False):
    # I make assumption that all old sdk dirs have "sdk" names.
    sdk_dir_name = "sdk"
    path = "."
    
    for root, dirs, files in os.walk(path):
        for name in dirs:
            if name == "sdk":
                full_path = root + "\\" + name
                new_full_path = root + "\\_old_" + name
                print("rename " + full_path + " ---> " + new_full_path)
                if not dry_run:
                    os.rename(full_path, new_full_path)
                    os.mkdir(full_path)
            
 
def get_file_pathes_couples(path_sdk_componets, skip_dirs = [], skip_files = []):
    mbed_list = []
    for root, dirs, files in os.walk(path_sdk_componets):
        procced = True
        cutted_root = root[len(path_sdk_componets):]
        
        for skip_d in skip_dirs:
            if 0 == string.find(cutted_root, skip_d):
                print(cutted_root)
                procced = False
         
        if procced:
            for file_name in files:
            
                procced = True
                for skip_f in skip_files:
                    if (-1) != string.find(file_name, skip_f):
                        print(file_name)
                        procced = False
                        
                if procced:
                    if file_name.endswith((".c", ".h")):
                        #cutted_path = cutted_root + "\\" + file_name
                        cutted_path = os.path.join(cutted_root, file_name)
                        #full_path = root + "\\" + file_name
                        full_path = os.path.join(root, file_name)
                        item = {"full_paty": full_path, "id": cutted_path, "cutted_root": cutted_root}
                        #mbed_list.append([full_path, cutted_path])
                        mbed_list.append(item)

    # for k in mbed_list:
        # print(k);
        
    # print(len(mbed_list))
    
    return mbed_list
    
def apply_replacement_id(mbed_list, replacemet_couples):
    for item in mbed_list:
        splited = os.path.split(item["id"])
        result = string.find(splited[1], replacemet_couples["old"])
        if result != -1:
            new_tail = replacemet_couples["new"] + splited[1][len(replacemet_couples["old"]):]
            item["id"] = os.path.join(splited[0],new_tail)
            #print('bingo!')
            #print(item)
        
    return mbed_list
            


id_replacements = {"old": "nrf_ble", "new": "ble"}

force_copy_dirs_list = [{"sdk_dir":"device", "mbed_dir": "sdk\\device"},
                        {"sdk_dir":"ble\\ble_dtm", "mbed_dir": "sdk\\ble\\ble_dtm"}]
force_copy_files_list = [{"sdk_file":"sdk_validation.h", "mbed_dir": "sdk"}]


 
#rename_sdk_old_dirs(os.getcwd(), False)
list_sdk = get_file_pathes_couples("C:\\nRF5_SDK_12.1.0\\components\\",
    ["ant", "nfc", "proprietary_rf", "serialization", "toolchain", "ble\\ble_services", "drivers_ext", "softdevice\\s212", "softdevice\\s332", "ble\\nrf_ble_qwr", "libraries\\bootloader"],
    ["dox_config.h", "ble_error_log.c", "ble_conn_params.c"])
list_mbed1 = get_file_pathes_couples("C:\\mbed\\mbed-os\\targets\\TARGET_NORDIC\\TARGET_NRF5\\_old_sdk\\")
list_mbed2 = get_file_pathes_couples("C:\\mbed\\mbed-os\\targets\\TARGET_NORDIC\\TARGET_NRF5\\TARGET_MCU_NRF52832\\_old_sdk\\")
list_mbed3 = get_file_pathes_couples("C:\\mbed\\mbed-os\\targets\\TARGET_NORDIC\\TARGET_NRF5\\TARGET_MCU_NRF51822_UNIFIED\\_old_sdk\\")

list_mbed = list_mbed1 + list_mbed2 + list_mbed3

list_mbed = apply_replacement_id(list_mbed, id_replacements)

print("\r\n")

licz = 0
for pathes_mbed in list_mbed:
    empty = True
    for pathes_sdk in list_sdk:
        if pathes_mbed["id"] == pathes_sdk["id"]:
            #print(pathes_mbed["id"])
            #print(pathes_mbed["full_paty"]+" "+pathes_sdk["full_paty"] + "\r\n")
            licz = licz + 1
            empty = False
            
    if empty:
        print("!!!" + pathes_mbed["full_paty"] + "\r\n")
    
print(licz)
print(len(list_mbed))
print(len(list_sdk))

#for pathes_sdk in list_sdk:


for pathes_sdk in list_sdk:
    empty = True
    for pathes_mbed in list_mbed:
        if pathes_mbed["id"] == pathes_sdk["id"]:
            #print(pathes_mbed["id"])
            #print(pathes_mbed["full_paty"]+" "+pathes_sdk["full_paty"] + "\r\n")
            licz = licz + 1
            empty = False
            
    if empty:
        
        make_hard_copy = False
        
        for hard_copy_dir in force_copy_dirs_list:
            if pathes_sdk["cutted_root"] == hard_copy_dir["sdk_dir"]:
                make_hard_copy = True
                print("**twarda_kopia_katalogu**")
                break
        
        if not make_hard_copy:
            for hard_copy_file in force_copy_files_list:
                #print(pathes_sdk["id"])
                if pathes_sdk["id"] == hard_copy_file["sdk_file"]:
                    make_hard_copy = True
                    print("**twarda_kopia_pliku**")
                    break
                
                
        print("!!!" + pathes_sdk["full_paty"] + "\r\n")
