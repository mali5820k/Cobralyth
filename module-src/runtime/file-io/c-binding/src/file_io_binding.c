#include "file.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static int32_t neg_errno(void) { return errno ? -errno : -1; }
static int32_t close_result(int fd, int32_t result) {
    if (close(fd) != 0 && result == 0) return neg_errno();
    return result;
}
static int32_t write_all_fd(int fd, const char* data, size_t len) {
    if (!data) return -EINVAL;
    size_t off = 0;
    while (off < len) {
        ssize_t n = write(fd, data + off, len - off);
        if (n < 0) { if (errno == EINTR) continue; return neg_errno(); }
        if (n == 0) return -EIO;
        off += (size_t)n;
    }
    return 0;
}
static int32_t write_flags(const char* path, const char* content, int flags, int newline) {
    if (!path || !content) return -EINVAL;
    int fd = open(path, flags, 0664);
    if (fd < 0) return neg_errno();
    int32_t rc = write_all_fd(fd, content, strlen(content));
    if (rc == 0 && newline) rc = write_all_fd(fd, "\n", 1);
    return close_result(fd, rc);
}
static int stat_path(const char* path, struct stat* st, int follow) {
    if (!path || !st) return -EINVAL;
    if ((follow ? stat(path, st) : lstat(path, st)) != 0) return neg_errno();
    return 0;
}
static char* join_path(const char* a, const char* b) {
    size_t al = strlen(a), bl = strlen(b);
    int slash = al > 0 && a[al-1] != '/';
    char* out = malloc(al + (size_t)slash + bl + 1);
    if (!out) return NULL;
    memcpy(out, a, al);
    if (slash) out[al++] = '/';
    memcpy(out + al, b, bl + 1);
    return out;
}

int32_t path_exists(const char* path) { struct stat st; return stat_path(path, &st, 1) == 0; }
int32_t file_exists(const char* path) { return is_file(path); }
int32_t directory_exists(const char* path) { return is_directory(path); }
int32_t is_file(const char* path) { struct stat st; return stat_path(path,&st,1)==0 && S_ISREG(st.st_mode); }
int32_t is_directory(const char* path) { struct stat st; return stat_path(path,&st,1)==0 && S_ISDIR(st.st_mode); }
int32_t is_symlink(const char* path) { struct stat st; return stat_path(path,&st,0)==0 && S_ISLNK(st.st_mode); }

int32_t create_file(const char* path) {
    if (!path) return -EINVAL;
    int fd = open(path, O_CREAT|O_WRONLY, 0664);
    return fd < 0 ? neg_errno() : close_result(fd, 0);
}
int32_t create_directory(const char* path) {
    if (!path) return -EINVAL;
    if (mkdir(path, 0775) == 0 || errno == EEXIST) return 0;
    return neg_errno();
}
int32_t create_directories(const char* path) {
    if (!path || !*path) return -EINVAL;
    char* copy = strdup(path);
    if (!copy) return -ENOMEM;
    for (char* p = copy + (copy[0] == '/' ? 1 : 0); ; ++p) {
        if (*p == '/' || *p == '\0') {
            char saved = *p;
            *p = '\0';
            if (*copy && mkdir(copy, 0775) != 0 && errno != EEXIST) {
                int32_t rc = neg_errno();
                free(copy);
                return rc;
            }
            *p = saved;
            if (saved == '\0') break;
        }
    }
    free(copy);
    return 0;
}
int32_t delete_file(const char* path) { if (!path) return -EINVAL; return unlink(path)==0 ? 0 : neg_errno(); }
int32_t delete_directory(const char* path) { if (!path) return -EINVAL; return rmdir(path)==0 ? 0 : neg_errno(); }
int32_t delete_symlink(const char* path) { return delete_file(path); }

