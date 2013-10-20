#include <fstream>
using namespace std;


//自定义的逻辑记录的形式
const int NULLTag = -20;	//关键码不存在的标记
struct MyRecord{
	int key;				//关键码
	char attribute[12];		//其他属性
};


//逻辑上的一个页块，里面可以存储若干个记录
const int block_count = 4;	//一个页块里的记录数
struct Block{
	struct MyRecord data[block_count];
	int next;
};


//定义散列文件类
//散列文件=散列目录文件+数据文件
const int len = 13;
class HashFile
{
private:
	char * dir_file_name;		//桶目录
	char * data_file_name;	//数据文件
	int directory[len];	//散列文件在内存中维护的目录，是磁盘目录文件的拷贝
public:
	HashFile(char *dir_file_name, char *data_file_name);
	bool insert(MyRecord a_record);
	bool search(MyRecord &a_record_only_key);
};


