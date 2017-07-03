#pragma once

#include <Memory\Memory.h>
#include <Utilities\Utilities.h>

namespace WoW
{
	template <typename T>
	class Variable
	{

	public:
		enum StoragePolicy
		{
			Local,
			Passive,
			Invasive,
		};

		Variable(StoragePolicy p) { this->policy = p; }

		Variable(DWORD addr, StoragePolicy p) { this->address = addr; this->retrieve(); this->policy = p; }
		Variable(DWORD addr) { this->address = addr; this->retrieve();  this->makePassive(); }
		Variable() : address(NULL) { this->makePassive(); }

		bool isInvasive() { return this->policy == Invasive; }
		bool isPassive() { return this->policy == Passive; }

		void makePassive() { this->policy = Passive; }
		void makeInvasive() { this->policy = Invasive; }

		// = operator for setting this->val and remote value
		// &= operator for setting address

		Variable<T>& operator = (T const &rhs)
		{
			std::vector<T, 
			this->write(rhs);
			return *this;
		}

		DWORD &operator &()
		{
			return this->address;
		}

		// Only called on explicit (TYPE) cast
		// Not called in example: printf("%d", intVar), where WoW::Variable<int> intVar(address);
		// Called in example: printf("%d", (int)intVar), where WoW::Variable<int> intVar(address);
		T & operator *()
		{
			this->retrieve();
			return this->val;
		}

		// Set address operator
		Variable<T>& operator &= (DWORD const &addr)
		{
			address = addr;
			this->retrieve();
			return *this;
		}

		// Return value without updating
		operator T()
		{
			return this->val;
		}

		Variable<T> &operator <<(const T &in)
		{
			this->makeInvasive();
			this->write(in);
			this->makePassive();
			return *this;
		}

	private:
		DWORD address;
		T val;

		StoragePolicy policy;

		void retrieve()
		{
			if (this->address != 0)
				rpm(this->address, &this->val, sizeof(this->val));
		}

		void write(const T in)
		{
			this->val = in;
			if (this->isInvasive())
				wpm(this->address, &this->val, sizeof(this->val));
		}
	};
};

template <typename T>
inline WoW::Variable<T> operator &(WoW::Variable<T> &lhs, T &rhs)
{
	return WoW::Variable<T>((T)lhs & rhs);
}

template <typename T>
inline WoW::Variable<T> operator &(T &lhs, WoW::Variable<T> &rhs)
{
	return WoW::Variable<T>((T)rhs & lhs);
}

template <typename T>
inline WoW::Variable<T> operator +(WoW::Variable<T> &lhs, T &rhs)
{
	return WoW::Variable<T>((T)lhs + rhs);
}

template <typename T>
inline WoW::Variable<T> operator +(T &lhs, WoW::Variable<T> &rhs)
{
	return WoW::Variable<T>((T)rhs + lhs);
}

template <typename T>
inline WoW::Variable<T> operator *(WoW::Variable<T> &lhs, T &rhs)
{
	return WoW::Variable<T>((T)lhs * rhs);
}

template <typename T>
inline WoW::Variable<T> operator *(T &lhs, WoW::Variable<T> &rhs)
{
	return WoW::Variable<T>((T)rhs * lhs);
}

template <typename T>
inline bool operator ==(WoW::Variable<T> &lhs, T &rhs)
{
	return (T)lhs == rhs;
}

template <typename T>
inline bool operator ==(T &lhs, WoW::Variable<T> &rhs)
{
	return (T)rhs == lhs;
}

template <typename T>
inline bool operator !=(T &lhs, WoW::Variable<T> &rhs)
{
	return !(rhs == lhs);
}

template <typename T>
inline bool operator !=(WoW::Variable<T> &lhs, T &rhs)
{
	return !(rhs == lhs);
}

template <typename T>
inline bool operator !=(WoW::Variable<T> &lhs, WoW::Variable<T> &rhs)
{
	return (T)lhs != (T)rhs;
}
