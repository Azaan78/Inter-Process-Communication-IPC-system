/*
 * Replace the following string of 0s with your student number
 * 240297260
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "job.h"

/* 
 * DO NOT EDIT the job_new function.
 */
job_t* job_new(pid_t pid, unsigned int id, unsigned int priority, 
    const char* label) {
    return job_set((job_t*) malloc(sizeof(job_t)), pid, id, priority, label);
}


/* 
 * TODO: you must implement this function
 */
job_t* job_copy(job_t* src, job_t* dst) {
    //SRC is original and DST is destination to be copied to

    //If SRC NULL then return NULL
    if (src == NULL) return NULL;

    //If length of source label not the same as max name size then return NULL
    if (strlen(src->label) != MAX_NAME_SIZE - 1) return NULL;

    //If destination equal to source then return desitnation
    if (dst == src) return dst;

    //If destination is NULL then allocate memory for job
    if (dst == NULL) {
        dst = malloc(sizeof(job_t));
        //If destination still NULL then return NULL
        if (dst == NULL) return NULL;
    }

    //Copies fields, source is copied as destination
    dst->pid = src->pid;
    dst->id = src->id;
    dst->priority = src->priority;
    memcpy(dst->label, src->label, MAX_NAME_SIZE);

    //Return coppied SRC as DST
    return dst;
}


/* 
 * TODO: you must implement this function
 */
void job_init(job_t* job) {
    //If job is NULL then return
    if (job == NULL) return;

    //Initialises variables for job to 0
    job->pid = 0;
    job->id = 0;
    job->priority = 0;
    //Replaces label with padded string for initialisation
    strncpy(job->label, PAD_STRING, MAX_NAME_SIZE - 1);
    job->label[MAX_NAME_SIZE - 1] = '\0';
}


/* 
 * TODO: you must implement this function
 */
bool job_is_equal(job_t* j1, job_t* j2) {
    //If job 1 and 2 are equal, return True
    if (j1 == j2) return true;

    //If job 1 or job 2 is NULL then return False
    if (j1 == NULL || j2 == NULL) return false;


	//If j1 and j2 are assigned to variables id and pid and...
    //...if strcmp returns 0 is True if it isn't 0 then the jobs don't match
	return (j1->pid == j2->pid &&
			j1->id == j2->id &&
			j1->priority == j2->priority &&
			strcmp(j1->label, j2->label) == 0);
}


/*
 * TODO: you must implement this function.
 * Hint:
 * - read the information in job.h about padding and truncation of job
 *      labels
 */
job_t* job_set(job_t* job, pid_t pid, unsigned int id, unsigned int priority,
    const char* label) {
    //If job is NULL then return NULL
    if (job == NULL) return NULL;

    //Loading the current data
    job->pid = pid;
    job->id = id;
    job->priority = priority;

    //If label is null or the first part of label is '\0' then replaces the begining of PAD_STRING with label
    if (label == NULL || label[0] == '\0') {
		memcpy(job->label, PAD_STRING, MAX_NAME_SIZE - 1);
    }
	else{
		size_t len = strlen(label);

		if (len >= MAX_NAME_SIZE - 1) {
			memcpy(job->label, label, MAX_NAME_SIZE - 1);
		}
		else{
			memcpy(job->label, label, len);
			memset(job->label + len, '*', (MAX_NAME_SIZE - 1) - len);
		}
	}

	job->label[MAX_NAME_SIZE - 1] = '\0';
	return job;
}


/*
 * TODO: you must implement this function.
 * Hint:
 * - see malloc and calloc system library functions for dynamic allocation, 
 *      and the documentation in job.h for when to do dynamic allocation
 */
char* job_to_str(job_t* job, char* str) {
    //If job is NULL or length of label is not the same as MAX_NAME_SIZE then return NULL
    if (job == NULL) return NULL;
	if (strlen(job->label) != MAX_NAME_SIZE - 1) return NULL;


    //If string is empty/NULL then
    if (str == NULL) {
        str = calloc(JOB_STR_SIZE, sizeof(char));
        if (str == NULL) return NULL;
    }

    snprintf(str, JOB_STR_SIZE, JOB_STR_FMT, job->pid, job->id, job->priority, job->label);

    return str;
}


/*
 * TODO: you must implement this function.
 * Hint:
 * - see the hint for job_to_str
 */
job_t* str_to_job(char* str, job_t* job) {
    if (str == NULL) return NULL;
    if (strlen(str) != (size_t)(JOB_STR_SIZE - 1)) return NULL;


    bool assigned = false;
    if (job == NULL) {
        job = malloc(sizeof(job_t));
        if (job == NULL) return NULL;
        assigned = true;
    }

    int pid_temp;
    unsigned int id_temp, pri_temp;
    char labelbuf[MAX_NAME_SIZE];

    int matched = sscanf(str, JOB_STR_FMT, &pid_temp, &id_temp, &pri_temp, labelbuf);

    if (matched != 4 || strlen(labelbuf) != MAX_NAME_SIZE - 1) {
        if (assigned) free(job);
        return NULL;
    }

    job->pid = (pid_t) pid_temp;
    job->id = id_temp;
	job->priority = pri_temp;
	memcpy(job->label, labelbuf, MAX_NAME_SIZE);

	return job;
}

/* 
 * TODO: you must implement this function
 * Hint:
 * - look at the allocation of a job in job_new
 */
void job_delete(job_t* job) {
	if (job != NULL) free(job);
		return;
}