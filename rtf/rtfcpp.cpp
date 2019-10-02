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

#include "rtfcpp.h"

RtfWriter::RtfWriter(QObject *parent) : QObject(parent)
{
    init();
    set_defaultformat();
}

RtfWriter::~RtfWriter()
{
    //qDebug()<<"del";
}

// Writes RTF document header
bool RtfWriter::write_header()
{
    // Set error flag
    bool result = true;

    // Standard RTF document header
    QString rtfText;
    rtfText += "{\\rtf1\\ansi\\ansicpg1252\\deff0{\\fonttbl";
    rtfText += _rtfFontTable;
    rtfText += "}{\\colortbl";
    rtfText += _rtfColorTable;
    rtfText += "}";
    rtfText += "{\\*\\generator rtflib ver. 1.0;}";
    rtfText += "\n{\\info{\\author rtflib ver. 1.0}{\\company ETC Company LTD.}}";

    // Writes standard RTF document header part
    _rtfString+=rtfText;

    return result;
}

void RtfWriter::init()
{
    _rtfFontTable.clear();
    _rtfFontTable += "{\\f0\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0604020202020204} Arial;}"
                     "{\\f1\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0a04020102020204} Arial Black;}"
                     "{\\f2\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0506020202030204} Arial Narrow;}"
                     "{\\f3\\fmodern\\fprq1\\fcharset0{\\*\\panose 020b0609030804020204} Bitstream Vera Sans Mono;}"
                     "{\\f4\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0603030804020204} Bitstream Vera Sans;}"
                     "{\\f5\\froman\\fprq2\\fcharset0{\\*\\panose 02060603050605020204} Bitstream Vera Serif;}"
                     "{\\f6\\froman\\fprq2\\fcharset0{\\*\\panose 02040602050305030304} Book Antiqua;}"
                     "{\\f7\\froman\\fprq2\\fcharset0{\\*\\panose 02050604050505020204} Bookman Old Style;}"
                     "{\\f8\\froman\\fprq2\\fcharset0{\\*\\panose 020a0402060406010301} Castellar{\\*\\falt Georgia};}"
                     "{\\f9\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0502020202020204} Century Gothic;}"
                     "{\\f10\\fscript\\fprq2\\fcharset0{\\*\\panose 030f0702030302020204} Comic Sans MS;}"
                     "{\\f11\\fmodern\\fprq1\\fcharset0{\\*\\panose 02070309020205020404} Courier New;}"
                     "{\\f12\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0603020102020204} Franklin Gothic Medium;}"
                     "{\\f13\\froman\\fprq2\\fcharset0{\\*\\panose 02020404030301010803} Garamond;}"
                     "{\\f14\\froman\\fprq2\\fcharset0{\\*\\panose 02040502050405020303} Georgia;}"
                     "{\\f15\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0706040902060204} Haettenschweiler;}"
                     "{\\f16\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0806030902050204} Impact;}"
                     "{\\f17\\fmodern\\fprq1\\fcharset0{\\*\\panose 020b0609040504020204} Lucida Console;}"
                     "{\\f18\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0602030504020204} Lucida Sans Unicode;}"
                     "{\\f19\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0604020202020204} Microsoft Sans Serif;}"
                     "{\\f20\\fscript\\fprq2\\fcharset0{\\*\\panose 03010101010201010101} Monotype Corsiva;}"
                     "{\\f21\\froman\\fprq2\\fcharset0{\\*\\panose 02040502050505030304} Palatino Linotype;}"
                     "{\\f22\\fscript\\fprq2\\fcharset0{\\*\\panose 03070502060502030205} Papyrus;}"
                     "{\\f23\\froman\\fprq2\\fcharset0{\\*\\panose 010a0502050306030303} Sylfaen;}"
                     "{\\f24\\froman\\fprq2\\fcharset2{\\*\\panose 05050102010706020507} Symbol;}"
                     "{\\f25\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0604030504040204} Tahoma;}"
                     "{\\f26\\froman\\fprq2\\fcharset0{\\*\\panose 02020603050405020304} Times New Roman;}"
                     "{\\f27\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0603020202020204} Trebuchet MS;}"
                     "{\\f28\\fswiss\\fprq2\\fcharset0{\\*\\panose 020b0604030504040204} Verdana;}";

    // Set RTF document default color table
    _rtfColorTable.clear();
    _rtfColorTable += "\\red0\\green0\\blue0;" ;
    _rtfColorTable += "\\red255\\green0\\blue0;" ;
    _rtfColorTable += "\\red0\\green255\\blue0;" ;
    _rtfColorTable += "\\red0\\green0\\blue255;" ;
    _rtfColorTable += "\\red255\\green255\\blue0;" ;
    _rtfColorTable += "\\red255\\green0\\blue255;" ;
    _rtfColorTable += "\\red0\\green255\\blue255;" ;
    _rtfColorTable += "\\red255\\green255\\blue255;" ;
    _rtfColorTable += "\\red128\\green0\\blue0;" ;
    _rtfColorTable += "\\red0\\green128\\blue0;" ;
    _rtfColorTable += "\\red0\\green0\\blue128;" ;
    _rtfColorTable += "\\red128\\green128\\blue0;" ;
    _rtfColorTable += "\\red128\\green0\\blue128;" ;
    _rtfColorTable += "\\red0\\green128\\blue128;" ;
    _rtfColorTable += "\\red128\\green128\\blue128;" ;

}