static int32_t copy_file_impl(const char* src, const char* dst, mode_t mode) {
    int in = open(src, O_RDONLY);
    if (in < 0) return neg_errno();
    int out = open(dst, O_CREAT|O_WRONLY|O_TRUNC, mode & 0777);
    if (out < 0) { int32_t e=neg_errno(); close(in); return e; }
    char buf[64 * 1024];
    int32_t rc = 0;
    for (;;) {
        ssize_t n = read(in, buf, sizeof(buf));
        if (n < 0) { if (errno == EINTR) continue; rc=neg_errno(); break; }
        if (n == 0) break;
        rc = write_all_fd(out, buf, (size_t)n);
        if (rc != 0) break;
    }
    if (close(in) != 0 && rc == 0) rc=neg_errno();
    return close_result(out, rc);
}
int32_t copy_file(const char* src, const char* dst) {
    if (!src || !dst) return -EINVAL;
    struct stat st; if (stat(src,&st)!=0) return neg_errno();
    if (!S_ISREG(st.st_mode)) return -EINVAL;
    return copy_file_impl(src,dst,st.st_mode);
}
int32_t copy_directory(const char* src, const char* dst) {
    if (!src || !dst) return -EINVAL;
    struct stat st; if (stat(src,&st)!=0) return neg_errno();
    if (!S_ISDIR(st.st_mode)) return -ENOTDIR;
    if (mkdir(dst, st.st_mode & 0777) != 0 && errno != EEXIST) return neg_errno();
    DIR* dir = opendir(src); if (!dir) return neg_errno();
    int32_t rc=0; struct dirent* ent;
    while ((ent=readdir(dir))) {
        if (!strcmp(ent->d_name,".") || !strcmp(ent->d_name,"..")) continue;
        char* s=join_path(src,ent->d_name); char* d=join_path(dst,ent->d_name);
        if (!s || !d) { free(s); free(d); rc=-ENOMEM; break; }
        struct stat child;
        if (lstat(s,&child)!=0) rc=neg_errno();
        else if (S_ISDIR(child.st_mode)) rc=copy_directory(s,d);
        else if (S_ISREG(child.st_mode)) rc=copy_file_impl(s,d,child.st_mode);
        else if (S_ISLNK(child.st_mode)) {
            char target[PATH_MAX+1]; ssize_t n=readlink(s,target,PATH_MAX);
            if (n<0) rc=neg_errno(); else { target[n]='\0'; if (symlink(target,d)!=0) rc=neg_errno(); }
        }
        free(s); free(d); if (rc!=0) break;
    }
    closedir(dir); return rc;
}
int32_t copy_path(const char* src, const char* dst) { return is_directory(src) ? copy_directory(src,dst) : copy_file(src,dst); }
int32_t delete_path_recursive(const char* path) {
    if (!path) return -EINVAL;
    struct stat st; if (lstat(path,&st)!=0) return neg_errno();
    if (!S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode)) return unlink(path)==0 ? 0 : neg_errno();
    DIR* dir=opendir(path); if (!dir) return neg_errno(); int32_t rc=0; struct dirent* ent;
    while ((ent=readdir(dir))) {
        if (!strcmp(ent->d_name,".") || !strcmp(ent->d_name,"..")) continue;
        char* p=join_path(path,ent->d_name); if(!p){rc=-ENOMEM;break;}
        rc=delete_path_recursive(p); free(p); if(rc!=0) break;
    }
    closedir(dir); if(rc==0 && rmdir(path)!=0) rc=neg_errno(); return rc;
}
int32_t rename_file(const char* a,const char* b){ if(!a||!b)return -EINVAL; return rename(a,b)==0?0:neg_errno(); }
int32_t rename_directory(const char* a,const char* b){ return rename_file(a,b); }
int32_t move_file(const char* a,const char* b){ return rename_file(a,b); }
int32_t move_directory(const char* a,const char* b){ return rename_file(a,b); }
int32_t create_symlink(const char* target,const char* linkp){ if(!target||!linkp)return -EINVAL; return symlink(target,linkp)==0?0:neg_errno(); }
int32_t create_hardlink(const char* target,const char* linkp){ if(!target||!linkp)return -EINVAL; return link(target,linkp)==0?0:neg_errno(); }

