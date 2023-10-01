#include "structures.h"

struct flock getLock(short l_type, short l_whence, off_t l_start, off_t l_len){
	struct flock mylock;
	mylock.l_type = l_type;
	mylock.l_whence = l_whence;
	mylock.l_start = l_start;
	mylock.l_len = l_len;
	mylock.l_pid = getpid();
	return mylock;
}