// Sets default RTF document formatting
void RtfWriter::set_defaultformat()
{
    // Set default RTF document formatting properties
    RTF_DOCUMENT_FORMAT df = {RTF_DOCUMENTVIEWKIND_PAGE, 100, 12240, 15840, 1800, 1800, 1440, 1440, false, 0, false, false};
    set_documentformat(df);

    // Set default RTF section formatting properties
    RTF_SECTION_FORMAT sf = {RTF_SECTIONBREAK_CONTINUOUS, false, true, 12240, 15840, 1800, 1800, 1440, 1440, 0, 720, 720, false, 720, 720, false, 1, 720, false, false};
    set_sectionformat(sf);

    // Set default RTF paragraph formatting properties
    RTF_PARAGRAPH_FORMAT pf = {RTF_PARAGRAPHBREAK_NONE, false, true, RTF_PARAGRAPHALIGN_LEFT, 0, 0, 0, 0, 0, 0, "", false, false, false, false, false, false};
    pf.BORDERS.borderColor = 0;
    pf.BORDERS.borderKind = RTF_PARAGRAPHBORDERKIND_NONE;
    pf.BORDERS.borderSpace = 0;
    pf.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
    pf.BORDERS.borderWidth = 0;
    pf.CHARACTER.animatedCharacter = 0;
    pf.CHARACTER.backgroundColor = 0;
    pf.CHARACTER.boldCharacter = false;
    pf.CHARACTER.capitalCharacter = false;
    pf.CHARACTER.doublestrikeCharacter = false;
    pf.CHARACTER.embossCharacter = false;
    pf.CHARACTER.engraveCharacter = false;
    pf.CHARACTER.expandCharacter = 0;
    pf.CHARACTER.fontNumber = 0;
    pf.CHARACTER.fontSize = 24;
    pf.CHARACTER.foregroundColor = 0;
    pf.CHARACTER.italicCharacter = false;
    pf.CHARACTER.kerningCharacter = 0;
    pf.CHARACTER.outlineCharacter = false;
    pf.CHARACTER.scaleCharacter = 100;
    pf.CHARACTER.shadowCharacter = false;
    pf.CHARACTER.smallcapitalCharacter = false;
    pf.CHARACTER.strikeCharacter = false;
    pf.CHARACTER.subscriptCharacter = false;
    pf.CHARACTER.superscriptCharacter = false;
    pf.CHARACTER.underlineCharacter = 0;
    pf.NUMS.numsChar = char(0x95);
    pf.NUMS.numsLevel = 11;
    pf.NUMS.numsSpace = 360;
    pf.SHADING.shadingBkColor = 0;
    pf.SHADING.shadingFillColor = 0;
    pf.SHADING.shadingIntensity = 0;
    pf.SHADING.shadingType = RTF_PARAGRAPHSHADINGTYPE_FILL;
    pf.TABS.tabKind = RTF_PARAGRAPHTABKIND_NONE;
    pf.TABS.tabLead = RTF_PARAGRAPHTABLEAD_NONE;
    pf.TABS.tabPosition = 0;
    pf.hyphenation = 1;
    set_paragraphformat(pf);

    // Set default RTF table row formatting properties
    RTF_TABLEROW_FORMAT rf = {RTF_ROWTEXTALIGN_LEFT, 0, 50, 50, 50, 50, 0};
    set_tablerowformat(rf);

    // Set default RTF table cell formatting properties
    RTF_TABLECELL_FORMAT cf = {RTF_CELLTEXTALIGN_CENTER, 0, 0, 0, 0, RTF_CELLTEXTDIRECTION_LRTB, true};
    cf.SHADING.shadingBkColor = 7;
    cf.SHADING.shadingFillColor = 0;
    cf.SHADING.shadingIntensity = 0;
    cf.SHADING.shadingType = RTF_PARAGRAPHSHADINGTYPE_FILL;
    cf.borderBottom.border = false;
    cf.borderBottom.BORDERS.borderColor = 0;
    cf.borderBottom.BORDERS.borderKind = RTF_PARAGRAPHBORDERKIND_NONE;
    cf.borderBottom.BORDERS.borderSpace = 0;
    cf.borderBottom.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
    cf.borderBottom.BORDERS.borderWidth = 5;
    cf.borderLeft.border = false;
    cf.borderLeft.BORDERS.borderColor = 0;
    cf.borderLeft.BORDERS.borderKind = RTF_PARAGRAPHBORDERKIND_NONE;
    cf.borderLeft.BORDERS.borderSpace = 0;
    cf.borderLeft.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
    cf.borderLeft.BORDERS.borderWidth = 5;
    cf.borderRight.border = false;
    cf.borderRight.BORDERS.borderColor = 0;
    cf.borderRight.BORDERS.borderKind = RTF_PARAGRAPHBORDERKIND_NONE;
    cf.borderRight.BORDERS.borderSpace = 0;
    cf.borderRight.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
    cf.borderRight.BORDERS.borderWidth = 5;
    cf.borderTop.border = false;
    cf.borderTop.BORDERS.borderColor = 0;
    cf.borderTop.BORDERS.borderKind = RTF_PARAGRAPHBORDERKIND_NONE;
    cf.borderTop.BORDERS.borderSpace = 0;
    cf.borderTop.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
    cf.borderTop.BORDERS.borderWidth = 5;
    set_tablecellformat(cf);
}

// Sets RTF document formatting properties
void RtfWriter::set_documentformat(const RTF_DOCUMENT_FORMAT &df)
{
    // Set new RTF document formatting properties
    _rtfDocFormat=df;
}


