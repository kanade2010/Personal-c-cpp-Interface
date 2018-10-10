/*
 * tools.c
 *
 * Copyright(C) Launch. All Rights Reserved.
 *
 * Created on: 2018-02-01
 *     Author: luo yongbo <yongbo.luo@cnlaunch.com>
 *
 *     Universal interface for common tools
 *
 */

#include "tools.h"

#define MAX_SIZE       1024       // the max size of buffer
#define MAX_FILE_LEN   (8*1024)   // the max size of file content

pthread_mutex_t g_config_mutex;


/*
 *  tools_delay_ms
 *
 *  Description:
 *  	sleep micro second
 *
 *  Parameters:
 *    ms: [in]
 *        micro second
 *
 *  Returns:
 *
 *  Comments:
 *
 */
void tools_delay_ms(int ms)
{
	struct timeval tv;
	
	tv.tv_sec = 0;
	tv.tv_usec = ms * 1000;
	select(0, NULL, NULL, NULL, &tv);
}


/*
 *  tools_delay_sec
 *
 *  Description:
 *  	sleep second
 *
 *  Parameters:
 *    sec: [in]
 *        second
 *
 *  Returns:
 *
 *  Comments:
 *
 */
void tools_delay_sec(int sec)
{
	struct timeval tv;
	
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	select(0, NULL, NULL, NULL, &tv);
}



/*
 *  tools_value_to_systemtime
 *
 *  Description:
 *  	switch time_t value to struct SYSTEMTIME buffer
 *
 *  Parameters:
 *    p_time: [out]
 *        system time struct pointer
 *    val: [in]
 *        time_t value
 *
 *  Returns:
 *    0 : successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int tools_value_to_systemtime(SYSTEMTIME *p_time, time_t *val)
{
	struct tm timeinfo;
	
	if (p_time==NULL || val==NULL)
	{
		////LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}

	localtime_r(val, &timeinfo);
	p_time->m_year      = timeinfo.tm_year + 1900;
	p_time->m_month     = timeinfo.tm_mon  + 1;
	p_time->m_day       = timeinfo.tm_mday;
	p_time->m_wday      = timeinfo.tm_wday;
	p_time->m_hour      = timeinfo.tm_hour;
	p_time->m_minute    = timeinfo.tm_min;
	p_time->m_second    = timeinfo.tm_sec;
	p_time->m_milliseconds = 0;
	
	return 0;
}

/*
 *  tools_systemtime_to_value
 *
 *  Description:
 *  	switch struct SYSTEMTIME buffer to time_t value
 *
 *  Parameters:
 *    p_time: [in]
 *        system time struct pointer
 *    val: [out]
 *        time_t value
 *
 *  Returns:
 *    >0: successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int tools_systemtime_to_value(SYSTEMTIME *p_time, time_t *val)
{
	struct tm timeinfo;
	
	if (p_time==NULL || val==NULL)
	{
		////LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	
	memset(&timeinfo, 0x0, sizeof(struct tm));

	timeinfo.tm_year = p_time->m_year - 1900;
	timeinfo.tm_mon  = p_time->m_month - 1;
	timeinfo.tm_mday = p_time->m_day;
	//timeinfo.tm_wday = p_time->m_wday;
	timeinfo.tm_hour = p_time->m_hour;
	timeinfo.tm_min  = p_time->m_minute;
	timeinfo.tm_sec  = p_time->m_second;
	
	*val = mktime(&timeinfo);

	return 0;
}


/*
 *  tools_get_system_time
 *
 *  Description:
 *  	get local system time
 *
 *  Parameters:
 *    p_time: [out]
 *        system time struct pointer
 *    val: [out]
 *        time_t value
 *
 *  Returns:
 *    0 : successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int tools_get_system_time(SYSTEMTIME *p_time, time_t *val)
{
	int ret = 0;
	time_t now = 0;
	
	if (p_time == NULL)
	{
		////LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	
	time(&now);	
	ret = tools_value_to_systemtime(p_time, &now);
	if (ret < 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. tools_value_to_systemtime() failed. ret=%d\n", ret);

	*val = now;

	return ret;
}


/*
 *  tools_set_system_time
 *
 *  Description:
 *  	set local system time
 *
 *  Parameters:
 *    p_time: [in]
 *        system time struct pointer
 *
 *  Returns:
 *    0 : successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int tools_set_system_time(SYSTEMTIME *p_time)
{
	int ret = 0;
	time_t val = 0;
	struct timeval tv;

	if (p_time == NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}

	ret = tools_systemtime_to_value(p_time, &val);
	if (ret < 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. tools_systemtime_to_value() failed. ret=%d\n", ret);

	tv.tv_sec  = val;  
	tv.tv_usec = 0;  
	ret = settimeofday(&tv, (struct timezone *)0);
	if(ret < 0)  
	{  
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. set system soft-clock fail. ret = %d\n", ret);  
    return -1;  
	} 
	
	return 0;
}


/*
 *  trim_space_string
 *
 *  Description:
 *  	Remove space char(s) at the beginning and the end of string
 *
 *  Parameters:
 *    output: [out]
 *        output string
 *    input: [in]
 *        input string
 *
 *  Returns:
 *
 *  Comments:
 *
 */
