#include "ThreadFuncs.hpp"

void count_lines(std::string &buf, unsigned int *th_all_lines,
                 unsigned int *th_blank_lines, unsigned int *th_comment_lines,
                 unsigned int *th_code_lines, bool *is_long_comment)
{
    (*th_all_lines)++;
    if ("" == buf) // if blank line
    {
        (*th_blank_lines)++;
        return;
    }
    if (*is_long_comment)
    {
        (*th_comment_lines)++;
        if (buf.find("*/") != std::string::npos) // if it is end
                                                 // of long comment
            *is_long_comment = false;
    }
    else
    {
        if (buf.find("/*") != std::string::npos) // if start of
                                                 // long comment
        {
            *is_long_comment = true;
            (*th_comment_lines)++;
            return;
        }
        if (buf.find("//") != std::string::npos) // if it is short comment
            (*th_comment_lines)++;
        else
            (*th_code_lines)++;
    }
}