int64_t file_size(const char* path){ struct stat st; return stat_path(path,&st,1)==0 ? (int64_t)st.st_size : -1; }
static int64_t dir_metric(const char* path, int count_only) {
    DIR* d=opendir(path); if(!d)return -1; int64_t total=0; struct dirent* e;
    while((e=readdir(d))){ if(!strcmp(e->d_name,".")||!strcmp(e->d_name,".."))continue; char* p=join_path(path,e->d_name); if(!p){total=-1;break;} struct stat st;
        if(lstat(p,&st)!=0){free(p);total=-1;break;} if(S_ISDIR(st.st_mode)){int64_t x=dir_metric(p,count_only); if(x<0){free(p);total=-1;break;} total+=x;} else if(S_ISREG(st.st_mode)) total += count_only ? 1 : (int64_t)st.st_size; free(p); }
    closedir(d); return total;
}
int64_t directory_size(const char* path){ return dir_metric(path,0); }
int64_t directory_num_files(const char* path){ return dir_metric(path,1); }
int32_t permissions(const char* path){ struct stat st; return stat_path(path,&st,1)==0 ? (int32_t)(st.st_mode&0777) : -1; }
int32_t directory_permissions(const char* path){ return permissions(path); }
int8_t permission_owner(const char* path){ int32_t m=permissions(path); return m<0?-1:(int8_t)((m>>6)&7); }
int8_t permission_group(const char* path){ int32_t m=permissions(path); return m<0?-1:(int8_t)((m>>3)&7); }
int8_t permission_other(const char* path){ int32_t m=permissions(path); return m<0?-1:(int8_t)(m&7); }
int32_t set_permissions(const char* path,int8_t owner,int8_t group,int8_t other){ if(!path||owner<0||owner>7||group<0||group>7||other<0||other>7)return -EINVAL; mode_t m=((mode_t)owner<<6)|((mode_t)group<<3)|(mode_t)other; return chmod(path,m)==0?0:neg_errno(); }
int32_t chmod_path(const char* path,int32_t mode){ if(!path)return -EINVAL; return chmod(path,(mode_t)(mode&0777))==0?0:neg_errno(); }
int32_t chown_path(const char* path,int32_t uid,int32_t gid){ if(!path)return -EINVAL; return chown(path,(uid_t)uid,(gid_t)gid)==0?0:neg_errno(); }

int32_t write_text(const char* p,const char* c){ return write_flags(p,c,O_CREAT|O_WRONLY|O_TRUNC,0); }
int32_t overwrite_file(const char* p,const char* c){ return write_text(p,c); }
int32_t append_text(const char* p,const char* c){ return write_flags(p,c,O_CREAT|O_WRONLY|O_APPEND,0); }
int32_t append_file(const char* p,const char* c){ return append_text(p,c); }
int32_t append_at_end(const char* p,const char* c){ return append_text(p,c); }
int32_t clear_file(const char* p){ return write_text(p,""); }
int32_t write_line(const char* p,const char* c){ return write_flags(p,c,O_CREAT|O_WRONLY|O_TRUNC,1); }
int32_t append_line(const char* p,const char* c){ return write_flags(p,c,O_CREAT|O_WRONLY|O_APPEND,1); }

int32_t write_line_at(const char* path, int64_t idx, const char* content) {
    if (!path || !content || idx < 0) return -EINVAL;
    struct stat st;
    if (stat(path, &st) != 0) return neg_errno();
    size_t path_len = strlen(path);
    char* temp_path = malloc(path_len + 24);
    if (!temp_path) return -ENOMEM;
    snprintf(temp_path, path_len + 24, "%s.clyth-XXXXXX", path);
    int temp_fd = mkstemp(temp_path);
    if (temp_fd < 0) { free(temp_path); return neg_errno(); }
    (void)fchmod(temp_fd, st.st_mode & 0777);
    FILE* in = fopen(path, "r");
    FILE* out = fdopen(temp_fd, "w");
    if (!in || !out) {
        int32_t rc = neg_errno();
        if (in) fclose(in);
        if (out) fclose(out); else close(temp_fd);
        unlink(temp_path); free(temp_path); return rc;
    }
    char* line = NULL; size_t cap = 0; ssize_t n; int64_t current = 0; int replaced = 0;
    while ((n = getline(&line, &cap, in)) >= 0) {
        if (current == idx) {
            if (fputs(content, out) == EOF || fputc('\n', out) == EOF) goto io_error;
            replaced = 1;
        } else if (fwrite(line, 1, (size_t)n, out) != (size_t)n) {
            goto io_error;
        }
        ++current;
    }
    if (!replaced && current == idx) {
        if (fputs(content, out) == EOF || fputc('\n', out) == EOF) goto io_error;
        replaced = 1;
    }
    if (!replaced) { free(line); fclose(in); fclose(out); unlink(temp_path); free(temp_path); return -ERANGE; }
    free(line);
    if (fclose(in) != 0 || fclose(out) != 0) { unlink(temp_path); free(temp_path); return -EIO; }
    if (rename(temp_path, path) != 0) { int32_t rc = neg_errno(); unlink(temp_path); free(temp_path); return rc; }
    free(temp_path); return 0;
io_error:
    free(line); fclose(in); fclose(out); unlink(temp_path); free(temp_path); return -EIO;
}

