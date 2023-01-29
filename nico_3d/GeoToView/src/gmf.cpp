#include "gmf.h"


//! Render calls

void ncGLCallList::execute() {

    for (unsigned int i=0;i<size();++i)
        at(i)->execute();
};




//! GMF Handler

gmfHandler::gmfHandler(ncGMF* gmf_)
{
   metGmfTag    = false;
   gmf          = gmf_;
}

bool gmfHandler::startElement(const QString & /* namespaceURI */, const QString & /* localName */, const QString &qName, const QXmlAttributes &attributes)
{
    fprintf(stderr,"<%s #attr=%d>\n",qName.toStdString().c_str(),attributes.count());

    currentText.clear();

    if (!metGmfTag && qName != "gmf") {
        errorStr = QObject::tr("The file is not an GMF file.");
        return false;
    }

    if (qName == "gmf") {
        QString version = attributes.value("version");
        if (!version.isEmpty() && version != "1.0") {
            errorStr = QObject::tr("The file is not an GMF version 1.0 file.");
            return false;
        }
        metGmfTag = true;
    }

    if (qName == "model") {

    }

    if (qName == "array") {
        gmf_array = new ncGMFArray;
        gmf_array->id     = attributes.value("id");
        gmf_array->format = attributes.value("format");
        gmf_array->count  = attributes.value("count").toInt();
        gmf_array->data   = malloc(gmf_array->count*4*sizeof(float));
    }

    if (qName == "vertex_array") {
        gmf_vertex_array.id     = attributes.value("id");
        gmf->mesh  = new ncMesh;
    }

    if (qName == "element_array") {
        gmf_element_array.id     = attributes.value("id");
        gmf_element_array.format = attributes.value("format");
        gmf_element_array.count  = attributes.value("count").toInt();
        gmf_element_array.data   = malloc(gmf_element_array.count*sizeof(unsigned int));
    }

    if (qName == "program") {
        gmf_program.id   = attributes.value("id");
    }

    if (qName == "vertex_shader") {
    }

    if (qName == "fragment_shader") {
    }

    if (qName == "render") {
    }

    if (qName == "glClearColor") {

        ncglClearColor* call = new ncglClearColor;
        if (!attributes.value("red").isEmpty())
            call->setRed(attributes.value("red").toFloat());
        if (!attributes.value("green").isEmpty())
            call->setGreen(attributes.value("green").toFloat());
        if (!attributes.value("blue").isEmpty())
            call->setBlue(attributes.value("blue").toFloat());
        if (!attributes.value("alpha").isEmpty())
            call->setAlpha(attributes.value("alpha").toFloat());
        gmf->render.push_back(call);
    }

    if (qName == "glClear") {

        ncglClear* call = new ncglClear;
        if (!attributes.value("mask").isEmpty())
            if (attributes.value("mask") == "GL_COLOR_BUFFER_BIT")
                call->add(GL_COLOR_BUFFER_BIT);
        gmf->render.push_back(call);
    }


   return true;
}

