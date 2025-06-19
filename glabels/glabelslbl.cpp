#include "glabelslbl.h"

GlabelsLbl::GlabelsLbl(QObject *parent) : QObject(parent)
{
    QDomDocument domDocument;
    QFile fileTemp(QDir::currentPath()+"/templates/glabels_szsm.xml");
    domDocument.setContent(&fileTemp, true);
    QDomNodeList list;
    QDomElement main = domDocument.documentElement();
    if ((main.tagName() == "labels-templates")||(main.tagName() == "Glabels-templates")) {
        list = main.elementsByTagName("Template");
        for (int i=0; i<list.size(); i++){
            QString nam=list.at(i).toElement().attribute("brand");
            map[nam]=list.at(i).toElement();
        }
    }
    fileTemp.close();
}

bool GlabelsLbl::createLbl(QString templates, bool rotate)
{
    doc.clear();
    bool ok=!map.value(templates).isNull();
    if (!ok){
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Ошибка шаблона"),QMessageBox::Ok);
    } else {
        doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
        root = doc.createElement("Glabels-document");
        doc.appendChild(root);
        root.appendChild(map.value(templates));
        obj = doc.createElement("Objects");
        obj.setAttribute("id", 0);
        obj.setAttribute("rotate", rotate? "True" : "False");
        root.appendChild(obj);
    }
    return ok;
}

void GlabelsLbl::newImage(double x, double y, double w, double h, QString fnam)
{
    QDomElement l=doc.createElement("Object-image");
    l.setAttribute("x",QString("%1mm").arg(x));
    l.setAttribute("y",QString("%1mm").arg(y));
    l.setAttribute("w",QString("%1mm").arg(w));
    l.setAttribute("h",QString("%1mm").arg(h));
    l.setAttribute("src",fnam);
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    obj.appendChild(l);
}

void GlabelsLbl::newBarcode(double x, double y, double w, double h, QString code)
{
    QDomElement l = doc.createElement("Object-barcode");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("backend","gnu-barcode");
    l.setAttribute("style","EAN-13");
    l.setAttribute("text","True");
    l.setAttribute("color","0x000000ff");
    l.setAttribute("data",code);
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    obj.appendChild(l);
}

void GlabelsLbl::newText(double x, double y, double w, double h, QString text, int size, bool bold, Qt::Alignment al, double spacing)
{
    QString hAl=QString("Left");
    QString vAl=QString("Center");
    if (al & Qt::AlignRight){
        hAl=QString("Right");
    } else if (al & Qt::AlignHCenter){
        hAl=QString("Center");
    }
    if (al & Qt::AlignTop){
        vAl=QString("Top");
    } else if (al & Qt::AlignBottom){
        vAl=QString("Bottom");
    }
    QDomElement l = doc.createElement("Object-text");
    l.setAttribute("x",QString("%1mm").arg(x));
    l.setAttribute("y",QString("%1mm").arg(y));
    l.setAttribute("w",QString("%1mm").arg(w));
    l.setAttribute("h",QString("%1mm").arg(h));
    l.setAttribute("justify",hAl);
    l.setAttribute("valign",vAl);
    l.setAttribute("auto_shrink","False");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    QDomElement span = doc.createElement("Span");
    span.setAttribute("color","0x000000ff");
    span.setAttribute("font_family","Liberation Sans");
    span.setAttribute("font_size",size);
    span.setAttribute("font_weight",bold ? "Bold" : "Regular");
    span.setAttribute("font_italic","False");
    span.setAttribute("line_spacing",spacing);
    span.appendChild(doc.createTextNode(text));
    l.appendChild(span);
    obj.appendChild(l);
}

void GlabelsLbl::newRect(double x, double y, double w, double h)
{
    QDomElement l = doc.createElement("Object-box");
    l.setAttribute("x",QString("%1mm").arg(x));
    l.setAttribute("y",QString("%1mm").arg(y));
    l.setAttribute("w",QString("%1mm").arg(w));
    l.setAttribute("h",QString("%1mm").arg(h));
    l.setAttribute("line_width","0.176389mm");
    l.setAttribute("line_color","0x000000ff");
    l.setAttribute("fill_color","0x00000000");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    obj.appendChild(l);
}

void GlabelsLbl::newLine(double x, double y, double dx, double dy)
{
    QDomElement l = doc.createElement("Object-line");
    l.setAttribute("x",QString("%1mm").arg(x));
    l.setAttribute("y",QString("%1mm").arg(y));
    l.setAttribute("dx",QString("%1mm").arg(dx));
    l.setAttribute("dy",QString("%1mm").arg(dy));
    l.setAttribute("line_width","0.176389mm");
    l.setAttribute("line_color","0x000000ff");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    obj.appendChild(l);
}

bool GlabelsLbl::saveFile(QString name)
{
    QFile file(name);
    bool ok=file.open(QIODevice::WriteOnly);
    if (ok) {
        QTextStream stream( &file );
        doc.save(stream,2,QDomNode::EncodingFromTextStream);
        file.close();
    }
    return ok;
}
