#include "hash.h"
#include <iostream>
using namespace std;


//���ʵ��
HashFile::HashFile(char *dir_file_name, char *data_file_name)
{
	this->dir_file_name = dir_file_name;
	this->data_file_name = data_file_name;	

	for(int i=0; i<len; i++) 
		this->directory[i] = -1;
	
}


//����һ����¼
bool HashFile::insert(MyRecord a_record)
{
	bool flag = false;	//����ɹ���־
	
	fstream dir_file(dir_file_name, ios::in|ios::out|ios::binary);
	if(!dir_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}
	dir_file.read((char *)directory, sizeof(directory));		//��hashͰĿ¼���뻺����
	fstream data_file(data_file_name, ios::in|ios::out|ios::binary);
	if(!data_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}

	int hash_value = a_record.key % len;	//����ؼ����Ӧ��hashֵ
	
	//����һ���߼�ҳ��,����ʼ��ҳ���е�ֵ
	struct Block tmp_block;					
	for(int i=0; i<block_count; i++)
	{
		tmp_block.data[i].key = NULLTag;
	}
	tmp_block.next = -1;

	if(directory[hash_value] == -1)
	{
		tmp_block.data[0] = a_record;	//�¼�¼����ҳ���ײ�
		data_file.seekp(0, ios::end);		//�����ļ�ָ���λ�õ��ļ�ĩβ
		data_file.write((char *)&tmp_block, sizeof(struct Block));		//���߼�ҳ����뵽�����ļ���ĩβ
		directory[hash_value] = data_file.tellp() / sizeof(Block) - 1;	//��ͰĿ¼��ָ�������ͷ�ڵ�		
		flag = true;
		goto END;
	} else {
		//��������ͻʱ�������������ҵ����һ���߼�ҳ�鲢��������ʱ�ڴ�ҳ����
		int node_number_tmp = directory[hash_value];
		while(1)
		{
			//���������ڴ�
			data_file.seekg(node_number_tmp*sizeof(Block), ios::beg);
			data_file.read((char *)&tmp_block, sizeof(struct Block));
			if(tmp_block.next == -1)
				break;
			else {node_number_tmp = tmp_block.next;}
		}
		
		//�������һ���߼�ҳ�飬�ҵ����ʵ�λ�ò����¼�����½�ҳ������¼
		for(int i=0; i<block_count; i++)
		{
			if(tmp_block.data[i].key==NULLTag) {
				tmp_block.data[i] = a_record;
				data_file.seekp(node_number_tmp*sizeof(Block), ios::beg);		//�����ļ�ָ���λ�õ��Ľ���λ��
				data_file.write((char *)&tmp_block, sizeof(struct Block));		//���߼�ҳ����뵽�����ļ���ĩβ
				flag = true;
				goto END;
			} 
		}

		//���һ�������������Ҫ����һ���µ��߼�ҳ��
		struct Block tmp_block_new;
		for(int i=0; i<block_count; i++)
		{
			tmp_block_new.data[i].key = NULLTag;
		}
		tmp_block_new.next = -1;
		
		//Ȼ���������£�1��������ҳ�� 2������������Ϣ 3���������ڶ�������λ
		tmp_block_new.data[0] = a_record;
		data_file.seekp(0, ios::end);		//�����ļ�ָ���λ�õ��ļ�ĩβ
		data_file.write((char *)&tmp_block_new, sizeof(struct Block));		//���µ��߼�ҳ����뵽�����ļ���ĩβ
		tmp_block.next = data_file.tellp() / sizeof(Block) - 1;			//��������ĩβ��Ϣ

		data_file.seekp(node_number_tmp*sizeof(Block), ios::beg);
		data_file.write((char *)&tmp_block, sizeof(Block));
		flag = true;	
		goto END;
	}

	//��hashͰĿ¼���ص����̲��ر��ļ���
	//�ٴ�ʹ���ļ�����д֮ǰ������ļ�����״̬����Ϊ�����ܵ�һ��û�����κζ���������û׼����
END:
	dir_file.clear();		
	dir_file.seekp(0, ios::beg);
	dir_file.write((char *)directory, sizeof(directory));
	dir_file.close();
	data_file.close();
	return flag;
}

//����һ����¼
bool HashFile::search(MyRecord &a_record_only_key)
{
	bool flag = false;

	ifstream dir_file(dir_file_name, ios::in|ios::binary);
	if(!dir_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}
	dir_file.read((char *)directory, sizeof(directory));		//��hashͰĿ¼���뻺����
	ifstream data_file(data_file_name, ios::in|ios::binary);
	if(!data_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}

	int hash_value = a_record_only_key.key % len;	//����ؼ����Ӧ��hashֵ

	if(directory[hash_value] == -1) {
		goto END;
	} else {
		//���������ҹؼ���
		struct Block tmp_block;
		int tmp_node_num = directory[hash_value];
		while(1)
		{
			data_file.clear();
			data_file.seekg(tmp_node_num*sizeof(Block), ios::beg);
			data_file.read((char *)&tmp_block, sizeof(Block));
			for(int i=0; i<block_count; i++)
			{
				if(tmp_block.data[i].key == a_record_only_key.key){
					strcpy(a_record_only_key.attribute, tmp_block.data[i].attribute);
					flag = true;
					goto END;
				}
			}

			//�ν��û���ҵ��ü�¼
			if(tmp_block.next == -1){
				goto END;
			} else {
				tmp_node_num = tmp_block.next;			
			}
		}

	}

END:
	dir_file.close();
	data_file.close();
	return flag;
}