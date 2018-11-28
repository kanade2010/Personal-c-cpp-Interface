#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#include "log.h"

#define MAX_SIZE_BUF     4096

typedef struct _NODE {
	void *data;
	void *next;
} NODE;

typedef struct _QUEUE {
	int   len;
	NODE *front;
	NODE *rear;
} QUEUE;

static char   g_log_filename[100] = {0x0};      // log file name
static FILE  *g_log_file          = NULL;       // log file descriptor
static QUEUE *g_log_queue         = NULL;       // log data queue
static sem_t  g_log_queue_sem;                  // semaphore for log message queue synchronization
static pthread_mutex_t g_log_write_mutex = PTHREAD_MUTEX_INITIALIZER;    // log write synchronization

static int g_log_level            = LOG_ALL;    // the write log message level 
static int g_screenshot_flag      = 1;          // Screenshot flag, 1: print to screen, 0: don't print to screen
static int g_init_flag            = 0;          // init flag


/*
 *  struct _MESSAGE
 *
 *  Description:
 *    len:
 *        the length of data
 *  	buf: 
 *        data buffer.
 *    
 */
typedef struct _MESSAGE {
	int    len;
	char  *buf;
} MESSAGE;


/*
 *  queue_create
 *
 *  Description:
 *  	create queue.
 *
 *  Parameters:
 *
 *  Returns:
 *    !=NULL: successful. the pointer of struct queue.
 *      NULL: failed. 
 *
 *  Comments:
 *
 */
static QUEUE *queue_create(void)
{
	QUEUE *p_queue = NULL;

	p_queue = (QUEUE *)malloc(sizeof(QUEUE));
	if (p_queue == NULL)
	{
		printf("queue_create: error. malloc() failed.\n");
		return NULL;
	}
	
	p_queue->len = 0;
	p_queue->front = NULL;
	p_queue->rear = NULL;
	return p_queue;
}



/*
 *  queue_destroy
 *
 *  Description:
 *    destroy.
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *
 *  Returns:
 *     0: successful.
 *    -1: failed.
 *
 *  Comments:
 *
 */
static int queue_destroy(QUEUE *p_queue)
{
	NODE *p_node = NULL;
	NODE *tmp = NULL;

	if (p_queue != NULL)
	{
		p_node = p_queue->front;
		while(p_node != NULL) 
		{
			tmp = p_node;
			p_node = p_node->next;
			free(tmp);
		}
		free(p_queue);
		p_queue = NULL;
	}

	return 0;
}



/*
 *  queue_size
 *
 *  Description:
 *    get queue node size
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *
 *  Returns:
 *    >=0: successful. the size of queue node
 *     -1: failed. 
 *      
 *  Comments:
 *
 */
/*
static int queue_size(QUEUE *p_queue)
{
	if (p_queue == NULL)
	{
		printf("queue_size: error. input parameter is NULL\n");
		return -1;
	}
	else
		return p_queue->len;
}
*/

/*
 *  queue_is_empty
 *
 *  Description:
 *    judge queue is empty
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *
 *  Returns:
 *    1 : true, queue is empty
 *    0 : false, queue is not empty
 *    -1: failed. 
 *      
 *  Comments:
 *
 */
static int queue_is_empty(QUEUE *p_queue)
{
	if (p_queue == NULL)
	{
		printf("queue_is_empty: error. input parameter is NULL\n");
		return -1;
	}
	
	if (p_queue->len == 0)
		return 1;
	else
		return 0;
}



/*
 *  queue_front
 *
 *  Description:
 *    return the first node in queue
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *
 *  Returns:
 *    pointer: return the first node data pointer, if NULL, there is no data. 
 *      
 *  Comments:
 *
 */
/*
static void *queue_front(QUEUE *p_queue)
{
	if (p_queue == NULL)
	{
		printf("queue_front: error. input parameter is NULL\n");
		return NULL;
	}
	
	if (p_queue->front == NULL)
		return NULL;
	else
		return p_queue->front->data;
}
*/