// Writes RTF document formatting properties
bool RtfWriter::write_documentformat()
{
    // Set error flag
    bool result = true;
    QString rtfT;

    QString landscape;
    if (_rtfDocFormat.landscape)
        landscape=QString("\\landscape");

    rtfT=QString("\\viewkind%1\\viewscale%2\\paperw%3\\paperh%4%5\\margl%6\\margr%7\\margt%8\\margb%9\\gutter%10")
            .arg(_rtfDocFormat.viewKind)
            .arg(_rtfDocFormat.viewScale)
            .arg(_rtfDocFormat.paperWidth)
            .arg(_rtfDocFormat.paperHeight)
            .arg(landscape)
            .arg(_rtfDocFormat.marginLeft)
            .arg(_rtfDocFormat.marginRight)
            .arg(_rtfDocFormat.marginTop)
            .arg(_rtfDocFormat.marginBottom)
            .arg(_rtfDocFormat.gutterWidth);

    if ( _rtfDocFormat.facingPages )
        rtfT+=QString("\\facingp");
    if ( _rtfDocFormat.readOnly )
        rtfT+=QString("\\annotprot");

    // Writes RTF document formatting properties
    _rtfString+=rtfT;

    return result;
}


// Sets RTF section formatting properties
void RtfWriter::set_sectionformat(const RTF_SECTION_FORMAT &sf)
{
    // Set new RTF section formatting properties
    _rtfSecFormat=sf;
}


// Writes RTF section formatting properties
bool RtfWriter::write_sectionformat()
{
    // Set error flag
    bool result = true;

    // RTF document text
    QString rtfT;

    // Format new section
    QString text, pgn, sbr;
    if ( _rtfSecFormat.newSection )
        text+=QString("\\sect");
    if ( _rtfSecFormat.defaultSection )
        text+=QString("\\sectd");
    if ( _rtfSecFormat.showPageNumber ){
        pgn=QString("\\pgnx%1\\pgny%2").arg(_rtfSecFormat.pageNumberOffsetX).arg(_rtfSecFormat.pageNumberOffsetY);
    }

    // Format section break
    switch (_rtfSecFormat.sectionBreak)
    {
    // Continuous break
    case RTF_SECTIONBREAK_CONTINUOUS:
        sbr+=QString("\\sbknone");
        break;

        // Column break
    case RTF_SECTIONBREAK_COLUMN:
        sbr+=QString("\\sbkcol");
        break;

        // Page break
    case RTF_SECTIONBREAK_PAGE:
        sbr+=QString("\\sbkpage");
        break;

        // Even-page break
    case RTF_SECTIONBREAK_EVENPAGE:
        sbr+=QString("\\sbkeven");
        break;

        // Odd-page break
    case RTF_SECTIONBREAK_ODDPAGE:
        sbr+=QString("\\sbkodd");
        break;
    }

    // Format section columns
    QString cols;
    if ( _rtfSecFormat.cols == true ){
        // Format columns
        cols=QString("\\cols%1\\colsx%2").arg(_rtfSecFormat.colsNumber).arg(_rtfSecFormat.colsDistance);

        if ( _rtfSecFormat.colsLineBetween ){
            cols+=QString("\\linebetcol");
        }
    }

    QString landscape;
    if (_rtfSecFormat.landscape){
        landscape=QString("\\lndscpsxn");
    }

    rtfT=QString("\n%1%2%3\\pgwsxn%4\\pghsxn%5%6\\marglsxn%7\\margrsxn%8\\margtsxn%9\\margbsxn%10\\guttersxn%11\\headery%12\\footery%13")
            .arg(text)
            .arg(sbr)
            .arg(cols)
            .arg(_rtfSecFormat.pageWidth)
            .arg(_rtfSecFormat.pageHeight)
            .arg(landscape)
            .arg(_rtfSecFormat.pageMarginLeft)
            .arg(_rtfSecFormat.pageMarginRight)
            .arg(_rtfSecFormat.pageMarginTop)
            .arg(_rtfSecFormat.pageMarginBottom)
            .arg(_rtfSecFormat.pageGutterWidth)
            .arg(_rtfSecFormat.pageHeaderOffset)
            .arg(_rtfSecFormat.pageFooterOffset);

    // Writes RTF section formatting properties
    _rtfString+=rtfT;

    // Return error flag
    return result;
}


// Starts new RTF section
int RtfWriter::start_section()
{
    // Set error flag
    int error = RTF_SUCCESS;

    // Set new section flag
    _rtfSecFormat.newSection = true;

    // Starts new RTF section
    if( !RtfWriter::write_sectionformat() )
        error = RTF_SECTIONFORMAT_ERROR;

    // Return error flag
    return error;
}


// Sets RTF paragraph formatting properties
void RtfWriter::set_paragraphformat(const RTF_PARAGRAPH_FORMAT &pf)
{
    // Set new RTF paragraph formatting properties
    _rtfParFormat=pf;
}


