#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QUrl url("http://192.168.205.131:5000/math");
    QUrlQuery query;
    query.addQueryItem("num1", "11");
    query.addQueryItem("num2", "125");
    url.setQuery(query);
    qDebug() << url;

    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonObj = jsonDoc.object();

            QString name = jsonObj.value("sum").toString();
            QString code = jsonObj.value("multiply").toString();

            qDebug() << "sum:" << name;
            qDebug() << "multiply:" << code;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        a.quit();
    });

    return a.exec();
}
