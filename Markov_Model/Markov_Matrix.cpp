/*��������Ҫ�����Ƕ����ݽ���ѵ�������ѵ�����ݵĲ������Ա��ж��ַ����Ƿ�ɶ���
   ����Ҫԭ������������ɷ�������big.txt(��Դ��http://norvig.com/spell-correct.html)��
   �����ݽ���ѵ������¼һ���ַ�������ֵ��ַ�����baidu:ba,ai,id,du�����ÿ���ַ������
   �ֵ��ַ���27�ֿ���(�����ո�),���ͨ�������ݵ�ѵ�������ǿ��Ի��ÿ���ַ�������ֵ�ÿ
   ���ַ��ĸ��ʣ���˱��������ջ���һ��27X27�ĸ��ʾ���Ȼ�����Ƕ�һЩ�ɶ����ַ���
   (good.txt)��һЩ���ɶ����ַ���(bad.txt)����ѵ����Ȼ��ѡȡһ����ֵ���ж��ַ����Ƿ�
   �ɶ����ж��ַ����Ƿ�ɶ��ĳ����gib_detect.cpp*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <ctime>

using namespace std;

typedef vector<string> vec_string;
typedef map<const char,unsigned int> map_char_int;
const char character[27]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
					'p','q','r','s','t','u','v','w','x','y','z',' '};
void normalize(string & str);
vec_string bigram(string & str);
double avg_transition_prob(string & str,double log_prob_mat[][27],map_char_int & my_map);
void train(string file1,string file2,string file3,map_char_int & my_map);

int main()
{
	clock_t start,end;
	start = clock();
	cout.setf(ios::showpoint);
	cout.precision(16);
	cout.setf(ios::fixed);
	map<const char,unsigned int> my_map;
	string filename1 = "big.txt";
	string filename2 = "good.txt";
	string filename3 = "bad.txt";

	for(int i = 0;i<27;i++)
	{
		my_map.insert(pair<const char,unsigned int>(character[i],i));
	}
	train(filename1,filename2,filename3,my_map);
	end = clock();
	cout<<"totaltime : "<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
	return 0;
}

/*���ַ������滯��ȥ�����֣�*��-��_�ȵ��ַ���ֻ������ĸ�Ϳո񣬲��Ҵ�д��ĸתΪСд*/
void normalize(string & str)
{
	for(int i = 0;i<str.length();i++)
	{
		if((str[i]>='a' && str[i]<='z') || (str[i] == ' '))
			continue;
		else if(str[i]>='A' && str[i] <= 'Z')
			str[i] = char(str[i]+32);
		else
		{
			str.erase(i,1);
			i--;
		}
	}
}

/*��������Ҫ����ȡ�ַ�����bigram,���洢��vector�С�*/
vec_string bigram(string & str)
{
	normalize(str);
	string s_sub;
	vec_string vs;
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

/*����ƽ��ת������*/
double avg_transition_prob(string & str,double log_prob_mat[][27],map_char_int & my_map)
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

/*����һ��ģ�ͣ������ʾ���*/
void train(string file1,string file2,string file3,map_char_int & my_map)
{
	unsigned int k = 27;
	double good_prob,bad_prob,threshold;
	vector<double> good_probs;
	vector<double> bad_probs;
	string line;
	vec_string vs;
	char a,b;
	ifstream file1_in,file2_in,file3_in;
	ofstream file_out;
	file_out.open("parameter.txt");
	file_out.setf(ios::showpoint);
	file_out.precision(16);
	file_out.setf(ios::fixed);
	unsigned int counts[27][27];
	//�����󸳳�ֵ������������������ÿ���ַ����Ѿ�������10�Σ�����Ϊһ�������ƽ������
	//��ˣ����һ���ַ�ת�����Ǵ���û�м���Ҳ������������ַ�����0�ĸ��ʡ�
	for(int i = 0;i<27;i++)
		for(int j = 0;j<27;j++)
			counts[i][j] = 10;
	
	file1_in.open(file1);
	file2_in.open(file2);
	file3_in.open(file3);
	if(!file1_in.is_open())
	{
		cout<<file1<<" can not open!"<<endl;
		exit(EXIT_FAILURE);
	}
	if(!file2_in.is_open())
	{
		cout<<file2<<" can not open!"<<endl;
		exit(EXIT_FAILURE);
	}
	if(!file3_in.is_open())
	{
		cout<<file3<<" can not open!"<<endl;
		exit(EXIT_FAILURE);
	}

	while(getline(file1_in,line))
	{
		if(!line.empty())
		{
			vs = bigram(line);
			int length = vs.size();
			for(int i = 0;i<length;i++)
			{
				a=vs[i][0];
				b=vs[i][1];
				counts[my_map[a]][my_map[b]] += 1;
			}
		}
	}

	double probability[27][27];//���ʾ���
	for(int i = 0;i<27;i++)
	{
		double sum = 0;
		for(int j = 0;j<27;j++)
			sum += counts[i][j];
		for(int j = 0;j<27;j++)
			probability[i][j] = log(counts[i][j] / sum);
		//����ʹ�ö������ʶ�����ֱ�ӵĸ�����Ϊ�˱��ⳤ�ı���ֵ���������
		//�������һ����������:
		//��http://squarecog.wordpress.com/2009/01/10/dealing-with-underflow-in-joint-probability-calculations/
	}

	for(int m = 0;m<27;m++)
	{
		for(int n = 0;n<27;n++)
			file_out<<probability[m][n]<<"   ";
		file_out<<endl;
	}

	while(getline(file2_in,line))
	{
		if(!line.empty())
		{
			good_prob = avg_transition_prob(line,probability,my_map);
			good_probs.push_back(good_prob);
		}
	}

	while(getline(file3_in,line))
	{
		if(!line.empty())
		{
			bad_prob = avg_transition_prob(line,probability,my_map);
			bad_probs.push_back(bad_prob);
		}
	}
	
	file1_in.close();
	file2_in.close();
	file3_in.close();

	sort(good_probs.begin(),good_probs.end());
	sort(bad_probs.begin(),bad_probs.end());
	if((*good_probs.begin()) > (*(bad_probs.end()-1)))
	{
		threshold = ((*good_probs.begin())+(*(bad_probs.end()-1))) / 2;//��ֵ
		file_out<<threshold;
		cout<<threshold<<endl;
		file_out.close();
	}
	else
		cout<<"min good probability is not bigger than max bad probability "<<endl;
}