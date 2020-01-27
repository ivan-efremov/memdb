#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <malloc.h>
#include "db.h"

struct timeval t1, t2, res;
struct timespec cpu_t1, cpu_t2;

static void printMallocStats()
{
  struct mallinfo minfo = mallinfo();
  std::cout << "Non-mmapped space allocated:        " << minfo.arena    << "\n"
            << "Number of free chunks:              " << minfo.ordblks  << "\n"
            << "Number of free fastbin blocks:      " << minfo.smblks   << "\n"
            << "Number of mmapped regions:          " << minfo.hblks    << "\n"
            << "Space allocated in mmapped regions: " << minfo.hblkhd   << "\n"
            << "Maximum total allocated space:      " << minfo.usmblks  << "\n"
            << "Space in freed fastbin blocks:      " << minfo.fsmblks  << "\n"
            << "Total allocated space:              " << minfo.uordblks << "\n"
            << "Total free space:                   " << minfo.fordblks << "\n"
            << "Top-most, releasable space:         " << minfo.keepcost << "\n"
            << std::endl;
}

static void init()
{
  #define _1GB (static_cast<long unsigned int>(1e+9))
  #define _2GB (2L * _1GB)
  #define _3GB (3L * _1GB)
  #define _4GB (4L * _1GB)
  const struct rlimit limit = { _4GB, _4GB };
  setrlimit(RLIMIT_AS, &limit);
}


int main(int argc, const char* argv[])
{
  init();
  try {
    PDatabase db = std::make_shared<Database>("test.db");
    PTable table = db->createTable("test");
    table->addColumn("id");
    table->addColumn("name");
    table->createIndex("id");
    table->createIndex("name");
    // test 1
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpu_t1);
    for(size_t i=0; i<1000000UL; ++i) {
      PRecord record = std::make_shared<Record>();
      record->emplace_back(std::make_shared<Data>((int64_t)i));
      record->emplace_back(std::make_shared<Data>(std::string("Test ") + std::to_string(i)));
      table->insertRecord(record);
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpu_t2);
    TIMESPEC_TO_TIMEVAL(&t1, &cpu_t1);
    TIMESPEC_TO_TIMEVAL(&t2, &cpu_t2);
    timersub(&t2, &t1, &res);
    std::cout << "insert: " << res.tv_sec << "sec " << res.tv_usec << "usec" << std::endl;
    //printMallocStats();
    // test 2
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpu_t1);
    std::string name;
    table->findByIndex("name", Data("Test 128"), [&name](PRecord record, size_t icolumn) {
      PData data = record->at(icolumn);
      name = data->m_data.m_str;
    });
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpu_t2);
    TIMESPEC_TO_TIMEVAL(&t1, &cpu_t1);
    TIMESPEC_TO_TIMEVAL(&t2, &cpu_t2);
    timersub(&t2, &t1, &res);
    std::cout << "find: " << res.tv_sec << "sec " << res.tv_usec << "usec" << std::endl;

  } catch(const std::exception& err) {
    std::cerr << "Error: " << err.what() << std::endl;
  }
  return 0;
}
