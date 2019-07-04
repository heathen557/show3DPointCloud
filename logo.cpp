/****************************************************************************
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

#include "logo.h"
#include <qmath.h>
#include<QDebug>
#include<math.h>
#include<QFile>


static char *strsep(char **s, const char *ct)
{
    char *sbegin = *s;
    char *end;

    if (sbegin == NULL)
        return NULL;

    end = strpbrk(sbegin, ct);
    if (end)
        *end++ = '\0';
    *s = end;
    return sbegin;
}

Logo::Logo()
    : m_count(0)
{
    m_data.resize(2500 * 6 *10);

    index = 1;

//    m_data.resize(100 * 6);
//    const GLfloat x1 = +0.06f;
//    const GLfloat y1 = -0.14f;
//    const GLfloat x2 = +0.14f;
//    const GLfloat y2 = -0.06f;
//    const GLfloat x3 = +0.08f;
//    const GLfloat y3 = +0.0000f;
//    const GLfloat x4 = +0.30f;
//    const GLfloat y4 = +0.22f;

    const GLfloat x1 = +0.0f;
    const GLfloat y1 = -0.0f;
    const GLfloat x2 = +0.0f;
    const GLfloat y2 = -0.0f;
    const GLfloat x3 = +0.0f;
    const GLfloat y3 = +0.0f;
    const GLfloat x4 = +0.0f;
    const GLfloat y4 = +0.0f;

    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);

    const int NumSectors = 1000;

    for (int i = 0; i < NumSectors; ++i) {
        GLfloat angle = (i * 2 * M_PI) / NumSectors;
        GLfloat angleSin = qSin(angle);
        GLfloat angleCos = qCos(angle);
//        const GLfloat x5 = 0.30f * angleSin;
//        const GLfloat y5 = 0.30f * angleCos;
//        const GLfloat x6 = 0.20f * angleSin;
//        const GLfloat y6 = 0.20f * angleCos;

        const GLfloat x5 = 0.0f * angleSin;
        const GLfloat y5 = 0.0f * angleCos;
        const GLfloat x6 = 0.0f * angleSin;
        const GLfloat y6 = 0.0f * angleCos;

        angle = ((i + 1) * 2 * M_PI) / NumSectors;
        angleSin = qSin(angle);
        angleCos = qCos(angle);
//        const GLfloat x7 = 0.20f * angleSin;
//        const GLfloat y7 = 0.20f * angleCos;
//        const GLfloat x8 = 0.30f * angleSin;
//        const GLfloat y8 = 0.30f * angleCos;

        const GLfloat x7 = 0.0f * angleSin;
        const GLfloat y7 = 0.0f * angleCos;
        const GLfloat x8 = 0.0f * angleSin;
        const GLfloat y8 = 0.0f * angleCos;

        quad(x5, y5, x6, y6, x7, y7, x8, y8);

        extrude(x6, y6, x7, y7);
        extrude(x8, y8, x5, y5);
    }

    readPCDFile();
//    qDebug()<<"m_data's size="<<m_data.size()<<endl;

    double x_min=100,y_min=100,z_min=100;
    double x_max=0,y_max=0,z_max=0;

    for(int i=0; i<m_data.size(); i+=6)
    {
//        qDebug()<<m_data[i]<<" "<<m_data[i+1]<<" "<<m_data[2+i]<<" "<<m_data[3+i]<<"  "<<m_data[4+i]<<"  "<<m_data[5+i]<<endl;
        m_data[3+i] = 0;
        m_data[4+i] = 0;
        m_data[5+i] = 0;

        if(m_data[i]>x_max)
            x_max = m_data[i];
        if(m_data[i]<x_min)
            x_min = m_data[i];

        if(m_data[i+1]>y_max)
            y_max = m_data[1+i];
        if(m_data[1+i]<y_min)
            y_min = m_data[i+1];

        if(m_data[i+2]>z_max)
            z_max = m_data[i];
        if(m_data[i+2]<z_min)
            z_min = m_data[i+2];
    }

    qDebug()<<"x_max="<<x_max<<"  x_min="<<x_min<<endl;
    qDebug()<<"y_max="<<y_max<<"  y_min="<<y_min<<endl;
    qDebug()<<"z_max="<<z_max<<"  z_min="<<z_min<<endl;
}

void Logo::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}

void Logo::quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4)
{
    QVector3D n = QVector3D::normal(QVector3D(x4 - x1, y4 - y1, 0.0f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, -0.0f), n);
    add(QVector3D(x4, y4, -0.0f), n);
    add(QVector3D(x2, y2, -0.0f), n);

    add(QVector3D(x3, y3, -0.0f), n);
    add(QVector3D(x2, y2, -0.0f), n);
    add(QVector3D(x4, y4, -0.0f), n);

    n = QVector3D::normal(QVector3D(x1 - x4, y1 - y4, 0.0f), QVector3D(x2 - x4, y2 - y4, 0.0f));

    add(QVector3D(x4, y4, 0.0f), n);
    add(QVector3D(x1, y1, 0.0f), n);
    add(QVector3D(x2, y2, 0.0f), n);

    add(QVector3D(x2, y2, 0.0f), n);
    add(QVector3D(x3, y3, 0.0f), n);
    add(QVector3D(x4, y4, 0.0f), n);
}

void Logo::extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    QVector3D n = QVector3D::normal(QVector3D(0.0f, 0.0f, -0.1f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, +0.0f), n);
    add(QVector3D(x1, y1, -0.0f), n);
    add(QVector3D(x2, y2, +0.0f), n);

    add(QVector3D(x2, y2, -0.0f), n);
    add(QVector3D(x2, y2, +0.0f), n);
    add(QVector3D(x1, y1, -0.0f), n);
}

//@brief:读取本地的pcd文件 ,
//获取三维坐标x,y,z
void Logo::readPCDFile()
{
//    double max = 0;
//     pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//     char strfilepath[256] = "position1.pcd";
//     if (-1 == pcl::io::loadPCDFile(strfilepath, *cloud)) {
//         qDebug()<<"读取PCD文件失败"<<endl;
//         return ;
//     }
//     int pointSize = cloud->points.size();

//     m_data.resize(pointSize * 6);
//     qDebug()<<QString::fromUtf8("dian点云数量")<<pointSize<<endl;
//     int j = 0;
//     for(int i=0; i<pointSize; i++)
//     {
//         float x = cloud->points[i].x/100.0;
//         float y = cloud->points[i].y/100.0;
//         float z = cloud->points[i].z/100.0;

//         m_data[j] = x;
//         m_data[j+1] = y;
//         m_data[j+2] = z;
//         j =j+6;
//     }


//     qDebug()<<QString::fromUtf8("m_data点云数量")<<m_data.size()<<endl;
}


void Logo::readPCDFile1()
{
    double max = 0;
//     pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//     char strfilepath[256] = "position1.pcd";
//     if (-1 == pcl::io::loadPCDFile(strfilepath, *cloud)) {
//         qDebug()<<"读取PCD文件失败"<<endl;
//         return ;
//     }
//     int pointSize = cloud->points.size();



//    QString fileName = "12-25/" + QString::number(index) + ".pcd";
      QString fileName = "PCD2/position" + QString::number(index) + ".pcd";

    index++;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QIODevice::Text))
    {
//        qDebug()<<"readPCDFile read over!"<<endl;
         return;
    }

    uchar* fpr = file.map(0, file.size());
    int cnt = 0;
    int subcnt = 0;
    char *substr;
    char *s = strdup((char*)fpr);

    int i = 0;
    while(substr= strsep(&s, "\n"))
    {
        cnt++;
        if(cnt<12)
            continue;

        char *lineSubStr;
        subcnt = 0;
        while(subcnt < 3)
        {
            lineSubStr = strsep(&substr, " ");
//            point[cnt][subcnt] = atof(lineSubStr);
//            qDebug()<< lineSubStr << ",index = "<<cnt<<endl;
            QString str = QString(lineSubStr);
//            if(subcnt == 2)
//            {
//            }else if(subcnt == 0)
//            {
//            }else if(subcnt == 1)
//            {
//            }

            m_data[i+subcnt] = str.toFloat()/100.0;

            subcnt++;
        }

        i += 6;
    }


     m_data.resize(cnt * 6);

//     qDebug()<<"data's size = "<<m_data.size()<<endl;

//     qDebug()<<QString::fromUtf8("dian点云数量")<<pointSize<<endl;
//     int j = 0;
//     for(int i=0; i<subcnt; i++)
//     {
//         float x = cloud->points[i].x/100.0;
//         float y = cloud->points[i].y/100.0;
//         float z = cloud->points[i].z/100.0;

//         m_data[j] = x;
//         m_data[j+1] = y;
//         m_data[j+2] = z;
//         j =j+6;
//     }


//     qDebug()<<QString::fromUtf8("m_data点云数量")<<m_data.size()<<endl;
}
