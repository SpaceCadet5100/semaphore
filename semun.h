#ifndef SEMUN_H
#define SEMUN_H

union semun {
	int 	val;
	struct 	semids_ds* buf;
	unsigned short* array;
	struct seminfo* __buff;
};

#endif
