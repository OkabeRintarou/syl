#include <list>
#include <unordered_map>

class AssistList
{

	using ListIteratorMap = std::unordered_map<int, std::list<int>::iterator>;
public:
	AssistList(size_t capacity,std::unordered_map<int,int> &map_)
		:cache_(map_)
	{
		count_ = 0;
		capacity_ = (capacity == 0) ? 1 : capacity;
	}

	void Push(int key)
	{
		auto itr = cache_.find(key);
		if (itr == cache_.end()){
			if (IsFull()){
				hash_.erase(LastUsed());
				list_.pop_back();
				
			}
			else{
				++count_;
			}
			list_.push_front(key);
			hash_.insert(ListIteratorMap::value_type(key, list_.begin()));
		}
		else{
			list_.erase(hash_[key]);
			hash_.erase(key);
			list_.push_front(key);
			
			hash_[key] = list_.begin();
		}
	}

	int LastUsed()
	{
		if (count_ != 0){
			return list_.back();
		}
		return -1;
	}

	bool IsFull()const{ return count_ == capacity_; }
	bool IsEmpty()const{ return count_ == 0; }
private:
	std::unordered_map<int, int> &cache_;
	std::unordered_map<int, std::list<int>::iterator> hash_;
	std::list<int> list_;
	size_t count_;
	size_t capacity_;
};

class LRUCache
{
public:
	LRUCache(int capacity)
		:assist_(capacity,cache_)
	{}

	int get(int key)
	{
		int r = -1;
		if (cache_.find(key) != cache_.end()){
			
			r = cache_[key];
			assist_.Push(key);
		}
		return r;
	}

	void set(int key, int value)
	{
		
		if (cache_.find(key) == cache_.end() && assist_.IsFull()){
			cache_.erase(assist_.LastUsed());
		}
		assist_.Push(key);
		cache_[key] = value;
		
	}
private:
	std::unordered_map<int, int> cache_;
	AssistList assist_;
};