bool gmfHandler::endElement(const QString & /* namespaceURI */, const QString & /* localName */, const QString &qName)
{

    if (qName == "array") {

        std::string mystr = currentText.toStdString();
        const char* Cstr = mystr.c_str();
        //fprintf(stderr,"Cstr = [ %s ] \n",Cstr);

        int n;
        for (unsigned int i=0;i<gmf_array->count;++i)
        {
            float *x = ((float*)gmf_array->data)+4*i;
            float *y = ((float*)gmf_array->data)+4*i+1;
            float *z = ((float*)gmf_array->data)+4*i+2;

            unsigned char *r = ((unsigned char*)gmf_array->data)+12+16*i+0;
            unsigned char *g = ((unsigned char*)gmf_array->data)+12+16*i+1;
            unsigned char *b = ((unsigned char*)gmf_array->data)+12+16*i+2;
            unsigned char *a = ((unsigned char*)gmf_array->data)+12+16*i+3;
            sscanf( Cstr,"%f %f %f %hhu %hhu %hhu %hhu %n" ,x,y,z,r,g,b,a,&n);
            Cstr+=n;
        }

        float*          ffff = (float*)gmf_array->data;
        unsigned char*  ubub = (unsigned char*)gmf_array->data;
        for (unsigned int i=0;i<gmf_array->count;++i)
        {
            fprintf(stderr,"%f %f %f %hhu %hhu %hhu %hhu\n",*(ffff+4*i),*(ffff+4*i+1),*(ffff+4*i+2),*(ubub+12+16*i+0),*(ubub+12+16*i+1),*(ubub+12+16*i+2),*(ubub+12+16*i+3));
        }


        gmf_array->pbo = new ncPBO;
        gmf_array->pbo->setup(gmf_array->count*4*sizeof(float),gmf_array->data);

        gmf->vert = new ncPBO;
        gmf->vert->setup(gmf_array->count*4*sizeof(float),gmf_array->data);

        gmf_array = 0;
    }

    if (qName == "element_array") {

        std::string mystr = currentText.toStdString();
        const char* Cstr = mystr.c_str();
        //fprintf(stderr,"Cstr = [ %s ] \n",Cstr);

        int n;
        for (unsigned int i=0;i<gmf_element_array.count;++i)
        {
            sscanf( Cstr,"%u %n" ,((unsigned int*)gmf_element_array.data)+i,&n);
            Cstr+=n;
        }

        unsigned int*  uiui = (unsigned int*)gmf_element_array.data;
        for (unsigned int i=0;i<gmf_element_array.count;++i)
        {
            fprintf(stderr,"%u \n",*(uiui+i));
        }

        gmf->ind = new ncIndexArray;
        gmf->ind->setup(gmf_element_array.count,(unsigned int*)gmf_element_array.data);

        free(gmf_element_array.data);
    }

    if (qName == "model") {
        gmf->mesh->bindAttrib(0,gmf->vert,3,GL_FLOAT		    ,GL_FALSE	, sizeof(ncc::v4f ),0					);
        gmf->mesh->bindAttrib(1,gmf->vert,4,GL_UNSIGNED_BYTE    ,GL_TRUE	, sizeof(ncc::v4f ),3*sizeof(ncc::v4ub)	);
    }

    if (qName == "program") {
        gmf->prog  = new ncProgram(gmf_program.vshader.toStdString().c_str() , 0 , gmf_program.fshader.toStdString().c_str() , 1);
    }

    if (qName == "vertex_shader") {
        gmf_program.vshader = currentText;
    }

    if (qName == "fragment_shader") {
        gmf_program.fshader = currentText;
    }



if (qName == "render") {

    }

    fprintf(stderr,"</%s>\n",qName.toStdString().c_str());
    return true;
}

bool gmfHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool gmfHandler::fatalError(const QXmlParseException &exception)
{
    fprintf(stderr,"error\n");

    QMessageBox::information(0, QObject::tr("GMF Handler"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString gmfHandler::errorString() const
{
    fprintf(stderr,"errorString\n");
    return errorStr;
}




//! GMF Reader

ncGMF* ncGMFReader::readFile(const char* fname) {

    ncGMF*      result  = new ncGMF();
    gmfHandler* handler = new gmfHandler(result);

    QXmlSimpleReader reader;
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);

    QFile file(fname);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        fprintf(stderr,"Could not read file %s\n",fname);
        return 0;
    }

    QXmlInputSource xmlInputSource(&file);
    if (!reader.parse(xmlInputSource))
        fprintf(stderr,"parsing failed...\n");

    delete handler;
    return result;
}




//! GMF model

ncGMF::ncGMF() {

    mesh    = 0;
    vert    = 0;
    ind     = 0;
}

ncGMF::~ncGMF() {


}




