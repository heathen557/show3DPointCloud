#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include<QString>
enum DiskspaceFlag
{
	NoSpace,
	HaveSpace
};
class globaldata
{
public:
	static int set_listening_flag;
	static QString current_path;
	static QString default_save_path;
	static QString strCssContent;
	static int client_flag;
	static int diskspace_flag;
	static QString connectionUI;
	static QString connectionwork1;
    static QString showfileinfo_conndb;
	static int LogCount;
	static QString oldLogFile;
};

#endif 
