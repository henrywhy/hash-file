#include <iostream>
#include "hash.h"
using namespace std;


int main(int argc, char *argv[])
{
	char * dir_file_name = "myfile.has";
	char * data_file_name = "myfile.dat";
	HashFile myfile(dir_file_name, data_file_name);
	while(1) {
		cout<<"�����б�"<<endl;		
		cout<<"1������һ����¼"<<endl;
		cout<<"2��������¼"<<endl;
		int choose;
		struct MyRecord tmp_record;
		cin>>choose;
		switch(choose)
		{
		case 1:
			cout<<"����key��attribute : ";
			cin>>tmp_record.key>>tmp_record.attribute;
			myfile.insert(tmp_record);
			break;
		case 2:
			cout<<"�����ѯkey : ";
			cin>>tmp_record.key;
			if(myfile.search(tmp_record)) {
				cout<<"��ѯ���: "<<tmp_record.key<<" : "<<tmp_record.attribute<<endl;
			} else {
				cout<<"�޼�¼��"<<endl;
			}
			break;
		default:
			cout<<"������ȷ���"<<endl;
		}
	}

	return 0;
}