static void trim_space_string(char *output, char *input)
{
	char *start = NULL;
	char *end   = NULL;
	char *temp  = NULL;
	
	temp = input;
	while (*temp == ' ')
	{
		++temp;
	}
	
	start = temp; //head point
	temp = input + strlen(input) - 1; 
	
	while (*temp == ' ')
	{
		--temp;
	}
	
	end = temp; //tail point
	for(input=start; input<=end; )
	{
		*output++ = *input++;
	}
	
	*output = '\0';
}


/*
 *  get_value
 *
 *  Description:
 *  	get key value from string 
 *
 *  Parameters:
 *    value: [out]
 *        value string
 *    key: [in]
 *        key string
 *    key_and_value: [in]
 *        "key=value" string
 *
 *  Returns:
 *
 *  Comments:
 *
 */
static int get_value(char *value, const char *key, char * key_and_value)
{
  char *p = NULL;
  char tmp_value[MAX_SIZE] = {0x0};

	if (key_and_value==NULL || key==NULL || value==NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	
  p = strstr(key_and_value, key);
  if (p == NULL)
  {
		//printf("get_value: no %s in %s\n", key, key_and_value);
		return -1;
  }

  p += strlen(key);
  trim_space_string(tmp_value, p);

  p = strstr(tmp_value, "=");
  if(p == NULL)
  {
  	//printf("get_value: error. no \"=\" in %s\n", key_and_value);
		return -1;
  }
 
  p += strlen("=");
  trim_space_string(tmp_value, p);

  p = strstr(tmp_value, "=");
  if(p != NULL)
  {
  	//printf("get_value: error. more \"=\" in %s\n", key_and_value);
		return -1;
  }

  trim_space_string(value, tmp_value);
  
  return 0;
}


/*
 *  tools_write_config
 *
 *  Description:
 *  	write key value to configure file 
 *
 *  Parameters:
 *    value: [in]
 *        value string
 *    key: [in]
 *        key string
 *    file_name: [in]
 *        configure file 
 *
 *  Returns:
 *		0 : successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int tools_write_config(char *value, char *key, char *file_name)
{
	int ret = 0;
	FILE *file = NULL;
	char file_name_bak[100]   = {0x0};
	char config_buf[MAX_SIZE] = {0x0};
	char buffer[MAX_SIZE]     = {0x0}; 
	long size = 0;
	int flag = 0;

	if (value==NULL || key==NULL || file_name==NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	
	file = fopen(file_name, "rb+");
	if (file == NULL)
		file = fopen(file_name, "wb+");
	if (file == NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. open %s failed\n", file_name);
		return -1;
	}
	
	//获得文件大小
	fseek(file, 0, SEEK_END); // 将文件指针指向末尾
	size = ftell(file);
	if (size > MAX_FILE_LEN)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. the size of file >= %d\n", MAX_FILE_LEN);
		fclose(file);
		return -1;
	}
	fseek(file, 0, SEEK_SET); //将文件指针指向开头
	
	size = 0;
	while ( !feof(file) )
	{
	  fgets(buffer, sizeof(buffer), file);
	
		if (strlen(buffer) == 0)
			continue; 	
		//if (strncmp(buffer,"#",1) == 0)
		//	continue;  
		if (buffer[strlen(buffer)-1] == 0x0a)	
			buffer[strlen(buffer)-1] = 0x0;
		if (buffer[strlen(buffer)-1] == 0x0d)	
			buffer[strlen(buffer)-1] = 0x0;	
	
	  if (strncmp(buffer,"#",1)!=0 && strstr(buffer, key)!=NULL)   //判断key是否存在
	  {
	  	if (flag == 0)
	  	{
	      flag = 1;
	      size += sprintf(config_buf+size, "%s = %s\n", key, value);
	    }
	  }
	  else
	  {	
      memcpy(config_buf+size, buffer, strlen(buffer));
      size += strlen(buffer);
	    config_buf[size] = '\n';
	    size += 1;
		}

	  memset(buffer, 0x0, sizeof(buffer));
	}

	if (flag != 1)  //如果不存在则把key value写入到最后一行
	{
		size += sprintf(config_buf+size, "%s = %s\n", key, value);
	}
	
	if (file != NULL)
		fclose(file);

	strcpy(file_name_bak, file_name);
	strcat(file_name_bak, ".bak");

	file = fopen(file_name_bak, "wb+");
	if (file == NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. open %s failed\n", file_name_bak);
		return -1;
	}
	ret = fwrite(config_buf, 1, size, file);
	if (ret < 0)
	{
		fclose(file);
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. fwrite %s failed.\n", file_name_bak);
		return -1;
	}
	fclose(file);
	
	ret = rename(file_name_bak, file_name);
	if (ret < 0)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. rename file %s to %s failed\n", file_name_bak, file_name);
		return -1;
	}
	else
		system("sync");

	return 0;
}


/*
 *  tools_read_config
 *
 *  Description:
 *  	read key value from configure file 
 *
 *  Parameters:
 *    value: [out]
 *        value string
 *    key: [in]
 *        key string
 *    file_name: [int]
 *        configure file 
 *
 *  Returns:
 *		>=0: successfully, the size of value string
 *    -1 : failed.
 *
 *  Comments:
 *
 */
int tools_read_config(char *value, const char *key, const char *file_name)
{
	FILE *file = NULL;
	char buffer[MAX_SIZE]  = {0}; 
	char tmp_value[MAX_SIZE] = {0};
	int size =0;
	
	if (value==NULL || key==NULL || file_name==NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	
	file = fopen(file_name, "rb"); //以只读方式打开
	if (file == NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. open %s failed\n", file_name);
		return -1;
	}
	
	while ( !feof(file) )
	{
		fgets(buffer, sizeof(buffer), file);
		
		if (strlen(buffer) == 0)
			continue; 	
		if (strncmp(buffer,"#",1) == 0)
			continue;  
		if (buffer[strlen(buffer)-1] == 0x0a)	
			buffer[strlen(buffer)-1] = 0x0;
		if (buffer[strlen(buffer)-1] == 0x0d)	
			buffer[strlen(buffer)-1] = 0x0;	

		get_value(tmp_value, key, buffer);
		if(strlen(tmp_value) != 0)
			break;
		
		memset(buffer, 0x0, sizeof(buffer));
		memset(tmp_value, 0x0, sizeof(tmp_value));
	}
	
	if (strlen(tmp_value) != 0)
	{
		size = strlen(tmp_value);
		memcpy(value, tmp_value, size);
	}
	else
		size = 0;

	if(file != NULL)
		fclose(file);
	
	return size;
}

static int is_memory_equal(unsigned char *src, unsigned char *dest, int length)
{
	int ret = 0;
	ret = memcmp(src, dest, length);
	if(ret == 0)
		return 1;
	else
		return 0;
}

static int get_key_index(unsigned char *key, int key_len, unsigned char *buf, int line_len)
{
	int i = 0, ret = 0;
	for(i = 0; i <= line_len - key_len; i++)
	{
		if(buf[i] == key[0])
		{
			ret = is_memory_equal(key, &buf[i], key_len);
			if(ret == 1)
			{
			    return i;
			}
		}
	}
	
	return -1;
}

static int get_key_and_value(unsigned char *key, int *key_len, unsigned char *value, int *val_len, unsigned char *buf, int line_len)
{
	int key_begin = 0, value_begin = 0, end = 0;
	if((key==NULL) || (key_len==NULL) || (value==NULL) || (val_len==NULL) || (buf==NULL))
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "Parameters error.\n");
		return -1;
	}
	
	key_begin = get_key_index((unsigned char*)"<key>", 5, buf, line_len);
	value_begin = get_key_index((unsigned char*)"<value>", 7, buf, line_len);
	end = get_key_index((unsigned char*)"<end>", 5, buf, line_len);
	
	if(key_begin >= 0)
	{
		if((value_begin > key_begin + 5) && (end > value_begin + 7))
		{
			*key_len = value_begin - key_begin - 5;
			memcpy(key, buf+key_begin+5, *key_len);
			
			*val_len = end - value_begin - 7;
			memcpy(value, buf+value_begin+7, *val_len);
			
			return 0;
		}
	}
	
	return -1;
}



