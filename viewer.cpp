#include <GL/glew.h>
#include "viewer.h"
#include <QGLShader>
#include <QGLShaderProgram>

Viewer::Viewer(QWidget *parent) :
    QGLViewer(parent)
{
}

void Viewer::init()
{
    glewInit();
    glDisable(GL_LIGHTING);
    setSceneCenter(qglviewer::Vec(50., 50., 0.0));
    setSceneRadius(50);
    showEntireScene();
    
    vshader = new QGLShader(QGLShader::Vertex);
    Q_ASSERT(vshader->compileSourceFile("displacement.vsh"));
    
    program = new QGLShaderProgram();
    program->addShader(vshader);
    
    Q_ASSERT(program->link());
    
    QImage image;
    Q_ASSERT(image.load("height_map.png"));
    texture = bindTexture(image, GL_TEXTURE_2D, GL_LUMINANCE);
    
    Q_ASSERT(glIsTexture(texture));
}

#define N 300
#define N_F float(N)
#define L (100.0/N_F)
#define UV_L (1.0/N_F)

void Viewer::draw()
{
    program->bind();
    
    if (not glIsList(list)) {
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                float s = x / N_F;
                float t = y / N_F;
                glBegin(GL_LINE_STRIP);
                    glTexCoord2f(s, t);
                    glVertex2f(x*L, y*L);
                    
                    glTexCoord2f(s+UV_L, t);
                    glVertex2f(x*L+L, y*L);
                    
                    glTexCoord2f(s+UV_L, t+UV_L);
                    glVertex2f(x*L+L, y*L+L);
                    
                    glTexCoord2f(s, t+UV_L);
                    glVertex2f(x*L, y*L+L);
                    
                    glTexCoord2f(s, t);
                    glVertex2f(x*L, y*L);
                glEnd();
            }
        }
        glEndList();
    }
    glCallList(list);
    glUseProgram(0);
}
