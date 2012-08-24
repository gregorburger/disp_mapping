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
    
    QImage image(100, 100, QImage::Format_RGB32);
    for (int i = 0; i < 5; ++i) {
        int grey = 256/5 * i;
        //image.setColor(i, qRgb(grey, grey, grey));
        for (int x = 0; x < 20; ++x) {
            for (int y = 0; y < 100; ++y) {
                image.setPixel(i*20+x, y, qRgb(grey, grey, grey));
            }
        }
    }
    height_map = bindTexture(image, GL_TEXTURE_2D, GL_LUMINANCE);
    Q_ASSERT(glIsTexture(height_map));
    
    glBindTexture(GL_TEXTURE_2D, height_map);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    Q_ASSERT(image.load("texture.png"));
    texture = bindTexture(image);
    
    Q_ASSERT(glIsTexture(texture));
    
    glDisable(GL_TEXTURE_2D);
}

#define N 100
#define N_F float(N)
#define L (100.0/N_F)
#define UV_L (1.0/N_F)

void Viewer::draw()
{
    glColor3f(0.3, 0.5, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 100);
    glVertex2f(100, 100);
    glVertex2f(100, 0);
    glEnd();
    program->bind();
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, height_map);
    program->setUniformValue("height_map", 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    program->setUniformValue("texture", 0);
    
    if (not glIsList(list)) {
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        glColor3f(1.0, 1.0, 1.0);
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                float s = x / (N_F-1);
                float t = y / (N_F-1);
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
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
}