/*
 *  queue_back
 *
 *  Description:
 *    return the last node in queue
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *
 *  Returns:
 *    pointer: return the last node data pointer, if NULL, there is no data.
 *      
 *  Comments:
 *
 */
/*
static void *queue_back(QUEUE *p_queue)
{
	if (p_queue == NULL)
	{
		printf("queue_back: error. input parameter is NULL\n");
		return NULL;
	}
	
	if (p_queue->rear == NULL)
		return NULL;
	else
		return p_queue->rear->data;
}
*/

/*
 *  queue_push
 *
 *  Description:
 *    enqueue data
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *    p_data: [in]
 *        the push data
 *
 *  Returns:
 *    >=0: successful. the length of queue
 *     -1: failed. 
 *      
 *  Comments:
 *
 */
static int queue_push(QUEUE *p_queue, void *p_data)
{
	NODE *p_node = NULL;

	if (p_queue==NULL || p_data==NULL)
	{
		printf("queue_push: error. input parameter is NULL\n");
		return -1;
	}

	p_node = (NODE *)malloc(sizeof(NODE));
	if (p_node == NULL)
	{
		printf("queue_push: error. create node fail.\n");
		return -1;
	}	
	p_node->data = p_data;
	p_node->next = NULL;

	if (p_queue->rear == NULL)
	{
		p_queue->rear = p_node;
		p_queue->front = p_queue->rear;
	}
	else
	{
		p_queue->rear->next = p_node;
		p_queue->rear = p_node;
	}
	p_queue->len += 1;
	
	return p_queue->len;
}


/*
 *  queue_pop
 *
 *  Description:
 *    dequeue and get node data
 *
 *  Parameters:
 *    p_queue: [in]
 *        the queue.
 *
 *  Returns:
 *    pointer: return the node data pointer, if NULL, there is no data.
 *      
 *  Comments:
 *
 */
static void *queue_pop(QUEUE *p_queue)
{
	NODE *p_node = NULL;
	void *p_data = NULL;

	if (p_queue->front == NULL)
		return NULL;

	p_node = p_queue->front;
	p_queue->front = p_queue->front->next;
	
	if (p_queue->front == NULL)
		p_queue->rear = p_queue->front;
	
	p_queue->len -= 1;
	
	p_data = p_node->data;
	free(p_node);

	return p_data;
}


/*
 *  create_message_node
 *
 *  Description:
 *  	create message node, malloc memory and initialize.
 *
 *  Parameters:
 *		size: [in]
 *		    the size of buffer in message. 
 *
 *  Returns:
 *    !=NULL: successful. the pointer of message 
 *      NULL: failed.
 *
 *  Comments:
 *
 */
static MESSAGE *create_message_node(int size)
{
	MESSAGE *p_mesg = NULL;
	char *ptr = NULL;
	
	p_mesg = (MESSAGE *)malloc(sizeof(MESSAGE));
	if (p_mesg == NULL)
	{
		printf("create_message_node: error. create message fail.\n");
		return NULL;
	}	
	
	ptr = (char *)malloc(size);
	if (ptr == NULL)
	{
		printf("create_message_node: error. create data buffer fail.\n");
		free(p_mesg);
		return NULL;
	}	
	memset(ptr, 0x0, size);
	
	p_mesg->len = size;
	p_mesg->buf = ptr;
	
	return p_mesg;
}


/*
 *  destroy_message_node
 *
 *  Description:
 *  	destroy message node, and free memory.
 *
 *  Parameters:
 *    p_mesg: [in]
 *        the message pointer
 *
 *  Returns:
 *     0: successful.
 *    -1: failed.
 *
 *  Comments:
 *
 */
static int destroy_message_node(MESSAGE *p_mesg)
{
	if (p_mesg == NULL)
	{
		printf("destroy_message_node: error. input parament p_mesg is NULL\n");
		return -1;
	}
	if (p_mesg->buf == NULL)
	{
		printf("destroy_task_node: error. input parament p_mesg->buf is NULL\n");
		return -1;
	}
	
	free(p_mesg->buf);
	free(p_mesg);

	return 0;
}

