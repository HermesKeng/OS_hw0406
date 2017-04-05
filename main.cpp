#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <cstdlib>
using namespace std;

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1
int main(int argc,char *argv[])
{
    int fd[2];
    pid_t pid;
    string fdPass;
    char send_msg[50];//it is used to send msg
    char recv_msg[50];//it is used to recv msg
    //Overall, if argc = 1 (only one parameter in the program input ),it means it is parent process
    //If argc is greater than 1 , it is child process
    if(argc==1){

        if(pipe(fd)==-1){//Check whether pipe build sucessfully
            cout<<"Pipe create fail !"<<endl;
        }
        pid=fork();//fork a new process

        if(pid<0){//Check whether pipe build sucessfulli
            cout<<"process create fail !"<<endl;
        }

        if(pid>0){//parent process-read pipe content
            cout<<"Parent pid : "<<getpid()<<endl;
            while(1){
                close(fd[WRITE_END]);//close write endnode when the process executing reading
                read(fd[READ_END],recv_msg,strlen(recv_msg)+1);//store the recv message into recv_msg
                cout<<"read : "<<recv_msg<<endl;//print recv message
                if(!strcmp("Q",recv_msg)){
                    break;
                }
                memset(recv_msg,' ',BUFFER_SIZE);
            }
            close(fd[READ_END]);//end the parent process and pipe
        }else{//child porcess-write pipe content
            //Convert int to string in order to pass value to new program
            stringstream ss;
            ss<<fd[WRITE_END];
            ss>>fdPass;
            //use it to run new same programe and create a new terminal
            execl("/usr/bin/xterm","xterm","-e","/home/s410385007/OS_Hw/OS_hw0406/bin/Debug/OS_hw0406",fdPass.c_str(),NULL);

        }
    }else{
        cout<<"Child pid : "<<getpid()<<endl;
        fd[WRITE_END]=atoi(argv[1]);
        while(1){
            cout<<"Please send the message ,if you want to end the program input 'Q'"<<endl;
            cin>>send_msg;
            close(fd[READ_END]);//close read endnode when the process executing writing
            write(fd[WRITE_END],send_msg,strlen(send_msg)+1);//write the message in the pipe
            if(!strcmp("Q",send_msg)){//if cin ==Q the program will stop and send message to let parent stop
                break;
            }
            memset(send_msg,' ',BUFFER_SIZE);
        }
        close(fd[WRITE_END]);//end the child process and pipe
    }

    return 0;
}
