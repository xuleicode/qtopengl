#include "glwidget.h"
#include <QImage>
const GLfloat PI = 3.1415926536f;

/*
 * OpenGL渲染管线分为两大部分，模型观测变换(ModelView Transformation)和投影变换(Projection Transformation)。
 * 做个比喻，计算机图形开发就像我们照相一样，目的就是把真实的场景在一张照相纸上表现出来。
 * 那么观测变换的过程就像是我们摆设相机的位置，选择好要照的物体，摆好物体的造型。
 * 而投影变换就像相机把真实的三维场景显示在相纸上一样。
 *
 * 管线：显卡在处理数据的时候是按照一个固定的顺序来的，而且严格按照这个顺序。
//
OpenGL中有一个坐标变换矩阵栈(ModelView)，栈顶就是当前坐标变换矩阵，
进入OpenGL管道的每个坐标(齐次坐标)都会先乘上这个矩阵，结果才是对应点在场景中的世界坐标。
OpenGL中的坐标变换都是通过矩阵运算完成的，要注意的是变换中的矩阵乘法是左乘，而矩阵乘法与算术乘法不同，不符合交换律(万一不明白去看矩阵代数书好了)。

矩阵栈操纵命令：
glPushMatrix(); 当前矩阵入栈，这时矩阵栈将栈顶值压入栈。
glPopMatrix(); 栈顶出栈，通常与上一条命令配合使用。
glLoadIdentity(); 将栈顶设为不变矩阵(就是对角线全为1其它为0的那个)。
glMultMatrix(M);将栈顶T设为M·T。


//
*/
GLWidget::GLWidget(QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
{
    setMinimumSize(320,240);
    resize(640,480);
    setWindowTitle(tr("第一个OpenGL程序"));
    short angle = 18;


    for(short i=0; i<5; i++) {
        Point[i][0] = cos(angle * PI/180);
        Point[i][1] = sin(angle * PI/180);
        Point[i][2] = 0.0;
        angle += 72;
    }

    rTri = rQuad = 0.0;

     xRot = yRot = zRot = 0.0;
}
GLWidget::~GLWidget()
{
}
void GLWidget::initializeGL()
{
    loadGLTextures();//    载入纹理。
    glEnable( GL_TEXTURE_2D );//启用纹理,纹理对象永远都是纯白色


    glShadeModel(GL_SMOOTH);//启用smooth shading(阴影平滑)
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);//设置清除屏幕时所用的颜色 RGB + Alpa

    //下面三行做的是关于depth buffer（深度缓存）。将深度缓存设想为屏幕后面的层。深度缓存不断的对物体进入屏幕内部有多深进行跟踪。
    //几乎所有在屏幕上显示3D场景OpenGL程序都使用深度缓存。它的排序决定那个物体先画
    glClearDepth(1.0);//设置深度缓存
    glEnable(GL_DEPTH_TEST);//启用深度测试。
    glDepthFunc(GL_LEQUAL);//所作深度测试的类型。

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//告诉OpenGL我们希望进行最好的透视修正。这会十分轻微的影响性能。但使得透视图看起来好一点
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, Point);
}
void GLWidget::paintGL()
{
    qDebug()<<"----------paintGL---------";
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//清除屏幕和深度缓存。
    glLoadIdentity();//重置当前的模型观察矩阵。
    //调用glLoadIdentity()之后，实际上将当前点移到了屏幕中心，
    //X坐标轴从左至右，Y坐标轴从下至上，Z坐标轴从里至外。
    //OpenGL屏幕中心的坐标值是X和Y轴上的0.0点。
    //中心左面的坐标值是负值，右面是正值。
    //移向屏幕顶端是正值，移向屏幕底端是负值。
    //移入屏幕深处是负值，移出屏幕则是正值。
    //在glLoadIdentity()调用之后，开始绘图
    glTranslatef( -1.5,  0.0, -6.0 );//相对与当前所在的屏幕位置移动平移
    //glRotate(d,x,y,z)：旋转，第一个参数为转动的度数，后三个参数表明是否绕该轴旋转。通常x,y,z中只有一个为1，其余为0，用连续几条旋转命令完成复杂旋转。由于矩阵运算的左乘特点，旋转命令的顺序与旋转动作的顺序正好相反。
    //glRotatef(45.0,1,0,0);
   // glRotatef( rTri,  0.0,  1.0,  0.0 );
    glRotatef( xRot,  1.0,  0.0,  0.0 ); //根据xRot、yRot、zRot的实际值来旋转正方体。
    glRotatef( yRot,  0.0,  1.0,  0.0 );
    glRotatef( zRot,  0.0,  0.0,  1.0 );

    glBindTexture( GL_TEXTURE_2D, texture[0] );//不能在glBegin()和glEnd()之间绑定纹理

   // glScalef(1,2,1);//放大缩小
    glBegin( GL_TRIANGLES );  //GL_TRIANGLES三点 GL_QUADS四点 GL_POLYGON多点
    {//glVertex中的点是相对最新的坐标原点的坐标值//vertex 顶点

        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3f(  0.0,  1.0,  0.0 );
        glColor3f(  0.0,  1.0,  0.0 );
        glVertex3f( -1.0, -1.0,  0.0 );
        glColor3f(  0.0,  0.0,  1.0 );
        glVertex3f(  1.0, -1.0,  0.0 );

        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3f(  0.0,  1.0,  0.0 );
        glColor3f(  0.0,  1.0,  0.0 );
        glVertex3f( -1.0, -1.0,  0.0 );
        glColor3f(  1.0,  1.0,  0.0 );
        glVertex3f(  0.0, 0.0,  -1.0 );

        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3f(  0.0,  1.0,  0.0 );
        glColor3f(  0.0,  0.0,  1.0 );
        glVertex3f(  1.0, -1.0,  0.0 );
        glColor3f(  1.0,  1.0,  0.0 );
        glVertex3f(  0.0, 0.0,  -1.0 );

        glColor3f(  0.0,  1.0,  0.0 );
        glVertex3f( -1.0, -1.0,  0.0 );
        glColor3f(  0.0,  0.0,  1.0 );
        glVertex3f(  1.0, -1.0,  0.0 );
        glColor3f(  1.0,  1.0,  0.0 );
        glVertex3f(  0.0, 0.0,  -1.0 );
    }
    glEnd();
    glTranslatef(  3.0,  0.0,  0.0 );
    glColor3f( 1.0, 1.0, 0 );
    //glRotatef( rQuad,  1.0,  0.0,  0.0 );
    glBegin( GL_QUADS );  //GL_TRIANGLES三点 GL_QUADS四点 GL_POLYGON多点
    {

/*
* 为了将纹理正确的映射到四边形上，必须将纹理的各个点映射到四边形的各个点上。如果映射错误的话，图像显示时可能上下颠倒，侧向一边或者什么都不是。
glTexCoord2f的第一个参数是X坐标。0.0是纹理的左侧。0.5是纹理的中点，1.0是纹理的右侧。
glTexCoord2f的第二个参数是Y坐标。0.0是纹理的底部。0.5是纹理的中点，1.0是纹理的顶部。
所以纹理的左上坐标是X：0.0，Y：1.0f，四边形的左上顶点是X：-1.0，Y：1.0。其余三点依此类推。
*/
       //glColor3f( 0.0, 1.0, 0.0 );

       glTexCoord2f( 0.0, 0.0 );  glVertex3f(  1.0,  1.0, -1.0 );
       glTexCoord2f( 1.0, 0.0 );  glVertex3f( -1.0,  1.0, -1.0 );
       glTexCoord2f( 1.0, 1.0 );  glVertex3f( -1.0,  1.0,  1.0 );
       glTexCoord2f( 0.0, 1.0 );  glVertex3f(  1.0,  1.0,  1.0 );

       //glColor3f( 1.0, 0.5, 0.0 );
       glVertex3f(  1.0, -1.0,  1.0 );
       glVertex3f( -1.0, -1.0,  1.0 );
       glVertex3f( -1.0, -1.0, -1.0 );
       glVertex3f(  1.0, -1.0, -1.0 );

       glColor3f( 1.0, 0.0, 0.0 );
       glVertex3f(  1.0,  1.0,  1.0 );
       glVertex3f( -1.0,  1.0,  1.0 );
       glVertex3f( -1.0, -1.0,  1.0 );
       glVertex3f(  1.0, -1.0,  1.0 );

       glColor3f( 1.0, 1.0, 0.0 );
       glVertex3f(  1.0, -1.0, -1.0 );
       glVertex3f( -1.0, -1.0, -1.0 );
       glVertex3f( -1.0,  1.0, -1.0 );
       glVertex3f(  1.0,  1.0, -1.0 );


       glColor3f( 0.0, 0.0, 1.0 );
       glVertex3f( -1.0,  1.0,  1.0 );
       glVertex3f( -1.0,  1.0, -1.0 );
       glVertex3f( -1.0, -1.0, -1.0 );
       glVertex3f( -1.0, -1.0,  1.0 );


       glColor3f( 1.0, 0.0, 1.0 );
       glVertex3f(  1.0,  1.0, -1.0 );
       glVertex3f(  1.0,  1.0,  1.0 );
       glVertex3f(  1.0, -1.0,  1.0 );
       glVertex3f(  1.0, -1.0, -1.0 );

    }
    glEnd();
    glTranslatef(  3.0,  0.0,  0.0 );
    //glDrawArrays(GL_LINE_LOOP, 0, 5);
    glBegin(GL_LINE_LOOP);
    {
        glArrayElement(1);
        glArrayElement(4);
        glArrayElement(2);
        glArrayElement(0);
        glArrayElement(3);
       }
    glEnd();

    xRot += 10;
    yRot += 10;
    zRot += 10;

   // rTri += 10;
   // rQuad -= 10;
}
void GLWidget::resizeGL(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );//重置当前的视口

    //矩阵栈切换：glMatrixMode(GL_MODELVIEWING或GL_PROJECTION);
    //本命令执行后参数所指矩阵栈就成为当前矩阵栈，以后的矩阵栈操纵命令将作用于它。
    glMatrixMode( GL_PROJECTION );//选择投影矩阵。
    glLoadIdentity();//重置投影矩阵。

    //此处透视按照基于窗口宽度和高度的45度视角来计算。0.1，100.0是我们在场景中所能绘制深度的起点和终点。
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );//建立透视投影矩阵
    glMatrixMode( GL_MODELVIEW );//选择模型观察矩阵。
    glLoadIdentity();//重置模型观察矩阵
}
void GLWidget::mouseDoubleClickEvent( QMouseEvent *event )
{
    if(windowState() &  Qt::WindowFullScreen)
        showNormal();
    else
        showFullScreen();
}
void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_F2:
        if(windowState() &  Qt::WindowFullScreen)
        {
            showNormal();
            setGeometry( 0, 0, 640, 480 );
        }
        else
        {
          showFullScreen();
        }
        updateGL();
        break;
      default:
        break;
    }
}

