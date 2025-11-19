qt_add_executable(tst_tcpserverhandler
    tst_tcpserverhandler.cpp
    ../filepathmanager.h
    ../filepathmanager.cpp
    ../tcpserverhandler.h
    ../tcpserverhandler.cpp
)

add_test(NAME tst_tcpserverhandler COMMAND tst_tcpserverhandler)

target_link_libraries(tst_tcpserverhandler PRIVATE Qt6::Network Qt::Test)
