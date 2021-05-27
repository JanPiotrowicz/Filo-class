#pragma once

template<typename FiloClass>
struct FiloIterator
{
public:
	using ValueType = typename FiloClass::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

public:
	FiloIterator(PointerType ptr)
		: m_Ptr(ptr) {}

	FiloIterator& operator++()
	{
		m_Ptr--;
		return *this;
	}

	FiloIterator& operator++(int)
	{
		FiloIterator iterator = *this;
		(*this)--;
		return iterator;
	}

	FiloIterator& operator--()
	{
		return m_Ptr++;
	}

	FiloIterator& operator--(int)
	{
		FiloIterator iterator = *this;
		(*this)++;
		return iterator;
	}

	ReferenceType operator->()
	{
		return m_Ptr;
	}

	ReferenceType operator*()
	{
		return *m_Ptr;
	}

	bool operator==(const FiloIterator& other) const
	{
		return m_Ptr == other.m_Ptr;
	}

	bool operator!=(const FiloIterator& other) const
	{
		return !(*this == other);
	}

private:
	PointerType m_Ptr = nullptr;
};

template<typename T>
class Filo
{
public:
	using ValueType = T;
	using Iterator = FiloIterator<Filo<T>>;

public:
	Filo()
	{
		ReAlloc(2);
	}

	~Filo()
	{
		::operator delete(m_Data, m_Capacity * sizeof(T));
	}

public:
	void push(T& var)
	{
		ReAllocIfNeeded();

		m_Data[m_Size] = var;
		m_Size++;
	}

	void push(const T& var)
	{
		ReAllocIfNeeded();

		m_Data[m_Size] = var;
		m_Size++;
	}

	void push(T&& var) noexcept
	{
		ReAllocIfNeeded();

		m_Data[m_Size] = std::move(var);
		m_Size++;
	}

	template<typename... Args>
	void push_many(Args&&... args) noexcept
	{
		ReAllocIfNeeded(sizeof...(Args));

		(PushNoReAlloc(args), ...);
	}

	template<typename... Args>
	void emplace(Args&&... args) noexcept
	{
		ReAllocIfNeeded();

		new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
		m_Size++;
	}

	void pop()
	{
		m_Data[m_Size - 1].~T();
		m_Size--;
	}

	T& operator[](size_t index)
	{
		return m_Data[m_Size - index - 1];
	}

	void print() const
	{
		for (size_t i = 0; i < m_Size; i++)
			std::cout << m_Data[m_Size - i - 1] << ' ';
		std::cout << '\n';
	}

	std::ostream& print(std::ostream& os) const
	{
		for (size_t i = 0; i < m_Size; i++)
			os << m_Data[m_Size - i - 1] << ' ';
		return os;
	}

	std::ostream& operator<<(std::ostream& os) const
	{
		for (size_t i = 0; i < m_Size; i++)
			os << m_Data[m_Size - i - 1] << ' ';
		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, const Filo& other)
	{
		for (size_t i = 0; i < other.m_Size; i++)
			os << other.m_Data[other.m_Size - i - 1] << ' ';
		return os;
	}

	size_t size() const
	{
		return m_Size;
	}

	T& first() const
	{
		return m_Data[m_Size - 1];
	}

	T& last() const
	{
		return m_Data[0];
	}

	bool empty() const
	{
		return m_Size == 0 ? true : false;
	}

	Iterator begin()
	{
		return m_Data + m_Size - 1;
	}

	Iterator end()
	{
		return m_Data - 1;
	}

private:
	void ReAlloc(size_t newCapacity) noexcept
	{
		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		for (size_t i = 0; i < std::min(m_Size, newCapacity); i++)
			newBlock[i] = std::move(m_Data[i]);

		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		::operator delete(m_Data, m_Capacity * sizeof(T));

		m_Data = newBlock;
		m_Capacity = newCapacity;
		m_Size = std::min(m_Size, newCapacity);
	}

	void ReAllocIfNeeded()
	{
		if (m_Size > m_Capacity)
			ReAlloc(2 * m_Capacity + 1);
	}

	void ReAllocIfNeeded(size_t sizeNeeded)
	{
		if (m_Size + sizeNeeded > m_Capacity)
			ReAlloc(m_Capacity + sizeNeeded);
	}

	void PushNoReAlloc(T& var)
	{
		new(&m_Data[m_Size]) T(var);
		m_Size++;
	}

private:
	T* m_Data = nullptr;
	size_t m_Capacity = 0;
	size_t m_Size = 0;
};