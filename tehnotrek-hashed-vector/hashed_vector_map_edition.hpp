#pragma once
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <unordered_map>

template <class T, class Allocator = std::allocator<T>>
class hashed_vector
{
public:

#pragma region Types
	typedef T value_type;
	typedef Allocator allocator_type;

	typedef const value_type& const_reference;
	typedef typename std::allocator_traits<Allocator>::pointer pointer;
	typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
	typedef typename std::allocator_traits<Allocator>::size_type size_type;
	typedef typename std::allocator_traits<Allocator>::difference_type difference_type;

	class reference {
		friend class hashed_vector;
		reference() noexcept = delete;
		size_type index;
		hashed_vector& hashed_vector_;
		reference(size_type index, hashed_vector& hashed_vector) noexcept :index(index), hashed_vector_(hashed_vector) {};
	public:
		~reference() = default;
		operator T() const noexcept { return hashed_vector_.vector[index]; }
		reference& operator=(const value_type& x) noexcept
		{
			auto range = hashed_vector_.map.equal_range(hashed_vector_.value_hasher(hashed_vector_.vector[index]));
			for (auto it = range.first; it != range.second; ++it)
			{
				if (it->second == index)
				{
					hashed_vector_.map.erase(it);
					break;
				}
			}
			hashed_vector_.vector[index] = x;
			hashed_vector_.map.insert(std::pair<std::size_t, size_type>(hashed_vector_.value_hasher(hashed_vector_.vector[index]), index));
			return *this;
		}
		reference& operator=(const reference& x) noexcept
		{
			auto range = hashed_vector_.map.equal_range(hashed_vector_.value_hasher(hashed_vector_.vector[index]));
			for (auto it = range.first; it != range.second; ++it)
			{
				if (it->second == index)
				{
					hashed_vector_.map.erase(it);
					break;
				}
			}
			hashed_vector_.vector[index] = hashed_vector_.vector[x.index];
			hashed_vector_.map.insert(std::pair<std::size_t, size_type>(hashed_vector_.value_hasher(hashed_vector_.vector[index]), index));
			return *this;
		}
	};

	class Iterator : public std::iterator<std::random_access_iterator_tag, value_type>
	{
	public:
		using difference_type = size_type;
		using reference = typename hashed_vector::reference;

		Iterator() : index(0) {}
		Iterator(size_type rhs, hashed_vector& hashed_vector) : index(rhs), hashed_vector_(hashed_vector) {}
		Iterator(const Iterator &rhs) : index(rhs.index), hashed_vector_(rhs.hashed_vector_) {}
		/* inline Iterator& operator=(Type* rhs) {_ptr = rhs; return *this;} */
		/* inline Iterator& operator=(const Iterator &rhs) {_ptr = rhs._ptr; return *this;} */
		inline Iterator& operator+=(difference_type rhs) { index += rhs; return *this; }
		inline Iterator& operator-=(difference_type rhs) { index -= rhs; return *this; }
		inline reference operator*() { return reference(index, hashed_vector_); }
		//inline Type* operator->() const { return index; } //TODO: ???
		inline reference operator[](difference_type rhs) const { return reference(index + rhs, hashed_vector_); }

		inline Iterator& operator++() { ++index; return *this; }
		inline Iterator& operator--() { --index; return *this; }
		inline Iterator operator++(int) const { Iterator tmp(*this); ++index; return tmp; }
		inline Iterator operator--(int) const { Iterator tmp(*this); --index; return tmp; }
		/* inline Iterator operator+(const Iterator& rhs) {return Iterator(_ptr+rhs.ptr);} */
		inline difference_type operator-(const Iterator& rhs) const { return Iterator(index - rhs.index, hashed_vector_); }
		inline Iterator operator+(difference_type rhs) const { return Iterator(index + rhs, hashed_vector_); }
		inline Iterator operator-(difference_type rhs) const { return Iterator(index - rhs, hashed_vector_); }
		friend inline Iterator operator+(difference_type lhs, const Iterator& rhs) { return Iterator(lhs + rhs.index, rhs.hashed_vector_); }
		friend inline Iterator operator-(difference_type lhs, const Iterator& rhs) { return Iterator(lhs - rhs.index, rhs.hashed_vector_); }

