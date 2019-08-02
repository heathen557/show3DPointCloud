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
//#include <qmath.h>
//#include<QDebug>
//#include<math.h>
//#include<QFile>
//#include <pcl/filters/statistical_outlier_removal.h>


//#include<pcl/visualization/cloud_viewer.h>
//#include<iostream>//标准C++库中的输入输出类相关头文件。
//#include<pcl/io/io.h>
//#include<pcl/io/pcd_io.h>//pcd 读写类相关的头文件。
//#include<pcl/io/ply_io.h>
//#include<pcl/point_types.h> //PCL中支持的点类型头文件。
//#include <iostream>

//#include <boost/thread/thread.hpp>
//#include <pcl/common/common_headers.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <pcl/console/parse.h>
//#include <pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/kdtree/kdtree_flann.h>
//#include <pcl/filters/bilateral.h>
//#include <pcl/filters/radius_outlier_removal.h>
//#include <pcl/filters/statistical_outlier_removal.h>
//#include<pcl/filters/fast_bilateral_omp.h>

//#include <pcl/kdtree/flann.h>
//#include <pcl/kdtree/kdtree.h>
//#include <pcl/search/flann_search.h>
//#include <pcl/search/kdtree.h>
//#include <pcl/range_image/range_image.h>
//#include <pcl/visualization/range_image_visualizer.h>

//#include <pcl/filters/passthrough.h>  //直通滤波相关

////多线程
// #include <boost/thread/thread.hpp>
// #include <fstream>
// #include <iostream>
// #include <stdio.h>
// #include <string.h>
// #include <string>
// //计时
// #include <time.h>
// //Bilateral Filter
// #include <pcl/filters/bilateral.h>//required
// #include <pcl/filters/fast_bilateral.h>
// #include <pcl/filters/fast_bilateral_omp.h>
//#include<QTime>


pcl::PointCloud<pcl::PointXYZRGB> pointCloudRgb;
bool  isShowPointCloud;
extern QMutex mutex;

//字符串解析，读取pcd文件时候用
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

Logo::Logo(QObject *parent):
    QObject(parent)
{
    m_data.resize(250000 * 6 *10);

    index = 1;
    m_count = 0;
    isShowPointCloud = false;

    const int NumSectors = 10000;

    for (int i = 0; i < NumSectors; ++i) {

//        m_data.append(0.0);
        GLfloat *p = m_data.data() + m_count;
        *p++ = 10000;
        *p++ = 10000;;
        *p++ = 10000;;
        *p++ = 1.0;
        *p++ = 1.0;
        *p++ = 1.0;
        m_count += 6;

    }

//    readPCDFile();

    double x_min=100,y_min=100,z_min=100;
    double x_max=0,y_max=0,z_max=0;

    for(int i=0; i<m_data.size(); i+=6)
    {

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

//    qDebug()<<"x_max="<<x_max<<"  x_min="<<x_min<<endl;
//    qDebug()<<"y_max="<<y_max<<"  y_min="<<y_min<<endl;
//    qDebug()<<"z_max="<<z_max<<"  z_min="<<z_min<<endl;
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
    double max = 0;
     pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
     char strfilepath[256] = "position2.pcd";
     if (-1 == pcl::io::loadPCDFile(strfilepath, *cloud)) {
         qDebug()<<"读取PCD文件失败"<<endl;
         return ;
     }
     int pointSize = cloud->points.size();

     m_data.resize(pointSize * 6);
     qDebug()<<QString::fromUtf8("dian点云数量")<<pointSize<<endl;
     int j = 0;
     for(int i=0; i<pointSize; i++)
     {
         float x = cloud->points[i].x/1.0;
         float y = cloud->points[i].y/1.0;
         float z = cloud->points[i].z/1.0;

         m_data[j] = x;
         m_data[j+1] = y;
         m_data[j+2] = z;
         m_data[j+3] = 1.0;
         m_data[j+4] = 0;
         m_data[j+5] = 0;

         j =j+6;
     }

     qDebug()<<QString::fromUtf8("m_data点云数量")<<m_data.size()<<endl;
}

//连续读取pcd文件的测试槽函数
void Logo::readPCDFile1()
{
    if(!isShowPointCloud)
        return;

    mutex.lock();
//    pcl::copyPointCloud(pointCloudRgb,needDealCloud_rgb);
    pcl::copyPointCloud(pointCloudRgb,DealedCloud_rgb);
    mutex.unlock();

/*
    //  基于统计运算的滤波算法
    QTime t1 = QTime::currentTime();
    //qDebug()<<"BEGIN = "<< t1.toString("hh:mm:ss.zzz")<<endl;
    pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor;
    sor.setInputCloud(needDealCloud_rgb.makeShared());
    sor.setMeanK(20);
    sor.setStddevMulThresh(0.001);
    sor.filter(DealedCloud_rgb);
    t1 = QTime::currentTime();
    //qDebug()<<"END = "<< t1.toString("hh:mm:ss.zzz")<<endl;

*/

//    qDebug()<<"the pointCloud num =  "<<DealedCloud.points.size()<<endl;
    int m = 0;
    for(int n=0; n<DealedCloud_rgb.points.size(); n++)
    {
        m_data[0+m] = DealedCloud_rgb.points[n].x;
        m_data[1+m] = DealedCloud_rgb.points[n].y;
        m_data[2+m] = DealedCloud_rgb.points[n].z;

         nrgb = *reinterpret_cast<int*>(&DealedCloud_rgb.points[n].rgb);
         nr = (nrgb >> 16) & 0x0000ff;
         ng = (nrgb >> 8) & 0x0000ff;
         nb = (nrgb) & 0x0000ff;

        m_data[3+m] = nr/255.0;
        m_data[4+m] = ng/225.0;
        m_data[5+m] = nb/255.0;


        m += 6;
    }
    m_data.resize(m);

}




Logo::~Logo()
{

}
