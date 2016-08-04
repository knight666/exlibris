#pragma once

namespace ExLibris {

	template <typename Type>
	class StlAllocator
		: public std::allocator<Type>
	{

	public:
		typedef std::allocator<Type>::pointer pointer;
		typedef std::allocator<Type>::const_pointer const_pointer;
		typedef std::allocator<Type>::reference reference;
		typedef std::allocator<Type>::const_reference const_reference;
		typedef std::allocator<Type>::size_type size_type;
		typedef std::allocator<Type>::difference_type difference_type;

	public:
		StlAllocator() throw()
			: std::allocator<Type>()
		{
		}

		StlAllocator(const StlAllocator& other) throw()
			: std::allocator<Type>(other)
		{
		}

		template <typename Other>
		StlAllocator(const std::allocator<Other>& other) throw()
			: std::allocator<Type>(other)
		{
		}

		~StlAllocator() { }

		pointer allocate(size_type count, const void* hint = nullptr)
		{
			return (pointer)GetAllocator()->Allocate("stl", count * sizeof(Type));
		}

		void deallocate(pointer data, size_type count)
		{
			GetAllocator()->Free("stl", data);
		}

	};

	typedef std::basic_string<char, std::char_traits<char>, StlAllocator<char>> String;
	typedef std::basic_stringstream<char, std::char_traits<char>, StlAllocator<char>> StringStream;

	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>> WideString;
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>> WideStringStream;

	template <class Key, class Type>
	struct Map
	{
		typedef std::map<Key, Type, std::less<Key>, StlAllocator<std::pair<const Key, Type>>> type;
	};

	template <typename Type>
	struct Vector
	{
		typedef std::vector<Type, StlAllocator<Type>> type;
	};

};