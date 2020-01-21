# memdb
Database in memory
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

3. Fast searching by index:
