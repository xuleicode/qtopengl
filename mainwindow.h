#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QObject>
#include <QtOpenGL/QtOpenGL>
#include <GL/GLU.h>
class MainWindow : public QGLWidget
{
    Q_OBJECT

public:

  MainWindow( QWidget* parent = 0, const char* name = 0, bool fs = false );
  ~MainWindow();

protected:

  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );

  void keyPressEvent( QKeyEvent *e );
  void loadGLTextures();

protected:

  bool fullscreen;
  GLfloat xRot, yRot, zRot;
  GLfloat zoom;
  GLfloat xSpeed, ySpeed;
  GLuint texture[3];
  GLuint filter;

  bool light;
};
#endif // MAINWINDOW_H
