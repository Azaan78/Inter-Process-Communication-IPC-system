/*
 * Replace the following string of 0s with your student number
 * 240297260
 */
#include <stdlib.h>
#include <stdbool.h>
#include "pri_jobqueue.h"

#include <limits.h>

/* 
 * TODO: you must implement this function that allocates a job queue and 
 * initialise it.
 * Hint:
 * - see job_new in job.c
 */
pri_jobqueue_t* pri_jobqueue_new() {
    pri_jobqueue_t* pjq = malloc(sizeof(pri_jobqueue_t));
    if (pjq == NULL) {
        return NULL;
    }
    pri_jobqueue_init(pjq);
    return pjq;
}

/* 
 * TODO: you must implement this function.
 */
void pri_jobqueue_init(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return;
    }

    pjq->buf_size = JOB_BUFFER_SIZE;
    pjq->size = 0;

    for (int i = 0; i < JOB_BUFFER_SIZE; i++) {
        job_init(&pjq->jobs[i]);
    }
}

/* 
 * TODO: you must implement this function.
 * Hint:
 *      - if a queue is not empty, and the highest priority job is not in the 
 *      last used slot on the queue, dequeueing a job will result in the 
 *      jobs on the queue having to be re-arranged
 *      - remember that the job returned by this function is a copy of the job
 *      that was on the queue
 */
job_t* pri_jobqueue_dequeue(pri_jobqueue_t* pjq, job_t* dst) {
    if (pjq == NULL || pri_jobqueue_is_empty(pjq)) {
        return NULL;
    }

    int best_index = -1;
    unsigned int best_priority = UINT_MAX;

    /* Find highest priority job (lowest priority value >= 1) */
    for (int i = 0; i < JOB_BUFFER_SIZE; i++) {
        if (pjq->jobs[i].priority >= 1 &&
            pjq->jobs[i].priority < best_priority) {
            best_priority = pjq->jobs[i].priority;
            best_index = i;
            }
    }

    if (best_index == -1) {
        return NULL;
    }

    job_t* result;
    if (dst != NULL) {
        job_copy(dst, &pjq->jobs[best_index]);
        result = dst;
    } else {
        job_t *src = &pjq->jobs[best_index];
        result = job_new(src->pid, src->id, src->priority, src->label);
        if (result == NULL) {
            return NULL;
        }
        job_copy(result, &pjq->jobs[best_index]);
    }

    /* Remove job from queue */
    job_init(&pjq->jobs[best_index]);
    for (int i = best_index; i < JOB_BUFFER_SIZE - 1; i++) {
        pjq->jobs[i] = pjq->jobs[i + 1];}

    job_init(&pjq->jobs[JOB_BUFFER_SIZE - 1]);
    pjq->size--;

    return result;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 * - if a queue is not full, and if you decide to store the jobs in 
 *      priority order on the queue, enqueuing a job will result in the jobs 
 *      on the queue having to be re-arranged. However, it is not essential to
 *      store jobs in priority order (it simplifies implementation of dequeue
 *      at the expense of extra work in enqueue). It is your choice how 
 *      you implement dequeue (and enqueue) to ensure that jobs are dequeued
 *      by highest priority job first (see pri_jobqueue.h)
 * - remember that the job passed to this function is copied to the 
 *      queue
 */
void pri_jobqueue_enqueue(pri_jobqueue_t* pjq, job_t* job) {
    if (pjq == NULL || job == NULL) {
        return;
    }

    if (pri_jobqueue_is_full(pjq)) {
        return;
    }

    if (job->priority < 1) {
        return;
    }

    /* Find first empty slot */
    for (int i = 0; i < JOB_BUFFER_SIZE; i++) {
        if (pjq->jobs[i].priority == 0) {
            job_copy(&pjq->jobs[i], job);
            pjq->size++;
            return;
        }
    }
}
   
/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_empty(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return true;
    }
    return pjq->size == 0;
}

/* 
 * TODO: you must implement this function.
 */
bool pri_jobqueue_is_full(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return false;
    }
    return pjq->size >= pjq->buf_size;
}

/* 
 * TODO: you must implement this function.
 * Hints:
 *      - remember that the job returned by this function is a copy of the 
 *      highest priority job on the queue.
 *      - both pri_jobqueue_peek and pri_jobqueue_dequeue require copying of 
 *      the highest priority job on the queue
 */
job_t* pri_jobqueue_peek(pri_jobqueue_t* pjq, job_t* dst) {
    if (pjq == NULL || pri_jobqueue_is_empty(pjq)) {
        return NULL;
    }

    int best_index = -1;
    unsigned int best_priority = UINT_MAX;

    for (int i = 0; i < JOB_BUFFER_SIZE; i++) {
        if (pjq->jobs[i].priority >= 1 &&
            pjq->jobs[i].priority < best_priority) {
            best_priority = pjq->jobs[i].priority;
            best_index = i;
            }
    }

    if (best_index == -1) {
        return NULL;
    }

    job_t* result;
    if (dst != NULL) {
        job_copy(dst, &pjq->jobs[best_index]);
        result = dst;
    } else {
        job_t *src = &pjq->jobs[best_index];
        result = job_new(src->pid, src->id, src->priority, src->label);
        if (result == NULL) {
            return NULL;
        }
        job_copy(result, &pjq->jobs[best_index]);
    }

    return result;
}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_size(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return 0;
    }
    return pjq->size;
}

/* 
 * TODO: you must implement this function.
 */
int pri_jobqueue_space(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return 0;
    }
    return pjq->buf_size - pjq->size;
}

/* 
 * TODO: you must implement this function.
 *  Hint:
 *      - see pri_jobqeue_new
 */
void pri_jobqueue_delete(pri_jobqueue_t* pjq) {
    if (pjq == NULL) {
        return;
    }
    free(pjq);
}
