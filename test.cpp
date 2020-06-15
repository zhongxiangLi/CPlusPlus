#include <iostream>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
using namespace std;

#define MAX_LEN 1024*16

string remove_ctl(string s) {
        string res;
        int i;
        int n = s.length();
        for (i = 0; i < n; i++) {

                if (s[i] >= 0X42)
                {
                        res = res + s[i];
                }
        }
        return res;
}
string remove_ctl_mutating(string s) {
        string res;
        int i;
        int n = s.length();
        for (i = 0; i < n ; i++) {

                if (s[i] >= 0X42)
                {
                        res +=  s[i];
                }
        }
        return res;
}

string remove_ctl_reserve(string s) {
        string res;
        int i;
        int n = s.length();
        res.reserve(n);
        for (i = 0; i <n; i++) {

                if (s[i] >= 0X42)
                {
                        res += s[i];
                }
        }
        return res;
}
string remove_ctl_ref_args(string &s) {
        string res;
        int i;
        int n = s.length();
        res.reserve(n);
        for (i = 0; i < n; i++) {

                if (s[i] >= 0X42)
                {
                        res += s[i];
                }
        }
}
void remove_ctl_const_ref(string &res,const string& s) {
        res.clear();
        int n = s.length();
        res.reserve(n);
        int i;

        for (i = 0; i < n; i++) {

                if (s[i] >= 0X42)
                {
                        res += s[i];
                }
        }
}


int main(int argc,char *argv[]){


        int num = 0;
        if(argc == 2)
                num=atoi(argv[1]);

        string sourcestr = "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM[];',./{}:>?!@#$%^&*()_+=-";
        int length = sourcestr.length();
        int i = 0;
        string s;
        s.reserve(MAX_LEN);
        srand((unsigned)time(NULL));
        for (i = 0; i < MAX_LEN; i++)
        {
                int k = rand() % (length - 1);
                s += sourcestr[k];
        }



        struct timespec time_start = { 0, 0 }, time_end = { 0, 0 };
        int sum = 0;
        for(i=0;i<10;i++){
                clock_gettime(CLOCK_REALTIME, &time_start);

		        int sum = 0;

                std::string res;
                switch(num){
                case 0:
                 res = remove_ctl(s);
                break;
                case 1:
                        res = remove_ctl_mutating(s);
                break;
                case 2:
                        res = remove_ctl_reserve(s);
                break;
                case 3:
                res = remove_ctl_ref_args(s);
                break;
                case 4:{
                string sourstr;
                remove_ctl_const_ref(sourstr,s);
                }
                break;
                default:
                        break;
                }

                std::string res;
#if 0 
                 res = remove_ctl(s);
#endif

#if 0   
                res = remove_ctl_mutating(s);
#endif

#if 0
                res = remove_ctl_reserve(s);
#endif

#if 0 
                res = remove_ctl_ref_args(s);
#endif

#if 0
                string sourstr;
                remove_ctl_const_ref(sourstr,s);
#endif

                clock_gettime(CLOCK_REALTIME, &time_end);

                int costtime = ((time_end.tv_sec-time_start.tv_sec)*1000000000+(time_end.tv_nsec-time_start.tv_nsec))/1000;

                sum +=costtime;

        }
        cout <<"agv = "<<sum/10<<endl;


        return 0;
}
          
		  

                                       