/*
 *  tools_read_bin_config
 *
 *  tools_read_bin_config:
 *  	read key value from bin configure file 
 *
 *  Parameters:
 *    value: [out]
 *        value string
 *    key: [in]
 *        key string
 *    key_len: [in]
 *        the size of key
 *    file_name: [in]
 *        configure file 
 *
 *  Returns:
 *		>=0: successfully, the size of value string
 *    -1 : failed.
 *
 *  Comments:
 *
 */
int tools_read_bin_config(unsigned char *value, void *key, int key_len, char *file_name)
{
  FILE *file = NULL;
	unsigned char buffer[MAX_SIZE]  = {0x0}; 
	unsigned char tmp_key[MAX_SIZE] = {0x0};
	unsigned char tmp_value[MAX_SIZE] = {0x0};
	int tmp_key_len = 0, tmp_value_len = 0, length = 0, val_len = -1;
	int size = 0, ret = 0;
	
	if (value==NULL || key==NULL || file_name==NULL || key_len<=0)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	
	file = fopen(file_name, "rb"); //以只读方式打开
	if (file == NULL)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. open %s failed\n", file_name);
		return -1;
	}
	
	while ( !feof(file) )
	{  
		size = fread(buffer, 1, MAX_SIZE, file);
		if(size > 0)
		{   
			memset(tmp_key, 0, sizeof(tmp_key));
			memset(tmp_value, 0, sizeof(tmp_value));
			ret = get_key_and_value(tmp_key, &tmp_key_len, tmp_value, &tmp_value_len, buffer, size);
			if(ret >= 0)
			{
				length = (tmp_key_len > key_len) ? key_len : tmp_key_len;
				ret = is_memory_equal((unsigned char*)key, tmp_key, length);
				if((ret == 1) && (tmp_key_len == key_len))
				{
					memcpy(value, tmp_value, tmp_value_len);
					val_len = tmp_value_len;
					break;
				}
				fseek(file, (tmp_key_len+tmp_value_len+17-size), SEEK_CUR); 
			}
		}
	}
	fclose(file);
	
	return val_len;
}


