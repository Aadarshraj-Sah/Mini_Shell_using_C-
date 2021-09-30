#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


using namespace std;
/* Welcome Note*/


void wlc_shell()
{
    printf("\033[H\033[J");
    cout<<"**********   WELCOME TO SHELL CREATED BY AADARSH   **********"<<endl;
    cout<<"**********     Rollno- 200101001, IIT GUWAHATI     **********"<<endl;
    sleep(3);
    printf("\033[H\033[J");
}

/* Current working directory*/
void CurrentWorkingDirectory(){
    char tmp[512];
    getcwd(tmp, 512);
    cout<<tmp<<endl;
}

/* External commands will be executed*/
void ExternalCommands(char** charlist)
{
    // Forking a child
    pid_t forkpid = fork();

    if (forkpid == -1) {
        return;
    }
    else if (forkpid == 0) {
        if (execvp(charlist[0], charlist) < 0) {
            cout<<"cannot process the command"<<endl;
        }
        exit(0); //successful termination of the program.
    } else {
        wait(NULL);
        return;
    }
}

/* parses according to space*/
void spaceseperator(char* string, char** commands)
{
    for (int i = 0; i < 100; i++) {
        commands[i] = strsep(&string, " ");

        if (commands[i] == NULL)
            break;
        if (strlen(commands[i]) == 0)
            i--;
    }
}

// Function to remove leading
// spaces from a given string
char* removeLeadingSpaces(char* str)
{
    static char str1[99];
    int count = 0, j, k;
  
    // Iterate String until last
    // leading space character
    while (str[count] == ' ') {
        count++;
    }
  
    // Putting string into another
    // string variable after
    // removing leading white spaces
    for (j = count, k = 0;
         str[j] != '\0'; j++, k++) {
        str1[k] = str[j];
    }
    str1[k] = '\0';
  
    return str1;
}

/* Identifies pipe and parses about it*/
bool pipe_finder(char* string, char** pipedcmd ){
    for (int i = 0; i < 2; i++) {
        pipedcmd[i] = strsep(&string, "|");
        if (pipedcmd[i] == NULL)
            break;
    }
    if (pipedcmd[1] == NULL)
        return false; // returns fasle if no pipe is found.
    else {
        return true;
    }
}

/* pipe cmd execution*/
void pipe_cmd(char** after_pipe, char** before_pipe)
{
    int array_pipe[2];
    pid_t a_p, b_p;

    if(pipe(array_pipe)>=0){
    a_p = fork();
        if(a_p == 0){
            dup2(array_pipe[1], STDOUT_FILENO);
            close(array_pipe[0]);
            close(array_pipe[1]);
            int x =execvp(after_pipe[0], after_pipe);
            if(x<0){
                cout<<"cannot process the command"<<endl;
            }
            exit(0);
        }
        else{
            b_p = fork();
            if (b_p == 0){
                dup2(array_pipe[0], STDIN_FILENO);
                close(array_pipe[1]);
                int z =execvp(before_pipe[0], before_pipe);
                if(z<0){
                    cout<<"cannot process the command"<<endl;
                }
                exit(0);
            }
           else {
                close(array_pipe[1]);
                waitpid(a_p, NULL, 0);
                waitpid(b_p, NULL, 0);
                return;
            }
        }
    }
}

/* checking > and parsing about > */
bool out_finder(char* string, char** outcmd){
    for (int i = 0; i < 2; i++) {
        outcmd[i] = strsep(&string, ">");
        if (outcmd[i] == NULL)
            break;
    }
    if (outcmd[1] == NULL || strlen(outcmd[1])==0)
        return false; // returns fasle if no > is found.
    else {
        return true;
    }
}

/* executing output redirection */
void out_ext(char** outcmd, char* out_filename){
    int f;
    pid_t identity;
    int status;
    identity = fork();

    if(identity == 0) {
        freopen(out_filename, "w+", stdout);
        f = execvp(outcmd[0], outcmd);
        if(f<0){
            cout<<"cannot process the command"<<endl;
        }
        exit(-1);
    }
    else{
        f = waitpid(identity, &status, 0);
    }
}

