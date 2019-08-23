﻿/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include<QMessageBox>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>



QVector<GLfloat> m_data;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_program(0)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }

    m_core = true;

    connect(&readFileTimer,SIGNAL(timeout()),this,SLOT(readFileSlot()));
//    readFileTimer.start(100);

    m_scale = 0.0;
    translate_x = 0;
    translate_y = 0;

    linkServer();
}

void GLWidget::linkServer()
{
    //    m_tcpSocket.connectToHost("10.0.1.221",6000);
    m_tcpSocket.connectToHost("127.0.0.1",6000);

    if(!m_tcpSocket.waitForConnected(300))
    {
//        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
        return;
    }else
    {
        //        QMessageBox::information(this, "QT网络通信", "ok");
    }
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(ClientRecvData()));
}

void GLWidget::ClientRecvData()
{
    quint32 mesg_len=0;
    qint64  readbyte = m_tcpSocket.bytesAvailable();
    if(readbyte<=0)
    {
        qDebug()<<QString::fromLocal8Bit("接收到的数据长度为空!")<<endl;
        return;
    }
    QByteArray buffer;
    quint16 flag;   //包头的标识 2个字节
    quint32 len;    //包头报文长度，json字符串的长度，4个字节
    buffer = m_tcpSocket.readAll();
    m_buffer.append(buffer);
    int totallen = m_buffer.size();


    while(totallen)
    {
        if(totallen < 6)    //不足6个字节（头部）
        {
            qDebug()<<QString::fromLocal8Bit("bagHead less six byte!!!!!");
            break;
        }

        //先进行包头判断，0x5A 0x5A , 因为有可能接收到的不是完整的报文
        int index = m_buffer.indexOf("ZZ");
        if(index<0)   //没有找到包头
        {
            qDebug()<<" 没有找到包头 "<<endl;
            m_buffer.clear();
            totallen = m_buffer.size();
            break;
        }else if(index>0)   //包头前面存在冗余
        {
            m_buffer = m_buffer.right(totallen - index);
            totallen = m_buffer.size();
        }


        QDataStream packet(m_buffer);
        packet>>flag>>len;       //获取长度 len  信息头暂时不用
        //      qDebug()<<" total ="<<totallen<<"   len="<<len<<endl;

        QByteArray json_Array;  //存储解析好的JSON 数据

        if(totallen-6>=len) //若是数据部分长度大于 或者等于 指定长度，说明后面可能有冗余数据
        {
            json_Array = m_buffer.mid(6,len);   //传递到下面做JSON数据解析
            qDebug()<<" frame ="<<json_Array<<endl;


            m_buffer = m_buffer.right(totallen-6-len);
            totallen = m_buffer.size();
            qDebug()<<QString::fromLocal8Bit("data Receive is ok or too much , m_buffer = ")<<m_buffer.size()<<endl;

        }
        else        //说明此时数据不够,等待下一次处理
        {
            break;
        }


        //以上数据为数据预处理


        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(json_Array, &jsonError);                     // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {                                                                                            // 解析未发生错误
            if (doucment.isObject()) {
                QJsonObject object = doucment.object();                                              // 转化为对象
                if (object.contains("table"))
                {                                                                                    // 包含指定的 key
                    QJsonValue val_flag = object.value("table");                                    // 获取指定 key 对应的 value
                    int flag = val_flag.toInt();
                    //                    qDebug() << "table " << flag;
                    if(1 == flag)              //query机型基本参数返回表
                    {
                        QJsonValue value_msg = object.value("msg");
                        if(value_msg.isArray())
                        {
                            QJsonArray msgArr = value_msg.toArray();
                            int size_ = msgArr.size();

                            int j = 0;
                            for(int i=0; i<size_; i++)
                            {

                                QJsonObject pointObject =  msgArr[i].toObject();
                                if (pointObject.contains("x") && pointObject.contains("y") && pointObject.contains("z"))
                                {
                                    QJsonValue val_x = pointObject.value("x");             // 获取指定 key 对应的 value
                                    float value_x = val_x.toDouble();

                                    QJsonValue val_y = pointObject.value("y");             // 获取指定 key 对应的 value
                                    float value_y= val_y.toDouble();

                                    QJsonValue val_z = pointObject.value("z");             // 获取指定 key 对应的 value
                                    float value_z= val_z.toDouble();

                                    //                                      qDebug()<<"x="<<value_x<<" y="<<value_x<<"  z="<<value_z<<endl;

                                    m_data[j] = value_x/10.0;
                                    m_data[j+1] = value_y/10.0;
                                    m_data[j+2] = value_z/10.0;

                                    j += 6;

                                }

                            }

                            m_data.resize(size_ * 6);
                        }     //if(value_msg.isArray())

                        update();    //刷新OPENGL 显示
                    }   //if(1 == flag)

                }  //if (object.contains("@table"))

            }else     //不是Json对象
            {
                qDebug()<<QString::fromLocal8Bit("the data is not a json object");

                break;
            }
        }else        //documnt is not null
        {
            qDebug()<<QString::fromLocal8Bit("data analyze error,can't translate to QJsonDocument file ~~~ ");
            break;
        }

    }//while
}


GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 800);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup()
{
    makeCurrent();
    m_logoVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

//static const char *vertexShaderSourceCore = NULL;
static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal =  normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

//static const char *fragmentShaderSourceCore = NULL;
static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39*NL, 1.0, 1.0*NL);\n"
    "   highp vec3 col = clamp(color + color * 0.8 * NL, 0.7, 1.0);\n"
    "   fragColor = vec4(vertNormal, 0.50);\n"
    "}\n";


