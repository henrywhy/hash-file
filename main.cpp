#include <iostream>
#include "hash.h"
using namespace std;


int main(int argc, char *argv[])
{
	char * dir_file_name = "myfile.has";
	char * data_file_name = "myfile.dat";
	HashFile myfile(dir_file_name, data_file_name);
	while(1) {
		cout<<"功能列表"<<endl;		
		cout<<"1、插入一条记录"<<endl;
		cout<<"2、检索记录"<<endl;
		int choose;
		struct MyRecord tmp_record;
		cin>>choose;
		switch(choose)
		{
		case 1:
			cout<<"输入key和attribute : ";
			cin>>tmp_record.key>>tmp_record.attribute;
			myfile.insert(tmp_record);
			break;
		case 2:
			cout<<"输入查询key : ";
			cin>>tmp_record.key;
			if(myfile.search(tmp_record)) {
				cout<<"查询结果: "<<tmp_record.key<<" : "<<tmp_record.attribute<<endl;
			} else {
				cout<<"无记录！"<<endl;
			}
			break;
		default:
			cout<<"输入正确序号"<<endl;
		}
	}

	return 0;
}
