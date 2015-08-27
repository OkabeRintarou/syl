#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Bank
{
public:
	int WildMan()const{ return Human_.first; }
	int ChurchMan()const{ return Human_.second;}
	Bank(int w, int c) :Human_(w, c){}
	bool operator==(const Bank & rhs)const{ return Human_ == rhs.Human_; }
	bool operator!=(const Bank & rhs)const{ return !((*this) == rhs); }
	bool IsValid()const
	{ 
		if (ChurchMan() == 0)return true;
		else return ChurchMan() >= WildMan() &&
			ChurchMan() >= 0 && WildMan() >= 0 &&
			ChurchMan() <= 3 && WildMan() <= 3;
	}
	bool Error()const{ return !IsValid(); }
	void AddWildMan(int num){ Human_.first += num; }
	void SubWildMan(int num){ Human_.first -= num; }
	void AddChurchMan(int num){ Human_.second += num; }
	void SubChurchMan(int num){ Human_.second -= num; }
private:
	std::pair<int, int> Human_;
};

struct State
{
public:
	enum DIRECTION {LEFT,RIGHT};
public:
	State() :this_(3, 3), that_(0, 0),boat_(LEFT){}
	State(int a, int b, int c, int d, DIRECTION dir) :this_(a, b), that_(c, d), boat_(dir){}

	bool operator==(const State & rhs)const
	{
		return this_ == rhs.this_ &&
			that_ == rhs.that_ &&
			boat_ == rhs.boat_;
	}
	bool operator!=(const State & rhs)const{ return !(*this == rhs); }

	bool IsTarget()const
	{ 
		return this_.ChurchMan() == 0 &&
			this_.WildMan() == 0 &&
			that_.WildMan() == 3 &&
			that_.ChurchMan() == 3;
	}
	bool Valid()const{ return this_.IsValid() && that_.IsValid(); }
	bool Error()const{ return this_.Error() || that_.Error(); }
	State Move(int a, int b);
	State Move(std::pair<int, int> p){ return Move(p.first, p.second); }
	std::string ToString()const
	{ 
		return std::string("((") + std::to_string(this_.WildMan()) + "," +
			std::to_string(this_.ChurchMan()) + std::string(") , ") +
			std::string("(") + std::to_string(that_.WildMan()) + "," +
			std::to_string(that_.ChurchMan()) + std::string(") , ") +
			(boat_ == LEFT ? "LEFT" : "RIGHT") + ")\t";
	}
private:
	Bank this_; // left bank
	Bank that_; // right bank
	DIRECTION boat_; 
	friend class Solution;
};

State State::Move(int a, int b)
{
	if (boat_ == LEFT){
		this_.SubWildMan(a);
		this_.SubChurchMan(b);
		that_.AddWildMan(a);
		that_.AddChurchMan(b);
		boat_ = RIGHT;
	}
	else{
		this_.AddWildMan(a);
		this_.AddChurchMan(b);
		that_.SubWildMan(a);
		that_.SubChurchMan(b);
		boat_ = LEFT;
	}
	return *this;
}

class Solution
{
public:
	Solution();
	void Find(std::vector<State> states,std::vector<int> & vmax);
	int Find();
private:
	void Init(){ states_.push_back(State(3, 3, 0, 0, State::LEFT)); }
	bool ValidStates(const std::vector<State> &,const State& );
private:
	std::vector<std::pair<int, int>> moves_;
	std::vector<State> mp_;
	std::vector<State> states_;
};

void DumpStates(const std::vector<State> &states)
{
	for (auto & s : states){
		cout << s.ToString() << "->";
	}
}

Solution::Solution()
{
	Init();
	moves_.push_back({ 0, 1 });
	moves_.push_back({ 1, 0 });
	moves_.push_back({ 2, 0 });
	moves_.push_back({ 0, 2 });
	moves_.push_back({ 1, 1 });
}

bool Solution::ValidStates(const std::vector<State> & states,const State & s)
{
	if (s.Error())return false;
	if (std::find(states.begin(), states.end(), s) != states.end()){
		return false;
	}
	return true;
}

int Solution::Find()
{
	std::vector<int> vmax;
	Find(states_,vmax);
	std::sort(vmax.begin(), vmax.end());
	return vmax.back();
}

void Solution::Find(std::vector<State> states,std::vector<int> & vmax)
{
	int count = 0;
	static int i = 0;
	

	for (auto & p : moves_){
		
		State back = states.back();
		State newState = back.Move(p);

		if (newState.IsTarget()){
			cout << "Soulution" << ++i << ": \n";
			vmax.push_back(i);
			states.push_back(newState);
			DumpStates(states);
			cout << "end \n\n";
		}
		else if (ValidStates(states,newState)){
			std::vector<State> newStates = states;
			newStates.push_back(newState);
			if (std::find(mp_.begin(),mp_.end(),newState) == mp_.end()){
				Find(newStates,vmax);
			}
			
		}
		else{
			++count;
		}

	}
	if (count == 5){
		if (std::find(mp_.begin(), mp_.end(), states_.back()) == mp_.end()){
			mp_.push_back(states.back());
		}
		return;
	}
}

int main()
{
	
	Solution s;
	cout << "Total " << s.Find() <<" solutions found!" << endl;

	return 0;
}