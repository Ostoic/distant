# distant
An experimental C++14 header-only library that aims to provide remote memory and process tools for 
the Windows operating system, with standard compliance and type safety in mind.

# Process Access Type Safety

Following the Windows "Access-Control Model", certain operations on process objects require
specific access rights to perform said operations. We can statically check access rights
at compile-time using a kind of "phantom type" to at least ensure proper access is requested when opening a process. 
The access rights template parameter is not entirely a phantom type, since it is actually used 
within the class itself, but it also serves a purpose similar to what phantom types do. Let us 
look at some of the benefits we might see when using templated access rights.

Suppose we would like to open the current process with the query_information access right. 
The following code would suffice in this request:

```c++
constexpr auto access = access_rights::query_information;
auto current = distant::process<access>::get_current();
```

If we would later like to check that the process is running, or perform a windows::wait operation
on the current process object, a compiler error will be thrown. This is because the above two operations
require at least the synchronize process access right. Therefore, following the above process access request
with query_information access rights, the code
```c++
if (current.is_active()) {...}
```
will produce a compiler error of the form:
```
Invalid access rights (distant::process::is_active): Process must have synchronize access right
```
If we do not statically check the access rights associated with a process, then errors such as the above 
one will be produced at run-time regardless. It is much easier to detect bugs through the compiler, instead of
at runtime, where simply forgetting the correct access rights for an API call can be harder to notice.

# Opening a System Process 

```c++
#include <iostream>

#include <distant\process.hpp>

template <distant::access_rights::process T>
void display_info(const distant::process<T>& p)
{
	// distant::process is explicitly convertible to bool
	if (p)
	{
		std::string active_string = (p.is_active()) ? "Active" : "Inactive";
		// Process information
		std::cout << "Process " << p.pid() << '\n';
		std::cout << "State: " << active_string << '\n';
		std::cout << "Name: " << p.name() << '\n';

		auto ms = p.memory_status();
		std::cout << "Private Usage: " << ms.private_usage() << " kb \n";
		std::cout << "Peak Private Usage: " << ms.peak_private_usage() << " kb \n";

		std::cout << "Working Set: " << ms.working_set() << " kb \n";
		std::cout << "Peak Working Set: " << ms.peak_working_set() << " kb \n";

		std::cout << "Number of Page Faults: " << ms.page_fault_count() << '\n';
		std::cout << "Number of Active Handles: " << ms.handle_count() << '\n';
	}
	else
		std::cout << "An error occured while opening process " << p.pid() << '\n';

	// Output a formatted last error message
	std::cout << "Last error: " << p.get_last_error() << '\n' << '\n';
}

int main()
{
	// Contains access rights for various windows objects
	using access_rights = distant::access_rights::process;
	using process = distant::process<>;
	
	// Request the following process access rights
	constexpr auto access = 
		access_rights::query_limited_information |
		access_rights::synchronize		 |
		access_rights::vm_read;

	// Open a system process (which has for example, process id 9148),
	// with the above process access rights.
	distant::process<access> system_proc(9148);
	
	// Open the current process with all_access (default template parameter is all_access).
	auto current = process::get_current();

	std::cout << "Current Process:" << std::endl;
	display_info(current);

	std::cout << "System Process:" << std::endl;
	display_info(system_proc);
	return 0;
}

```

Note that the number of handles actively owned by the current process above will vary. Moreover, 
access to process with pid 9148 will not always return a valid process object.
An example of output of the above program is:

```
Current Process:
Process 9640
State: Active
Active Handles: 13
File path: C:\Users\Owner\Documents\Visual Studio 2017\Projects\distant dev\Release\distant dev.exe
Last error: The operation completed successfully.

System Process:
An error occured while opening process 9148
Last Error: Access is denied.
```

# A Remark About Code Bloat

Although we would like to rely on the compiler properly "optimizing out" the access rights as a template parameter,
so as to reduce the compiler creating a class for every used access right in the program, it is sometimes the case
that such copies will creep their way into binaries. Such a case was observed when using Visual Studio 2017 with the
following program:

```c++
int main()
{
	// Use the process access_rights enum exclusively
	using access_rights = distant::access_rights::process;
	using process = distant::process<>;

	// Attempt to get information to perform a virtual memory read, and 
	// process query information, which are both required for process::file_path()
	constexpr auto access = access_rights::vm_read
						  | access_rights::query_information;

	// Open the process which has pid 12664
	distant::process<access> proc(12664);

	// Open the current process with all_access
	const auto current = process::get_current();

	const auto cur_fp = current.file_path();
	const auto proc_fp = proc.file_path();

	printf("File path for current = %s\n", cur_fp.c_str());
	printf("File path for proc = %s\n", proc_fp.c_str());
	
	return 0;
}
```

The output for running both inlined and noinlined versions is as follows:

```
main = 0xe311f0
File path for current = C:\Users\Owner\Documents\Visual Studio 2017\Projects\distance dev\Release\distance dev.exe
File path for proc = C:\Program Files (x86)\Common Files\Adobe\ARM\1.0\AdobeARM.exe
```

It seems that Adobe Reader must have gotten caught in our tests. 

# System Snapshots

Accessing data of a snapshot constructs a kernel::object, whose type is passed as a template parameter. This is done via 
snapshot_iterators, where dereferencing an iterator returns a newly-constructed kernel::object. Using system::snapshot, it is possible to iterate through a list of kernel objects that are active at the time of 
query. Typically this would be done through a call to CreateToolhelp32Snapshot, with the correct flag corresponding
to the type of object you want to query. Then a do {...} while (Process32Next(...)); loop would suffice for collecting
the information desired. However, we can simply write a C++11 range-based for loop with the given snapshot object as our
range.

# Traversing the Process List
```c++
// When iterating through each process, attempt to open the process with access_rights::all_access.
using process = distant::process<>;

// Create a system snapshot of all currently active processes
distant::system::snapshot<process> snapshot;

if (snapshot)
{
	// Display information about each proccess
	// Note: display_info is as defined as in the first example above
	// display_info also performs the correct error handling
	for (auto proc : snapshot)
		display_info(proc);
}
else
{
	std::cout << "Unable to create system::snapshot: " << snapshot.get_last_error() << std::endl;
	return;
}
	
```
# Search By Executable Name

```c++

auto find_process(const std::string& name)
{
	using process = distant::process<>;
	distant::system::snapshot<process> snapshot;

	if (snapshot)
	{
		auto it = std::find_if(snapshot.begin(), snapshot.end(), [&](const auto& p)
		{
			return p.name() == name;
		});

		if (it != snapshot.end()) return *it;
	}

	return process{};
}

int main()
{
	auto found = find_process("Explorer.exe");
	
	if (found)
	{
		std::cout << "Explorer.exe found!\n";
		display_info(found);
	}
	else
		std::cout << "Unable to find process with that name\n";
		
	return 0;
}

```
