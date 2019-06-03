#pragma once

namespace distant::synch
{
	class condition_variable
	{
	public:
		condition_variable();

		condition_variable(const condition_variable&) = delete;
		condition_variable& operator=(const condition_variable&) = delete;
	private:
	};

} // namespace distant::synch

namespace distant
{
	using synch::condition_variable;
}