/* checking > and parsing about >*/
bool in_finder(char* string, char**incmd){
    for (int i = 0; i < 2; i++) {
        incmd[i] = strsep(&string, "<");
        if (incmd[i] == NULL)
            break;
    }
    if (incmd[1] == NULL)
        return false; // returns fasle if no < is found.
    else {
        return true;
    }
}

/* executing input redirection */
void in_ext(char** incmd, char* in_filename){
    int f;
    pid_t identity;
    int status;
    identity = fork();

    if(identity == 0) {
        freopen(in_filename, "r", stdin);
        f = execvp(incmd[0], incmd);
        if(f<0){
            cout<<"cannot process the command"<<endl;
        }
        exit(-1);
    }
    else{
        f = waitpid(identity, &status, 0);
    }
}


/* execution of appendment redirection*/
void app_ext(char** appcmd, char* app_filename){
    int f;
    pid_t identity;
    int status;
    identity = fork();

    if(identity == 0) {
        freopen(app_filename, "a+", stdout);
        f = execvp(appcmd[0], appcmd);
        exit(-1);
    }
    else{
        f = waitpid(identity, &status, 0);
    }
}

/* checking environment variables and executing it*/
bool EnvironmentVariables(string s){
    if(s=="echo $USER" || s== "USER"){
        string username;
        username = getenv("USER");
        cout<<username<<endl;
        return true;
    }
    else if(s=="echo $PATH" || s== "PATH"){
        string path;
        path = getenv("PATH");
        cout<<path<<endl;
        return true;
    }
    else if(s=="echo $PWD" || s== "PWD"){
        string pwd;
        pwd = getenv("PWD");
        cout<<pwd<<endl;
        return true;
    }
    else if(s=="echo $HOME" || s== "HOME"){
        string home;
        home = getenv("HOME");
        cout<<home<<endl;
        return true;
    }
    else if(s=="echo $SHELL" || s== "SHELL"){
        string shell;
        shell = getenv("SHELL");
        cout<<shell<<endl;
        return true;
    }
    else if(s=="echo $TERM" || s=="TERM"){
        string term;
        term = getenv("TERM");
        cout<<term<<endl;
        return true;
    }
    else if(s.substr(0,6)=="echo $"){
        string env = s.substr(6,s.length()-6);
        string evalue;
        evalue = getenv(env.c_str());
        cout<<evalue<<endl;
        return true;
    }
    
    return false;
}

/* implementing setenv in my shell*/
int shell_setenv(char** a){
    
    if(a[1] == NULL || a[2]== NULL ){
        cout<<"setenv not provided";
        return -1;
    }
    else{
        int value = setenv(a[1], a[2] ,1);
    }
    return 1;
}


