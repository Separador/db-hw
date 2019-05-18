/* This implements an mpi (middle point insertion) in-memory cache.
 *
 * This program reads decimal positive numbers from stdin and checks
 * if they hit the cache.
 * As a result the program returns an overall hit count.
 * It also uses command line args to control the "hot" zone size,
 * the promotion threshold, the time and the reduction ratio.
 *
 * CMD ARGS:	<cache cap> - max cache capacity (cold + hot sizes);
 * 		<hot cap> - max hot zone capacity, < cache_cap;
 * 		<hot pass> - hit counts to pass to move elem from cold to hot;
 * 		<reduct_time> - period to reduce hit counts in hot zone by <reduct_pcnt>;
 * 		<reduct_pcnt> - percent value to reduce hit count by;
 * INPUT:	unsigned interger from 0 to 4095
 * OUTPUT:	resulting cache hit count
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

struct node {
	struct node *prev;
	struct node *next;
};

struct data {
	struct node node;
	unsigned val;
	unsigned hit;
};

struct cache {
	struct data *hhead;
	struct data *htail;
	struct data *chead;
	struct data *ctail;
	unsigned cap;
	unsigned hsize;
	unsigned csize;
	unsigned hcap;
	unsigned hpass;		/* value to pass for promotion */
	unsigned hitcount;
} cache;

void list_rmnode(struct node *node)
{
	if (node->prev != NULL) {
		node->prev->next = node->next;
	}
	if (node->next != NULL) {
		node->next->prev = node->prev;
	}
	node->prev = NULL;
	node->next = NULL;
}

void cache_init(unsigned cap, unsigned hcap, unsigned hpass)
{
	cache.hhead = NULL;
	cache.htail = NULL;
	cache.chead = NULL;
	cache.ctail = NULL;
	cache.hsize = 0;
	cache.csize = 0;
	cache.cap = cap;
	cache.hcap = hcap;
	cache.hpass = hpass;
}

void cache_rmctail()
{
	struct data *t = cache.ctail;
	cache.ctail = (struct data *)cache.ctail->node.next;
	cache.ctail->node.prev = NULL;
	free(t);
}

void cache_csizeinc()
{
	cache.csize++;
	while (cache.csize + cache.hsize >= cache.cap) {
		cache_rmctail();
		cache.csize--;
	}
}

void cache_demote(struct data *data)
{
	/* remove from hot zone */
	if (data == cache.hhead)
		cache.hhead = (struct data *)data->node.prev;
	if (data == cache.htail)
		cache.htail = (struct data *)data->node.next;
	list_rmnode(&data->node);
	cache.hsize--;
	/* add to tail of cold zone */
	if (cache.chead == NULL) {
		cache.chead = data;
		cache.ctail = cache.chead;
	} else {
		cache.ctail->node.prev = &data->node;
		data->node.next = &cache.ctail->node;
		cache.ctail = data;
	}
	cache_csizeinc();
}

void cache_promote(struct data *data)
{
	/* remove from the cold zone */
	if (data == cache.chead)
		cache.chead = (struct data *)data->node.prev;
	if (data == cache.ctail)
		cache.ctail = (struct data *)data->node.next;
	list_rmnode(&data->node);
	cache.csize--;
	/* add to head of the hot zone */
	if (cache.hhead == NULL) {
		cache.hhead = data;
		cache.htail = cache.hhead;
	} else {
		cache.hhead->node.next = &data->node;
		data->node.prev = &cache.hhead->node;
		cache.hhead = data;
	}
	cache.hsize++;
	if (cache.hsize >= cache.hcap)
		cache_demote(cache.htail);
}

bool in_cache(unsigned val)
{
	struct data *h = cache.hhead;
	while (h != NULL) {
		if (h->val == val) {
			cache.hitcount++;
			h->hit++;
			return true;
		}
		h = (struct data *)h->node.prev;
	}
	h = cache.chead;
	while (h != NULL) {
		if (h->val == val) {
			cache.hitcount++;
			h->hit++;
			if (h->hit >= cache.hpass)
				cache_promote(h);
			return true;
		}
		h = (struct data *)h->node.prev;
	}
	return false;
}

void cache_add(unsigned val)
{
	if (cache.chead == NULL) {
		cache.chead = calloc(1, sizeof(struct data));
		cache.ctail = cache.chead;
	} else {
		cache.chead->node.next = calloc(1, sizeof(struct data));
		cache.chead->node.next->prev = &cache.chead->node;
		cache.chead = (struct data *)cache.chead->node.next;
	}
	cache.chead->val = val;
	cache.chead->hit = 0;
	cache_csizeinc();
}

void cache_hotreduce(int reduc_pcnt)
{
	struct data *h = cache.hhead;
	while (h != NULL) {
		h->hit = h->hit * (100 - reduc_pcnt) / 100;
		if (h->hit < cache.hpass)
			cache_demote(h);
		h = (struct data *)h->node.prev;
	}
}

void cache_cleanup()
{
	struct data *h = cache.hhead;
	while (h != NULL) {
		cache.hhead = (struct data *)cache.hhead->node.prev;
		free(h);
		h = cache.hhead;
	}
	h = cache.chead;
	while (h != NULL) {
		cache.chead = (struct data *)cache.chead->node.prev;
		free(h);
		h = cache.chead;
	}
}

int main(int argc, char **argv)
{
	if (argc != 6) {
		perror("usage: mpicache <cache_cap> <hot_cap> "
			"<hot_pass> <reduct_time> <reduct_pcnt>");
		exit(1);
	}
	int cap = atoi(argv[1]);
	int hot_cap = atoi(argv[2]);
	int hot_pass = atoi(argv[3]);
	int reduc_time = atoi(argv[4]);
	int reduc_pcnt = atoi(argv[5]);
	int time = 0;
	int v = 0;
	cache_init(cap, hot_cap, hot_pass);
	while (scanf("%d", &v) != EOF) {
		if (!in_cache(v))
			cache_add(v);
		if (++time % reduc_time == 0) {
			cache_hotreduce(reduc_pcnt);
		}
	}
	printf("Overall hit count %d\n", cache.hitcount);
	cache_cleanup();
	return 0;
}