		inline bool operator==(const Iterator& rhs) const { return index == rhs.index; }
		inline bool operator!=(const Iterator& rhs) const { return index != rhs.index; }
		inline bool operator>(const Iterator& rhs) const { return index > rhs.index; }
		inline bool operator<(const Iterator& rhs) const { return index < rhs.index; }
		inline bool operator>=(const Iterator& rhs) const { return index >= rhs.index; }
		inline bool operator<=(const Iterator& rhs) const { return index <= rhs.index; }
	private:
		size_type index;
		hashed_vector& hashed_vector_;
	};
	typedef Iterator iterator;

	class Const_iterator : public std::iterator<std::random_access_iterator_tag, value_type>
	{
	public:
		using difference_type = size_type;
		typedef const_reference reference;
		Const_iterator() : index(0) {}
		Const_iterator(size_type rhs, const std::vector<value_type>& vector) : index(rhs), vector(vector) {}
		Const_iterator(const Const_iterator &rhs) : index(rhs.index), vector(rhs.vector) {}
		/* inline Const_iterator& operator=(Type* rhs) {_ptr = rhs; return *this;} */
		/* inline Const_iterator& operator=(const Const_iterator &rhs) {_ptr = rhs._ptr; return *this;} */
		inline Const_iterator& operator+=(difference_type rhs) { index += rhs; return *this; }
		inline Const_iterator& operator-=(difference_type rhs) { index -= rhs; return *this; }
		inline const_reference operator*() const { return vector[index]; }
		inline const_pointer operator->() const { return &(vector[index]); } //TODO: ???
		inline const_reference operator[](difference_type rhs) const { return vector[index + rhs]; }

		inline Const_iterator& operator++() { ++index; return *this; }
		inline Const_iterator& operator--() { --index; return *this; }
		inline Const_iterator operator++(int) const { Const_iterator tmp(*this); ++index; return tmp; }
		inline Const_iterator operator--(int) const { Const_iterator tmp(*this); --index; return tmp; }
		/* inline Const_iterator operator+(const Const_iterator& rhs) {return Const_iterator(_ptr+rhs.ptr);} */
		inline difference_type operator-(const Const_iterator& rhs) const { return Const_iterator(index - rhs.index, vector); }
		inline Const_iterator operator+(difference_type rhs) const { return Const_iterator(index + rhs, vector); }
		inline Const_iterator operator-(difference_type rhs) const { return Const_iterator(index - rhs, vector); }
		friend inline Const_iterator operator+(difference_type lhs, const Const_iterator& rhs) { return Const_iterator(lhs + rhs.index, rhs.vector); }
		friend inline Const_iterator operator-(difference_type lhs, const Const_iterator& rhs) { return Const_iterator(lhs - rhs.index, rhs.vector); }

		inline bool operator==(const Const_iterator& rhs) const { return index == rhs.index; }
		inline bool operator!=(const Const_iterator& rhs) const { return index != rhs.index; }
		inline bool operator>(const Const_iterator& rhs) const { return index > rhs.index; }
		inline bool operator<(const Const_iterator& rhs) const { return index < rhs.index; }
		inline bool operator>=(const Const_iterator& rhs) const { return index >= rhs.index; }
		inline bool operator<=(const Const_iterator& rhs) const { return index <= rhs.index; }
	private:
		size_type index;
		const std::vector<value_type>& vector;
	};
	typedef Const_iterator const_iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

#pragma endregion 

#pragma region Construct/copy/destroy
	hashed_vector() noexcept : hashed_vector(Allocator()) { }
	explicit hashed_vector(const Allocator& alloc) noexcept : vector(alloc), map() {};
	explicit hashed_vector(size_type n, const Allocator& alloc = Allocator()) : vector(n, alloc), map() {};
	hashed_vector(size_type n, const value_type& value, const Allocator& alloc = Allocator()) : vector(n, value, alloc), map()
	{
		for (size_type i = 0; i < n; ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(value), i));
		}
	}
	template <class InputIterator>
	hashed_vector(InputIterator first, InputIterator last,
		const Allocator& alloc = Allocator()) : vector(first, last, alloc), map()
	{
		for (size_type i = 0; i < vector.size(); ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
		}
	}
	hashed_vector(const hashed_vector& x) : vector(x.vector), map(x.map) {};
	hashed_vector(hashed_vector&& x) noexcept : vector(x.vector), map(x.map) {};
	hashed_vector(const hashed_vector& x, const Allocator& alloc) : vector(x.vector, alloc), map(x.map) {};
	hashed_vector(hashed_vector&& x, const Allocator& alloc) : vector(x.vector, alloc), map(x.map) {};
	hashed_vector(std::initializer_list<T> list, const Allocator& alloc = Allocator()) : vector(list, alloc), map()
	{
		for (size_type i = 0; i < vector.size(); ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
		}
	}
	~hashed_vector() = default;
	hashed_vector& operator=(const hashed_vector& x)
	{
		hashed_vector tmp(x);
		this->swap(tmp);
		return *this;
	}
	hashed_vector& operator=(hashed_vector&& x)
		noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
			std::allocator_traits<Allocator>::is_always_equal::value)
	{
		this->swap(x);
		return *this;
	}
	hashed_vector& operator=(std::initializer_list<T> list)
	{
		hashed_vector tmp(list);
		this->swap(tmp);
		return *this;
	}
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		map.clear();
		vector.assign(first, last);
		for (size_type i = 0; i < vector.size(); ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
		}
	}
	void assign(size_type n, const T& u)
	{
		map.clear();
		vector.assign(n, u);
		for (size_type i = 0; i < vector.size(); ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
		}
	}
	void assign(std::initializer_list<T> list)
	{
		map.clear();
		vector.assign(list);
		for (size_type i = 0; i < vector.size(); ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
		}
	}
	allocator_type get_allocator() const noexcept { return vector.get_allocator(); }
