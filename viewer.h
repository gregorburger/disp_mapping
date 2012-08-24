#ifndef VIEWER_H
#define VIEWER_H

#include <QGLViewer/qglviewer.h>

class QGLShaderProgram;
class QGLShader;

class Viewer : public QGLViewer
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = 0);
    
    virtual void init();
    virtual void draw();
    
signals:
    
private:
    QGLShader *vshader;
    QGLShaderProgram *program;
    GLuint texture;
    GLuint list;
};

#endif // VIEWER_H