// Writes RTF paragraph formatting properties
bool RtfWriter::write_paragraphformat()
{
    // Set error flag
    bool result = true;

    // Format new paragraph
    QString text;
    if ( _rtfParFormat.newParagraph ){
        text+=QString("\\par");
    }
    if ( _rtfParFormat.defaultParagraph ){
        text+=QString("\\pard");
    }
    if ( _rtfParFormat.tableText == false ){
        text+=QString("\\plain");
    } else {
        text+=QString("\\intbl");
    }

    QString hyp=QString("\\hyphpar%1").arg(_rtfParFormat.hyphenation);
    text+=hyp;

    switch (_rtfParFormat.paragraphBreak)
    {
    // No break
    case RTF_PARAGRAPHBREAK_NONE:
        break;

        // Page break;
    case RTF_PARAGRAPHBREAK_PAGE:
        text+=QString("\\page");
        break;

        // Column break;
    case RTF_PARAGRAPHBREAK_COLUMN:
        text+=QString("\\column");
        break;

        // Line break;
    case RTF_PARAGRAPHBREAK_LINE:
        text+=QString("\\line");
        break;
    }

    // Format aligment
    switch (_rtfParFormat.paragraphAligment)
    {
    // Left aligned paragraph
    case RTF_PARAGRAPHALIGN_LEFT:
        text+=QString("\\ql");
        break;

        // Center aligned paragraph
    case RTF_PARAGRAPHALIGN_CENTER:
        text+=QString("\\qc");
        break;

        // Right aligned paragraph
    case RTF_PARAGRAPHALIGN_RIGHT:
        text+=QString("\\qr");
        break;

        // Justified aligned paragraph
    case RTF_PARAGRAPHALIGN_JUSTIFY:
        text+=QString("\\qj");
        break;
    }

    // Format tabs
    if ( _rtfParFormat.paragraphTabs == true )
    {
        // Set tab kind
        switch ( _rtfParFormat.TABS.tabKind )
        {
        // No tab
        case RTF_PARAGRAPHTABKIND_NONE:
            break;

            // Centered tab
        case RTF_PARAGRAPHTABKIND_CENTER:
            text+=QString("\\tqc");
            break;

            // Flush-right tab
        case RTF_PARAGRAPHTABKIND_RIGHT:
            text+=QString("\\tqr");
            break;

            // Decimal tab
        case RTF_PARAGRAPHTABKIND_DECIMAL:
            text+=QString("\\tqdec");
            break;
        }

        // Set tab leader
        switch ( _rtfParFormat.TABS.tabLead )
        {
        // No lead
        case RTF_PARAGRAPHTABLEAD_NONE:
            break;

            // Leader dots
        case RTF_PARAGRAPHTABLEAD_DOT:
            text+=QString("\\tldot");
            break;

            // Leader middle dots
        case RTF_PARAGRAPHTABLEAD_MDOT:
            text+=QString("\\tlmdot");
            break;

            // Leader hyphens
        case RTF_PARAGRAPHTABLEAD_HYPH:
            text+=QString("\\tlhyph");
            break;

            // Leader underline
        case RTF_PARAGRAPHTABLEAD_UNDERLINE:
            text+=QString("\\tlul");
            break;

            // Leader thick line
        case RTF_PARAGRAPHTABLEAD_THICKLINE:
            text+=QString("\\tlth");
            break;

            // Leader equal sign
        case RTF_PARAGRAPHTABLEAD_EQUAL:
            text+=QString("\\tleq");
            break;
        }

        // Set tab position
        QString tb=QString("\\tx%1").arg(_rtfParFormat.TABS.tabPosition);
        text+=tb;
    }

    // Format bullets and numbering
    if ( _rtfParFormat.paragraphNums == true )
    {
        QString nums=QString("{\\*\\pn\\pnlvl%1\\pnsp%2\\pntxtb %3}")
                .arg(_rtfParFormat.NUMS.numsLevel)
                .arg(_rtfParFormat.NUMS.numsSpace)
                .arg(_rtfParFormat.NUMS.numsChar);
        text+=nums;
    }

    // Format paragraph borders
    if ( _rtfParFormat.paragraphBorders == true )
    {
        QString border;
        // Format paragraph border kind
        switch (_rtfParFormat.BORDERS.borderKind)
        {
        // No border
        case RTF_PARAGRAPHBORDERKIND_NONE:
            break;

            // Border top
        case RTF_PARAGRAPHBORDERKIND_TOP:
            border+=QString("\\brdrt" );
            break;

            // Border bottom
        case RTF_PARAGRAPHBORDERKIND_BOTTOM:
            border+=QString("\\brdrb");
            break;

            // Border left
        case RTF_PARAGRAPHBORDERKIND_LEFT:
            border+=QString("\\brdrl");
            break;

            // Border right
        case RTF_PARAGRAPHBORDERKIND_RIGHT:
            border+=QString("\\brdrr");
            break;

            // Border box
        case RTF_PARAGRAPHBORDERKIND_BOX:
            border+=QString("\\box");
            break;
        }

        // Format paragraph border type
        border+=RtfWriter::get_bordername(_rtfParFormat.BORDERS.borderType);

        QString brd;
        brd=QString("\\brdrw%1\\brsp%2")
                .arg(_rtfParFormat.BORDERS.borderWidth)
                .arg(_rtfParFormat.BORDERS.borderSpace);
        border+=brd;
        text+=border;
        QString brdcol;
        brdcol=QString("\\brdrcf%1").arg(_rtfParFormat.BORDERS.borderColor);

        text+=brdcol;
    }

    // Format paragraph shading
    if ( _rtfParFormat.paragraphShading == true )
    {
        QString shading;
        shading=QString("\\shading%1").arg(_rtfParFormat.SHADING.shadingIntensity);
        text+=RtfWriter::get_shadingname( _rtfParFormat.SHADING.shadingType, false );

        QString shcol;
        shcol=QString("\\cfpat%1\\cbpat%2")
                .arg(_rtfParFormat.SHADING.shadingFillColor)
                .arg(_rtfParFormat.SHADING.shadingBkColor);
        text+=shcol;
    }

    QString anim;
    if (_rtfParFormat.CHARACTER.animatedCharacter>0){
        anim=QString("\\animtext%d").arg(_rtfParFormat.CHARACTER.animatedCharacter);
    }

    QString font;
    font=QString("%1\\expndtw%2\\kerning%3\\charscalex%4\\f%5\\fs%6\\cf%7")
            .arg(anim)
            .arg(_rtfParFormat.CHARACTER.expandCharacter)
            .arg(_rtfParFormat.CHARACTER.kerningCharacter)
            .arg(_rtfParFormat.CHARACTER.scaleCharacter)
            .arg(_rtfParFormat.CHARACTER.fontNumber)
            .arg(_rtfParFormat.CHARACTER.fontSize)
            .arg(_rtfParFormat.CHARACTER.foregroundColor);

    if ( _rtfParFormat.CHARACTER.boldCharacter )
        font+=QString("\\b");
    else
        font+=QString("\\b0");
    if ( _rtfParFormat.CHARACTER.capitalCharacter )
        font+=QString("\\caps");
    else
        font+=QString("\\caps0");
    if ( _rtfParFormat.CHARACTER.doublestrikeCharacter )
        font+=QString("\\striked1");
    else
        font+=QString("\\striked0");
    if ( _rtfParFormat.CHARACTER.embossCharacter )
        font+=QString("\\embo");
    if ( _rtfParFormat.CHARACTER.engraveCharacter )
        font+=QString("\\impr");
    if ( _rtfParFormat.CHARACTER.italicCharacter )
        font+=QString("\\i");
    else
        font+=QString("\\i0");
    if ( _rtfParFormat.CHARACTER.outlineCharacter )
        font+=QString("\\outl");
    else
        font+=QString("\\outl0");
    if ( _rtfParFormat.CHARACTER.shadowCharacter )
        font+=QString("\\shad");
    else
        font+=QString("\\shad0");
    if ( _rtfParFormat.CHARACTER.smallcapitalCharacter )
        font+=QString("\\scaps");
    else
        font+=QString("\\scaps0");
    if ( _rtfParFormat.CHARACTER.strikeCharacter )
        font+=QString("\\strike");
    else
        font+=QString("\\strike0");
    if ( _rtfParFormat.CHARACTER.subscriptCharacter )
        font+=QString("\\sub");
    if ( _rtfParFormat.CHARACTER.superscriptCharacter )
        font+=QString("\\super");

    switch (_rtfParFormat.CHARACTER.underlineCharacter)
    {
    // None underline
    case 0:
        font+=QString("\\ulnone");
        break;

        // Continuous underline
    case 1:
        font+=QString("\\ul");
        break;

        // Dotted underline
    case 2:
        font+=QString("\\uld");
        break;

        // Dashed underline
    case 3:
        font+=QString("\\uldash");
        break;

        // Dash-dotted underline
    case 4:
        font+=QString("\\uldashd");
        break;

        // Dash-dot-dotted underline
    case 5:
        font+=QString("\\uldashdd");
        break;

        // Double underline
    case 6:
        font+=QString("\\uldb");
        break;

        // Heavy wave underline
    case 7:
        font+=QString("\\ulhwave");
        break;

        // Long dashed underline
    case 8:
        font+=QString("\\ulldash");
        break;

        // Thick underline
    case 9:
        font+=QString("\\ulth");
        break;

        // Thick dotted underline
    case 10:
        font+=QString("\\ulthd");
        break;

        // Thick dashed underline
    case 11:
        font+=QString("\\ulthdash");
        break;

        // Thick dash-dotted underline
    case 12:
        font+=QString("\\ulthdashd");
        break;

        // Thick dash-dot-dotted underline
    case 13:
        font+=QString("\\ulthdashdd");
        break;

        // Thick long dashed underline
    case 14:
        font+=QString("\\ulthldash");
        break;

        // Double wave underline
    case 15:
        font+=QString("\\ululdbwave");
        break;

        // Word underline
    case 16:
        font+=QString("\\ulw");
        break;

        // Wave underline
    case 17:
        font+=QString("\\ulwave");
        break;
    }

    // Set paragraph tabbed text

    QString rtfT;
    if ( _rtfParFormat.tabbedText == false ){
        rtfT=QString("\n%1\\fi%2\\li%3\\ri%4\\sb%5\\sa%6\\sl%7%8 {%9}")
                .arg(text)
                .arg(_rtfParFormat.firstLineIndent)
                .arg(_rtfParFormat.leftIndent)
                .arg(_rtfParFormat.rightIndent)
                .arg(_rtfParFormat.spaceBefore)
                .arg(_rtfParFormat.spaceAfter)
                .arg(_rtfParFormat.lineSpacing)
                .arg(font)
                .arg(_rtfParFormat.paragraphText);

    } else {
        rtfT=QString("\\tab %1").arg(_rtfParFormat.paragraphText);
    }

    // Writes RTF paragraph formatting properties
    _rtfString+=rtfT;

    // Return error flag
    return result;
}


