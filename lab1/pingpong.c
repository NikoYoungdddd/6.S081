#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main()
{
    int pfd1[2];//parent写->child读
    int pfd2[2];//child写->parent读
    pipe(pfd1);
    pipe(pfd2);
    if(fork()!=0)
    {
        //parent
        int parent_pid=getpid();
        char parent_msg[50];
        close(pfd1[0]);
        close(pfd2[1]);
        //parent send a byte to child by pipe1
        char* ping="a";
        write(pfd1[1],ping,1);
        //close pipe write

        if(read(pfd2[0],parent_msg,1)!=0)
        {
            printf("%d: received pong\n",parent_pid);
        }
        exit(0);
    }
    else
    {
        //child
        int child_pid=getpid();
        char child_msg[50];
        close(pfd1[1]);
        close(pfd2[0]);
        //child receive byte

        if(read(pfd1[0],child_msg,1)!=0)
        {
            printf("%d: received ping\n",child_pid);
        }

        //close pipe read

        //child send a byte
        write(pfd2[1],child_msg,1);
        exit(0);
    }
}
