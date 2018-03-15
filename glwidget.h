#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QtOpenGL/QtOpenGL>
#include <GL/GLU.h>
class GLWidget : public QGLWidget
{
    Q_OBJECT
    public:
        GLWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
        ~GLWidget();
    signals:

    public slots:

    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);
        void mouseDoubleClickEvent( QMouseEvent *event );
        void keyPressEvent(QKeyEvent *e);


        GLfloat rTri;
        GLfloat rQuad;


        void loadGLTextures();
        GLfloat xRot, yRot, zRot;
        GLuint texture[1];//用来存储一个纹理

    private:
        GLfloat Point[5][3];
};

#endif // GLWIDGET_H