/*
 *  tools_write_bin_config_with_lock
 *
 *  Description:
 *  	read bin key value from configure file 
 *
 *  Parameters:
 *    value: [out]
 *        value string
 *    key: [in]
 *        key string
 *    key_len:[in]
 *        key string length
 *    file_name: [int]
 *        configure file 
 *
 *  Returns:
 *		>=0: successfully, the size of value string
 *    -1 : failed.
 *
 *  Comments:
 *
 */
int tools_write_bin_config_with_lock(unsigned char *value, int val_len, unsigned char *key, int key_len, char *file_name)
{
	int ret = 0;
	FILE *file_r = NULL, *file_w = NULL;
	char file_name_bak[100] = {0x0};
	unsigned char config_buf[MAX_SIZE] = {0x0};
	unsigned char buffer[MAX_SIZE] = {0x0};
	unsigned char tmp_key[MAX_SIZE] = {0x0};
	unsigned char tmp_value[MAX_SIZE] = {0x0};
	int tmp_key_len = 0, tmp_value_len = 0, length = 0;
	long size = 0;
	int flag = 0;
	
	if(value==NULL || key==NULL || file_name==NULL || val_len<=0 || key_len<=0)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. input parameter is NULL\n");
		return -1;
	}
	strcpy(file_name_bak, file_name);
	strcat(file_name_bak, ".bak");
	
	file_r = fopen(file_name, "rb+");
    
	if ((file_r == NULL) )
	{
		file_r = fopen(file_name, "wb+");
	}
    file_w = fopen(file_name_bak, "wb+");
    if ((file_w == NULL) || (file_r == NULL))
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. open %s failed\n", file_name_bak);
		return -1;
	}

    //获得文件大小
	fseek(file_r, 0, SEEK_END); // 将文件指针指向末尾
	size = ftell(file_r);
	if (size > MAX_FILE_LEN)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. the size of file >= %d\n", MAX_FILE_LEN);
		fclose(file_r);
        fclose(file_w);
		return -1;
	}
	fseek(file_r, 0, SEEK_SET); //将文件指针指向开头

	size = 0;

	while( !feof(file_r) )
	{
		size = fread(buffer, 1, MAX_SIZE, file_r);
		
		if(size > 0)
		{
			memset(tmp_key, 0, sizeof(tmp_key));
			memset(tmp_value, 0, sizeof(tmp_value));
			ret = get_key_and_value(tmp_key, &tmp_key_len, tmp_value, &tmp_value_len, buffer, sizeof(buffer));
			if(ret >= 0)
			{
				memset(config_buf, 0, sizeof(config_buf));
				
				length = (tmp_key_len > key_len) ? key_len : tmp_key_len;
				ret = is_memory_equal(key, tmp_key, length);
				
				memcpy(config_buf, (void*)"<key>", 5);
				memcpy(config_buf+5, tmp_key, tmp_key_len);
				memcpy(config_buf+5+tmp_key_len, (void*)"<value>", 7);
				
				if((ret == 1) && (tmp_key_len == key_len))
				{
					memcpy(config_buf+5+tmp_key_len+7, value, val_len);
					memcpy(config_buf+5+tmp_key_len+7+val_len, (void*)"<end>", 5);
					length = 17 + tmp_key_len + val_len;
					fwrite(config_buf, 1, length, file_w);
					flag = 1;
				}
				else
				{
				  memcpy(&config_buf[12+tmp_key_len], tmp_value, tmp_value_len);
				  memcpy(&config_buf[12+tmp_key_len+tmp_value_len], (void*)"<end>", 5);
				  length = 17 + tmp_key_len + tmp_value_len;
				  fwrite(config_buf, 1, length, file_w);
				}
				
				fseek(file_r, (tmp_key_len+tmp_value_len+17-size), SEEK_CUR);
			}
			else
			{
				//LOG_WRITE(RUN_LOG, LOG_ERROR, "wrong file\n");
			}
		}
	}

	if(flag == 0)
	{
		memset(config_buf, 0, sizeof(config_buf));
		memcpy(config_buf, (void*)"<key>", 5);
		memcpy(config_buf+5, key, key_len);
		memcpy(config_buf+5+key_len, (void*)"<value>", 7);
		memcpy(config_buf+5+key_len+7, value, val_len);
		memcpy(config_buf+5+key_len+7+val_len, (void*)"<end>", 5);
		length = 17 + key_len + val_len;
		fwrite(config_buf, 1, length, file_w);
	}

	fclose(file_r);
	fclose(file_w);

	ret = rename(file_name_bak, file_name);
	if (ret < 0)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. rename file %s to %s failed\n", file_name_bak, file_name);
		return -1;
	}
	else
		system("sync");

	return 0;
}


