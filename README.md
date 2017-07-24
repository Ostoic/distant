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

The output of the above program is:

```
Current Process:
PID = 8108
Is Valid  = 1
Is Running  = 1
Has 13 handles active

System Process:
Process has an invalid handle.
Last Error: Access is denied.
```

Note that the number of handles actively owned by the current process above will vary. Moreover, 
access to process with pid 2304 will not always return a valid process object.