char* read_text(const char* path) {
    if (!path) return NULL;
    int fd = open(path, O_RDONLY);
    if (fd < 0) return NULL;
    struct stat st;
    if (fstat(fd, &st) != 0) {
        close(fd);
        return NULL;
    }
    size_t cap = st.st_size > 0 ? (size_t)st.st_size + 1 : 4096;
    char* out = malloc(cap);
    if (!out) {
        close(fd);
        return NULL;
    }
    size_t len = 0;
    for (;;) {
        if (len + 4096 + 1 > cap) {
            size_t nc = cap * 2;
            if (nc < len + 4097) nc = len + 4097;
            char* next = realloc(out, nc);
            if (!next) {
                free(out);
                close(fd);
                return NULL;
            }
            out = next;
            cap = nc;
        }
        ssize_t n = read(fd, out + len, cap - len - 1);
        if (n < 0) {
            if (errno == EINTR) continue;
            free(out);
            close(fd);
            return NULL;
        }
        if (n == 0) break;
        len += (size_t)n;
    }
    out[len] = '\0';
    close(fd);
    return out;
}

char* read_line(const char* path, int64_t idx) {
    if (!path || idx < 0) return NULL;
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    char* line = NULL;
    size_t cap = 0;
    ssize_t n;
    int64_t current = 0;
    while ((n = getline(&line, &cap, f)) >= 0) {
        if (current++ == idx) {
            while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r')) line[--n] = '\0';
            fclose(f);
            return line;
        }
    }
    free(line);
    fclose(f);
    return NULL;
}

int32_t dos2unix(const char* path){ char* in=read_text(path); if(!in)return -ENOENT; size_t n=strlen(in),j=0; for(size_t i=0;i<n;i++){ if(in[i]=='\r'&&i+1<n&&in[i+1]=='\n')continue; in[j++]=in[i]; } in[j]='\0'; int32_t rc=write_text(path,in); free(in); return rc; }
int32_t read_to_stdout(const char* path){ char* s=read_text(path); if(!s)return -ENOENT; int32_t rc=write_all_fd(STDOUT_FILENO,s,strlen(s)); free(s); return rc; }
int32_t read_line_to_stdout(const char* path,int64_t idx){ char* s=read_line(path,idx); if(!s)return -ENOENT; int32_t rc=write_all_fd(STDOUT_FILENO,s,strlen(s)); if(rc==0)rc=write_all_fd(STDOUT_FILENO,"\n",1); free(s); return rc; }
int32_t list_files_to_stdout(const char* path){ DIR* d=opendir(path); if(!d)return neg_errno(); struct dirent* e; int32_t rc=0; while((e=readdir(d))){if(!strcmp(e->d_name,".")||!strcmp(e->d_name,".."))continue; rc=write_all_fd(STDOUT_FILENO,e->d_name,strlen(e->d_name)); if(rc==0)rc=write_all_fd(STDOUT_FILENO,"\n",1); if(rc)break;} closedir(d); return rc; }
int32_t file_info_to_stdout(const char* path){ struct stat st; if(stat(path,&st)!=0)return neg_errno(); char b[256]; int n=snprintf(b,sizeof(b),"path=%s size=%lld mode=%03o\n",path,(long long)st.st_size,(unsigned)(st.st_mode&0777)); return n<0?-EIO:write_all_fd(STDOUT_FILENO,b,(size_t)n); }
int32_t directory_info_to_stdout(const char* path){ char b[256]; int64_t files=directory_num_files(path), bytes=directory_size(path); if(files<0||bytes<0)return -ENOENT; int n=snprintf(b,sizeof(b),"path=%s files=%lld size=%lld mode=%03o\n",path,(long long)files,(long long)bytes,(unsigned)permissions(path)); return n<0?-EIO:write_all_fd(STDOUT_FILENO,b,(size_t)n); }
int32_t has_extension(const char* path,const char* ext){ if(!path||!ext)return 0; size_t a=strlen(path),b=strlen(ext); return a>=b && strcmp(path+a-b,ext)==0; }

