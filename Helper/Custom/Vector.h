#pragma once

template<class t> class vector
{
public:
	vector();

	explicit vector(int s);

	vector(const vector& arg);

	vector<t>& operator=(const vector<t>& arg);

	~vector();

	class iterator;

	iterator begin();

	const iterator begin() const;

	iterator end();

	const iterator end() const;

	const iterator cbegin() const;

	const iterator cend() const;

	bool empty() const;

	uintptr_t capacity() const;

	void reserve(int newmalloc);

	void resize(int newsize, t val = t());

	uintptr_t size() const;

	uintptr_t max_size() const;

	void shrink_to_fit();

	void clear();

	void push_back(const t& d);

	void pop_back();

	t& at(int n);

	const t& at(int n) const;

	t& operator[](int i);

	const t& operator[](int i) const;

	t& front();

	const t& front() const;

	t& back();

	const t& back() const;

	t* data();

	const t* data() const;
private:
	uintptr_t _size;
	t* _elements;
	uintptr_t _space;
};

template<class t> class vector<t>::iterator
{
public:
	iterator(t* p)
		:_curr(p)
	{}

	iterator& operator++()
	{
		_curr++;
		return *this;
	}

	iterator& operator--()
	{
		_curr--;
		return *this;
	}

	t& operator*()
	{
		return *_curr;
	}

	bool operator==(const iterator& b) const
	{
		return *_curr == *b._curr;
	}

	bool operator!=(const iterator& b) const
	{
		return *_curr != *b._curr;
	}

private:
	t* _curr;
};

template<class t>
vector<t>::vector()
	:_size(0), _elements(0), _space(0)
{}

template<class T>
vector<T>::vector(int s)
	: _size(s), _elements(new T[s])
{
	for (int index = 0; index < _size; ++index)
		_elements[index] = T();
}

template<class t>
 vector<t>::vector(const vector& arg)
	:_size(arg._size), _elements(new t[arg._size])
{
	for (int index = 0; index < arg._size; ++index)
		_elements[index] = arg._elements[index];
}

template<class t>
 vector<t>& vector<t>::operator=(const vector<t>& a)
{
	if (this == &a) return *this;


	if (a._size <= _space)
	{
		for (int index = 0; index < a._size; ++index)
		{
			_elements[index] = a._elements[index];
			_size = a._size;
			return *this;
		}
	}

	t* p = new t[a._size];

	for (int index = 0; index < a._size; ++index)
		p[index] = a._elements[index];

	_size = a._size;
	_space = a._size;
	_elements = p;
	return *this;
}


template<class t>
vector<t>::~vector()
{
}

template<class t>
 typename vector<t>::iterator vector<t>::begin()
{
	return vector<t>::iterator(&_elements[0]);
}

template<class t>
 typename const vector<t>::iterator vector<t>::begin() const
{
	return vector<t>::iterator(&_elements[0]);
}

template<class t>
 typename vector<t>::iterator vector<t>::end()
{
	return vector<t>::iterator(&_elements[_size]);
}

template<class t>
 typename const vector<t>::iterator vector<t>::end() const
{
	return vector<t>::iterator(&_elements[_size]);
}

template<class t>
 typename const vector<t>::iterator vector<t>::cbegin() const
{
	return vector<t>::iterator(&_elements[0]);
}

template<class t>
 typename const vector<t>::iterator vector<t>::cend() const
{
	return vector<t>::iterator(&_elements[_size]);
}

template<class t>
 bool vector<t>::empty() const
{
	return (_size == 0);
}

template<class t>
 uintptr_t vector<t>::capacity() const
{
	return _space;
}

template<class t>
 void vector<t>::reserve(int newalloc)
{
	if (newalloc <= _space) return;

	t* p = new t[newalloc];

	for (int i = 0; i < _size; ++i)
		p[i] = _elements[i];

	_elements = p;

	_space = newalloc;
}

template<class t>
 void vector<t>::resize(int newsize, t val)
{
	reserve(newsize);

	for (int index = _size; index < newsize; ++index)
		_elements[index] = t();

	_size = newsize;
}

template<class t>
 uintptr_t vector<t>::size() const
{
	return _size;
}

template<class t>
 void vector<t>::clear()
{
	reserve(0);

	for (int index = 0; index < _size; ++index)
		_elements[index] = t();

	_size = uintptr_t(0);
	_space = uintptr_t(0);
}

template<class t>
 void vector<t>::push_back(const t& d)
{
	if (_space == 0)
		reserve(8);
	else if (_size == _space)
		reserve(2 * _space);

	_elements[_size] = d;

	++_size;
}

template<class t>
 t& vector<t>::at(int n)
{
	return _elements[n];
}

template<class t>
 const t& vector<t>::at(int n) const
{
	return _elements[n];
}

template<class t>
 t& vector<t>::operator[](int i)
{
	return _elements[i];
}

template<class t>
 const t& vector<t>::operator[](int i) const
{
	return _elements[i];
}

template<class t>
 t& vector<t>::front()
{
	return _elements[0];
}

template<class t>
 const t& vector<t>::front() const
{
	return _elements[0];
}

template<class t>
 t& vector<t>::back()
{
	return _elements[_size - 1];
}

template<class t>
 const t& vector<t>::back() const
{
	return _elements[_size - 1];
}

template<class t>
 t* vector<t>::data()
{
	return _elements;
}

template<class t>
 const t* vector<t>::data() const
{
	return _elements;
}