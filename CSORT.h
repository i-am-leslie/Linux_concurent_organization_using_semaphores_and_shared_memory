#define TEXT_SZ 2048
#define SIZE_ARRAY 5

struct shared_use_st {
	char R[SIZE_ARRAY];
	char G[SIZE_ARRAY];
	
	int flag;
};

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

