#include "DirectHandle.hpp"

#include <dirent.h> // for showind directories
#include "List.hpp" // for List

void dir_handle(const char *path, List<std::string> *list)
{
    DIR *dir = opendir(path);
    if (!dir)
        return;
    struct dirent *directory = 0;
    while ((directory = readdir(dir)))
    {
        if (!strcmp(directory->d_name, ".") ||
            !strcmp(directory->d_name, ".."))
            continue;
        std::string local_path(path);
        local_path += directory->d_name;
        // check if our files are .cpp .c .h .hpp
        if (local_path.find(".hpp", local_path.length() - 5) !=
                std::string::npos ||
            local_path.find(".cpp", local_path.length() - 5) !=
                std::string::npos ||
            local_path.find(".h", local_path.length() - 3) !=
                std::string::npos ||
            local_path.find(".c", local_path.length() - 3) !=
                std::string::npos)
        {
            list->Push(&local_path);
        }
        else
        {
            local_path += '/';
            dir_handle(local_path.c_str(), list);
        }
    }
    if (directory)
        free(directory);
    closedir(dir);
}