/*
 *  tools_write_bin_conf_with_check
 *
 *  Description:
 *  	增加写参数设置判断，若参数值未改变，则不写文件
 *
 *  Parameters:
 *    value: [out]
 *        value string
 *    key: [in]
 *        key string
 *    key_len:[in]
 *        key string length
 *    file_name: [int]
 *        configure file 
 *
 *  Returns:
 *	  =0: successfully
 *    -1 : failed.
 *
 *  Comments:
 *
 */
int tools_write_bin_config_with_check(unsigned char *value, int val_len, void *key, int key_len, char *file_name)
{
    int ret = 0;
    unsigned char tmp_value[MAX_SIZE] = {0x0};
    int length = 0;

    ret = tools_read_bin_config(tmp_value, key, key_len, file_name);

    if(ret == val_len)   //判断参数是否与文件中的一致
    {
        if(is_memory_equal(value, tmp_value, val_len))
        {
            return 0;
        }
    }
    
    ret = tools_write_bin_config_with_lock(value, val_len, (unsigned char*)key, key_len, file_name);
    return ret;
}


static int is_digital(unsigned char *str, int length)
{
	int i = 0;
	for(i = 0; i < length; i++)
	{
		if((str[i] > 0x39) || (str[i] < 0x30))
		{
			return -1;
		}
	}

	return 0;
}


