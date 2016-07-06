
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// I think I used this in something for CreditShop.  May have to rewrite.

typedef struct iter {
	FILE* f;
	char* buf;
	int buflen;
	int bufcount;
	int prior_final;
	int xind;
	unsigned int flags;
	char delim;
} iter;
unsigned int _iter_MALLOC_BUF = 0x1;
unsigned int _iter_FINAL_READ = 0x2;
unsigned int _iter_LAST_LINE  = 0x4;

char* _iter_next_buffer(iter*);

iter* alloc_iter()
{ return (iter*)malloc(sizeof(iter)); }
iter* init_iter(iter* it, FILE* f, char* buf, int buflen, char delim)
{
	it->flags = 0;
	it->f = f;
	it->buflen = buflen;
	it->bufcount = 0;
	if (buf == 0) {
		it->buf = (buf ? buf : (char*)malloc(sizeof(char)*buflen+1));
		it->flags |= _iter_MALLOC_BUF;
	}
	else
		it->buf = buf;
	it->buf[it->buflen] = 0;
	it->prior_final = 0;
	it->xind = 0;
	it->delim = delim;
	
	// Initial buffer read and first line setup.
	_iter_next_buffer(it);
	
	return it;
}
iter* new_iter(FILE* f, char* buf, int buflen, char delim)
{ return init_iter(alloc_iter(),f,buf,buflen,delim); }

iter* del_iter(iter* it)
{
	//fprintf(stderr,"%08x\n",it->flags);
	if ((it->flags & _iter_MALLOC_BUF) && it->buf)
		free(it->buf);
	return it;
}
void free_iter(iter* it)
{ free(del_iter(it)); }

char* _iter_next_buffer(iter* it)
{
	if (feof(it->f)) {
		// Reached EOF, last buffer read.
		it->flags |= _iter_FINAL_READ;
		return 0;
	}
	
	//fprintf(stderr,"_iter_next_buffer: %i,%i,%08x\n",it->bufcount,it->xind,it->flags);
	
	// NOTE: This iterator depends on is buflen being longer than the largest line.
	
	char* buf = it->buf;
	//int prior_final = it->prior_final;
	int prior_final = it->xind;
	
	// Fill beginning of buffer with buf[prior_final:].
	//fprintf(stderr,"sliding over: [%s]\n",&buf[prior_final]);
	int i=0;
	if (prior_final > 0) {
		int tail_len = it->buflen - prior_final;
		for (i=0; i < tail_len; ++i)
			buf[i] = buf[i+prior_final];
	}
	
	// Read in the rest.
	int count = 0;
	count = fread(&buf[i], sizeof(buf[0]), it->buflen-i, it->f);
	if (count < it->buflen-i) {
		// Is this redundant?
		//fprintf(stderr,"FINAL READ!\n");
		it->flags |= _iter_FINAL_READ;
		// Enforce a double zero, or *a* zero?... on the start of the next buffer?
		if (i+count <= it->buflen)
			buf[i+count] = 0;
	}
	
	it->xind = 0;
	it->bufcount += 1;
	
	//fprintf(stderr,"buf: %s\n",buf);
	return buf;
}

char* iter_next(iter* it)
{
	if (it->flags & _iter_LAST_LINE)
		return 0;
	
	char* buf = it->buf;
	int buflen = it->buflen;
	
	//fprintf(stderr,"xind: %i\n",it->xind);
	// Double zeros.  We need to force a double-zero on the final buffer read.
	if (!buf[it->xind] || it->xind >= it->buflen) {
		if (it->flags & _iter_FINAL_READ) {
			// Reached the end of everything, I guess...
			it->flags |= _iter_LAST_LINE;
			return 0;
		}
		else {
			// Buffer reset trailing line to beginning, so gotta do again.
			//fprintf(stderr,"Vrap daggle.\n");
			_iter_next_buffer(it);
			return iter_next(it);
		}
	}
	
	char* c = &buf[it->xind];
	
	int i;
	for (i=it->xind; buf[i] && i < buflen; ++i) {
		////fprintf(stderr,"%i,%c\n",i,buf[i]);
		if (buf[i] == '\n') {
			buf[i] = 0;
			break;
		}
	}
	++i;
	if (i >= buflen && buf[i-1] != '\n') {
		//fprintf(stderr,"This one, here... but with i,xind,buflen: %i, %i, %i\n",i,it->xind,buflen);
		_iter_next_buffer(it);
		return iter_next(it);
	}
	
	it->xind = i;
	//fprintf(stderr,"setting xind to: %i\n",i);
	// At this point, the returned string is ready to go.  Just need to set xind.
	
	return c;
}

int iter_has_next(iter* it)
{
	if (it->flags & _iter_LAST_LINE)
		return 0;
	return 1;
}

int main(int argc, const char** argv)
{
	//fprintf(stderr,"%s\n","Who even knows the even what?");
	
	FILE* f;
	const char* fname = 0;
	char* buf;
	
	if (argc > 1)
		fname = argv[1];
	f = fopen(fname,"r");
	
	int buflen = 120;
	iter* fiter = new_iter(f,0,buflen,'\n');
	
	buf = fiter->buf;
	
	int h = -1;
	char* s = 0;
	char c[1024];
	char n = 0;
	int i = 0;
	do {
		//fprintf(stderr,"n: %i, %i, %08x\n",n,fiter->bufcount,fiter->flags);
		s = iter_next(fiter);
		printf("%s\n",s);
		i++;
	} while (s && i < 15);
	
	free_iter(fiter);
	fclose(f);
	
	return 0;
}

