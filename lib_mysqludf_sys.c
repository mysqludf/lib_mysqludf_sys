/* 
	lib_mysqludf_sys - a library with miscellaneous (operating) system level functions
	Copyright (C) 2007  Roland Bouman 
	Copyright (C) 2008-2009  Roland Bouman and Bernardo Damele A. G.
	web: http://www.mysqludf.org/
	email: mysqludfs@gmail.com, bernardo.damele@gmail.com
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32)
#define DLLEXP __declspec(dllexport) 
#else
#define DLLEXP
#endif

#ifdef STANDARD
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#endif
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>
#include <stdlib.h>

#include <ctype.h>

#ifdef HAVE_DLOPEN
#ifdef	__cplusplus
extern "C" {
#endif

#define LIBVERSION "lib_mysqludf_sys version 0.0.3"

#ifdef __WIN__
#define SETENV(name,value)		SetEnvironmentVariable(name,value);
#else
#define SETENV(name,value)		setenv(name,value,1);		
#endif

DLLEXP 
my_bool lib_mysqludf_sys_info_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
);

DLLEXP 
void lib_mysqludf_sys_info_deinit(
	UDF_INIT *initid
);

DLLEXP 
char* lib_mysqludf_sys_info(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
);

/**
 * sys_get
 * 
 * Gets the value of the specified environment variable.
 */
DLLEXP 
my_bool sys_get_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
);

DLLEXP 
void sys_get_deinit(
	UDF_INIT *initid
);

DLLEXP 
char* sys_get(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
);

/**
 * sys_set
 * 
 * Sets the value of the environment variables.
 * This function accepts a set of name/value pairs
 * which are then set as environment variables.
 * Use sys_get to retrieve the value of such a variable 
 */
DLLEXP 
my_bool sys_set_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
);

DLLEXP 
void sys_set_deinit(
	UDF_INIT *initid
);

DLLEXP 
long long sys_set(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error
);

/**
 * sys_exec
 * 
 * executes the argument commandstring and returns its exit status.
 * Beware that this can be a security hazard.
 */
DLLEXP 
my_bool sys_exec_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
);

DLLEXP 
void sys_exec_deinit(
	UDF_INIT *initid
);

DLLEXP 
my_ulonglong sys_exec(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error
);

/**
 * sys_eval
 * 
 * executes the argument commandstring and returns its standard output.
 * Beware that this can be a security hazard.
 */
DLLEXP 
my_bool sys_eval_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
);

DLLEXP 
void sys_eval_deinit(
	UDF_INIT *initid
);

DLLEXP 
char* sys_eval(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
);


#ifdef	__cplusplus
}
#endif

/**
 * lib_mysqludf_sys_info
 */
my_bool lib_mysqludf_sys_info_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	my_bool status;
	if(args->arg_count!=0){
		strcpy(
			message
		,	"No arguments allowed (udf: lib_mysqludf_sys_info)"
		);
		status = 1;
	} else {
		status = 0;
	}
	return status;
}
void lib_mysqludf_sys_info_deinit(
	UDF_INIT *initid
){
}
char* lib_mysqludf_sys_info(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
){
	strcpy(result,LIBVERSION);
	*length = strlen(LIBVERSION);
	return result;
}

my_bool sys_get_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	if(args->arg_count==1
	&&	args->arg_type[0]==STRING_RESULT){
		initid->maybe_null = 1;
		return 0;
	} else {
		strcpy(
			message
		,	"Expected exactly one string type parameter"
		);		
		return 1;
	}
}
void sys_get_deinit(
	UDF_INIT *initid
){
}
char* sys_get(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
){
	char* value = getenv(args->args[0]);
	if(value == NULL){
		*is_null = 1;
	} else {
		*length = strlen(value);
	} 
	return value;
}

my_bool sys_set_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	if(args->arg_count!=2){
		strcpy(
			message
		,	"Expected exactly two arguments"
		);		
		return 1;
	}
	if(args->arg_type[0]!=STRING_RESULT){
		strcpy(
			message
		,	"Expected string type for name parameter"
		);		
		return 1;
	}
	args->arg_type[1]=STRING_RESULT;
	if((initid->ptr=malloc(
		args->lengths[0]
	+	1
	+	args->lengths[1]
	+	1
	))==NULL){
		strcpy(
			message
		,	"Could not allocate memory"
		);		
		return 1;
	}	
	return 0;
}
void sys_set_deinit(
	UDF_INIT *initid
){
	if (initid->ptr!=NULL){
		free(initid->ptr);
	}
}
long long sys_set(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error
){	
	char *name = initid->ptr;
	char *value = name + args->lengths[0] + 1; 
	memcpy(
		name
	,	args->args[0]
	,	args->lengths[0]
	);
	*(name + args->lengths[0]) = '\0';
	memcpy(
		value
	,	args->args[1]
	,	args->lengths[1]
	);
	*(value + args->lengths[1]) = '\0';
	return SETENV(name,value);		
}

my_bool sys_exec_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	unsigned int i=0;
	if(args->arg_count == 1
	&& args->arg_type[i]==STRING_RESULT){
		return 0;
	} else {
		strcpy(
			message
		,	"Expected exactly one string type parameter"
		);		
		return 1;
	}
}
void sys_exec_deinit(
	UDF_INIT *initid
){
}
my_ulonglong sys_exec(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error
){
	return system(args->args[0]);
}

my_bool sys_eval_init(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *message
){
	unsigned int i=0;
	if(args->arg_count == 1
	&& args->arg_type[i]==STRING_RESULT){
		return 0;
	} else {
		strcpy(
			message
		,	"Expected exactly one string type parameter"
		);		
		return 1;
	}
}
void sys_eval_deinit(
	UDF_INIT *initid
){
	if(initid->ptr)
	{
		free(initid->ptr);
		initid->ptr = NULL;
	}
}
char* sys_eval(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
){
	// Rewrite by ZhengWang wa56abc@gmail.com
	// result is char[256], we could use it or we alloc new buffer & free it at deinit
	FILE *pipe = popen(args->args[0], "r");
	if(!pipe)
	{
		*error = 1;
		*length = 0;
		*is_null = 1;
		initid->ptr = NULL;
		memset(result, 0, 256);

		return result;
	}

	unsigned len_ = fread(result, 1, 256, pipe);
	if( len_ < 256 )
	{	
		// For Length < 256, we store string in result, and return it directly
		pclose(pipe);
		*length = len_;
		return result;
	}

	char* buf_ = (char*)malloc(len_);
	unsigned long bufLen_ = len_;
	memcpy(buf_ + 0, result, len_);

	while((len_ = fread(result, 1, 256, pipe)) > 0 )
	{
		buf_ = (char*)realloc(buf_, bufLen_ + len_);
		memcpy(buf_ + bufLen_, result, len_);
		bufLen_ += len_;
	}
	
	pclose(pipe);
	*length = bufLen_;
	initid->ptr = buf_;
	memset(result, 0, 256);

	return buf_;
}


#endif /* HAVE_DLOPEN */
