#include "hashed_vector_map_edition.hpp"
#include <iostream>
using namespace std;
int main(int argc, char * argv[], char * env[])
{
	hashed_vector<int> v = { 1,2,3,4 };
	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = *it;
		//it[0] = 5;
		std::cout << s << std::endl;
	}
	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = it[0];
		std::cout << it[0] << std::endl;
	}
	cout << v.size() << endl;
	cout << v.max_size() << endl;
	v.resize(2);
	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = it[0];
		std::cout << it[0] << std::endl;
	}
	v.resize(7, 3);
	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = it[0];
		std::cout << it[0] << std::endl;
	}
	try
	{
		cout << v.at(5) << endl;
	}
	catch (std::out_of_range e)
	{
		cout << e.what() << endl;
	}
	try
	{
		cout << v.at(10) << endl;
	}
	catch (std::out_of_range e)
	{
		cout << e.what() << endl;
	}
	cout << v[0] << v.back() << v.front() << endl;
	v.front() = 100;
	v.back() = 101;
	cout << v[0] << v.back() << v.front() << endl;

	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = it[0];
		std::cout << it[0] << std::endl;
	}
	v.pop_back();
	v.pop_back();

	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = it[0];
		std::cout << it[0] << std::endl;
	}

	v.clear();
	for (hashed_vector<int>::const_reverse_iterator it = v.crbegin(); it != v.crend(); ++it)
	{
		auto s = it[0];
		std::cout << it[0] << std::endl;
	}

	cout << "Find tests" << endl;

	v = { 1,2,3,4,5,6,7,8,6,4,9,4 };

	for (hashed_vector<int>::const_iterator it = v.cbegin(); it != v.cend(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << "Find(4): " << *v.find(4) << endl;
	cout << "Find(1): " << *v.find(1) << endl;
	cout << "Find(6): " << *v.find(6) << endl;
	cout << "Find(9): " << *v.find(9) << endl;
	if (v.find(10) == v.end())
	{
		cout << "Find(10): can't find 10, that's good" << endl;
	}
	cout << "Find_hash tests" << endl;

	std::hash<int> hasher;

	cout << "Find_hash(hash(4)): " << *v.find_hash(hasher(4)) << endl;
	cout << "Find_hash(hash(1)): " << *v.find_hash(hasher(1)) << endl;
	cout << "Find_hash(hash(6)): " << *v.find_hash(hasher(6)) << endl;
	cout << "Find_hash(hash(9)): " << *v.find_hash(hasher(9)) << endl;
	if (v.find_hash(hasher(10)) == v.end())
	{
		cout << "Find(10): can't find 10, that's good" << endl;
	}

	*v.find(9) = 11;
	cout << "Find(11): " << *v.find(11) << endl;
	for (hashed_vector<int>::const_iterator it = v.cbegin(); it != v.cend(); ++it)
	{
		std::cout << *it << std::endl;
	}


	hashed_vector<int> v2 = { 1,2,3,4 };
	cout << (v2 == v2) << endl;
	cout << (v == v2) << endl;
	cout << (v > v2) << endl;
	swap(v, v2);
	for (hashed_vector<int>::const_iterator it = v.cbegin(); it != v.cend(); ++it)
	{
		std::cout << *it << std::endl;
	}
	system("PAUSE");
	return 0;
}
