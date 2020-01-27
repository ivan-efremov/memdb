/**
 * @file db.cpp
 */
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <iostream>
#include "db.h"


static inline size_t str2hash(const char* a_str, size_t a_length)
{
  const char *end = a_str + a_length;
  size_t hash = 0UL;
  while(a_str != end) {
    int ch = (int)(*a_str++);
    hash = (size_t)(ch + (hash << 6) + (hash << 16) - hash);
  }
  return hash;
}

static inline size_t bin2hash(const void* a_buf, size_t a_length)
{
  const uint8_t* buf = (const uint8_t*)a_buf;
  size_t h = 0UL;
  while(a_length--) {
    h += *buf++;
    h *= 0x9e3779b1;
  }
  return h;
}

/**
 * @class Data.
 */
Data::Data():
  m_type(tNull), m_size(0UL)
{
  m_data.m_bin = NULL;
}

Data::Data(const Data& a_data):
  m_type(tNull), m_size(0UL)
{
  operator = (a_data);
}

Data::Data(Data&& a_data):
  m_type(a_data.m_type), m_size(a_data.m_size)
{
  memcpy(m_data.m_bin, a_data.m_data.m_bin, a_data.m_size);
  a_data.m_type = tNull;
  a_data.m_size = 0UL;
  a_data.m_data.m_bin = NULL;
}

Data::Data(int8_t a_i8):
  m_type(tInt8), m_size(sizeof(int8_t))
{
  m_data.m_i8 = a_i8;
}

Data::Data(int16_t a_i16):
  m_type(tInt16), m_size(sizeof(int16_t))
{
  m_data.m_i16 = a_i16;
}

Data::Data(int32_t a_i32):
  m_type(tInt32), m_size(sizeof(int32_t))
{
  m_data.m_i32 = a_i32;
}

Data::Data(int64_t a_i64):
  m_type(tInt64), m_size(sizeof(int64_t))
{
  m_data.m_i64 = a_i64;
}

Data::Data(double a_real):
  m_type(tReal), m_size(sizeof(double))
{
  m_data.m_real = a_real;
}

Data::Data(const std::string& a_str):
  m_type(tString), m_size(a_str.length() + 1)
{
  m_data.m_str = new char [m_size];
  memcpy(m_data.m_str, a_str.data(), m_size);
}

Data::Data(const void* a_bin, size_t a_length):
  m_type(tBinary), m_size(a_length)
{
  m_data.m_bin = new char [m_size];
  memcpy(m_data.m_bin, a_bin, a_length);
}

Data::~Data()
{
  if(m_type == tString || m_type == tBinary) {
    delete[] m_data.m_str;
  }
}

Data& Data::operator=(const Data& a_data)
{
  if(this != &a_data) {
    if(m_type == tString || m_type == tBinary) {
      delete[] m_data.m_str;
    }
    switch(a_data.m_type) {
    case tString:
      m_type = tString;
      m_size = a_data.m_size;
      m_data.m_str = new char [a_data.m_size];
      memcpy(m_data.m_str, a_data.m_data.m_str, a_data.m_size);
      break;
    case tBinary:
      m_type = tBinary;
      m_size = a_data.m_size;
      m_data.m_bin = new char [a_data.m_size];
      memcpy(m_data.m_bin, a_data.m_data.m_bin, a_data.m_size);
      break;
    case tInt8:
      m_type = tInt8;
      m_size = a_data.m_size;
      m_data.m_i8 = a_data.m_data.m_i8;
      break;
    case tInt16:
      m_type = tInt16;
      m_size = a_data.m_size;
      m_data.m_i16 = a_data.m_data.m_i16;
      break;
    case tInt32:
      m_type = tInt32;
      m_size = a_data.m_size;
      m_data.m_i32 = a_data.m_data.m_i32;
      break;
    case tInt64:
      m_type = tInt64;
      m_size = a_data.m_size;
      m_data.m_i64 = a_data.m_data.m_i64;
      break;
    case tReal:
      m_type = tReal;
      m_size = a_data.m_size;
      m_data.m_real = a_data.m_data.m_real;
      break;
    case tNull:
      m_type = tNull;
      m_size = 0UL;
      m_data.m_bin = NULL;
      break;
    default:
      throw std::runtime_error("Data::operator=(): undefined type: " + std::to_string(a_data.m_type));
    }
  }
  return *this;
}

Data& Data::operator=(Data&& a_data)
{
  if(this != &a_data) {
    if(m_type == tString || m_type == tBinary) {
      delete[] m_data.m_str;
    }
    m_type = a_data.m_type;
    m_size = a_data.m_size;
    memcpy(m_data.m_bin, a_data.m_data.m_bin, a_data.m_size);
    a_data.m_type = tNull;
    a_data.m_size = 0UL;
    a_data.m_data.m_bin = NULL;
  }
  return *this;
}

