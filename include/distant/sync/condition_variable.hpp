#pragma once

namespace distant::sync
{
	class condition_variable
	{
	public:
		condition_variable();

		condition_variable(const condition_variable&) = delete;
		condition_variable& operator=(const condition_variable&) = delete;
	private:
	};

} // namespace distant::sync

namespace distant
{
	using sync::condition_variable;
}
