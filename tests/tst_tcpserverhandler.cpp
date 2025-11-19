#include <QTest>
#include <QSignalSpy>

#include "../tcpserverhandler.h"

class TestTcpServerHandler: public QObject
{
    Q_OBJECT

private slots:
    void serverStart();
    void serverStop();
};

void TestTcpServerHandler::serverStart()
{
    TcpServerHandler handler;
    QSignalSpy errorSpy(&handler, &TcpServerHandler::errorOccurred);

    handler.startServer();
    QVERIFY(handler.isRunning());

    handler.startServer();
    QVERIFY(handler.isRunning());
    QVERIFY(errorSpy.count() == 1);
    QVERIFY(errorSpy.at(0).at(0).toString() == "Сервер уже запущен");


    TcpServerHandler handler2;
    QSignalSpy errorSpy2(&handler2, &TcpServerHandler::errorOccurred);

    handler2.startServer();
    QVERIFY(!handler2.isRunning());
    QVERIFY(errorSpy2.count() == 1);
    QVERIFY(errorSpy2.at(0).at(0).toString() == "Не удалось запустить сервер: The bound address is already in use");
}

void TestTcpServerHandler::serverStop()
{
    TcpServerHandler handler;
    QSignalSpy errorSpy(&handler, &TcpServerHandler::errorOccurred);

    handler.startServer();
    QVERIFY(handler.isRunning());
    QVERIFY(!handler.serverAddress().isEmpty());

    handler.stopServer();
    QVERIFY(!handler.isRunning());
    QVERIFY(handler.serverAddress().isEmpty());
    QVERIFY(errorSpy.count() == 0);
}

QTEST_MAIN(TestTcpServerHandler)
#include "tst_tcpserverhandler.moc"