// Starts new RTF paragraph
int RtfWriter::start_paragraph(const QString text, bool newPar)
{
    int error;
    QStringList l=text.split("\n");
    for (int i=0; i<l.size();i++){
        bool b= (i==0)? newPar : true;
        _rtfParFormat.paragraphText = encode(l.at(i));

        // Set new paragraph
        _rtfParFormat.newParagraph = b;
        // Set error flag
        error = RTF_SUCCESS;
        // Copy paragraph text

        // Starts new RTF paragraph
        if( !RtfWriter::write_paragraphformat() )
            error = RTF_PARAGRAPHFORMAT_ERROR;
    }

    // Return error flag
    return error;
}


// Gets RTF document formatting properties
RTF_DOCUMENT_FORMAT* RtfWriter::get_documentformat()
{
    // Get current RTF document formatting properties
    return &_rtfDocFormat;
}

// Gets RTF section formatting properties
RTF_SECTION_FORMAT* RtfWriter::get_sectionformat()
{
    // Get current RTF section formatting properties
    return &_rtfSecFormat;
}

// Gets RTF paragraph formatting properties
RTF_PARAGRAPH_FORMAT* RtfWriter::get_paragraphformat()
{
    // Get current RTF paragraph formatting properties
    return &_rtfParFormat;
}

