// An "Antiafk" program that prevents the character from timing out in-game.
// For World of Warcraft version 3.3.5 123450

#include <iostream>
#include <thread>

#include <distant/process.hpp>
#include <distant/virtual_memory.hpp>

int main()
{
	using distant::process_rights;
	using distant::page_protection;

	// Open the process with pid 137016.
	// For simplicity, this is assumed to be the target wow.exe, version 3.3.5 123450.
	distant::process<distant::vm_rw_op> wow(50872); 

	if (!wow)
	{
		std::cerr << "Unable to open wow\n";
		return 0;
	}

	// Static addresses corresponding to the last action in-game, and the current timestamp.
	constexpr distant::address last_action_address = 0x00B499A4;
	constexpr distant::address timestamp_address = 0x00B1D618;

	// Get a reference to the last time we moved the mouse in-game
	auto last_action = distant::make_virtual_reference<int>(wow, last_action_address);

	// Get a read-only reference to the current wow time
	// Note that timestamp is a constant reference, which means writing is not permitted.
	const auto timestamp = distant::make_virtual_reference<const int>(wow, timestamp_address);

	try
	{
		while (true)
		{
			// Wait 1 second
			std::this_thread::sleep_for(std::chrono::seconds(1));

			// Read the current time
			int current_time = timestamp;
			std::cout << "Previous last_action: " << last_action << '\n';
			// This amounts to the following code: current_time = distant::memory::read<int>(wow, timestamp_address);

			// Write the current time to the last action.
			last_action = current_time;
			// This amounts to the following code: distant::memory::write<int>(wow, last_action_address, current_time);

			std::cout << "Modified last_action: " << last_action << '\n';
			std::cout << "Timestamp: " << timestamp << "\n\n";
		}
	}
	catch (distant::windows_error& e)
	{
		std::cout << e.what() << '\n';
	}
}

// https://wowgaming.altervista.org/aowow