static int32_t shellf(const char* fmt,const char* a,const char* b,const char* c){ char cmd[PATH_MAX*3]; int n=snprintf(cmd,sizeof(cmd),fmt,a?a:"",b?b:"",c?c:""); if(n<0||(size_t)n>=sizeof(cmd))return -ENAMETOOLONG; int rc=system(cmd); return rc==0?0:-1; }
int32_t make_tarball(const char* src,const char* out,const char* comp,int32_t level){(void)level; if(!src||!out)return -EINVAL; const char* flag=(comp&&strcmp(comp,"xz")==0)?"-cJf":(comp&&strcmp(comp,"zstd")==0)?"--zstd -cf":"-czf"; char fmt[128]; snprintf(fmt,sizeof(fmt),"tar %s \"%%s\" \"%%s\"",flag); return shellf(fmt,out,src,NULL);}
int32_t extract_tarball(const char* a,const char* out){return shellf("mkdir -p \"%s\" && tar -xf \"%s\" -C \"%s\"",out,a,out);}
int32_t make_zip(const char* src,const char* out,int32_t level){(void)level;return shellf("zip -qr \"%s\" \"%s\"",out,src,NULL);}
int32_t extract_zip(const char* a,const char* out){return shellf("unzip -q \"%s\" -d \"%s\"",a,out,NULL);}
int32_t scp(const char* src,const char* dst){return shellf("scp \"%s\" \"%s\"",src,dst,NULL);}
int32_t ssh(const char* server,const char* command){return shellf("ssh \"%s\" \"%s\"",server,command,NULL);}
int32_t print(const char* text){return text?write_all_fd(STDOUT_FILENO,text,strlen(text)):-EINVAL;}
int32_t println(const char* text){int32_t rc=print(text);return rc?rc:write_all_fd(STDOUT_FILENO,"\n",1);}


char* absolute_path(const char* path) {
    if (!path) return NULL;
    char* resolved = realpath(path, NULL);
    if (resolved) return resolved;
    if (path[0] == '/') return strdup(path);
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd))) return strdup(path);
    size_t a = strlen(cwd), b = strlen(path);
    char* out = (char*)malloc(a + 1 + b + 1);
    if (!out) return NULL;
    memcpy(out, cwd, a); out[a] = '/'; memcpy(out + a + 1, path, b + 1);
    return out;
}

char* path_dirname_copy(const char* path) {
    if (!path || !*path) return strdup(".");
    size_t n = strlen(path);
    while (n > 1 && path[n - 1] == '/') n--;
    size_t i = n;
    while (i > 0 && path[i - 1] != '/') i--;
    if (i == 0) return strdup(".");
    while (i > 1 && path[i - 1] == '/') i--;
    char* out = (char*)malloc(i + 1);
    if (!out) return NULL;
    memcpy(out, path, i);
    out[i] = '\0';
    return out;
}

char* glob_text(const char* pattern) {
    if (!pattern) return strdup("");
    glob_t matches;
    memset(&matches, 0, sizeof(matches));
    int rc = glob(pattern, 0, NULL, &matches);
    if (rc != 0) { globfree(&matches); return strdup(""); }
    size_t total = 1;
    for (size_t i = 0; i < matches.gl_pathc; i++) total += strlen(matches.gl_pathv[i]) + 1;
    char* out = (char*)malloc(total);
    if (!out) { globfree(&matches); return NULL; }
    size_t pos = 0;
    for (size_t i = 0; i < matches.gl_pathc; i++) {
        size_t n = strlen(matches.gl_pathv[i]);
        memcpy(out + pos, matches.gl_pathv[i], n); pos += n;
        out[pos++] = '\n';
    }
    out[pos] = '\0';
    globfree(&matches);
    return out;
}


