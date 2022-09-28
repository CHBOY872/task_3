#include <iostream>       // for std
#include <thread>         // for threads
#include <string.h>       // for strstr
#include <sys/time.h>     // for timeval
#include <mutex>          // for mutexes
#include <string>         // for std::string
#include <fstream>        // for std::ifstream, std::ofstream
#include <errno.h>        // for errno
#include "ThreadList.hpp" // ThreadList ThreadListCursor ThreadList::Iterator
#include "List.hpp"       // for List

#include "ThreadFuncs.hpp"
#include "DirectHandle.hpp"

enum
{
#ifndef MAXTHREADS
#define MAXTHREADS 4
    max_threads = MAXTHREADS,
#else
    max_threads = MAXTHREADS
#endif
};

static unsigned int all_lines = 0;     // all lines count
static unsigned int blank_lines = 0;   // blank lines count
static unsigned int code_lines = 0;    // code lines count
static unsigned int comment_lines = 0; // comment lines count

static std::mutex m_all_lines;     // mutex for all lines
static std::mutex m_blank_lines;   // mutex for blank lines
static std::mutex m_code_lines;    // mutex for code lines
static std::mutex m_comment_lines; // mutes for comment lines

void count_function(List<std::string> *files_list)
{
    std::string buf;                   // buffer where our line
                                       // will be written
    unsigned int th_all_lines = 0;     // local var for all lines
    unsigned int th_blank_lines = 0;   // local var for blank lines
    unsigned int th_code_lines = 0;    // local var for code lines
    unsigned int th_comment_lines = 0; // loval var for comment lines
    bool is_long_comment = false;      // bool flag if there is a long comment

    List<std::string>::Iterator *it = files_list->Iterate();
    while (it->More())
    {
        ListCursor<std::string> cursor = it->Next();
        std::ifstream *stream = new std::ifstream(cursor->c_str(), std::ios::in);

        for (; std::getline(*stream, buf);) // while is not an end of line
            count_lines(buf, &th_all_lines, &th_blank_lines,
                        &th_comment_lines, &th_code_lines, &is_long_comment);
        stream->close();
        delete stream;
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

int main(int argc, const char **argv)
{
    struct timeval start; // value of time on start
    struct timeval end;   // value of time on end

    gettimeofday(&start, 0); // get current time on start

    if (argc != 3)
    {
        std::cout << "Usage: <direction path> <file to write res>\n";
        return 1;
    }

    std::string path(argv[1]);
    if (path.back() != '/') // check if argument was written
        path += '/';
    List<std::string> file_list;
    dir_handle(path.c_str(), &file_list);

    List<std::string> lists_for_threads[max_threads];

    List<std::string>::Iterator *it = file_list.Iterate();
    int i;
    while (it->More())
    {
        for (i = 0; i < max_threads; i++)
        {
            lists_for_threads[i].Push(it->Next());
            if (!it->More())
                break;
        }
    }
    delete it;

    ThreadList threads;
    for (i = 0; i < max_threads; i++)
        threads.Push(count_function, &lists_for_threads[i]);

    ThreadList::Iterator *thread_it = threads.Iterate();
    while (thread_it->More())
        thread_it->Next()->join();
    delete thread_it;

    gettimeofday(&end, 0); // get current day on end

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

    f << "count C/C++ files: " << file_list.GetCount() << std::endl;
    f << "time of excecution: " << time_res_s << "."
      << time_res_us << "sec" << std::endl;
    f << "all lines: " << all_lines << std::endl;
    f << "blank lines: " << blank_lines << std::endl;
    f << "code lines " << code_lines << std::endl;
    f << "comment lines " << comment_lines << std::endl;
    f.close();
    return 0;
}