/*
 *  get_oldest_filename
 *
 *  Description:
 *  	获取最旧的日志文件名
 *
 *  Parameters:
 *    new_path: [out]
 *        最旧文件名
 *    old_path: [in]
 *        原文件名
 *    max_count: [in]
 *        文件最多备份个数
 *
 */
static void get_oldest_filename(char *new_path, char *old_path, int max_count)
{
	int i;
	struct stat info;
	time_t mtime = 0; //文件修改时间
	char tmp[LOG_MAX_FILE_NAME_LENGTH];
	
    for(i = 1; i < max_count; i++)
    {
        sprintf(tmp, "%s.%d", old_path, i);
        if (access(tmp, F_OK) < 0)
        {
            strcpy(new_path, tmp);
            return;
        }else{
            if (stat(tmp, &info) == 0)
            {
                if ((mtime == 0) || (info.st_mtime < mtime))
                {
                    strcpy(new_path, tmp);
                    mtime = info.st_mtime;
                }
            }
        }
    }
}

static int check_file_exist()
{
	if(g_log_file == NULL)
	{
		return -1;
	}

	if (access(g_log_filename, F_OK) < 0)
	{
		fclose(g_log_file);
		g_log_file = fopen(g_log_filename, "ab+");
        if (g_log_file == NULL)
        {
	        printf("error. fopen() %s failed\n", g_log_filename);
	        return -1;
        }
	}

	return 0;
}

/*
 *  check_log_full
 *
 *  Description:
 *  	检测日志文件是否已满,如果已满，则将当前日志文件更名为备份文件
 *
 *  Returns:
 *    0 : 操作成功
 *    -1: 操作失败
 */
static int check_log_full()
{
	struct stat info;
	char new_path[LOG_MAX_FILE_NAME_LENGTH];
	
	if (stat(g_log_filename, &info) == 0)
	{
		if (info.st_size > LOG_MAX_FILE_SIZE)
		{
			memset(new_path, 0, sizeof(new_path));
			get_oldest_filename(new_path, g_log_filename, LOG_MAX_FILE_COUNT);
            fclose(g_log_file);
			g_log_file = NULL;
            rename(g_log_filename, new_path);
            
            g_log_file = fopen(g_log_filename, "ab+");
	        if (g_log_file == NULL)
	        {
		        printf("error. fopen() %s failed\n", g_log_filename);
		        return -1;
	        }
        }
	}

    return 0;
}

/*
 *  log_write_process_thread
 *
 *  Description:
 *    thread callback function
 *  	write log file
 *
 *  Parameters:
 *		param: [in]
 *			thread parameter
 *
 *  Returns:
 *
 *  Comments:
 *
 */
static void *log_write_file_thread(void *param)
{
	int ret = 0;
	MESSAGE *p_mesg = NULL;
	
	printf("log_write_process_thread start\n");
	
	while(1)
	{
		sem_wait(&g_log_queue_sem);
		if (queue_is_empty(g_log_queue) != 0)
			continue;

		p_mesg = NULL;
		pthread_mutex_lock(&g_log_write_mutex);
		p_mesg = (MESSAGE *)queue_pop(g_log_queue);
		pthread_mutex_unlock(&g_log_write_mutex);
		if (p_mesg != NULL)
		{
			ret = check_log_full();
			if (ret < 0)
			{
			    printf("log_write_process_thread: error. check_log_full() log failed. ret=%d\n", ret);
			    continue;
			}

			ret = check_file_exist();
			if(ret < 0)
			{
				printf("log_write_process_thread: error. check_file_exist() log failed. ret=%d\n", ret);
			    continue;
			}

			ret = fwrite(p_mesg->buf, p_mesg->len, 1, g_log_file);
			if (ret < 0)
				printf("log_write_process_thread: error. fwrite() log failed. ret=%d\n", ret);
			else
				fflush(g_log_file);
		}

		destroy_message_node(p_mesg);	
	}
	
	pthread_exit(0); 
}


