#include "globaldata.h"

//ʵ����

int globaldata::set_listening_flag=0;
QString globaldata::current_path="";
QString globaldata::default_save_path="E:/record";
QString globaldata::strCssContent="";
int globaldata::client_flag=0;		//Զ�̿ͻ������ӱ�־
int globaldata::diskspace_flag=DiskspaceFlag::HaveSpace;	//���̿ռ�δ����diskspace_flag=1�����̿ռ䲻��
QString globaldata::connectionUI="connectUI";
QString globaldata::connectionwork1="connectwork1";
QString globaldata::showfileinfo_conndb="showfileinfo_conndb";
int globaldata::LogCount=0;
QString globaldata::oldLogFile="";
