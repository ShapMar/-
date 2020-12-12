//АЛГОРИТМ SRR 
#include <locale>
#include <iostream>

using namespace std;

struct process
{
	int t; //время выполнения
	int entry_time; //время появления
	double priority; //приоритет

	int last_tact; //последний такт, при котором процессу были выделены ресурсы ЦП
	int dt; //сколько раз выполнился процесс на данный момент
	int T; //общее время в системе

	bool is_new; //является ли этот процесс новым или выбранным
	bool is_entered; //поступил ли в систему процесс

};

bool check_enter_time(process* p, int k)
{
	for (int i = 0; i != k; ++i)
	{
		if (p[i].entry_time == p[k].entry_time)
		{
			cout << "Процесс с таким временем появления уже есть, введите другое значение" << endl;
			return 1;
		}
	}
	return 0;
}

int find_max(process* p, int n)
{
	int indx=-1;
	int max = -1;
	for (int i = 0; i != n; ++i)
	{
		if ((p[i].priority > max)&&(p[i].is_entered))
		{
			max = p[i].priority;
			indx = i;
		}
		else if ((p[i].priority == max) && (p[i].is_entered)) //при равентсве приоритетов квант отдаётся тому процессу, который раньше поставлен в очередь
		{
			if (p[i].last_tact < p[indx].last_tact)
			{
				max = p[i].priority;
				indx = i;
			}
		}
	}
	return indx;
}

void give_CPU(process* p, int indx, int tact, int& sum_completed_p)
{
	p[indx].last_tact = tact;
	p[indx].dt++;
	if (p[indx].is_new)
		p[indx].is_new = false;
	if (p[indx].t == p[indx].dt)
	{
		p[indx].is_entered = false;
		sum_completed_p++;
	}
}

const double P0 = 0; //приоритет процесса при поступлении

int main()
{
	setlocale(LC_ALL, "russian");
	int n; //количество процессов
	cout << "Введите количество процессов" << endl;
	do
		cin >> n;
	while (n < 1);
	process* p = new process[n];

	for (int i = 0; i != n; ++i) //ввод параметров процессов
	{
		cout << "Введите время выполнения для " << i + 1 <<" процесса"<<endl;
		do
			cin >> p[i].t;
		while (p[i].t < 1);
		cout << "Введите время появления для " << i + 1 << " процесса" << endl;
		do
			cin >> p[i].entry_time;
		while ((p[i].entry_time < 0)||(check_enter_time(p,i)));
		p[i].priority = P0;
		p[i].is_new = true;
		p[i].is_entered = false;
		p[i].dt = 0;
		p[i].T = 0;
		p[i].last_tact = 0;
	}

	double dA, dB; //возрастание приоритетов
	cout << "Введите dA" << endl;	
	do
		cin >> dA;
	while (dA < 1);
	cout << "Введите dB" << endl;
	do
		cin >> dB;
	while ((dB/dA <= 0)|| (dB / dA >= 1));

	int sum_completed_p = 0; //количество выполненных процессов

	cout << "=====================================================" << endl;
	cout << "1-процессу был выделен квант времени в данный такт" << endl << "0-процессу не был выделен квант времени в данный такт" << endl;
	cout << "=====================================================" << endl;
	for (int i = 0; i != n; i++)
		cout <<i+1<<" ";	
	cout << endl;
	int tact = 1; //такт
	while (sum_completed_p != n)
	{
		for(int i = 0; i != n; i++) //вход процесса
			if (p[i].entry_time == tact - 1)
				p[i].is_entered = true;
		int indx=find_max(p,n); //индекс процесса, который получит квант времени
		if (indx != -1)
			give_CPU(p, indx, tact,sum_completed_p);
		for (int i = 0; i != n; i++)
		{
			if (p[i].is_entered)
			{
				if (p[i].is_new)
					p[i].priority += dA;
				else
					p[i].priority += dB;
			}

			if (i == indx)
			{
				p[i].T++;
				cout<<"1 ";
			}
			else if (!p[i].is_entered)
				cout << "  ";
			else
			{
				p[i].T++;
				cout << "0 ";
			}			
		}
		cout << "такт "<< tact<< endl;
		tact++;
	}
	cout << "=====================================================" << endl;
	double sT = 0, SM = 0, SR = 0, SP = 0;
	for (int i = 0; i != n; i++)
	{
		sT += p[i].T;
		SM = SM + (p[i].T - p[i].t);
		SR += (double(p[i].t) / p[i].T);
		SP += (double(p[i].T) / p[i].t);
		cout <<"Процесс "<<i+1<<": "<<"T="<< p[i].T<<", M="<< p[i].T - p[i].t<<", R="<< (double(p[i].t) / p[i].T)<<", P="<< (double(p[i].T) / p[i].t) <<endl;
	}
	cout << "=====================================================" << endl;
	cout << "Tср="<< sT / n<<", Mср=" << SM / n <<", Rср="<< SR / n <<", Pср="<< SP / n <<endl;
	cout << "=====================================================" << endl;
	delete[] p;
	system("pause");
	return 0;
}