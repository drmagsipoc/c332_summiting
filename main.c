#include <stdio.h>
#include <stdlib.h>

enum arg_parse_enum
{
	CL_PARSE_PASS = 0,
	CL_PARSE_FAIL,
};


enum exit_codes
{
	SUCCESS = 0,
	PARSE_ERROR,
	OUT_OF_MEMORY_ERROR,
};

/* Dynamic programming solution */
struct solution_node_struct
{
	long *path;
	long *score;
};

int parse_input(int cnt, char *argv[], unsigned int *valid_ptr)
{
	unsigned long i;
	unsigned long valid_cnt = 0;

	/* fail condition 1, if there are other args */
	if (cnt == 1) {
		return CL_PARSE_FAIL;
	}

	/* validate the int stream, skip the first arg */
	for (i = 1; i < cnt; i++) {
		printf("parsing argv[%lu] = %s\n", i, argv[i]);
		valid_cnt++;
	}

	/* fail condition 2, if there are no valid summit values */
	if (valid_cnt == 0) {
		return CL_PARSE_FAIL;
	}

	/* we save count of peaks that are non-zero */
	*valid_ptr = valid_cnt;

	return CL_PARSE_PASS;
}


void get_summit_array(long *summits_ptr, char *input_vec[],
                      int input_num)
{
	int i, val_ctr;
	long val;

	if (summits_ptr == NULL) {
		exit(OUT_OF_MEMORY_ERROR);
	}

	for (i = 0, val_ctr = 0; i < input_num; i++) {
		val = strtol(input_vec[i], NULL, 10);
		summits_ptr[val_ctr] = val;
		val_ctr++;
	}
}

int solve_peaks(const long * const peaks_ptr, const int peak_cnt)
{
	struct solution_node_struct solve;
	long i, j;
	long start;

	/* initialize solution members */
	solve.path = malloc(peak_cnt * sizeof(long));
	if (solve.path == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}
	solve.score = malloc(peak_cnt * sizeof(long));
	if (solve.score == NULL) {
		free(solve.path);
		return OUT_OF_MEMORY_ERROR;
	}

	/* solve for the solution using dynamic programming */

	/* initialize score data */
	for (i = 0; i < peak_cnt; i++) {
		solve.score[i] = 1;
	}

	for (i = peak_cnt - 1; i >= 0; i--) {
		/* initialize the next path as invalid (last peak) */
		solve.path[i] = peak_cnt;
		for (j = i + 1; j < peak_cnt; j++) {
			if (    (peaks_ptr[j] > peaks_ptr[i])
			     && (solve.score[j] >= solve.score[i])) {
				/* point to the next peak */
				solve.path[i] = j;
				/* update the score of the current peak */
				solve.score[i] = solve.score[j] + 1;
			}
		}
	}

	/* print solution, find the best starting position */
	start = 0;
	for (i = 0; i < peak_cnt; i++) {
		if (solve.score[i] > solve.score[start]) {
			start = i;
		}
	}

	for (i = start; i < peak_cnt; i = solve.path[i]) {
		printf("%ld ", peaks_ptr[i]);
	}

	free(solve.score);
	free(solve.path);

	return SUCCESS;
}

int main(int argc, char *argv[])
{
	unsigned int valid_cnt = 0;
	int i;
	long *summits_ptr = NULL;

	if (parse_input(argc, argv, &valid_cnt) == CL_PARSE_FAIL) {
		return PARSE_ERROR;
	}

	summits_ptr = malloc(valid_cnt * sizeof(*summits_ptr));
	if (summits_ptr == NULL) {
		return OUT_OF_MEMORY_ERROR;
	}

	get_summit_array(summits_ptr, &argv[1], argc - 1);

	/* verify the int array */
	for (i = 0; i < valid_cnt; i++) {
		printf("summit[%i] = %ld\n", i, summits_ptr[i]);
	}

	/* actual computation of the optimal solution */
	solve_peaks(summits_ptr, valid_cnt);

	free(summits_ptr);

	return SUCCESS;
}
