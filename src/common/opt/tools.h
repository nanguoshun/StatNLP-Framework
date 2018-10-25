//
// Created by  ngs on 25/10/2018.
//

#ifndef STATNLP_TOOLS_H
#define STATNLP_TOOLS_H

#include <iostream>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
class CommonTool{
public:

    /**
     * print the total free memory, used memory and the application used memory.
     * @param info_str
     * @param pre_size
     * @return
     */
    inline static long PrintMemoryUsed(std::string info_str, int pre_size){
        long cur_size = PrintMemoryInfo(info_str);
        long used = pre_size - cur_size;
        std::cout << "the memory used for aforementioned step is: "<<used<<std::endl;
        return cur_size;
    }

    /**
     * print the total free memory and used memory
     * @param info_str
     * @return
     */
    inline static long PrintMemoryInfo(std::string info_str){
        vm_size_t page_size;
        mach_port_t mach_port;
        mach_msg_type_number_t count;
        vm_statistics64_data_t vm_stats;

        mach_port = mach_host_self();
        long  free_memory;
        long  used_memory;
        count = sizeof(vm_stats) / sizeof(natural_t);
        if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
            KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                              (host_info64_t)&vm_stats, &count))
        {
            free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
            used_memory = ((int64_t)vm_stats.active_count +
                                     (int64_t)vm_stats.inactive_count +
                                     (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
            free_memory = free_memory/(1024.0*1024);
            used_memory = used_memory/(1024.0*1024);
            std::cout << info_str <<", the size of free memory: "<< free_memory <<"MB,  the size of used memory: "<<used_memory<<"MB"<<std::endl;
        }
        return free_memory;
    }
};
#endif //STATNLP_TOOLS_H
