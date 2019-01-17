#include <iostream>
#include <string>
#include <set>

template<class T>
std::set<T> intersect(std::set<T>& lhs, std::set<T>& rhs)  
{
	std::set<T> result_set;
	typename std::set<T>::iterator it1;
	typename std::set<T>::iterator it2;

	for(it1 = lhs.begin(); it1 != lhs.end();  ++it1)
	{
		if(rhs.find((*it1)) != rhs.end()) //found
		{
			result_set.insert(*it1);
		}
	}
	return result_set;
}

//find union for OR command 
template<class T>
std::set<T> union_set(std::set<T>& lhs, std::set<T>& rhs)  
{
	std::set<T> result_set;
	typename std::set<T>::iterator it1;
	typename std::set<T>::iterator it2;

	for(it1 = lhs.begin(); it1 != lhs.end();  ++it1)
	{
		result_set.insert(*it1);
	}
	for(it2 = rhs.begin(); it2 != rhs.end();  ++it2)
	{
		result_set.insert(*it2);
	}
	return result_set;
}

