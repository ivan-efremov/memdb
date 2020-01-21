/**
 * @file db.h
 */
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>


enum DataTypes {
  tNull,
  tInt8,
  tInt16,
  tInt32,
  tInt64,
  tReal,
  tString,
  tBinary
};

struct Data {
  int   m_type;       // тип данных
  int   m_size;       // размер в байтах
  union {
    int8_t    m_i8;   // 1-байтовое целое
    int16_t   m_i16;  // 2-байтовое целое
    int32_t   m_i32;  // 4-байтовое целое
    int64_t   m_i64;  // 8-байтовое целое
    double    m_real; // вещественное число
    char*     m_str;  // строка
    void*     m_bin;  // бинарные данные
  } m_data;
              Data();
              Data(const Data& a_data);
              Data(Data&& a_data);
              Data(int8_t  a_i8);
              Data(int16_t a_i16);
              Data(int32_t a_i32);
              Data(int64_t a_i64);
              Data(double  a_real);
              Data(const std::string& a_str);
              Data(const void* a_bin, size_t a_length);
             ~Data();
  Data&       operator=(const Data& a_data);
  Data&       operator=(Data&& a_data);
  size_t      hash() const;
  std::string toString() const;
};

typedef std::shared_ptr<Data>  PData;


struct Index {
  typedef std::deque<size_t>              ListRecordIds;
  typedef std::shared_ptr<ListRecordIds>  PListRecordIds;
  size_t                            m_column;
  std::map<size_t, PListRecordIds>  m_index;
};

typedef std::shared_ptr<Index>  PIndex;


typedef std::vector<PData>       Record;
typedef std::shared_ptr<Record>  PRecord;


struct Table {
  typedef std::deque<PRecord>::iterator Iterator;
  std::string                   m_name;
  std::vector<std::string>      m_columns;
  std::deque<PRecord>           m_records;
  std::map<std::string, PIndex> m_indexes;
              Table(const std::string& a_name);
  void        addColumn(const std::string& a_name);
  void        insertRecord(PRecord a_record);
  void        updateRecord();
  void        deleteRecord(size_t a_irecord);
  void        createIndex(const std::string& a_columnName);
  void        print() const;
  ssize_t     getColumnNumber(const std::string& a_columnName) const;
  std::string getColumnName(size_t a_number) const;
  Iterator    begin();
  Iterator    end();
};

typedef std::shared_ptr<Table>  PTable;


struct Database {
  std::string        m_name;
  std::list<PTable>  m_tables;
              Database(const std::string& a_name);
  PTable      createTable(const std::string& a_name);
};

typedef std::shared_ptr<Database>  PDatabase;
