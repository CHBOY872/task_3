#include <iostream>       // for std
#include <thread>         // for threads
#include <dirent.h>       // for showind directories
#include <string.h>       // for strstr
#include <sys/time.h>     // for timeval
#include <mutex>          // for mutexes
#include <string>         // for std::string
#include <fstream>        // for std::ifstream, std::ofstream
#include <errno.h>        // for errno
#include "ThreadList.hpp" // ThreadList ThreadListCursor ThreadList::Iterator

static unsigned int all_lines = 0;      // all lines count
static unsigned int blank_lines = 0;    // blank lines count
static unsigned int code_lines = 0;     // code lines count
static unsigned int comment_lines = 0;  // comment lines count

std::mutex m_all_lines;                 // mutex for all lines
std::mutex m_blank_lines;               // mutex for blank lines
std::mutex m_code_lines;                // mutex for code lines
std::mutex m_comment_lines;             // mutes for comment lines

void function(std::ifstream *stream)
{
    std::string buf;                    // buffer where our line
                                        // will be written
    unsigned int th_all_lines = 0;      // local var for all lines
    unsigned int th_blank_lines = 0;    // local var for blank lines
    unsigned int th_code_lines = 0;     // local var for code lines
    unsigned int th_comment_lines = 0;  // loval var for comment lines
    bool is_long_comment = false;       // bool flag if there is a long comment
    for (; std::getline(*stream, buf);) // while is not an end of line
    {
        th_all_lines++;
        if ("" == buf)                  // if blank line
        {
            th_blank_lines++;
            continue;
        }
        if (is_long_comment)
        {
            th_comment_lines++;
            if (buf.find("*/") != std::string::npos) // if it is end
                                                     // of long comment
                is_long_comment = false;
        }
        else
        {
            if (buf.find("/*") != std::string::npos) // if start of
                                                     // long comment
            {
                is_long_comment = true;
                th_comment_lines++;
                continue;
            }
            if (buf.find("//") != std::string::npos) // if it is short comment
                th_comment_lines++;
            else
                th_code_lines++;
        }
    }

    /* write results to our variables */
    m_all_lines.lock();
    all_lines += th_all_lines;
    m_all_lines.unlock();

    m_blank_lines.lock();
    blank_lines += th_blank_lines;
    m_blank_lines.unlock();

    m_code_lines.lock();
    code_lines += th_code_lines;
    m_code_lines.unlock();

    m_comment_lines.lock();
    comment_lines += th_comment_lines;
    m_comment_lines.unlock();
}

void dir_handle(const char *path, ThreadList *list)
{
    if (strstr(path, "/../") || strstr(path, "/./"))
        return;
    DIR *dir = opendir(path);
    if (!dir)
        return;
    struct dirent *directory;
    while ((directory = readdir(dir)))
    {
        if (strstr(directory->d_name, ".cpp") ||        // check if our files
            strstr(directory->d_name, ".hpp") ||        // are .cpp .c .h .hpp
            strstr(directory->d_name, ".c") ||
            strstr(directory->d_name, ".h"))
        {
            std::string p(path);
            p += std::string(directory->d_name);
            std::ifstream *s = new std::
                ifstream(p, std::ios::in);
            if ((*s).is_open())                     // if file was opened
                list->Push(function, s);            // put our threads to list
            else
            {
                delete s;
                std::cerr << strerror(errno) << std::endl; // print error
                return;
            }
        }
        else
        {
            std::string new_path(path);
            new_path += directory->d_name;
            new_path += '/';
            dir_handle(new_path.c_str(), list);
        }
    }
    free(directory);
    closedir(dir);
}

int main(int argc, const char **argv)
{
    struct timeval start;                           // value of time on start
    struct timeval end;                             // value of time on end

    gettimeofday(&start, 0);                        // get current time on start

    if (argc != 3)
    {
        std::cout << "Usage: <direction path> <file to write res>\n";
        return 1;
    }

    std::string path(argv[1]);
    if (path.back() != '/')                     // check if argument was written
    {                                           // like a directory
        std::cout << "Path should end by " << '/' << std::endl;
        return 2;
    }

    ThreadList list;                                // create list of threads

    dir_handle(argv[1], &list);

    ThreadList::Iterator *iter = list.Iterate();    // create the iterator
                                                    // of thread list
    while (iter->More())
        (iter->Next())->join();                     // run our threads
    delete iter;

    gettimeofday(&end, 0);                          // get current day on end

    time_t time_res_s = end.tv_sec - start.tv_sec;
    useconds_t time_res_us = end.tv_usec - start.tv_usec;

    std::ofstream f(argv[2], std::ios::out | std::ios::trunc);
                                                    // open or create file
                                                    // in where results
                                                    // will be written
    if (!f)
    {
        perror("file:");
        return 5;
    }

    f << "count C/C++ files: " << list.GetCount() << std::endl;
    f << "time of excecution: " << time_res_s << "."
      << time_res_us << "sec" << std::endl;
    f << "all lines: " << all_lines << std::endl;
    f << "blank lines: " << blank_lines << std::endl;
    f << "code lines " << code_lines << std::endl;
    f << "comment lines " << comment_lines << std::endl;
    f.close();
    return 0;
}