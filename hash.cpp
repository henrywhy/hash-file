#include "hash.h"
#include <iostream>
using namespace std;


//类的实现
HashFile::HashFile(char *dir_file_name, char *data_file_name)
{
	this->dir_file_name = dir_file_name;
	this->data_file_name = data_file_name;	

	for(int i=0; i<len; i++) 
		this->directory[i] = -1;
	
}


//插入一条记录
bool HashFile::insert(MyRecord a_record)
{
	bool flag = false;	//插入成功标志
	
	fstream dir_file(dir_file_name, ios::in|ios::out|ios::binary);
	if(!dir_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}
	dir_file.read((char *)directory, sizeof(directory));		//将hash桶目录读入缓冲区
	fstream data_file(data_file_name, ios::in|ios::out|ios::binary);
	if(!data_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}

	int hash_value = a_record.key % len;	//计算关键码对应的hash值
	
	//申请一个逻辑页块,并初始化页块中的值
	struct Block tmp_block;					
	for(int i=0; i<block_count; i++)
	{
		tmp_block.data[i].key = NULLTag;
	}
	tmp_block.next = -1;

	if(directory[hash_value] == -1)
	{
		tmp_block.data[0] = a_record;	//新记录插入页块首部
		data_file.seekp(0, ios::end);		//调整文件指针的位置到文件末尾
		data_file.write((char *)&tmp_block, sizeof(struct Block));		//将逻辑页块插入到数据文件的末尾
		directory[hash_value] = data_file.tellp() / sizeof(Block) - 1;	//让桶目录项指向单链表表头节点		
		flag = true;
		goto END;
	} else {
		//当产生冲突时，遍历单链表，找到最后一个逻辑页块并拷贝到临时内存页块中
		int node_number_tmp = directory[hash_value];
		while(1)
		{
			//交换结点进内存
			data_file.seekg(node_number_tmp*sizeof(Block), ios::beg);
			data_file.read((char *)&tmp_block, sizeof(struct Block));
			if(tmp_block.next == -1)
				break;
			else {node_number_tmp = tmp_block.next;}
		}
		
		//遍历最后一个逻辑页块，找到合适的位置插入记录或者新建页块插入记录
		for(int i=0; i<block_count; i++)
		{
			if(tmp_block.data[i].key==NULLTag) {
				tmp_block.data[i] = a_record;
				data_file.seekp(node_number_tmp*sizeof(Block), ios::beg);		//调整文件指针的位置到改结点的位置
				data_file.write((char *)&tmp_block, sizeof(struct Block));		//将逻辑页块插入到数据文件的末尾
				flag = true;
				goto END;
			} 
		}

		//最后一个结点已满，需要开辟一个新的逻辑页块
		struct Block tmp_block_new;
		for(int i=0; i<block_count; i++)
		{
			tmp_block_new.data[i].key = NULLTag;
		}
		tmp_block_new.next = -1;
		
		//然后做三件事：1、插入新页块 2、更新链表信息 3、将倒数第二个结点归位
		tmp_block_new.data[0] = a_record;
		data_file.seekp(0, ios::end);		//调整文件指针的位置到文件末尾
		data_file.write((char *)&tmp_block_new, sizeof(struct Block));		//将新的逻辑页块插入到数据文件的末尾
		tmp_block.next = data_file.tellp() / sizeof(Block) - 1;			//更新链表末尾信息

		data_file.seekp(node_number_tmp*sizeof(Block), ios::beg);
		data_file.write((char *)&tmp_block, sizeof(Block));
		flag = true;	
		goto END;
	}

	//将hash桶目录读回到磁盘并关闭文件流
	//再次使用文件流读写之前先清除文件流的状态，因为它可能第一次没读到任何东西，导致没准备好
END:
	dir_file.clear();		
	dir_file.seekp(0, ios::beg);
	dir_file.write((char *)directory, sizeof(directory));
	dir_file.close();
	data_file.close();
	return flag;
}

//检索一条记录
bool HashFile::search(MyRecord &a_record_only_key)
{
	bool flag = false;

	ifstream dir_file(dir_file_name, ios::in|ios::binary);
	if(!dir_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}
	dir_file.read((char *)directory, sizeof(directory));		//将hash桶目录读入缓冲区
	ifstream data_file(data_file_name, ios::in|ios::binary);
	if(!data_file)
	{
		cout<<"open file error!"<<endl;
		exit(1);
	}

	int hash_value = a_record_only_key.key % len;	//计算关键码对应的hash值

	if(directory[hash_value] == -1) {
		goto END;
	} else {
		//搜索链表找关键码
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

			//次结点没有找到该记录
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