static char* clyth_path_join_impl(const char** parts, size_t count) {
    if (!parts || count == 0) return strdup("");
    size_t total = 1;
    for (size_t i = 0; i < count; i++) {
        const char* p = parts[i] ? parts[i] : "";
        total += strlen(p) + 1;
    }
    char* out = (char*)malloc(total);
    if (!out) return NULL;
    size_t pos = 0;
    for (size_t i = 0; i < count; i++) {
        const char* p = parts[i] ? parts[i] : "";
        while (i > 0 && *p == '/') p++;
        if (pos > 0 && out[pos - 1] != '/' && *p) out[pos++] = '/';
        size_t n = strlen(p);
        memcpy(out + pos, p, n); pos += n;
        while (pos > 1 && out[pos - 1] == '/' && i + 1 < count) pos--;
    }
    out[pos] = '\0';
    return out;
}
char* path_join2(const char* a,const char* b){const char* p[]={a,b};return clyth_path_join_impl(p,2);}
char* path_join3(const char* a,const char* b,const char* c){const char* p[]={a,b,c};return clyth_path_join_impl(p,3);}
char* path_join4(const char* a,const char* b,const char* c,const char* d){const char* p[]={a,b,c,d};return clyth_path_join_impl(p,4);}
char* path_join5(const char* a,const char* b,const char* c,const char* d,const char* e){const char* p[]={a,b,c,d,e};return clyth_path_join_impl(p,5);}
char* path_basename_copy(const char* path){
    if(!path||!*path)return strdup(""); size_t n=strlen(path); while(n>1&&path[n-1]=='/')n--; size_t i=n; while(i>0&&path[i-1]!='/')i--; size_t len=n-i; char* out=malloc(len+1); if(!out)return NULL; memcpy(out,path+i,len); out[len]='\0'; return out;
}
char* path_extension_copy(const char* path){
    char* base=path_basename_copy(path); if(!base)return NULL; char* dot=strrchr(base,'.'); if(!dot||dot==base){free(base);return strdup("");} char* out=strdup(dot); free(base); return out;
}
char* current_working_directory_copy(void){size_t cap=256; for(;;){char* out=malloc(cap); if(!out)return NULL; if(getcwd(out,cap))return out; free(out); if(errno!=ERANGE)return NULL; cap*=2;}}
char* environment_value_copy(const char* name){const char* value=name?getenv(name):NULL; return strdup(value?value:"");}
char* make_temp_directory_copy(const char* prefix){
    const char* p=(prefix&&*prefix)?prefix:"clyth-"; const char* root=getenv("TMPDIR"); if(!root||!*root)root="/tmp"; size_t n=strlen(root)+1+strlen(p)+6+1; char* out=malloc(n); if(!out)return NULL; snprintf(out,n,"%s/%sXXXXXX",root,p); if(!mkdtemp(out)){free(out);return NULL;} return out;
}


char* directory_entries_text(const char* path) {
    if (!path) return strdup("");
    DIR* d = opendir(path); if (!d) return strdup("");
    size_t cap=1024,len=0; char* out=malloc(cap); if(!out){closedir(d);return NULL;}
    struct dirent* e;
    while((e=readdir(d))){
        if(!strcmp(e->d_name,".")||!strcmp(e->d_name,".."))continue;
        size_t pn=strlen(path)+1+strlen(e->d_name)+1; char* full=malloc(pn); if(!full){free(out);closedir(d);return NULL;}
        snprintf(full,pn,"%s/%s",path,e->d_name); struct stat st; int isdir=(stat(full,&st)==0&&S_ISDIR(st.st_mode)); free(full);
        size_t n=strlen(e->d_name); size_t need=1+1+n+1;
        if(len+need+1>cap){while(len+need+1>cap)cap*=2; char* next=realloc(out,cap); if(!next){free(out);closedir(d);return NULL;} out=next;}
        out[len++]=isdir?'D':'F'; out[len++]='\t'; memcpy(out+len,e->d_name,n); len+=n; out[len++]='\n';
    }
    closedir(d); out[len]='\0'; return out;
}