// Starts new RTF table row
int RtfWriter::start_tablerow()
{
    // Set error flag
    int error = RTF_SUCCESS;
    QString tblrw;
    // Format table row aligment
    switch (_rtfRowFormat.rowAligment)
    {
    // Left align
    case RTF_ROWTEXTALIGN_LEFT:
        tblrw+=QString("\\trql");
        break;

        // Center align
    case RTF_ROWTEXTALIGN_CENTER:
        tblrw+=QString("\\trqc");
        break;

        // Right align
    case RTF_ROWTEXTALIGN_RIGHT:
        tblrw+=QString("\\trqr");
        break;
    }

    // Writes RTF table data
    QString rtfText;
    rtfText=QString("\n\\trowd\\trgaph115%1\\trleft%2\\trrh%3\\trpaddb%4\\trpaddfb3\\trpaddl%5\\trpaddfl3\\trpaddr%6\\trpaddfr3\\trpaddt%7\\trpaddft3")
            .arg(tblrw)
            .arg(_rtfRowFormat.rowLeftMargin)
            .arg(_rtfRowFormat.rowHeight)
            .arg(_rtfRowFormat.marginBottom)
            .arg(_rtfRowFormat.marginLeft)
            .arg(_rtfRowFormat.marginRight)
            .arg(_rtfRowFormat.marginTop);

    _rtfString+=rtfText;

    // Return error flag
    return error;
}


// Ends RTF table row
int RtfWriter::end_tablerow()
{
    // Set error flag
    int error = RTF_SUCCESS;

    // Writes RTF table data
    _rtfString+=QString("\n\\trgaph115\\row\\pard");

    // Return error flag
    return error;
}


// Starts new RTF table cell
int RtfWriter::start_tablecell(int rightMargin, TypeMerge horisontalMerge, TypeMerge verticalMerge)
{
    // Set error flag
    int error = RTF_SUCCESS;

    QString tblcla;
    // Format table cell text aligment
    switch (_rtfCellFormat.textVerticalAligment)
    {
    // Top align
    case RTF_CELLTEXTALIGN_TOP:
        tblcla=QString("\\clvertalt");
        break;

        // Center align
    case RTF_CELLTEXTALIGN_CENTER:
        tblcla=QString("\\clvertalc");
        break;

        // Bottom align
    case RTF_CELLTEXTALIGN_BOTTOM:
        tblcla=QString("\\clvertalb");
        break;
    }

    QString tblcld;
    // Format table cell text direction
    switch (_rtfCellFormat.textDirection)
    {
    // Left to right, top to bottom
    case RTF_CELLTEXTDIRECTION_LRTB:
        tblcld=QString("\\cltxlrtb");
        break;

        // Right to left, top to bottom
    case RTF_CELLTEXTDIRECTION_RLTB:
        tblcld=QString("\\cltxtbrl");
        break;

        // Left to right, bottom to top
    case RTF_CELLTEXTDIRECTION_LRBT:
        tblcld=QString("\\cltxbtlr");
        break;

        // Left to right, top to bottom, vertical
    case RTF_CELLTEXTDIRECTION_LRTBV:
        tblcld=QString("\\cltxlrtbv");
        break;

        // Right to left, top to bottom, vertical
    case RTF_CELLTEXTDIRECTION_RLTBV:
        tblcld=QString("\\cltxtbrlv");
        break;
    }

    QString tbclbrb, tbclbrl, tbclbrr, tbclbrt;
    // Format table cell border
    if ( _rtfCellFormat.borderBottom.border == true )
    {
        // Bottom cell border
        QString tbclbt=QString("\\clbrdrb");
        QString border = RtfWriter::get_bordername(_rtfCellFormat.borderBottom.BORDERS.borderType);
        tbclbrb=QString("%1%2\\brdrw%3\\brsp%4\\brdrcf%5")
                .arg(tbclbt)
                .arg(border)
                .arg(_rtfCellFormat.borderBottom.BORDERS.borderWidth)
                .arg(_rtfCellFormat.borderBottom.BORDERS.borderSpace)
                .arg(_rtfCellFormat.borderBottom.BORDERS.borderColor);
    }
    if ( _rtfCellFormat.borderLeft.border == true )
    {
        // Left cell border
        QString tbclbt=QString("\\clbrdrl");
        QString border = RtfWriter::get_bordername(_rtfCellFormat.borderLeft.BORDERS.borderType);
        tbclbrl=QString("%1%2\\brdrw%3\\brsp%4\\brdrcf%5")
                .arg(tbclbt)
                .arg(border)
                .arg(_rtfCellFormat.borderLeft.BORDERS.borderWidth)
                .arg(_rtfCellFormat.borderLeft.BORDERS.borderSpace)
                .arg(_rtfCellFormat.borderLeft.BORDERS.borderColor);
    }
    if ( _rtfCellFormat.borderRight.border == true )
    {
        // Right cell border
        QString tbclbt=QString("\\clbrdrr");
        QString border = RtfWriter::get_bordername(_rtfCellFormat.borderRight.BORDERS.borderType);
        tbclbrr=QString("%1%2\\brdrw%3\\brsp%4\\brdrcf%5")
                .arg(tbclbt)
                .arg(border)
                .arg(_rtfCellFormat.borderRight.BORDERS.borderWidth)
                .arg(_rtfCellFormat.borderRight.BORDERS.borderSpace)
                .arg(_rtfCellFormat.borderRight.BORDERS.borderColor);
    }
    if ( _rtfCellFormat.borderTop.border == true )
    {
        QString tbclbt=QString("\\clbrdrt");

        QString border = RtfWriter::get_bordername(_rtfCellFormat.borderTop.BORDERS.borderType);
        tbclbrt=QString("%1%2\\brdrw%3\\brsp%4\\brdrcf%5")
                .arg(tbclbt)
                .arg(border)
                .arg(_rtfCellFormat.borderTop.BORDERS.borderWidth)
                .arg(_rtfCellFormat.borderTop.BORDERS.borderSpace)
                .arg(_rtfCellFormat.borderTop.BORDERS.borderColor);
    }

    // Format table cell shading
    QString shading;
    if ( _rtfCellFormat.cellShading == true )
    {
        QString sh = RtfWriter::get_shadingname( _rtfCellFormat.SHADING.shadingType, true );
        // Set paragraph shading color
        shading=QString("%1\\clshdgn%2\\clcfpat%3\\clcbpat%4")
                .arg(sh)
                .arg(_rtfCellFormat.SHADING.shadingIntensity)
                .arg(_rtfCellFormat.SHADING.shadingFillColor)
                .arg(_rtfCellFormat.SHADING.shadingBkColor);
        /*shading=QString("%1\\clcfpat%3\\clcbpat%4")
                .arg(sh)
                .arg(_rtfCellFormat.SHADING.shadingFillColor)
                .arg(_rtfCellFormat.SHADING.shadingBkColor);*/
    }

    QString merg;
    if (horisontalMerge==RtfWriter::BEGIN_MERGE){
        merg+= QString("\\clmgf");
    } else if (horisontalMerge==RtfWriter::MERGE){
        merg+= QString("\\clmrg");
    }

    if (verticalMerge==RtfWriter::BEGIN_MERGE){
        merg+= QString("\\clvmgf");
    } else if (verticalMerge==RtfWriter::MERGE){
        merg+= QString("\\clvmrg");
    }

    QString rtfText;
    rtfText=QString("\n\\tcelld%1%2%3%4%5%6%7%8\\cellx%9")
            .arg(merg)
            .arg(tblcla)
            .arg(tblcld)
            .arg(tbclbrb)
            .arg(tbclbrl)
            .arg(tbclbrr)
            .arg(tbclbrt)
            .arg(shading)
            .arg(rightMargin);
    _rtfString+=rtfText;

    return error;
}


