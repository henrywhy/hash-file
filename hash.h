#include <fstream>
using namespace std;


//�Զ�����߼���¼����ʽ
const int NULLTag = -20;	//�ؼ��벻���ڵı��
struct MyRecord{
	int key;				//�ؼ���
	char attribute[12];		//��������
};


//�߼��ϵ�һ��ҳ�飬������Դ洢���ɸ���¼
const int block_count = 4;	//һ��ҳ����ļ�¼��
struct Block{
	struct MyRecord data[block_count];
	int next;
};


//����ɢ���ļ���
//ɢ���ļ�=ɢ��Ŀ¼�ļ�+�����ļ�
const int len = 13;
class HashFile
{
private:
	char * dir_file_name;		//ͰĿ¼
	char * data_file_name;	//�����ļ�
	int directory[len];	//ɢ���ļ����ڴ���ά����Ŀ¼���Ǵ���Ŀ¼�ļ��Ŀ���
public:
	HashFile(char *dir_file_name, char *data_file_name);
	bool insert(MyRecord a_record);
	bool search(MyRecord &a_record_only_key);
};