void GLWidget::loadGLTextures()//载入纹理
{
  QImage tex, buf;
  if ( !buf.load( "./data/NeHe.bmp" ) )//载入纹理图片
  {
    qWarning( "Could not read image file, using single-color instead." );
    QImage dummy( 128, 128, QImage::Format_RGB32 );
    dummy.fill( Qt::green );//如果载入不成功，自动生成一个128*128的32位色的绿色图片。
    buf = dummy;
  }
  tex = QGLWidget::convertToGLFormat( buf );
//QGLWidget类中提供的一个静态函数converToGLFormat()，专门用来转换图片的。

  glGenTextures( 1, &texture[0] );//创建一个纹理。告诉OpenGL我们想生成一个纹理名字
  glBindTexture( GL_TEXTURE_2D, texture[0] );//纹理名字texture[0]绑定到纹理目标上
  //用来自位图数据生成的典型纹理。2D纹理只有高度（在Y轴上）和宽度（在X轴上）。
  //主函数将纹理名字指派给纹理数据。我们创建的纹理将存储在&texture[0]的指向的内存区域。

  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    //这里真正的创建纹理。GL_TEXTURE_2D:2D纹理。
    //数字零代表图像的详细程度，通常就由它为零去了。
    //数字三是数据的成分数。因为图像是由红色数据，绿色数据，蓝色数据三种组分组成。
    //tex.width()是纹理的宽度。tex.height()是纹理的高度。数字零是边框的值，一般就是零。
    //GL_RGBA 告诉OpenGL图像数据由红、绿、蓝三色数据以及alpha通道数据组成，这个是由于QGLWidget类的converToGLFormat()函数的原因。
    //GL_UNSIGNED_BYTE 意味着组成图像的数据是无符号字节类型的。
    //最后tex.bits()告诉OpenGL纹理数据的来源。

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  //上面的两行告诉OpenGL在显示图像时，当它比放大得原始的纹理大（GL_TEXTURE_MAG_FILTER）或缩小得比原始得纹理小（GL_TEXTURE_MIN_FILTER）时OpenGL采用的滤波方式。
  //通常这两种情况下我都采用GL_LINEAR。这使得纹理从很远处到离屏幕很近时都平滑显示。使用GL_LINEAR需要CPU和显卡做更多的运算。如果您的机器很慢，您也许应该采用GL_NEAREST。过滤的纹理在放大的时候，看起来斑驳的很。您也可以结合这两种滤波方式。在近处时使用GL_LINEAR，远处时GL_NEAREST。

}
