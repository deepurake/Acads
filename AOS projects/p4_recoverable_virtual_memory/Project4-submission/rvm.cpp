#include "rvm.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <assert.h> 
#include <fstream>
#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

segment *get_segment(rvm_t rvm,void * seg_ptr);
using namespace std;

bool dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}
rvm_t rvm_init(const char *directory)
{
	bool exists = dirExists(directory);
	char * mydir = (char *)malloc(100);
	strcpy(mydir,get_current_dir_name());
	strcat(mydir,"/");
	strcat(mydir,directory);

	
	// Create directory
	mkdir(mydir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	strcat(mydir,"/");


	char log_file[100];

	strcpy(log_file,mydir);
	strcat(log_file,"log");

	// create the data struct for maintaining rvm
	rvm_t my_rvm = (rvm_t)malloc(sizeof(rvm_struct));
	my_rvm = new (my_rvm) rvm_struct;
	my_rvm->directory =  (const char *)mydir;
	map<const char *,void *> mapping;
	my_rvm->mapping = mapping;

	if(exists)
	{
		rvm_truncate_log(my_rvm);
	}
	return (rvm_t)my_rvm;
}
int get_size(const char * file_path)
{
	if(!is_file_exist(file_path))
	{
		return 0;
	}
	FILE * file = fopen(file_path,"r+");

	fseek(file, 0, SEEK_END);
	int file_size = ftell(file);

	fclose(file);
	return file_size;
}
void *rvm_map(rvm_t rvm, const char *segname, int size_to_create)
{

	segment* myseg = (segment *)malloc(sizeof(segment));
	myseg = new (myseg) segment;
	void* new_ptr = malloc(size_to_create);

	bool exists = (rvm->mapping.find(segname)!= rvm->mapping.end());
	// copy old memory if exists and delete old memory
	if(exists)
	{
		memcpy(new_ptr,(const void *)myseg->data,myseg->size);
		rvm->mapping[segname] = (void *)myseg;
	}
	else
	{
		char file_path[100];
		strcpy(file_path,rvm->directory);
		strcat(file_path,segname);
		if(is_file_exist(file_path))
		{
			rvm_truncate_log(rvm);
			int size_to_read = size_to_create;

			
			int file_size = get_size(file_path);
			if(size_to_read > file_size)
			{
				size_to_read = file_size;
			}

			FILE * file = fopen(file_path,"r+");
			fread(new_ptr,sizeof(char),size_to_read,file);
			fclose(file);
		}
		rvm->mapping.insert(pair<const char *,void *>(segname,(void *)myseg));
	}

	// update params
	myseg->seg_name = segname;
	myseg->size = size_to_create;
	myseg->data = new_ptr;
	myseg->mod = false;
	myseg->myrvm = rvm;

	char file_path[100];
	strcpy(file_path,rvm->directory);
	strcat(file_path,segname);
	FILE * data = fopen(file_path,"w");
	fwrite(myseg->data,sizeof(char),size_to_create,data);
	fclose(data);	

	return (void *)myseg->data;
}
void rvm_unmap(rvm_t rvm, void *segbase)
{
	// remove mapping
	segment* myseg = get_segment(rvm,segbase);
	rvm->mapping.erase(myseg->seg_name);
	free(myseg->data);
	free(myseg->tid);
	free(myseg);
}
void rvm_destroy(rvm_t rvm, const char *segname)
{
	// Delete the file
	char file_path[100];
	strcpy(file_path,rvm->directory);
	strcat(file_path,segname);
}

segment *get_segment(rvm_t rvm,void * seg_ptr)
{
	std::map<const char *,void *>::iterator iter;
 	for (iter = rvm->mapping.begin(); iter != rvm->mapping.end(); iter++) {
 		if(((segment *)(iter->second))->data == seg_ptr)
 		{
 			return (segment *)(iter->second);
 		}
 	}
 	return NULL;
}

trans_t rvm_begin_trans(rvm_t rvm, int numsegs, void **segbases)
{
	for(int i = 0; i<numsegs; i++)
	{
		// validate that no segment pointer is being modified
		segment *ptr = get_segment(rvm,segbases[i]);
		assert(ptr != NULL);
		if(ptr->mod)
		{
			return (trans_t)-1;
		}
	}
	trans_t tid = (trans_t)malloc(sizeof(transaction));
	tid = new (tid) transaction;
	tid->myrvm = rvm;

	// set mod values 
	for(int i = 0; i<numsegs; i++)
	{
		// validate that no segment pointer is being modified
		segment *ptr = get_segment(rvm,segbases[i]);
		ptr->mod = true;
		ptr->tid = (void *)tid;
	}
	return tid;
}

void rvm_about_to_modify(trans_t tid, void *segbase, int offset, int size)
{
	segment *ptr = get_segment(tid->myrvm, segbase);
	atm_t myatm = (atm_t)malloc(sizeof(atm));
	myatm = new (myatm) atm;
	myatm->data = malloc(size);
	myatm->start = offset;
	myatm->size = size;
	memcpy(myatm->data,(const void *)((char *)segbase+offset),(size_t)size);
	ptr->atm_list.push_back(myatm);
}

void clean_seg(segment *seg_ptr,bool commit)
{
	FILE *file; 	
	int file_size = 0;
	char file_path[100];
	if(commit)
	{
		// flush the chunks to log file

		// open log file
		strcpy(file_path,seg_ptr->myrvm->directory);
		strcat(file_path,"log");
		file_size = get_size(file_path);
		file = fopen(file_path,"a");

	}
	while(!seg_ptr->atm_list.empty())
	{
		if(commit)
		{
			atm_t front = seg_ptr->atm_list.front();
			char start[100];
			char size[100];
			char const * str1 = patch::to_string(front->start).c_str();
			memcpy(start,str1,strlen(str1));
			char const * str2 = patch::to_string(front->size).c_str();
			memcpy(size,str2,strlen(str2));
			fwrite(start,sizeof(char),100,file);
			fwrite(size,sizeof(char),100,file);
			fwrite(seg_ptr->seg_name,sizeof(char),100,file);
			fwrite((seg_ptr->data+(front->start)),sizeof(char),front->size,file);
			
		}
		free(seg_ptr->atm_list.front()->data);
		delete seg_ptr->atm_list.front(), seg_ptr->atm_list.pop_front();
	}
	if(commit)
	{
		fflush(file);
		fclose(file);
		if(file_size > 2048)
		{
			rvm_truncate_log(seg_ptr->myrvm);
		}
	}
	seg_ptr->tid = NULL;
	seg_ptr->mod = false;

}
void clean_trans(trans_t tid,bool commit)
{
	rvm_t myrvm = tid->myrvm;
	std::map<const char *,void *>::iterator iter;
 	for (iter = myrvm->mapping.begin(); iter != myrvm->mapping.end(); iter++) {
 		if(((trans_t)(((segment *)(iter->second))->tid)) == tid)
 		{
 			clean_seg((segment *)(iter->second),commit);
 		}
 	}
}
void rvm_commit_trans(trans_t tid)
{

	clean_trans(tid,true);
}

void rvm_abort_trans(trans_t tid)
{
	std::map<const char *,void *>::iterator iter;
	rvm_t rvm = tid->myrvm;
 	for (iter = rvm->mapping.begin(); iter != rvm->mapping.end(); iter++) {
 		if((trans_t)(((segment *)(iter->second))->tid) == tid)
 		{
 			segment *seg_ptr = (segment *)(iter->second);
 			std::list<atm_t>::reverse_iterator rit=seg_ptr->atm_list.rbegin();
 			for (; rit!=seg_ptr->atm_list.rend(); ++rit)
 			{
 				memcpy(((char *)seg_ptr->data)+((*rit)->start),(char *)((*rit)->data),(*rit)->size);
 			}
 			clean_seg(seg_ptr,false);
 		}
 	}
}

int construct_int(char* array)
{
	return  (array[3] << 24) | (array[2] << 16) | (array[1] << 8) | (array[0]);
}
void rvm_truncate_log(rvm_t rvm)
{
	char file_path[100];
	strcpy(file_path,rvm->directory);
	strcat(file_path,"log");
	
	int file_size = get_size(file_path);
	char * buffer = (char*) malloc (sizeof(char)*file_size);

	if(!is_file_exist(file_path))
	{
		return;
	}
	FILE *file = fopen(file_path,"r+");
	fread(buffer,sizeof(char),file_size,file);


	char * array = buffer;
	bool incomplete = false;
	while(1)
	{
		int offset = &array[0] - &buffer[0];
		if(offset == file_size)
		{
			break;
		}
		char mystr[100];
		memcpy(mystr,array,100);
		int start = atoi(mystr);
		array = array + 100;
		memcpy(mystr,array,100);
		int size = atoi(mystr);
		array = array + 100;

		offset = &array[0] - &buffer[0];

		char name [100];
		memcpy(name,array,100);
		array += 100;

		offset = &array[0] - &buffer[0];
		if((offset + size) > file_size)
		{
			incomplete = true;
			fclose(file);
			file = fopen(file_path,"w");

			fwrite(&start,sizeof(int),1,file);
			fwrite(&size,sizeof(int),1,file);
			fwrite(name,sizeof(char),100,file);
			fwrite(array,sizeof(char),(file_size - offset),file);

			break;
		}

		char data_path[100];
		strcpy(data_path,rvm->directory);
		strcat(data_path,name);

		FILE * data = fopen(data_path,"r+");
		fseek(data,start,SEEK_SET);
		fwrite(array,sizeof(char),size,data);
		fflush(data);
		fclose(data);
		array += size;

	}
	fclose(file);
	remove(file_path);

}
