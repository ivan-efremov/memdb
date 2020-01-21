# memdb
Database in memory

For build, cd this project and run shell command $ make all
For clean binary files run shell command $ make clean

Easy create database in memory, for example,
1. Create table person:

    PDatabase db = std::make_shared<Database>("data_base"); ///< Create DB
    PTable table = db->createTable("test_table");  ///< Create table
    table->addColumn("id");       ///< Add column id
    table->addColumn("name");     ///< Add column name
    table->addColumn("phone");    ///< Add column phone
    table->addColumn("address");  ///< Add column address

2. Create index by name and phone:

    table->createIndex("name");
    table->createIndex("phone");

3. Add records to table:

    PRecord record = std::make_shared<Record>();
    record->emplace_back(std::make_shared<Data>(id));
    record->emplace_back(std::make_shared<Data>("taxi"));
    record->emplace_back(std::make_shared<Data>("+74999999999"));
    record->emplace_back(std::make_shared<Data>("Moscow"));
    table->insertRecord(record);

4. Fast searching by index:

    table->findByIndex("phone", "+74999999999", [&inum](PRecord record, size_t icolumn) {
      PData data = record->at(icolumn);
      inum = data->m_data.m_i32;
    });
