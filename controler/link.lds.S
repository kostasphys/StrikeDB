SECTIONS
{
    . = 0x10000;
    __start_test = .;
    .sdb.test : {*(.sdb.test)};
    __end_test;
}