/*
 *  tools_string_to_bcd
 *
 *  Description:
 *  	将字符串转BCD码
 *
 *  Parameters:
 *    output: [out]
 *        转换后的BCD码
 *    output_size: [in]
 *        需要转换的BCD码长度，默认为待转换的字符串长度的一半
 *    input: [in]
 *        待转换的字符串
 *    input_size:[in]
 *		  待转换的字符串长度，input_size = output_size * 2	
 *  Returns:
 *	  0: successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int tools_string_to_bcd(unsigned char *output, int output_size, unsigned char *input, int input_size)
{
	unsigned char value = 0;
	int i = 0, j = 0;
	int ret = 0;

	if(input_size != output_size * 2)
	{
		return -1;
	}

	ret = is_digital(input, input_size);
	if(ret < 0)
		return -1;

	for(i = 0; i < input_size; i+=2)
	{
		value = (input[i] - 0x30) * 10;
		value += (input[i + 1] - 0x30);
		output[j] = DEC2BCD(value);
		j++;
	}

	return 0;
}

/*
 *  tools_config_mutex_init
 *
 *  Description:
 *  	initialize config write mutex
 *
 *  Parameters:
 *	
 *  Returns:
 *
 *  Comments:
 *
 */
void tools_init_config(void)
{
	pthread_mutex_init(&g_config_mutex, NULL);
}


/*
 *  tools_write_bin_config
 *
 *  Description:
 *  	read bin key value from configure file 
 *
 *  Parameters:
 *    value: [out]
 *        value string
 *    key: [in]
 *        key string
 *    key_len:[in]
 *        key string length
 *    file_name: [int]
 *        configure file 
 *
 *  Returns:
 *		>=0: successfully, the size of value string
 *    -1 : failed.
 *
 *  Comments:
 *
 */
