# distant
A set of windows C++ process and remote memory tools 

```c++
#include <iostream>

#include <distant\process.hpp>

template <distant::access_rights::process T>
void display_info(const distant::process<T>& p)
{
	// In the case of any error opening the process,
	// its handle will be equivalent to distant::invalid_handle.
	if (p.get_handle() == distant::invalid_handle)
	{
		std::cout << "Process has an invalid handle." << std::endl;
		
		// If the handle is invalid, output a formatted last error message
		std::cout << "Last Error: " << p.get_last_error();
	}
	else
	{
		// Process information
		std::cout << "PID = " << p.get_id() << std::endl;
		std::cout << "Is Valid  = " << p.valid() << std::endl;
		std::cout << "Is Running  = " << p.is_running() << std::endl;
		std::cout << "Has " << p.get_handle_count() << " handles active" << std::endl;
	}
}

int main()
{
	// Contains access rights for various windows objects
	using distant::access_rights;
	
	// constexpr so we can use access rights as template parameters.
	constexpr auto access = 
		access_rights::process::query_information | 
		access_rights::process::terminate		  |
		access_rights::process::synchronize;

	// Open a system process (which has for example, process id 2304),
	// with the above process access_rights.
	distant::process<access> system_proc(2304);
	
	// Open the current process with all_access (default template parameter is all_access).
	auto current = distant::process<>::get_current();

	std::cout << "Current:" << std::endl;
	display_info(current);

	std::cout << "system_proc:" << std::endl;
	display_info(system_proc);
	return 0;
}

```
Ideas:

	- Implement vm::allocate, vm::deallocate
	
	- Implement distant::process_entry (or whichever name I decide upon) using 
	  an iterator type. Instead of the usual Process32Next while loop, we would 
	  simply use an iterator loop.
	  
	  Dereferencing the iterator would return either a process or process_entry.
	- Finish the custom allocator
	
	- Make windows::synchro similar to shared_ptr with reference counts and all that