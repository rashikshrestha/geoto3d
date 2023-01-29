#ifndef __NC_GMF_H__
#define __NC_GMF_H__

#include <QXmlDefaultHandler>
#include <QMessageBox>

#include "ncpbo.h"

class ncGLCall {
public:
    virtual void execute() = 0;
};

class ncGLCallList : public std::vector<ncGLCall*> {
public:
        void execute();
};

//! ncGMF TAGS

struct ncGMFArray {
    QString         id;
    QString         format;
    unsigned int    count;
    void*           data;
    ncPBO*          pbo;
};

struct ncGMFElementArray {
    QString         id;
    QString         format;
    unsigned int    count;
    void*           data;
};

struct ncGMFVertexArray {
    QString         id;
};

struct ncGMFProgram {
    QString         id;
    QString         vshader;
    QString         fshader;
};



//! ncGMF

class ncGMF {

public:
    ncGMF();
    ~ncGMF();

    ncMesh*         mesh;
    ncPBO*          vert;
    ncIndexArray*   ind;
    ncProgram*      prog;
    ncGLCallList    render;
};



//! ncGMF Render calls

class ncglClearColor : public ncGLCall {

public:
    ncglClearColor() {
            r = g = b = a = 0.0f;
    }

    void setRed(GLfloat red) {
        r = red;
    }

    void setGreen(GLfloat green) {
        g = green;
    }

    void setBlue(GLfloat blue) {
        b = blue;
    }

    void setAlpha(GLfloat alpha) {
        a = alpha;
    }

    void execute() {
        glClearColor(r,g,b,a);
    }

    GLfloat r,g,b,a;
};

class ncglClear : public ncGLCall {

public:
    ncglClear() {
            mask = 0;
    }

    void add(GLbitfield m) {
        mask |= m;
    }

    void execute() {
        glClear(mask);
    }

    GLbitfield mask;
};





//! gmfHandler

class gmfHandler : public QXmlDefaultHandler
{
public:
    gmfHandler(ncGMF* gmf_);

    bool    startElement(const QString &namespaceURI, const QString &localName,
                        const QString &qName, const QXmlAttributes &attributes);
    bool    endElement(const QString &namespaceURI, const QString &localName,
                        const QString &qName);
    bool    characters(const QString &str);
    bool    fatalError(const QXmlParseException &exception);
    QString errorString() const;

private:
    QString     currentText;
    QString     errorStr;
    bool        metGmfTag;

    ncGMF*      gmf;

    ncGMFArray*             gmf_array;
    ncGMFElementArray       gmf_element_array;
    ncGMFVertexArray        gmf_vertex_array;
    ncGMFProgram            gmf_program;
};





//! ncGMFReader

class ncGMFReader {

public:
    static ncGMF* readFile(const char* fname);
};

#endif
