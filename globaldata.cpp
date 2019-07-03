#include "globaldata.h"

//实例化

int globaldata::set_listening_flag=0;
QString globaldata::current_path="";
QString globaldata::default_save_path="E:/record";
QString globaldata::strCssContent="";
int globaldata::client_flag=0;		//远程客户端连接标志
int globaldata::diskspace_flag=DiskspaceFlag::HaveSpace;	//磁盘空间未满；diskspace_flag=1：磁盘空间不足
QString globaldata::connectionUI="connectUI";
QString globaldata::connectionwork1="connectwork1";
QString globaldata::showfileinfo_conndb="showfileinfo_conndb";
int globaldata::LogCount=0;
QString globaldata::oldLogFile="";