// Ends RTF table cell
int RtfWriter::end_tablecell()
{
    // Set error flag
    int error = RTF_SUCCESS;

    // Writes RTF table data

    _rtfString+=QString("\n\\cell ");

    // Return error flag
    return error;
}


// Gets RTF table row formatting properties
RTF_TABLEROW_FORMAT* RtfWriter::get_tablerowformat()
{
    // Get current RTF table row formatting properties
    return &_rtfRowFormat;
}


// Sets RTF table row formatting properties
void RtfWriter::set_tablerowformat(const RTF_TABLEROW_FORMAT &rf)
{
    // Set new RTF table row formatting properties
    _rtfRowFormat=rf;
}


// Gets RTF table cell formatting properties
RTF_TABLECELL_FORMAT* RtfWriter::get_tablecellformat()
{
    // Get current RTF table cell formatting properties
    return &_rtfCellFormat;
}


// Sets RTF table cell formatting properties
void RtfWriter::set_tablecellformat(const RTF_TABLECELL_FORMAT &cf)
{
    // Set new RTF table cell formatting properties
    _rtfCellFormat=cf;
}


// Gets border name
QString RtfWriter::get_bordername(int border_type)
{
    QString border;
    switch (border_type)
    {
    // Single-thickness border
    case RTF_PARAGRAPHBORDERTYPE_STHICK:
        border=QString("\\brdrs");
        break;

        // Double-thickness border
    case RTF_PARAGRAPHBORDERTYPE_DTHICK:
        border=QString("\\brdrth");
        break;

        // Shadowed border
    case RTF_PARAGRAPHBORDERTYPE_SHADOW:
        border=QString("\\brdrsh");
        break;

        // Double border
    case RTF_PARAGRAPHBORDERTYPE_DOUBLE:
        border=QString("\\brdrdb");
        break;

        // Dotted border
    case RTF_PARAGRAPHBORDERTYPE_DOT:
        border=QString("\\brdrdot");
        break;

        // Dashed border
    case RTF_PARAGRAPHBORDERTYPE_DASH:
        border=QString("\\brdrdash");
        break;

        // Hairline border
    case RTF_PARAGRAPHBORDERTYPE_HAIRLINE:
        border=QString("\\brdrhair");
        break;

        // Inset border
    case RTF_PARAGRAPHBORDERTYPE_INSET:
        border=QString("\\brdrinset");
        break;

        // Dashed border (small)
    case RTF_PARAGRAPHBORDERTYPE_SDASH:
        border=QString("\\brdrdashsm");
        break;

        // Dot-dashed border
    case RTF_PARAGRAPHBORDERTYPE_DOTDASH:
        border=QString("\\brdrdashd");
        break;

        // Dot-dot-dashed border
    case RTF_PARAGRAPHBORDERTYPE_DOTDOTDASH:
        border=QString("\\brdrdashdd");
        break;

        // Outset border
    case RTF_PARAGRAPHBORDERTYPE_OUTSET:
        border=QString("\\brdroutset");
        break;

        // Triple border
    case RTF_PARAGRAPHBORDERTYPE_TRIPLE:
        border=QString("\\brdrtriple");
        break;

        // Wavy border
    case RTF_PARAGRAPHBORDERTYPE_WAVY:
        border=QString("\\brdrwavy");
        break;

        // Double wavy border
    case RTF_PARAGRAPHBORDERTYPE_DWAVY:
        border=QString("\\brdrwavydb");
        break;

        // Striped border
    case RTF_PARAGRAPHBORDERTYPE_STRIPED:
        border=QString("\\brdrdashdotstr");
        break;

        // Embossed border
    case RTF_PARAGRAPHBORDERTYPE_EMBOSS:
        border=QString("\\brdremboss");
        break;

        // Engraved border
    case RTF_PARAGRAPHBORDERTYPE_ENGRAVE:
        border=QString("\\brdrengrave");
        break;
    }

    return border;
}


