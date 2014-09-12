#pragma once

#include <exception>

namespace worldlib
{
	namespace internal
	{

		class InvalidOptional : public std::exception
		{
			virtual const char *what() const throw() override { return "Attemped to access the value of an optional that was in an uninitialized state."; }
		};

		template <typename T>
		class Optional
		{
			T internalValue;
			bool representsValidState;
		public:
			Optional() { representsValidState = false; }
			Optional(T newValue) { representsValidState = true; internalValue = newValue; }
			Optional(const Optional &other)
			{
				representsValidState = other.representsValidState;
				if (representsValidState)
					internalValue = other.internalValue;
			}

			Optional &operator =(const Optional &other)
			{
				representsValidState = other.representsValidState;
				if (representsValidState)
					internalValue = other.internalValue;
				else
					internalValue = T();

				return *this;
			}

			bool isValid() const { return representsValidState; }
			const T &get() const { return internalValue; }
			T &get() { return internalValue; }

		};
	}
}