int tools_write_bin_config(unsigned char *value, int val_len, void *key, int key_len, char *file_name)
{
	int ret = 0;
	pthread_mutex_lock(&g_config_mutex);
	ret = tools_write_bin_config_with_lock(value, val_len, (unsigned char*)key, key_len, file_name);
	pthread_mutex_unlock(&g_config_mutex);
	return ret;
}

unsigned short tools_big_endian_to_ushort(unsigned char *data)
{
	unsigned short value = 0;
	value = ((unsigned short)data[0] << 8) + (unsigned short)data[1];
	return value;
}

unsigned int tools_big_endian_to_uint(unsigned char *data)
{
	unsigned int value = 0;
	value = ((unsigned int)data[0] << 24) + ((unsigned int)data[1] << 16) + ((unsigned int)data[2] << 8) + (unsigned int)data[3];  
	return value;
}

void tools_ushort_to_big_endian(unsigned char *data, unsigned short value)
{
	*data++ = (unsigned char)(value >> 8);
	*data++ = (unsigned char)(value & 0xFF);
}

void tools_uint_to_big_endian(unsigned char *data, unsigned int value)
{
	*data++ = (unsigned char)(value >> 24);
	*data++ = (unsigned char)(value >> 16);
	*data++ = (unsigned char)(value >> 8);
	*data++ = (unsigned char)(value & 0xFF);
}

unsigned short tools_little_endian_to_ushort(unsigned char *data)
{
	unsigned short value = 0;
	value = ((unsigned short)data[1] << 8) + (unsigned short)data[0];
	return value;
}

unsigned int tools_little_endian_to_uint(unsigned char *data)
{
	unsigned int value = 0;
	value = ((unsigned int)data[3] << 24) + ((unsigned int)data[2] << 16) + ((unsigned int)data[1] << 8) + (unsigned int)data[0];  
	return value;
}

void tools_ushort_to_little_endian(unsigned char *data, unsigned short value)
{
	*data++ = (unsigned char)(value & 0xFF);
	*data++ = (unsigned char)(value >> 8);
}

void tools_uint_to_little_endian(unsigned char *data, unsigned int value)
{
	*data++ = (unsigned char)(value & 0xFF);
	*data++ = (unsigned char)(value >> 8);
	*data++ = (unsigned char)(value >> 16);
	*data++ = (unsigned char)(value >> 24);
}

/*
 *  create_thread
 *
 *  Description:
 *    creadt thread
 *
 *  Parameters:
 *    comm_handler:
 *        recall function
 *		param: [in]
 *			  thread parameter
 *
 *  Returns:
 *    0 : successfully.
 *    -1: failed.
 *
 *  Comments:
 *
 */
int create_thread(thread_handler_t comm_handler, void *param)
{
	int ret = 0;
	pthread_t thread_id = 0;	
	pthread_attr_t attr;

	ret = pthread_attr_init(&attr);
	if (ret != 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. pthread_attr_init failed\n");
	ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);    //Set thread inheritance
	if (ret != 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. pthread_attr_setinheritsched failed\n");
	ret = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);                //set thread scheduling policy
	if (ret != 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. pthread_attr_setschedpolicy failed\n");
	ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);             //Set thread scope
	if (ret != 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. pthread_attr_setscope failed\n");
	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    //set thread separation state property
	if (ret != 0)
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. pthread_attr_setdetachstate failed\n");
	
	ret = pthread_create(&thread_id, &attr, comm_handler, (void *)param);	
	if (ret != 0)
	{
		//LOG_WRITE(RUN_LOG, LOG_ERROR, "error. create mcu_task_processor_thread() failed\n");
		return -1;
	}

	pthread_attr_destroy(&attr);
	
	return 0;
}


