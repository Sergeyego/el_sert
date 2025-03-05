#include "reader.h"
#include "ui_reader.h"

Reader::Reader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reader)
{
    ui->setupUi(this);
    id_ship=-1;

    view = new QWebEngineView(this);
    ui->verticalLayoutView->addWidget(view);
    view->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    view->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    view->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);

    connect(ui->radioButtonRus,SIGNAL(clicked(bool)),this,SLOT(reload()));
    connect(ui->radioButtonEng,SIGNAL(clicked(bool)),this,SLOT(reload()));
    connect(ui->radioButtonMix,SIGNAL(clicked(bool)),this,SLOT(reload()));

    connect(ui->pushButtonPrint,SIGNAL(clicked(bool)),this,SLOT(print()));

}

Reader::~Reader()
{
    delete ui;
}

void Reader::setCurrentIdShip(int id)
{
    id_ship=id;
    reload();
}

QString Reader::getCurrentLang()
{
    QString lang="mix";
    if (ui->radioButtonRus->isChecked()){
        lang="ru";
    } else if (ui->radioButtonEng->isChecked()){
        lang="en";
    }
    return lang;
}

void Reader::reload()
{
    QUrl url = QUrl("http://"+Rels::instance()->appServer()+"/s3/local/"+QString::number(id_ship)+"/"+getCurrentLang());
    view->load(url);
}

void Reader::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QEventLoop loop;
        bool result;
        auto printPreview = [&](bool success) { result = success; loop.quit(); };
        view->page()->print(&printer, std::move(printPreview));
        loop.exec();
        if (!result) {
            QPainter painter;
            if (painter.begin(&printer)) {
                QFont font = painter.font();
                font.setPixelSize(20);
                painter.setFont(font);
                painter.drawText(QPointF(10,25),QStringLiteral("Could not generate print preview."));
                painter.end();
            }
        }
    }
}
