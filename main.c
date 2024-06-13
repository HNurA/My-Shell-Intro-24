/**authors Hilal Nur Albayrak 22120205056
 *         Begüm Karabaş 22120205002
 *last update: 31.03.2024
 * 
 * bu dosya shell tasarlama programı yapmaktadir
 */

/*kütüphaneler include edildi
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>

/*trog girdilerinin başına /bin/bash path ini ekler ve execv çağırılır
 */
int tprog(const char *input) {
    char command[256] = "./";
    strncat(command, input, 255 - strlen(command)); // Girdiye "./" ön eki ekle

    char *args[] = {"/bin/bash", "-c", command, NULL}; // execv'ye iletilen argümanlar
    execv("/bin/bash", args);
    return 0;
}

/*execv ile komut çalıştırılamaz ise which fonksiyonu çalıştırılır.
 *bu fonksiyon which komutunu kullanarak dosyanın path ni bulur.
 * bu path kullanılarak execv çağırılır. 
 */
int which(char usrin[]){

    char *full_path = NULL;
            char which_command[256];
            snprintf(which_command, sizeof(which_command), "which %s", usrin);
            FILE *which_output = popen(which_command, "r");
            if (which_output == NULL) {
                perror("Error opening pipe for which");
                exit(EXIT_FAILURE);
            }
            char temp_path[256];
            if (fgets(temp_path, sizeof(temp_path), which_output) == NULL) {
                return 0;
            }
            pclose(which_output);
            size_t len = strlen(temp_path);
                if (len > 0 && temp_path[len - 1] == '\n') {
                temp_path[len - 1] = '\0';
            }
            full_path = strdup(temp_path);

            char *args[] = {full_path, NULL}; 
            execv(full_path, args);
            free(full_path);

    return 0;

};

/*girdi doğru bir şekilde alındı mı diye kontrol edilir
 */
int readusrin(char *buf) {
    int r = read(0, buf, 256);
    if (r <= 0) {
        return -1;
    } else {
        return 0;
    }
}

/*girdi ayraçlara (/t '' /n) göre parçalara ayrılır 
 *parçalar tokens dizisinde tutulur. 
 */
int tokenize(char *str, char *tokens[], int maxtoken) {
    char *delim = "\t \n";
    char *token = strtok(str, delim);
    tokens[0] = NULL;
    int i = 0;

    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, delim);
    }
    tokens[i] = NULL;

    return i;
}

/*girilen komutun başına /usr/bin/ path i ekler
 */
int addpathname(char *dest, char *str) {
    char path[128] = "/usr/bin/";
    strncpy(dest, path, strlen(path) + 1);
    strncat(dest, str, strlen(str));
    return 0;
}

/*loglamanın kaydedileceği log dosyasını açar
 */
int open_log_file() {
    int log_fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC,  0666);
    if (log_fd == -1) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }
    return log_fd;
}

/*log dosyasını kapatır
 */
void close_log_file(int log_fd) {
    if (close(log_fd) == -1) {
        perror("Error closing log file");
        exit(EXIT_FAILURE);
    }
}

/*zaman dalgalarını gettimeofday() fonksiyonunu kullarak alır
 * bunları log dosyasına time\t command şeklinde yazdırır
 */
void log_command(const char *command, int log_fd) {

    if(log_fd == -1) {
        return; 
    }
    
    struct timeval current_time;
    
    gettimeofday(&current_time, NULL);

    /* Zamana karşılık gelen string oluşturulur */
    char time_str[30];
    strftime(time_str, 30, "%Y-%m-%d %H:%M:%S", localtime(&current_time.tv_sec));

    char log_entry[100];
    sprintf(log_entry, "[%s.%06ld]\t%s\n", time_str, current_time.tv_usec, command);
    if (write(log_fd, log_entry, strlen(log_entry)) == -1) {
        perror("Dosyaya yazma hatası");
        exit(EXIT_FAILURE);
    }
}

/* main fonksiyonu içerisinde diğer fonksiyonlar çağırılır. 
 * fork() fonksiyonu kullanılarak child process burada oluşturulur.
 * kullanıcı exit girene kadar girdi almaya devam eder. 
*/
int main(int argc, char *argv[]) {

    char c = '$';
    char usrin[256] = {0};
    char tempusrin[256] = {0};
    char path[128] = {0};
    char *newargv[10];
    int log_fd = open_log_file();

    /* kullanıcı exit girene kadar girdi almaya devam eder
    */
    while (strncmp(usrin, "exit", 5) != 0) {
        write(1, &c, 1);
        memset(usrin, 0, sizeof(usrin));
        if (readusrin(usrin) == -1) {
            perror("Read error");
            exit(EXIT_FAILURE);
        }

        /*\n girilirse de döngüye devam edilmesini sağlar*/
        if (strlen(usrin) <= 1) { 
            continue;
        }
        /* girdinin son karakteri \n den \0 a çevirlir*/
        usrin[strlen(usrin) - 1] = '\0';  

        size_t copy_length = sizeof(tempusrin) - 1; 
        strncpy(tempusrin, usrin, copy_length);
        tempusrin[copy_length] = '\0'; 

        tokenize(usrin, newargv, 10);

        addpathname(path, newargv[0]);

        /*yeni process oluşturulur*/
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { /*child process*/
            execv(path, newargv);

            int a=which(usrin);

            /*execv başarısız olursa*/
            char pwd[256];
            FILE *fp = popen("pwd", "r"); 
            if (fp == NULL) {
                perror("Error opening pipe for pwd");
                exit(EXIT_FAILURE);
            }
            if (fgets(pwd, sizeof(pwd), fp) == NULL) {
                perror("Error reading pwd output");
                exit(EXIT_FAILURE);
            }
            pclose(fp); 

            pwd[strlen(pwd) - 1] = '\0'; 

            char programPath[512];
            snprintf(programPath, sizeof(programPath), "%s/%s", pwd, usrin); 

            char *args[] = {programPath, NULL}; 
            

            if(strncmp(usrin, "exit", 5) == 0){
                break;
            }else{
                if(strncmp(usrin, "tprog", 5) == 0){
                    tprog(tempusrin);
                }
                if(strncmp(newargv[0], "\\n", 2) == 0){
                    continue;
                }
                execv(programPath, args);
                printf("%s: command not found\n", newargv[0]);
            }

            exit(EXIT_FAILURE);
            
        } else { /*parent process*/ 
            wait(NULL); /*wait for the child process to finish*/ 
            log_command(tempusrin,log_fd);
        }

        /*Dizileri bosaltma*/
        memset(path, 0, sizeof(path));
        for (int i = 0; i < 10; i++) {
            newargv[i] = NULL;
        }
    }
    close_log_file(log_fd);
    return 0;
}
