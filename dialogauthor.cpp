#include "dialogauthor.h"
#include "ui_dialogauthor.h"

DialogAuthor::DialogAuthor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAuthor)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(updInfo()));
    updInfo();
}

DialogAuthor::~DialogAuthor()
{
    delete ui;
}

QByteArray DialogAuthor::getJson()
{
    QJsonObject object;
    object.insert("authTitle",ui->lineEditAuthTitle->text());
    object.insert("auth",ui->lineEditAuth->text());
    object.insert("checkTitle",ui->lineEditCheckTitle->text());
    object.insert("check",ui->lineEditCheck->text());
    object.insert("appTitle",ui->lineEditAppTitle->text());
    object.insert("app",ui->lineEditApp->text());
    object.insert("norm",ui->lineEditNorm->text());
    object.insert("date",ui->dateEdit->date().toString("yyyy/MM/dd"));
    object.insert("isSign",ui->checkBoxSig->isChecked());
    object.insert("authSig",QString(mapSig.value(1).toBase64()));
    object.insert("checkSig",QString(mapSig.value(2).toBase64()));
    object.insert("appSig",QString(mapSig.value(3).toBase64()));
    object.insert("normSig",QString(mapSig.value(4).toBase64()));
    QJsonDocument doc(object);
    return doc.toJson(QJsonDocument::Indented);
}

void DialogAuthor::updInfo()
{
    QSqlQuery query;
    mapSig.clear();
    query.prepare("select ps.id_role, (select snam||'#'||job from kamin_get_inf(:dat,ps.id_empl)), ps.sig "
                  "from pasport_sig ps "
                  "inner join ( "
                  "select id_role as id_role, max(dat) as dat from pasport_sig where dat <= :dat "
                  "group by id_role) as c on c.id_role=ps.id_role and c.dat=ps.dat");
    query.bindValue(":dat",ui->dateEdit->date());
    if (query.exec()){
        while(query.next()){
            int id_role=query.value(0).toInt();
            mapSig.insert(id_role,query.value(2).toByteArray());
            QStringList el=query.value(1).toString().split('#');
            QString empl = (el.size())? el.at(0) : "";
            QString job = (el.size()>1)? el.at(1) : "";
            if (id_role==1){
                ui->lineEditAuthTitle->setText(job);
                ui->lineEditAuth->setText(empl);
            } else if (id_role==2){
                ui->lineEditCheckTitle->setText(job);
                ui->lineEditCheck->setText(empl);
            } else if (id_role==3){
                ui->lineEditAppTitle->setText(job);
                ui->lineEditApp->setText(empl);
            } else if (id_role==4){
                ui->lineEditNorm->setText(empl);
            }
        }
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}
