#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>

static int32_t last_status = 0;
int bash(const char *command) { return command ? system(command) : 1; }

char* process_host_arch(void) {
#if defined(__x86_64__)
    return strdup("amd64");
#elif defined(__aarch64__)
    return strdup("arm64");
#elif defined(__arm__)
    return strdup("arm");
#else
    return strdup("unknown");
#endif
}

char* process_look_path(const char* executable) {
    if (!executable || !*executable) return strdup("");
    if (strchr(executable, '/')) return access(executable, X_OK) == 0 ? strdup(executable) : strdup("");
    const char* env = getenv("PATH"); if (!env) return strdup("");
    char* paths = strdup(env); if (!paths) return NULL;
    char* save = NULL;
    for (char* dir = strtok_r(paths, ":", &save); dir; dir = strtok_r(NULL, ":", &save)) {
        size_t n = strlen(dir)+1+strlen(executable)+1; char* candidate = malloc(n); if(!candidate){free(paths);return NULL;}
        snprintf(candidate,n,"%s/%s",dir,executable);
        if(access(candidate,X_OK)==0){free(paths);return candidate;} free(candidate);
    }
    free(paths); return strdup("");
}

char* process_shell_quote(const char* value) {
    if(!value) return strdup("''");
    size_t n=2; for(const char* p=value;*p;p++) n += (*p=='\'') ? 4 : 1;
    char* out=malloc(n+1); if(!out)return NULL; size_t j=0; out[j++]='\'';
    for(const char* p=value;*p;p++){ if(*p=='\''){ memcpy(out+j,"'\\''",4); j+=4; } else out[j++]=*p; }
    out[j++]='\''; out[j]='\0'; return out;
}

char* process_run_capture(const char* command) {
    if(!command){last_status=127;return strdup("");}
    size_t n=strlen(command)+6; char* full=malloc(n); if(!full){last_status=127;return NULL;}
    snprintf(full,n,"%s 2>&1",command);
    FILE* pipe=popen(full,"r"); free(full); if(!pipe){last_status=127;return strdup(strerror(errno));}
    size_t cap=4096,len=0; char* out=malloc(cap); if(!out){pclose(pipe);last_status=127;return NULL;}
    char buf[2048]; while(fgets(buf,sizeof(buf),pipe)){size_t m=strlen(buf); if(len+m+1>cap){while(len+m+1>cap)cap*=2; char* next=realloc(out,cap); if(!next){free(out);pclose(pipe);last_status=127;return NULL;} out=next;} memcpy(out+len,buf,m);len+=m;}
    out[len]='\0'; int status=pclose(pipe); if(status==-1)last_status=127; else if(WIFEXITED(status))last_status=WEXITSTATUS(status); else if(WIFSIGNALED(status))last_status=128+WTERMSIG(status); else last_status=1; return out;
}
int32_t process_last_exit_status(void){return last_status;}