static const char *vertexShaderSource =
        "attribute vec4 vertex;\n"
        "attribute vec3 normal;\n"
        "varying vec3 vert;\n"
        "varying vec3 vertNormal;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "uniform mat3 normalMatrix;\n"
        "void main() {\n"
        "   vert = vertex.xyz;\n"
        "   vertNormal = normalMatrix * normal;\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";


//static const char *fragmentShaderSource = NULL;
//static const char *fragmentShaderSource =
//        "varying highp vec3 vert;\n"
//        "varying highp vec3 vertNormal;\n"
//        "uniform highp vec3 lightPos;\n"
//        "void main() {\n"
//        "   highp vec3 L = normalize(lightPos - vert);\n"
//        "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
//        "   highp vec3 color = vec3(1.0, 1.0, 1.0);\n"
//        "   highp vec3 col = clamp(color * 0.8 + color * 0.8 * NL, 0.0, 1.0);\n"
//        "   gl_FragColor = vec4(col, 1.0);\n"
//        "}\n";

static const char *fragmentShaderSource =
        "varying highp vec3 vert;\n"
        "varying highp vec3 vertNormal;\n"
        "uniform highp vec3 lightPos;\n"
        "void main() {\n"
        "   highp vec3 L = normalize(lightPos - vert);\n"
        "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
        "   highp vec3 color = vec3(0.8, 1.0, 1.0);\n"
        "   highp vec3 col = clamp(color * 0.8 + color * 0.8 * NL, 0.9, 1.0);\n"
        "   gl_FragColor = vec4(0.0,1.0,0.0, 1.0);\n"
        "}\n";

void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -50);
    //    m_camera.translate(0.0,0.0, -10);    //相当于平移相机的位置

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(10, 10, 10));

    m_program->release();


    verticalView_slot();
}

void GLWidget::setupVertexAttribs()
{
    m_logoVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_logoVbo.release();
}

void GLWidget::paintGL()
{
    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();

    m_world.translate(translate_x,translate_y,0);

    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    float scale = 1.0 + m_scale;
    if(scale >0)
    {
        m_world.scale(scale, scale, scale);
    }else
    {
        m_world.scale(0.01, 0.01, 0.01);
    }




    //    m_world.scale(1.0+0.5, 1.0+0.5, 5.0+0.5);


    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    //    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());
    glDrawArrays(GL_POINTS, 0, m_logo.vertexCount());

    //    glDrawArrays(GL_LINES, m_logo.vertexCount()-500, m_logo.vertexCount());

    m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    //    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);

    m_proj.perspective(45.0f, GLfloat(w) / h, 0.1f, 1000.0f);

    //gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)

    //fovy, 这个最难理解,我的理解是,眼睛睁开的角度,即,视角的大小,如果设置为0,相当你闭上眼睛了,所以什么也看不到,如果为180,那么可以认为你的视界很广阔,
    //aspect, 这个好理解,就是实际窗口的纵横比,即x/y
    //zNear, 这个呢,表示你近处,的裁面,
    //zFar, 表示远处的裁面,
}


//鼠标按下事件
void GLWidget::mousePressEvent(QMouseEvent *event)
{

    m_lastPos = event->pos();
//    qDebug()<<"m_lastPos ="<<m_lastPos<<endl;


}

// 中键释放事件
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
//    if(event->button() & Qt::MidButton)
//    {
//        translate_x = (event->x() - m_lastPos.x())/50.0;
//        translate_y = -(event->y() - m_lastPos.y())/30.0;
//        qDebug()<<"translate_x = "<<translate_x<<"  translate_y="<<translate_y<<endl;
//    }
}



//鼠标移动事件
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //    m_logo.readPCDFile();

    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    qDebug()<<" x="<<m_xRot + 8 * dy<<",y="<<m_yRot + 8 * dx<<",z="<<m_zRot + 8 * dx<<endl;


    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
        m_lastPos = event->pos();
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
        m_lastPos = event->pos();
    }else if(event->buttons() & Qt::MidButton)
    {
        float temp_x = (event->x() - m_lastPos.x())/50.0;
        float temp_y = -(event->y() - m_lastPos.y())/30.0;

//        translate_x = temp_x + translate_x;
//        translate_y = temp_y + translate_y;

        translate_x = temp_x ;
        translate_y = temp_y ;

//        qDebug()<<"translate_X ="<<translate_x<<"   translate_y = "<<translate_y<<endl;

        update();
    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0){//如果滚轮往上滚
        //        qDebug()<<"已经检测到向上滚轮"<<endl;
        m_scale += 0.03*10;
        update();

    }else{//同样的 如果向下滚轮
        //        qDebug()<<"已经检测到向下滚轮..."<<endl;
        m_scale -= 0.03*10;
        update();
    }
}

void GLWidget::readFileSlot()
{
    m_logo.readPCDFile1();

    update();

//    qDebug()<<"here update"<<endl;
}


void GLWidget::frontView_slot()
{
    setXRotation(1456);
    setYRotation(2888);
    setZRotation(2880);

}
void GLWidget::endView_slot()
{
    setXRotation(1384);
    setYRotation(2824);
    setZRotation(1456);

}
void GLWidget::verticalView_slot()
{
    //仰视图
//    setXRotation(240);
//    setYRotation(2896);
//    setZRotation(0);

    //俯视图
    setXRotation(2736);
    setYRotation(5712);
    setZRotation(0);
}


