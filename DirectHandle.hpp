#ifndef DIRECTHANDLE_HPP_SENTRY
#define DIRECTHANDLE_HPP_SENTRY

#include "List.hpp" // for List
#include <string>   // for std::string

void dir_handle(const char *path, List<std::string> *list);

#endif