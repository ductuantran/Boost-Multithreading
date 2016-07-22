#include <iostream>
#include <string>
#include <conio.h>
#include <boost\thread.hpp>
#include <boost\parameter\keyword.hpp>
#include <boost\parameter\name.hpp>
#include <boost\parameter\preprocessor.hpp>
#include <string>
#include <malloc.h>

using namespace std;
using namespace boost;
using namespace boost::this_thread;
using namespace boost::parameter;

class data_in
{
private:
	boost::thread* t1;
	boost::mutex m1;
	bool must_stop1;
	char *arr;
	int *cnt,limit,circ;

public:

		  data_in(char *arr,int &length,int limit,int circular)
			{
				this->t1=NULL;
				this->must_stop1=false;
				this->arr=arr;
				this->cnt=&length;
				this->limit=limit;
				this->circ=circular;
			}
		
		~data_in()
			{
				if (t1!=NULL) delete t1;
			}

		void create_thread()
			{
				t1=new boost::thread(&data_in::input,this);
			}

		boost::thread* get_thread()
			{
				return t1;
			}
		
		void stop()
			{
				m1.lock();
				must_stop1=true;
				m1.unlock();
			}

		bool get_must_stop()
			{
				return must_stop1;
			}
			
		void input()
			{
				bool stop;
				int num=*cnt;
				char x;
				do
					{
						cout << "\n Nhap : ";
						fflush(stdin); scanf("%c",&x);
						m1.lock();
						arr[num]=x;
						(*cnt)++;
						num++;
						if (*cnt==this->circ) num=0;
						m1.unlock();

						//if (num==limit) this->stop();

						m1.lock();
						stop=must_stop1;
						m1.unlock();
					}
				while(stop==false);
			}

};


class data_out
{
private:
	boost::thread* t2;
	boost::mutex m2;
	bool must_stop2;
	char *arr;
	int cnt, *upper_bound, limit, circ;

public:
		data_out (char *arr,int &length,int limit,int circular)
			{
				this->t2=NULL;
				this->must_stop2=false;
				this->cnt=0;
				this->arr=arr;
				this->upper_bound=&length;
				this->limit=limit;
				this->circ=circular;
			}
		
		~data_out()
			{
				if (t2!=NULL) delete t2;
			}

		void create_thread()
			{
				t2=new boost::thread(&data_out::output,this);
			}

		boost::thread* get_thread()
			{
				return t2;
			}
		
		void stop()
			{
				m2.lock();
				must_stop2=true;
				m2.unlock();
			}

		int get_cnt()
			{
				return cnt;
			}
			
		void output()
			{
				bool stop;
				int num=cnt;
				do
					{
						m2.lock();
						if (cnt<*upper_bound)
							{
								printf("\n Hien thi phan tu thu %d : %c\n",cnt,arr[cnt]);
								cnt++;
								num++;
								if (cnt==this->circ) cnt=0;
							}
						m2.unlock();

						boost::this_thread::sleep(boost::posix_time::millisec(1000));

						//if (num==limit) this->stop();

						m2.lock();
						stop=must_stop2;
						m2.unlock();
					}
				while(stop==false);
			}

};

BOOST_PARAMETER_NAME(a)
BOOST_PARAMETER_NAME(b)
BOOST_PARAMETER_NAME(c)
BOOST_PARAMETER_FUNCTION (
    (void),  
    disp,
    tag,  // part of the deep magic. If you use BOOST_PARAMETER_NAME you need to put "tag" here.
    (required 
        (a,(int)) 
        (b,(float))
		(c,(char))
    )
    //(optional // names, types, and default values of all optional parameters.
    //    (baz, (bool) , false)
    //    (bonk, (std::string), "default value")
    // ) 
) 

{
    printf("\n Cac tham so la : %d, %f, %c",a,b,c);
}




int main ()
{
	 
  static char arr[100];
  char c;
  int length=0, limit=20, circular=5;
  data_in test(arr,length,limit,circular);
  data_out test2(arr,length,limit,circular);
  boost::thread* th1;
  boost::thread* th2;

  try
	{
			  test.create_thread();
			  th1=test.get_thread();
			  test2.create_thread();
			  th2=test2.get_thread();
	 }

  catch(std::exception& exc)
	  {
		  cout << "Uncaught exception : " << exc.what();
	  }

  th1->join();
  th2->join();
  
  cout << "\n Preparing to stop ... ";
  cout << "\n Program stopped !!!";
  cout << "\n length la : " << length;
  
  getch();
  return 0;
}
