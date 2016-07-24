//typedef int rvm_t;
#include <map>
#include <list>

using namespace std;
using std::list;
typedef struct {
	const char * directory;
	map<const char *,void *> mapping;
} rvm_struct;
typedef rvm_struct *rvm_t;

typedef struct{
	void *data;
	int start;
	int size;
} atm;
typedef atm *atm_t;

typedef struct{
	const char * seg_name;
	rvm_t myrvm;
	int size;
	void *data;
	bool mod;
	void * tid;
	list<atm_t> atm_list;
} segment;



typedef struct{
	rvm_t myrvm;
} transaction;

typedef transaction *trans_t;