/*
 *  log_init
 *
 *  Description:
 *  	initilize logger
 *
 *  Parameters:
 *    log_level: [in]
 *        the logger 
 *    screenshot_flag: [in]
 *        Screenshot flag, 1: print to screen, 0: don't print to screen
 *    log_filename: [in]
 *        the logger file name
 *
 *  Returns:
 *     0: successful.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int log_init( int log_level, int screenshot_flag, char *log_filename)
{
	int ret = 0;
	FILE *file = NULL;
	pthread_t thread_id = 0;	
	pthread_attr_t attr;
	
	if (log_filename == NULL)
	{
		printf("log_init: error. input parameter filename == NULL\n");
		return -1;
	}

	file = fopen(log_filename, "ab+");
	if (file == NULL)
	{
		printf("log_init: error. fopen() %s failed\n", log_filename);
		return -1;
	}
	
	strcpy(g_log_filename, log_filename);
	g_log_file = file;
	g_log_level = log_level;
	g_screenshot_flag = screenshot_flag;
	
	ret = pthread_attr_init(&attr);
	if (ret != 0)
		printf("log_init: error. pthread_attr_init failed\n");
	ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);    //Set thread inheritance
	if (ret != 0)
		printf("log_init: error. pthread_attr_setinheritsched failed\n");
	ret = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);                //set thread scheduling policy
	if (ret != 0)
		printf("log_init: error. pthread_attr_setschedpolicy failed\n");
	ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);             //Set thread scope
	if (ret != 0)
		printf("log_init: error. pthread_attr_setscope failed\n");
	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    //set thread separation state property
	if (ret != 0)
		printf("log_init: error. pthread_attr_setdetachstate failed\n");
	
	ret = pthread_create(&thread_id, &attr, log_write_file_thread, NULL);	
	if (ret != 0)
	{
		printf("log_init: error. create mcu_task_processor_thread() failed\n");
		return -1;
	}
	pthread_attr_destroy(&attr);
	
	ret = sem_init(&g_log_queue_sem, 0, 1);  // iniialization value =1
	if (ret < 0)
	{
		printf("log_init: error. sem_init() failed. ret=%d\n", ret);
		return -1;
	}

	pthread_mutex_init(&g_log_write_mutex, NULL);

	g_log_queue = queue_create();
	if (g_log_queue == NULL)
	{
		printf("log_init: error. queue_create() failed\n");
		return -1;
	}
		
	g_init_flag = 1;
		
	return 0;
}


/*
 *  log_exit
 *
 *  Description:
 *  	exit logger
 *
 *  Parameters:
 *
 *  Returns:
 *     0: successful.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int log_exit(void)
{	
	int ret = 0;

	if (g_log_file != NULL)
	{
		fclose(g_log_file);
		g_log_file = NULL;
	}
	
	pthread_mutex_destroy(&g_log_write_mutex);
	
	ret = queue_destroy(g_log_queue);
	if (ret < 0)
	{
		printf("log_exit: error. queue_destroy() failed\n");
		return -1;
	}
	
	g_init_flag = 0;
	return 0;
}


/*
 *  log_write
 *
 *  Description:
 *  	initilize logger
 *
 *  Parameters:
 *    file_name: [in]
 *        source code file name
 *    func_name: [in]
 *        source code function name
 *    code_line: [in]
 *        source code line
 *    log_level: [in]
 *        the logger level
 *    buf: [in]
 *        the write buffer data
 *
 *  Returns:
 *    >0: successful. the size of write buffer data 
 *     0: successful. the log level is lower, no need to write
 *    -1: failed.
 *
 *  Comments:
 *
 */