size_t Data::hash() const
{
  switch(m_type) {
  case tString:
    return str2hash(m_data.m_str, m_size);
  case tInt8:
    return bin2hash(&m_data.m_i8, m_size);
  case tInt16:
    return bin2hash(&m_data.m_i16, m_size);
  case tInt32:
    return bin2hash(&m_data.m_i32, m_size);
  case tInt64:
    return bin2hash(&m_data.m_i64, m_size);
  case tReal:
    return bin2hash(&m_data.m_real, m_size);
  case tBinary:
    return bin2hash(m_data.m_bin, m_size);
  }
  return 0UL; ///< tNull
}

std::string Data::toString() const
{
  switch(m_type) {
  case tString:
    return std::string(m_data.m_str, m_size - 1);
  case tInt8:
    return std::to_string(m_data.m_i8);
  case tInt16:
    return std::to_string(m_data.m_i16);
  case tInt32:
    return std::to_string(m_data.m_i32);
  case tInt64:
    return std::to_string(m_data.m_i64);
  case tReal:
    return std::to_string(m_data.m_real);
  case tBinary:
    {
      char           buf[4] = { 0 };
      const uint8_t* bin = (const uint8_t*)m_data.m_bin;
      const uint8_t* end = bin + m_size;
      std::string    str;
      str.reserve(2 * m_size);
      while(bin != end) {
        sprintf(buf, "%02x", *bin++);
        str += buf;
      }
      return str;
    }
  }
  return std::string();
}


/**
 * @class Table
 */
Table::Table(const std::string& a_name):
  m_name(a_name)
{
}

void Table::addColumn(const std::string& a_name)
{
  m_columns.emplace_back(a_name);
  for(auto &record : m_records) {
    record->emplace_back(std::make_shared<Data>());
  }
}

void Table::insertRecord(PRecord a_record)
{
  const size_t columns = m_columns.size();
  if(a_record->size() != columns) {
    a_record->resize(columns, std::make_shared<Data>());
  }
  m_records.emplace_back(a_record);
  const size_t irec = m_records.size() - 1;
  for(auto &it : m_indexes) {
    PIndex index = it.second;
    PData data = a_record->at(index->m_column);
    const size_t hash = data->hash();
    auto idx = index->m_index.find(hash);
    if(idx != index->m_index.end()) {
      Index::PListRecordIds listRecIds(idx->second);
      listRecIds->emplace_back(irec);
    } else {
      Index::PListRecordIds listRecIds(
        std::make_shared<Index::ListRecordIds>()
      );
      listRecIds->emplace_back(irec);
      index->m_index.emplace(hash, listRecIds);
    }
  }
}

ssize_t Table::getColumnNumber(const std::string& a_columnName) const
{
  auto itColumn = std::find(m_columns.begin(), m_columns.end(), a_columnName);
  if(itColumn == m_columns.end()) {
    return -1L;
  }
  return itColumn - m_columns.begin();
}

std::string Table::getColumnName(size_t a_number) const
{
  return m_columns.at(a_number);
}

void Table::createIndex(const std::string& a_columnName)
{
  const ssize_t column = getColumnNumber(a_columnName);
  if(column == -1L) {
    throw std::runtime_error("column not find");
  }
  PIndex index = std::make_shared<Index>();
  index->m_column = (size_t)column;
  if(!m_indexes.emplace(a_columnName, index).second) {
    throw std::runtime_error("index already exist");
  }
  for(auto it = m_records.begin(); it != m_records.end(); ++it) {
    PData data = (*it)->at(column);
    const size_t irec = it - m_records.begin();
    const size_t hash = data->hash();
    auto idx = index->m_index.find(hash);
    if(idx != index->m_index.end()) {
      Index::PListRecordIds listRecIds(idx->second);
      listRecIds->emplace_back(irec);
    } else {
      Index::PListRecordIds listRecIds(
        std::make_shared<Index::ListRecordIds>()
      );
      listRecIds->emplace_back(irec);
      index->m_index.emplace(hash, listRecIds);
    }
  }
}

void Table::findByIndex(
  const std::string& a_columnName,
  const Data& a_data,
  const std::function<void(PRecord,size_t)>& a_fn) const
{
  auto idx = m_indexes.find(a_columnName);
  if(idx != m_indexes.end()) {
    PIndex index = idx->second;
    auto it = index->m_index.find(a_data.hash());
    if(it != index->m_index.end()) {
      Index::PListRecordIds recIds = it->second;
      for(auto irec=recIds->begin(); irec!=recIds->end(); ++irec) {
        PRecord record = m_records.at(*irec);
        a_fn(record, index->m_column);
      }
    }
  }
}

void Table::deleteRecord(size_t a_irecord)
{
  m_records.erase(m_records.begin() + a_irecord);
}

Table::Iterator Table::begin()
{
  return m_records.begin();
}

Table::Iterator Table::end()
{
  return m_records.end();
}

void Table::print() const
{
  for(auto& column : m_columns) {
    std::cout << column << '\t';
  }
  std::cout << std::endl;
  for(auto &record : m_records) {
    for(auto &data : *record) {
      std::cout << data->toString() << '\t';
    }
    std::cout << std::endl;
  }
}


/**
 * @class Database.
 */
Database::Database(const std::string& a_name):
  m_name(a_name)
{
}

PTable Database::createTable(const std::string& a_name)
{
  m_tables.emplace_back(std::make_shared<Table>(a_name));
  return m_tables.back();
}