#pragma endregion Done

#pragma region Iterators
	iterator begin() noexcept { return iterator(0, *this); }
	const_iterator begin() const noexcept { return const_iterator(0, vector); }
	iterator end() noexcept { return iterator(size(), *this); }
	const_iterator end() const noexcept { return const_iterator(size(), vector); }
	reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
	reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
	const_iterator cbegin() const noexcept { return const_iterator(0, vector); }
	const_iterator cend() const noexcept { return const_iterator(size(), vector); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
#pragma endregion Done

#pragma region Capacity
	size_type size() const noexcept { return vector.size(); }
	size_type max_size() const noexcept { return std::min(vector.max_size(), map.max_size()); }
	void resize(size_type sz)
	{
		size_type original_size = size();
		if (sz > original_size)
		{
			vector.resize(sz);
			for (size_type i = original_size; i < sz; ++i)
			{
				map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
			}
		}
		else if (sz < original_size)
		{
			for (size_type i = sz; i < original_size; ++i)
			{
				auto range = map.equal_range(value_hasher(vector[i]));
				for (auto it = range.first; it != range.second; ++it)
				{
					if (it->second == i)
					{
						map.erase(it);
						break;
					}
				}
			}
			vector.resize(sz);
		}
	}
	void resize(size_type sz, const T& c)
	{
		size_type original_size = vector.size();
		if (sz > original_size)
		{
			vector.resize(sz, c);
			for (size_type i = original_size; i < sz; ++i)
			{
				map.insert(std::pair<std::size_t, size_type>(value_hasher(c), i));
			}
		}
		else if (sz < original_size)
		{
			for (size_type i = sz; i < original_size; ++i)
			{
				auto range = map.equal_range(value_hasher(vector[i]));
				for (auto it = range.first; it != range.second; ++it)
				{
					if (it->second == i)
					{
						map.erase(it);
						break;
					}
				}
			}
			vector.resize(sz, c);
		}
	}
	size_type capacity() const noexcept { return vector.capacity(); }
	bool empty() const noexcept { return vector.empty(); }
	void reserve(size_type n) { vector.reserve(n); map.reserve(n); }
	void shrink_to_fit() { vector.shrink_to_fit(); }
#pragma endregion Done

#pragma region Element access
	reference operator[](size_type n) { return reference(n, *this); }
	const_reference operator[](size_type n) const { return vector[n]; }
	const_reference at(size_type n) const { return vector.at(n); }
	reference at(size_type n)
	{
		if (n >= size())
		{
			throw std::out_of_range("Index out of range");
		}
		return reference(n, *this);
	}
	reference front() { return reference(0, *this); }
	const_reference front() const { return vector.front(); }
	reference back() { return reference(size() - 1, *this); }
	const_reference back() const { return vector.back(); }
#pragma endregion Done

#pragma region Data access
	///<summary>Warning: modifying data in array results in undefined behaviour for hashed_vector</summary>
	value_type* data() noexcept { return vector.data(); }
	const value_type* data() const noexcept { return vector.data(); }
#pragma endregion Done

#pragma region Modifiers
	template <class... Args> void emplace_back(Args&&... args)
	{
		vector.emplace_back(args);
		map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[size() - 1]), size() - 1));
	}
	void push_back(const T& x)
	{
		vector.push_back(x);
		map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[size() - 1]), size() - 1));
	}
	void push_back(T&& x)
	{
		vector.push_back(x);
		map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[size() - 1]), size() - 1));
	}
	void pop_back()
	{
		auto range = map.equal_range(value_hasher(vector.back()));
		for (auto it = range.first; it != range.second; ++it)
		{
			if (it->second == size() - 1)
			{
				map.erase(it);
				break;
			}
		}
		vector.pop_back();
	}
	template <class... Args> iterator emplace(const_iterator position, Args&&... args)
	{
		iterator result = vector.emplace(position, args);
		rehash();
		return result;
	}
	iterator insert(const_iterator position, const T& x)
	{
		iterator result = vector.insert(position, x);
		rehash();
		return result;
	}
	iterator insert(const_iterator position, T&& x)
	{
		iterator result = vector.insert(position, x);
		rehash();
		return result;
	}
	iterator insert(const_iterator position, size_type n, const T& x)
	{
		iterator result = vector.insert(position, n, x);
		rehash();
		return result;
	}
	template <class InputIterator>
	iterator insert(const_iterator position,
		InputIterator first, InputIterator last)
	{
		iterator result = vector.insert(position, first, last);
		rehash();
		return result;
	}
	iterator insert(const_iterator position, std::initializer_list<T> il)
	{
		iterator result = vector.insert(position, il);
		rehash();
		return result;
	}
	iterator erase(const_iterator position)
	{
		iterator result = vector.erase(position);
		rehash();
		return result;
	}
	iterator erase(const_iterator first, const_iterator last)
	{
		iterator result = vector.erase(first, last);
		rehash();
		return result;
	}
	void swap(hashed_vector& x)
		noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
			std::allocator_traits<Allocator>::is_always_equal::value)
	{
		vector.swap(x.vector);
		map.swap(x.map);
	}
	void clear() noexcept
	{
		vector.clear();
		map.clear();
	}