int log_write(const char *file_name, const char *func_name, int code_line, int log_level, char *buf, ...)
{

	int  ret = 0;
	int  count = 0;
	char level_buf[100] = {0};
	char tmp_buf[MAX_SIZE_BUF] = {0x0};
	time_t now;
	struct tm timeinfo;
	MESSAGE *p_mesg = NULL;	
	va_list va;

	if (buf == NULL)
	{
		printf("log_write: error. input parameter is NULL.\n");
		return -1;
	}
	
	if (log_level > g_log_level)
		return 0;
	else
	{
		switch (log_level)
		{
			case LOG_FATAL:
				strcpy(level_buf, "FATAL");
				break;
			case LOG_ERROR:
				strcpy(level_buf, "ERROR");
				break;
			case LOG_WARN:
				strcpy(level_buf, "WARN");
				break;
			case LOG_INFO:
				strcpy(level_buf, "INFO");
				break;
			case LOG_TRACE:
				strcpy(level_buf, "TRACE");
				break;
			case LOG_DEBUG:
				strcpy(level_buf, "DEBUG");
				break;
			case LOG_ALL:
				strcpy(level_buf, "ALL");
				break;
			default:
				printf("log_write: error. level is not supported. level=%d\n", log_level);
				return -1;					
		}
	}
	
	now = time(NULL);
	localtime_r(&now, &timeinfo);
	
	count = 0;
	count += 	snprintf(tmp_buf+count, MAX_SIZE_BUF-count-1, "%04d-%02d-%02d %02d:%02d:%02d ", timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
	count += 	snprintf(tmp_buf+count, MAX_SIZE_BUF-count-1, "[%5s]", level_buf);

	if (file_name != NULL)
		count += 	snprintf(tmp_buf+count, MAX_SIZE_BUF-count-1, "[%s]", file_name); 
	if (func_name != NULL)
		count += 	snprintf(tmp_buf+count, MAX_SIZE_BUF-count-1, "[%d][%s] ", code_line, func_name);
	
	va_start(va, buf);
	ret = vsnprintf(tmp_buf+count, MAX_SIZE_BUF-count-1, buf, va);
	va_end(va);	
	if (ret < 0)
	{
		printf("log_write: error. vsnprintf() failed. ret=%d\n", ret);
		return -1;
	}
	count += ret;
	
	if (g_screenshot_flag == 1)
		printf("%s", tmp_buf);
	
	if (g_init_flag == 0)   //uninitialized
		return count;

	p_mesg = create_message_node(count);
	if (p_mesg == NULL)
	{
		printf("log_write: error. create_message_node() failed\n");
		return -1;
	}
	memcpy(p_mesg->buf, tmp_buf, count);
	p_mesg->len = count;
	
	pthread_mutex_lock(&g_log_write_mutex);
	ret = queue_push(g_log_queue, (void *)p_mesg);
	pthread_mutex_unlock(&g_log_write_mutex);
	if (ret < 0)
	{
		destroy_message_node(p_mesg);	
		printf("log_write: error. destroy_message_node() failed. ret=%d\n", ret);
		return -1;
	}
	else
	{
		sem_post(&g_log_queue_sem); 
		return count;	
	}
}



/*
 *  log_sprintf_hex
 *
 *  Description:
 *  	sprintf hexadecimal number to buffer
 *
 *  Parameters:
 *    output: [out]
 *        the sprintf result buffer
 *    hex_data: [in]
 *        key string
 *    hex_size: [in]
 *        the size of hex_data
 *
 *  Returns:
 *		>0: successfully. the size of output
 *    -1: failed.
 *
 *  Comments:
 *
 */
int log_sprintf_hex(unsigned char *output, unsigned char *hex_data, int hex_size)
{
	int ret = 0;
	int i = 0;
	int count = 0;

	if (output==NULL || hex_data==NULL)
	{
		printf("log_sprintf_hex: error. input parameter is NULL\n");
		return -1;
	}
    
    if(hex_size > 1000)
    {
        hex_size = 1000;
    }

	for (i=0; i<hex_size; i++)
	{
		ret = sprintf(output+count, "%02x", hex_data[i]);
		count += ret;
	}
	
	return count;
}