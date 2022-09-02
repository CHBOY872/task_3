#ifndef THREADFUNCS_HPP_SENTRY
#define THREADFUNCS_HPP_SENTRY

#include <string>

void count_lines(std::string &buf, unsigned int *th_all_lines,
                 unsigned int *th_blank_lines, unsigned int *th_comment_lines,
                 unsigned int *th_code_lines, bool *is_long_comment);

#endif