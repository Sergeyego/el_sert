#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    current_id_part=-1;
    current_id_ship=-1;
    sertType=0;
    lang="ru";
}

int SertBuild::getType()
{
    return sertType;
}

svData SertBuild::getSData()
{
    return sdata;
}

void SertBuild::build(int id_part, int id_ship, QString name, QString prefix)
{
    current_id_part=id_part;
    current_id_ship=id_ship;
    sname=name;
    spref=prefix;
    rebuild();
}

QString SertBuild::getName()
{
    return sname;
}

void SertBuild::rebuild()
{
    updSData();
    QString docs;
    for (sertData d : sdata){
        int id_doc=d.id_doc;
        bool en = mapSert.value(id_doc,d.en);
        if (en){
            if (!docs.isEmpty()){
                docs+="|";
            }
            docs+=QString::number(id_doc);
        }
    }
    int id = (current_id_ship>=0) ? current_id_ship : current_id_part;
    QString path=QString(Rels::instance()->appServer()+"/certificates/"+spref+"/%1/%2?lang=%3&part=%4&docs=%5").arg(sertType).arg(id).arg(lang).arg((current_id_ship>=0) ? "false" : "true").arg(docs);
    QByteArray resp;
    bool ok=HttpSyncManager::sendGet(path,resp);
    if (ok){
        loadDoc(resp);
    } else {
        this->clear();
    }
    emit sigRefresh();
}

void SertBuild::setType(int t)
{
    sertType=t;
    this->rebuild();
}

void SertBuild::setLang(QString l)
{
    lang=l;
    this->rebuild();
}

void SertBuild::setDocEn(int id_doc, bool en)
{
    mapSert[id_doc]=en;
    this->rebuild();
}

void SertBuild::setDefaultDoc()
{
    mapSert.clear();
    this->rebuild();
}

void SertBuild::clearCache()
{
    map.clear();
}

int SertBuild::getIdShip()
{
    return current_id_ship;
}

QString SertBuild::getLang()
{
    return lang;
}

QString SertBuild::getPrefix()
{
    return spref;
}

void SertBuild::loadDoc(const QString &html)
{
    QMap <QString, QByteArray> tmpRes;
    QStringList resList;
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
                    resList.push_back(name);
                    if (!map.contains(name)){
                        QByteArray resp;
                        bool ok=HttpSyncManager::sendGet(Rels::instance()->appServer()+"/"+name,resp);
                        if (ok){
                            if (!name.contains("qrcode")){
                                map.insert(name,resp);
                            } else {
                                tmpRes.insert(name,resp);
                            }
                        }
                    }
                }
            }
        }
        bl = bl.next();
    }
    this->clear();
    for (QString res : resList){
        if (map.contains(res)){
            this->addResource(QTextDocument::ImageResource,res,map.value(res));
        } else {
            this->addResource(QTextDocument::ImageResource,res,tmpRes.value(res));
        }
    }
    this->setHtml(html);
}

void SertBuild::updSData()
{
    QByteArray resp;
    sdata.clear();
    bool ok=HttpSyncManager::sendGet(Rels::instance()->appServer()+"/"+spref+"/sertdata/"+QString::number(current_id_part),resp);
    if (ok){
        QJsonDocument respDoc;
        respDoc=QJsonDocument::fromJson(resp);
        QJsonArray json=respDoc.array();
        for (QJsonValue v : json){
            sertData s;
            s.id_doc=v.toObject().value("id_doc").toInt();
            s.nom_doc=v.toObject().value("nom_doc").toString();
            s.ved_short=v.toObject().value("ved_short").toString();
            s.en=mapSert.value(s.id_doc,v.toObject().value("en").toBool());
            sdata.push_back(s);
        }
    }
}
