#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;

//PCB结构
struct PCB
{
	std::string name;//进程名
	long pid;
	int pri;//进程优先级
	long round; //进程轮转时间片
	long long cputime; //进程占用的时间
	long needtime;   //进程到完成还要的时间
	enum state{READY,RUN,FINISH};
	state process_state;//进程状态
	static long long next_pid;
};

bool ComparePCB(PCB const & lhs, PCB const & rhs)
{
	return lhs.pri > rhs.pri;
}

bool operator<(PCB const & lhs, PCB const & rhs)
{
	return lhs.pri < rhs.pri;
}

bool operator>(PCB const & lhs, PCB const & rhs)
{
	return lhs.pri > rhs.pri;
}

long long PCB::next_pid = 0;

class Process
{
public:
	Process(){}
	Process(size_t size){ 
		pcb.resize(size);
		random_initialize();
	}

	void priority_schedule();
	void round_robin();
	const std::vector<PCB> & GetProcess()const{ return pcb; }
private:
	void random_initialize();
	std::vector<PCB> pcb;
};

void Process::random_initialize()
{
	cout << std::string(64, '-');
	cout << endl << "初始化进程控制块:\n";
	cout << std::string(64, '-');
	cout << endl;
	for (PCB & p : pcb){
		char id[32];
		p.pid = PCB::next_pid++;
		_itoa_s(p.pid, id, 10);
		p.name = std::string("进程") + id;
		p.pri = 40 + rand() % 30;
		p.cputime = 0;
		p.needtime = 10 + rand() % 20;
	}
}

void Process::priority_schedule()
{
	static const int ROUND = 10;
	cout << std::string(64, '-');
	cout << endl << "优先级调度法:\n";
	cout << std::string(64, '-');
	cout << endl;

	std::queue<PCB> q;
	std::sort(pcb.begin(), pcb.end(),ComparePCB);
	for (PCB const & p : pcb){
		q.push(p);
	}
	cout << "线程名\t优先级\tcputime\tneedtime\n";
	while (!q.empty()){
		PCB & top = q.front();
		cout << top.name << '\t' << top.pri << '\t' << top.cputime << '\t' << top.needtime << '\n';
		top.cputime += ROUND;
		top.needtime -= ROUND;

		if (top.needtime <= 0){
			q.pop();
		}
		else{
			PCB new_pcb = top;
			q.pop();
			q.push(new_pcb);
		}
	}
	
}

void Process::round_robin()
{
	cout << std::string(64,'-');
	cout << endl << "时间片轮转法:\n";
	cout << std::string(64, '-');
	cout << endl;

	static const int ROUND = 10;
	int index = 0;

	cout << "线程名\tcputime\tneedtime\n";
	while (!pcb.empty()){
		PCB & head = pcb.at(index);
		std::cout << head.name + "\t" << head.cputime << "\t" << head.needtime << "\n";
		head.cputime += ROUND;
		head.needtime -= ROUND;
		if (head.needtime <= 0){
			auto itr = pcb.begin() + index;
			pcb.erase(itr);
			index--;
		}
		if (index >= pcb.size() - 1){
			index = 0;
		}
		else{
			++index;
		}
	}
}

int main()
{
	Process processes1(5);
	for (const PCB & p : processes1.GetProcess()){
		cout << p.name << ' ' << p.pri << ' ' << p.needtime << ' '<<p.cputime<<endl;
	}
	processes1.round_robin();

	Process processes2(5);
	for (const PCB & p : processes2.GetProcess()){
		cout << p.name << ' ' << p.pri << ' ' << p.needtime << ' ' << p.cputime << endl;
	}
	processes2.priority_schedule();

	return 0;
}
