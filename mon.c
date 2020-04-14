
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef THREAD_N
#define THREAD_N 4
#endif

#ifndef ITER_N
#define ITER_N 15
#endif

#ifndef gettid
#define gettid() syscall(SYS_gettid)
#endif

int resources = 100;

pthread_cond_t cond;
pthread_mutex_t resource_mutex;

void decrease( FILE * log ) {
    fprintf( log, "Blocking mutex \n" );
    pthread_mutex_lock( &resource_mutex );
    if( resources > 0 ) {
        fprintf( log, "Consumed resource\n" );
        resources--;
        pthread_cond_signal( &cond );
    }
    fprintf( log, "Remaining resource %d\n", resources );
    fprintf( log, "Mutex unlock\n" );
    pthread_mutex_unlock( &resource_mutex );
}

void * thread_runner( void * args ) {

    /* Get the log file from the args */
    FILE * log = ( ( FILE * )args );

    /* Start processing */
    int tid = gettid();

    fprintf( log, "Thread started id: %d \n", tid );

    fprintf( log, "Starting for loop inside thread %d\n", tid );

    int j;
    for( j = 0; j < ITER_N; j++ ) {
        fprintf( log, "Iter %d on thread %d\n", j, tid );

        fprintf( log, "Mutex locked from thread ID: %d\n", tid );
        decrease( log );

    }

}

int main() {

    /* Open log file */
    FILE * log_file = fopen( "log_monitor.txt", "wSS" );

    /* Lets get ready the thread pool and sem */
    printf(" Creating thread pool \n");
    pthread_t thread_pool[ THREAD_N ];


    /* Lets start the threads and join them after completion */
    printf(" Thread create \n");
    int i;
    for( i = 0; i < THREAD_N; i++ ) {
        pthread_create( &thread_pool[ i ], NULL, &thread_runner, log_file );   
    }

    printf(" Thread join \n");
    for( i = 0; i < THREAD_N; i++ ) {
        pthread_join( thread_pool[i], NULL );
    }

    printf(" File close \n");
    fclose( log_file );

}