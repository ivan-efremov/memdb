#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <sys/time.h>
#include "db.h"

struct timeval t1, t2, res;

int main(int argc, const char* argv[])
{
  try {
    PDatabase db = std::make_shared<Database>("data_base");
    PTable table = db->createTable("test_table");
    table->addColumn("id");
    table->addColumn("host");
    table->addColumn("ip");
    table->addColumn("num");
    table->createIndex("id");
    table->createIndex("num");
    gettimeofday(&t1, NULL);
    for(size_t i=0; i<10000; ++i) {
      PRecord record = std::make_shared<Record>();
      record->emplace_back(std::make_shared<Data>((int64_t)i));
      record->emplace_back(std::make_shared<Data>("myhost"));
      record->emplace_back(std::make_shared<Data>("192.168.1.1"));
      record->emplace_back(std::make_shared<Data>((int32_t)(random())));
      table->insertRecord(record);
    }
    gettimeofday(&t2, NULL);
    timersub(&t2, &t1, &res);
    std::cout << "insert: " << res.tv_sec << "." << res.tv_usec << std::endl;

    gettimeofday(&t1, NULL);
    const int32_t fnum = (int32_t)random();
    int32_t inum = -1;
    table->findByIndex("num", fnum, [&inum](PRecord record, size_t icolumn) {
      PData data = record->at(icolumn);
      inum = data->m_data.m_i32;
    });
    gettimeofday(&t2, NULL);
    timersub(&t2, &t1, &res);
    std::cout << "find: " << fnum << " -> " << inum << " " << res.tv_sec << "." << res.tv_usec << std::endl;
    std::cout << "hash: " << Data(fnum).hash() << " " << Data(inum).hash() << std::endl;

  } catch(const std::exception& err) {
    std::cerr << "Error: " << err.what() << std::endl;
  }
  return 0;
}