#pragma endregion Done

	iterator find(const_reference x)
	{
		auto range = map.equal_range(value_hasher(x));
		for (auto it = range.first; it != range.second; ++it)
		{
			if (vector[(it->second)] == x)
			{
				return iterator(it->second, *this);
			}
		}
		return end();
	}
	const_iterator find(const_reference x) const
	{
		auto range = map.equal_range(value_hasher(x));
		for (auto it = range.first; it != range.second; ++it)
		{
			if (vector[(it->second)] == x)
			{
				return const_iterator(it->second, vector);
			}
		}
		return cend();
	}

	iterator find_hash(std::size_t hash)
	{
		auto result = map.find(hash);
		if (result != map.end())
		{
			return iterator(result->second, *this);
		}
		return end();
	}
	const_iterator find_hash(std::size_t hash) const
	{
		auto result = map.find(hash);
		if (result != map.end())
		{
			return const_iterator(result->second, vector);
		}
		return cend();
	}

private:
	std::vector<value_type, allocator_type> vector;
	std::unordered_multimap<std::size_t, size_type> map;
	std::hash<value_type> value_hasher;

	void rehash()
	{
		map.clear();
		for (size_type i = 0; i < size(); ++i)
		{
			map.insert(std::pair<std::size_t, size_type>(value_hasher(vector[i]), i));
		}
	}

	friend bool operator==(const hashed_vector<T, Allocator>& x, const hashed_vector<T, Allocator>& y) { return x.vector == y.vector; }
	friend bool operator< (const hashed_vector<T, Allocator>& x, const hashed_vector<T, Allocator>& y) { return x.vector < y.vector; }
	friend bool operator!=(const hashed_vector<T, Allocator>& x, const hashed_vector<T, Allocator>& y) { return x.vector != y.vector; }
	friend bool operator> (const hashed_vector<T, Allocator>& x, const hashed_vector<T, Allocator>& y) { return x.vector > y.vector; }
	friend bool operator>=(const hashed_vector<T, Allocator>& x, const hashed_vector<T, Allocator>& y) { return x.vector >= y.vector; }
	friend bool operator<=(const hashed_vector<T, Allocator>& x, const hashed_vector<T, Allocator>& y) { return x.vector <= y.vector; }
	friend void swap(hashed_vector<T, Allocator>& x, hashed_vector<T, Allocator>& y)
		noexcept(noexcept(x.swap(y))) {
		x.swap(y);
	}
};