// Gets shading name
QString RtfWriter::get_shadingname(int shading_type, bool cell)
{
    QString shading;
    if ( cell == false )
    {
        switch (shading_type)
        {
        // Fill shading
        case RTF_PARAGRAPHSHADINGTYPE_FILL:
            shading=QString("");
            break;

            // Horizontal background pattern
        case RTF_PARAGRAPHSHADINGTYPE_HORIZ:
            shading=QString("\\bghoriz");
            break;

            // Vertical background pattern
        case RTF_PARAGRAPHSHADINGTYPE_VERT:
            shading=QString("\\bgvert");
            break;

            // Forward diagonal background pattern
        case RTF_PARAGRAPHSHADINGTYPE_FDIAG:
            shading=QString("\\bgfdiag");
            break;

            // Backward diagonal background pattern
        case RTF_PARAGRAPHSHADINGTYPE_BDIAG:
            shading=QString("\\bgbdiag");
            break;

            // Cross background pattern
        case RTF_PARAGRAPHSHADINGTYPE_CROSS:
            shading=QString("\\bgcross");
            break;

            // Diagonal cross background pattern
        case RTF_PARAGRAPHSHADINGTYPE_CROSSD:
            shading=QString("\\bgdcross");
            break;

            // Dark horizontal background pattern
        case RTF_PARAGRAPHSHADINGTYPE_DHORIZ:
            shading=QString("\\bgdkhoriz");
            break;

            // Dark vertical background pattern
        case RTF_PARAGRAPHSHADINGTYPE_DVERT:
            shading=QString("\\bgdkvert");
            break;

            // Dark forward diagonal background pattern
        case RTF_PARAGRAPHSHADINGTYPE_DFDIAG:
            shading=QString("\\bgdkfdiag");
            break;

            // Dark backward diagonal background pattern
        case RTF_PARAGRAPHSHADINGTYPE_DBDIAG:
            shading=QString("\\bgdkbdiag");
            break;

            // Dark cross background pattern
        case RTF_PARAGRAPHSHADINGTYPE_DCROSS:
            shading=QString("\\bgdkcross");
            break;

            // Dark diagonal cross background pattern
        case RTF_PARAGRAPHSHADINGTYPE_DCROSSD:
            shading=QString("\\bgdkdcross");
            break;
        }
    }
    else
    {
        switch (shading_type)
        {
        // Fill shading
        case RTF_CELLSHADINGTYPE_FILL:
            shading=QString("");
            break;

            // Horizontal background pattern
        case RTF_CELLSHADINGTYPE_HORIZ:
            shading=QString("\\clbghoriz");
            break;

            // Vertical background pattern
        case RTF_CELLSHADINGTYPE_VERT:
            shading=QString("\\clbgvert");
            break;

            // Forward diagonal background pattern
        case RTF_CELLSHADINGTYPE_FDIAG:
            shading=QString("\\clbgfdiag");
            break;

            // Backward diagonal background pattern
        case RTF_CELLSHADINGTYPE_BDIAG:
            shading=QString("\\clbgbdiag");
            break;

            // Cross background pattern
        case RTF_CELLSHADINGTYPE_CROSS:
            shading=QString("\\clbgcross");
            break;

            // Diagonal cross background pattern
        case RTF_CELLSHADINGTYPE_CROSSD:
            shading=QString("\\clbgdcross");
            break;

            // Dark horizontal background pattern
        case RTF_CELLSHADINGTYPE_DHORIZ:
            shading=QString("\\clbgdkhoriz");
            break;

            // Dark vertical background pattern
        case RTF_CELLSHADINGTYPE_DVERT:
            shading=QString("\\clbgdkvert");
            break;

            // Dark forward diagonal background pattern
        case RTF_CELLSHADINGTYPE_DFDIAG:
            shading=QString("\\clbgdkfdiag");
            break;

            // Dark backward diagonal background pattern
        case RTF_CELLSHADINGTYPE_DBDIAG:
            shading=QString("\\clbgdkbdiag");
            break;

            // Dark cross background pattern
        case RTF_CELLSHADINGTYPE_DCROSS:
            shading=QString("\\clbgdkcross");
            break;

            // Dark diagonal cross background pattern
        case RTF_CELLSHADINGTYPE_DCROSSD:
            shading=QString("\\clbgdkdcross");
            break;
        }
    }

    return shading;
}

bool RtfWriter::insert_image(QImage &image, int width, int height)
{
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"PNG");
    buffer.close();
    bool ok = !arr.isEmpty();
    if(ok){
        QString rtfIm;
        rtfIm=QString("\n{\\pict\\pngblip\\picwgoal%1\\pichgoal%2\\picscalex%3\\picscaley%4\n").arg(width).arg(height).arg(100).arg(100);
        rtfIm+=buffer.data().toHex();
        rtfIm+=QString("}");
        _rtfString+=rtfIm;
    }
    return ok;
}

void RtfWriter::end_doc()
{
    _rtfString+=QString("\n\\par}");
}

void RtfWriter::start_doc()
{
    _rtfString.clear();
    int error = RTF_SUCCESS;
    if ( !write_header() )
        error = RTF_HEADER_ERROR;
    // Write RTF document formatting properties
    if ( !write_documentformat())
        error = RTF_DOCUMENTFORMAT_ERROR;

    // Create first RTF document section with default formatting
    _rtfSecFormat.newSection=false;
    write_sectionformat();
}

bool RtfWriter::saveDoc(QString filename)
{
    QFile file(filename);
    bool ok=file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (ok){
        QTextStream out(&file);
        out.setCodec(QTextCodec::codecForName("CP1252"));
        out<<_rtfString;
        file.close();
    }
    return ok;
}

const QString RtfWriter::encode(const QString &s)
{
    QString so;
    for (int i=0; i<s.size(); i++){
        if (s.at(i)>=0 && s.at(i)<128){
            so+=s.at(i);
        } else {
            so+=QString("\\u%1?").arg(s.at(i).unicode());
        }
    }
    return so;
}

int RtfWriter::fromMmSize(double mm)
{
    return (mm*1440)/25.4;
}

double RtfWriter::toMmSize(int size)
{
    return (25.4*size)/1440;
}

