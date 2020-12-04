#pragma once

#include <vector>
#include <cassert>
#include <stdint.h>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <functional>
#include <random>

template<typename T>
class Backpack_t {
	static_assert(std::is_integral_v<T>, "T must be an integer type");
	
	static constexpr size_t T_BIT = sizeof(T) * CHAR_BIT;

public:
	Backpack_t(size_t itemCount) : bitCount(itemCount) {
		data.resize((bitCount + T_BIT - 1) / T_BIT);
	}

	Backpack_t(const Backpack_t<T>& parent1, const Backpack_t<T>& parent2) : Backpack_t(parent1.bitCount) {
		Backpack_t<T> both = parent1 & parent2;
		Backpack_t<T> one = parent1 ^ parent2;
		return both | (one & random(bitCount));
	}

	bool operator[](size_t idx) const {
		return (data[idx / T_BIT] >> (idx % T_BIT)) & 0x1;
	}

	Backpack_t<T> operator&(const Backpack_t<T>& rhs) const {
		assert(rhs.bitCount == bitCount);

		Backpack_t<T> result(bitCount);

		std::transform(data.cbegin(), data.cend(), rhs.data.cbegin(), result.data.begin(), std::bit_and<T>());

		return result;
	} 

	Backpack_t<T> operator|(const Backpack_t<T>& rhs) const {
		assert(rhs.bitCount == bitCount);

		Backpack_t<T> result(bitCount);

		std::transform(data.cbegin(), data.cend(), rhs.data.cbegin(), result.data.begin(), std::bit_or<T>());

		return result;
	}

	Backpack_t<T> operator^(const Backpack_t<T>& rhs) const {
		assert(rhs.bitCount == bitCount);

		Backpack_t<T> result(bitCount);

		std::transform(data.cbegin(), data.cend(), rhs.data.cbegin(), result.data.begin(), std::bit_xor<T>());

		return result;
	}

	Backpack_t<T>& operator&=(const Backpack_t<T>& rhs) {
		*this = *this & rhs;
	}

	Backpack_t<T>& operator|=(const Backpack_t<T>& rhs) {
		*this = *this | rhs;
	}

	Backpack_t<T>& operator^=(const Backpack_t<T>& rhs) {
		*this = *this ^ rhs;
	}

	void mutate(uint32_t numer, uint32_t denom) {
		assert(denom > 0);
		std::uniform_int_distribution<uint32_t> dist(1, denom);
		
		for (size_t i = 0; i < bitCount; i++) {
			if (dist(std::rand) <= numer) {
				flip(i);
			}
		}

	}

	void flip(size_t idx) {
		data[idx / T_BIT] ^= 0x1 << (idx % T_BIT);
	}

	size_t size() const {
		return bitCount;
	}

	static Backpack_t<T> random(size_t itemCount) {
		Backpack_t<T> result(itemCount);
		for (size_t i = 0; i < result.data.size(); i++) {
			result.data[i] = std::rand();
		}
		return result;
	}



private:
	std::vector<T> data;
	size_t bitCount;
};


// print operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const Backpack_t<T>& bp) {
	for (size_t i = 0; i < bp.size(); i++) {
		os << bp[i] ? "1" : "0";
	}

	return os;
}


using Backpack = Backpack_t<uint32_t>;