/* main function*/
int main() {
    vector<string> his; //stores all inputs in a session
    
    char homewd[512];
    getcwd(homewd,512); // stores homedir
    wlc_shell();
    
    while(1){
        
        int temp=0; // if temp remains 0 then external cmds will be executed
        CurrentWorkingDirectory();
        char* input;
        char* args[100];
        char* extArguments[100];
        char* cd[100];
        char* extpipedArg[2];
        char* inredirection[2];
        char* outredirection[2];
        char* appredirection[2];
        
        input = readline(">>> ");
        if (strlen(input) != 0) {
            add_history(input); // will help to go in previous cmd using upper arrow key
        }
        
        string input_string(input); // char* to string
        
        his.push_back(input_string); // storing history of inputs
       
       
        /* creation of history.txt in all directories*/
        ofstream historytxt;
        historytxt.open ("history.txt",std::ios_base::app);
        historytxt <<input_string<<"\n";
        
        
/* checking presence of >> (append) function */
        string s(input);
        int pos=0;
        int temp1=0;
        for(int i=0; i<s.length()-1; i++){
            if(s[i]=='>' && s[i+1]=='>'){
                pos=i+2;
                temp1=1; // >> present
                break;
            }
        }
        string s1 =(s.substr(0,pos-2));             // left half of >>
        string s2= (s.substr(pos,s.length()-pos));  // right half of >>
        
        char str1[s1.length()+1];
        str1[s1.length()]='\0';
        char str2[s2.length()+1];
        str2[s2.length()]='\0';

        for(int i=0; i<s1.length(); i++){
                str1[i]=s1[i];
        }
        
        for(int i=0; i<s2.length(); i++){
                str2[i]=s2[i];
        }
        
        appredirection[0]=str1;  // left half of >>
        appredirection[1]=str2;  // right half of >>
/*      append checking completed     */
        
        
        
        if(input_string =="exit") //exit command to Exit
        {
            remove( "history.txt");
            fstream file;
            file.open("history.txt",ios::out);
            free(input);
            break;
        }
        else if(EnvironmentVariables(input_string)) //Environment variables execution
        {
            temp=1;
            continue;
        }
        else if(s.substr(0,6)=="setenv"){  //setenv executor
            spaceseperator(input, args);
            shell_setenv(args);
            
        }
        else if(input_string=="cd"){  // returning to home directory
            temp=1;
            remove( "history.txt");
            fstream file;
            file.open("history.txt",ios::out);
            chdir(homewd);
            continue;
        }
        else if(input_string[0]=='c' && input_string[1]=='d'){  // going to any directory
            temp=1;
            spaceseperator(input, cd);
            chdir(cd[1]);
            continue;
        }
        else if(input_string=="history"){  // history cmd execution
            temp=1;
            for(int i=0; i<his.size(); i++){
                cout<<his[i]<<endl;
            }
        }
        else if(input_string=="help"){  //help to my shell
            temp=1;
            cout<<"WELCOME TO MY SHELL HELP"<<endl;
            cout<<"Supporting commands list:"<<endl;
            cout<<"'cd' to navigate to any directory"<<endl;
            cout<<"'USER'  to know user"<<endl;
            cout<<"'PATH'  to know path"<<endl;
            cout<<"'PWD'  to know working directory"<<endl;
            cout<<"'HOME'  to know home directory"<<endl;
            cout<<"'history'  to see all the inputs in that session"<<endl;
            cout<<"All other Environment variable available"<<endl;
            cout<<"Single level piping can be used"<<endl;
            cout<<"All kinds of single level Input redirection"<<endl;
            cout<<"All kinds of single level output redirection"<<endl;
            cout<<"All kinds of single level append redirection"<<endl;
            cout<<"cmd <input.txt >output.txt   can be used "<<endl;
            cout<<"cmd <input.txt >>output.txt   can be used "<<endl;
            cout<<"All external commands can be executed"<<endl;

        }
        else if(pipe_finder(input,extpipedArg)){   //execution of pipe cmd
            temp=1;
            spaceseperator(extpipedArg[0],extArguments);
            spaceseperator(extpipedArg[1],extpipedArg);
            pipe_cmd(extArguments,extpipedArg);
            continue;
        }
       
        else if(out_finder(input, outredirection)){  //execution of output redirection cmd
            temp=1;
            for(int i=0; i<100; i++){
                if(outredirection[0][i]=='<'){
                    outredirection[0][i]=' ';
                }
            }
            spaceseperator(outredirection[0],extArguments);
            out_ext(extArguments, removeLeadingSpaces(outredirection[1]));
        }
        else if(temp1==1){     //execution of append redirection cmd
            temp=1;
            for(int i=0; i<100; i++){
                if(appredirection[0][i]=='<'){
                    appredirection[0][i]=' ';
                }
            }
            spaceseperator(appredirection[0],extArguments);
            app_ext(extArguments,removeLeadingSpaces(appredirection[1]));
        }
        else if(in_finder(input, inredirection)){   //execution of input redirection cmd
            temp=1;
            spaceseperator(inredirection[0],extArguments);
            in_ext(extArguments, removeLeadingSpaces(inredirection[1]));
        }
        
        else if(temp==0){    //temp is still 0 => none of the above function is executed
            spaceseperator(input, extArguments);// executing external arguments
            ExternalCommands(extArguments);
            continue;
        }
        else if(input_string.substr(0,4)=="echo"){
            cout<<input_string.substr(5,input_string.size()-5)<<endl;
        }
        else{
            cout<<"command not found."<<endl;  //erroraneous cmd
        }
        
    }
    return 0;
}
