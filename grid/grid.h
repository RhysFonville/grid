// Rhys Fonville
// 11/15/23
// grid.h

#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

template <typename T>
class Grid {
public:
	template <typename T>
	struct GridIterator {
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = T*;
		using pointer           = value_type*;
		using reference         = value_type&;
		
		GridIterator(pointer ptr) : m_ptr(ptr) {}

		reference operator*() const { return *m_ptr; }
		pointer operator->() { return m_ptr; }
		GridIterator& operator++() { m_ptr++; return *this; }
		GridIterator operator++(T) { GridIterator tmp = *this; (*this)++; return tmp; }
		friend bool operator== (const GridIterator& a, const GridIterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const GridIterator& a, const GridIterator& b) { return a.m_ptr != b.m_ptr; };  

	private:
		pointer m_ptr;
	};

	using iterator = GridIterator<T>;

	using Vec = std::vector<T>;

	//https://stackoverflow.com/questions/11134497/constant-sized-vector#:~:text=like%20to%20use-,unique_ptr,-%3A
	using Arr = T*;
	using ArrGrid = Arr*;

	// Arrtor compatibility
	static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
		"because of [container.requirements].");

	~Grid() noexcept {
		for (int i = 0; i < _size_x; i++)
			delete[] grid[i];
		delete[] grid;
	}

	Grid() noexcept { }

	Grid(size_t x, size_t y) : grid(new T*[x]), _size_x(x), _size_y(y) {
		for (int i = 0; i < x; i++) {
			grid[i] = new T[y];
		}
	}

	Grid(size_t size) : Grid(size, size) { }

	Vec & operator[](size_t i) noexcept {
		return Vec(grid[i]);
	}

	Vec & at(size_t i) {
		if (_size_x <= i) throw std::out_of_range("Index was greater than x size.");

		return Vec(grid[i]);
	}

	Vec & at(size_t x, size_t y) {
		if (_size_x <= x) throw std::out_of_range("Index was greater than x size.");
		if (_size_y <= y) throw std::out_of_range("Index was greater than x size.");

		return at(x)[y];
	}

	iterator front() const {
		if (_size_x == 0) throw std::out_of_range("Grid is empty.");
		return grid[0];
	}

	iterator back() const noexcept {
		if (_size_x == 0) throw std::out_of_range("Grid is empty.");
		return grid[_size_x-1];
	}

	iterator begin() {
		return iterator(&grid[0]);
	}

	iterator end()   {
		return iterator(&grid[_size_x]);
	}

	Vec & row(size_t i) noexcept {
		Vec ret;
		
		for (Arr& arr : grid) {
			ret.emplace_back(arr[i]);
		}

		return ret;
	}

	Vec & coloumn(size_t i) noexcept {
		return Vec(grid[i]);
	}

	size_t size_x() const noexcept {
		return _size_x;
	}

	size_t size_y() const noexcept {
		return _size_y;
	}

	void clear() noexcept {
		for (int i = 0; i < _size_x; i++) {
			for (int j = 0; j < _size_y; j++) {
				grid[i][j] = T();
			}
		}
	}

private:
	ArrGrid grid = nullptr;
	size_t _size_x = 0, _size_y = 0;
};
