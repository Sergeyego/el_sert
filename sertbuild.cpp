#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    current_id_part=-1;
    current_id_ship=-1;
    sertType=0;
    lang="ru";
}

QString SertBuild::getNomPart()
{
    return "1111";
}

QString SertBuild::getYearSert()
{
    return "2024";
}

QString SertBuild::getYearPart()
{
    return "2024";
}

QString SertBuild::getNomSert()
{
    return "2222";
}

int SertBuild::getType()
{
    return sertType;
}

svData SertBuild::getSData()
{
    return sdata;
}

void SertBuild::build(int id_part, int id_ship)
{
    current_id_part=id_part;
    current_id_ship=id_ship;
    rebuild();
}

void SertBuild::rebuild()
{
    int id = (current_id_ship>=0) ? current_id_ship : current_id_part;
    QString path=QString("/certificates/elrtr/%1/%2?lang=%3&part=%4").arg(sertType).arg(id).arg(lang).arg((current_id_ship>=0) ? "false" : "true");
    QByteArray req, resp;
    if (sendRequest(path,"GET",req,resp)){
        loadDoc(resp);
        updSData();
    } else {
        this->clear();
    }
}

void SertBuild::setType(int t)
{
    sertType=t;
    this->build(current_id_part,current_id_ship);
}

void SertBuild::setLang(QString l)
{
    lang=l;
    this->build(current_id_part,current_id_ship);
}

void SertBuild::setDocEn(int id_doc, bool en)
{

}

void SertBuild::setDefaultDoc()
{
    this->build(current_id_part,current_id_ship);
}

void SertBuild::clearCache()
{
    map.clear();
}

bool SertBuild::sendRequest(QString path, QString req, const QByteArray &data, QByteArray &respData)
{
    QNetworkRequest request(QUrl("http://192.168.1.114:7000"+path));
    //request.setRawHeader("Accept","application/xml");
    request.setRawHeader("Accept-Charset", "UTF-8");
    //request.setRawHeader("Content-Type", "application/xml");
    request.setRawHeader("User-Agent", "Appszsm");
    QEventLoop loop;
    QNetworkAccessManager man;
    connect(&man,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    QNetworkReply *reply;
    if (req=="GET"){
        reply=man.get(request);
    } else if (req=="POST"){
        reply=man.post(request,data);
    } else if (req=="DELETE"){
        reply=man.deleteResource(request);
    } else {
        reply=man.sendCustomRequest(request,req.toUtf8()/*,data*/);
    }
    if (!reply->isFinished()){
        loop.exec();
    }
    respData=reply->readAll();
    bool ok=(reply->error()==QNetworkReply::NoError);
    if (!ok){
        QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n"+respData,QMessageBox::Cancel);
    }
    reply->deleteLater();
    return ok;
}

void SertBuild::loadDoc(const QString &html)
{
    QMap <QString, QByteArray> tmpRes;
    QTextDocument tmpDoc;
    tmpDoc.setHtml(html);
    QTextBlock bl = tmpDoc.begin();
    while(bl.isValid()){
        QTextBlock::iterator it;
        for(it = bl.begin(); !(it.atEnd()); ++it){
            QTextFragment currentFragment = it.fragment();
            if(currentFragment.isValid()){
                if(currentFragment.charFormat().isImageFormat()){
                    QTextImageFormat imgFmt = currentFragment.charFormat().toImageFormat();
                    QString name=imgFmt.name();
                    if (!map.contains(name)){
                        QByteArray req, resp;
                        if (sendRequest(name,"GET",req,resp)){
                            if (!name.contains("qrcode")){
                                map.insert(name,resp);
                            }
                            tmpRes.insert(name,resp);
                        }
                    } else {
                        tmpRes.insert(name,map.value(name));
                    }
                }
            }
        }
        bl = bl.next();
    }
    this->clear();
    QMapIterator<QString, QByteArray> i(tmpRes);
    while (i.hasNext()) {
        i.next();
        this->addResource(QTextDocument::ImageResource,i.key(),i.value());
    }
    this->setHtml(html);
}

void SertBuild::updSData()
{
    QByteArray req, resp;
    if (sendRequest("/elrtr/sertdata/"+QString::number(current_id_part),"GET",req,resp)){
        //qDebug()<<resp;
    }
}
