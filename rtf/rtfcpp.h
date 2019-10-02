//#pragma once
/*
Copyright (c) <year> <copyright holders>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef RTFWRITER_H
#define RTFWRITER_H

#include "rtfdefs.h"
#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QStringList>
#include <QImage>
#include <QBuffer>

class RtfWriter : public QObject
{
    Q_OBJECT
    public:

    enum TypeMerge {
        NO_MERGE = 0,
        BEGIN_MERGE,
        MERGE
    };
        explicit RtfWriter(QObject *parent=0);
        ~RtfWriter();
        RTF_DOCUMENT_FORMAT* get_documentformat();							// Gets RTF document formatting properties
        void set_documentformat(const RTF_DOCUMENT_FORMAT &df);					// Sets RTF document formatting properties
        RTF_SECTION_FORMAT* get_sectionformat();							// Gets RTF section formatting properties
        void set_sectionformat(const RTF_SECTION_FORMAT &sf);						// Sets RTF section formatting properties
        int start_section();												// Starts new RTF section
        RTF_PARAGRAPH_FORMAT* get_paragraphformat();						// Gets RTF paragraph formatting properties
        void set_paragraphformat(const RTF_PARAGRAPH_FORMAT &pf);					// Sets RTF paragraph formatting properties
        int start_paragraph(const QString text, bool newPar);						// Starts new RTF paragraph
        void set_defaultformat();											// Sets default RTF document formatting
        int start_tablerow();												// Starts new RTF table row
        int end_tablerow();													// Ends RTF table row
        int start_tablecell(int rightMargin, TypeMerge horisontalMerge=RtfWriter::NO_MERGE, TypeMerge verticalMerge=RtfWriter::NO_MERGE);// Starts new RTF table cell
        int end_tablecell();												// Ends RTF table cell
        RTF_TABLEROW_FORMAT* get_tablerowformat();							// Gets RTF table row formatting properties
        void set_tablerowformat(const RTF_TABLEROW_FORMAT &rf);					// Sets RTF table row formatting properties
        RTF_TABLECELL_FORMAT* get_tablecellformat();						// Gets RTF table cell formatting properties
        void set_tablecellformat(const RTF_TABLECELL_FORMAT &cf);					// Sets RTF table cell formatting properties
        QString get_bordername(int border_type);								// Gets border name
        QString get_shadingname(int shading_type, bool cell);					// Gets shading name
        bool insert_image(QImage &image, int width, int height);				// Loads image from QImage
        void start_doc();
        void end_doc();
        bool saveDoc(QString filename);
        const QString encode(const QString &s);
        static int fromMmSize(double mm);
        static double toMmSize(int size);

protected:
        void init();														// Sets global RTF library params
        bool write_header();												// Writes RTF document header
        bool write_documentformat();										// Writes RTF document formatting properties
        bool write_sectionformat();											// Writes RTF section formatting properties
        bool write_paragraphformat();										// Writes RTF paragraph formatting properties

    private:
        RTF_DOCUMENT_FORMAT  _rtfDocFormat;					// RTF document formatting params
        RTF_SECTION_FORMAT   _rtfSecFormat;					// RTF section formatting params
        RTF_PARAGRAPH_FORMAT _rtfParFormat;					// RTF paragraph formatting params
        RTF_TABLEROW_FORMAT  _rtfRowFormat;					// RTF table row formatting params
        RTF_TABLECELL_FORMAT _rtfCellFormat;                // RTF table cell formatting params
        QString _rtfFontTable;
        QString _rtfColorTable;
        QString _rtfString;
};

#endif // RTFWRITER_H
