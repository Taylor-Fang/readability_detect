/*本程序主要功能为了检测字符串是否可读*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct gib_model
{
	unsigned int counts[27][27];
	double threshold;
};
typedef vector<string> vec_string;
typedef map<const char,unsigned int> map_char_int;
const char character[27]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
					'p','q','r','s','t','u','v','w','x','y','z',' '};
void normalize(string  & str);
vec_string bigram(string & str);
double avg_transition_prob(string & str,double log_prob_mat[27][27],map_char_int & my_map);

int main()
{
	string str;
	double model_probs[27][27];
	double thresh,threshold;
	int num = 0;
	map<const char,unsigned int> my_map;
	for(int i = 0;i<27;i++)
	{
		my_map.insert(pair<const char,unsigned int>(character[i],i));
	}

	ifstream fin("parameter.txt");//马尔可夫转移概率矩阵
	if(!fin)
	{
		cout<<"can not open"<<endl;
		return 0;
	}
	for(int i = 0;i<27;i++)
		for(int j = 0;j<27;j++)
			fin>>model_probs[i][j];
	fin>>threshold;
	cout<<"Please input a string: "<<endl;
	while(1)
	{
		getline(cin,str);
		if(!str.empty())
		{
			thresh = avg_transition_prob(str,model_probs,my_map);
			if(thresh > threshold)
			{
				cout<<"readable"<<endl;
			}
			else
			{
				cout<<"unreadable"<<endl;
			}
		}
	}
}

//消除字符串其他字符，如数字，#，$...等等
void normalize(string & str)
{
	for(int i = 0;i<str.length();i++)
	{
		if((str[i]>='a' && str[i]<='z') || (str[i]>='A' && str[i] <= 'Z') || (str[i] == ' '))
			str[i] = tolower(str[i]);
		else
		{
			str.erase(i,1);
			i--;
		}
	}
}

//获取字符串的相邻双字符
vec_string bigram(string & str)
{
	normalize(str);
	vec_string vs;
	string s_sub;
	if(!str.empty())
	{
		unsigned int length = str.length();
		for(unsigned int i = 0;i<length-1;i++)
		{
			s_sub = str.substr(i,2);
			vs.push_back(s_sub);
		}
	}
	return vs;
}

//求字符串可读性概率
double avg_transition_prob(string & str,double log_prob_mat[27][27],map_char_int & my_map)
{
	double log_prob = 0.0;
	char a,b;
	unsigned int length,transition_ct = 0;
	vec_string str_bigram = bigram(str);
	length = str_bigram.size();
	for(unsigned int i = 0;i<length;i++)
	{
		a=str_bigram[i][0];
		b=str_bigram[i][1];
		log_prob += log_prob_mat[my_map[a]][my_map[b]];
		transition_ct += 1;
	}
	if(transition_ct == 0)
		return exp(log_prob);
	else
		return exp(log_prob/transition_ct);
}