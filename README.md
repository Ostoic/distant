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


# On Potential Code Bloat of distant::process<uint>

For each instantiation of the class template distant::process<access_rights>, a copy of that class is created.
This is the purpose of templates, to provide a generic blueprint of a piece of code where upon its instantiation, that particular
version of the code is created for the instantiated type. Although we enjoy the type safety provided by creating distant::process
a class template (phantom type), the above situation is undesirable. The implementation of distant::process depends very loosely on its templated access rights, so we can assume process<1> will act identically to process<2>, however they will have their own copies of their implementation, which is wasteful.

Indeed, this seems to be an issue for the phantom type idiom, as the encoded type is no more than a compile-time identifier to help improve the safety of the code you write. Despite this issue, we can usually rely on the fact that the compiler is a brilliant assembler to "optimize out" the phantom type, or in our case, the access rights for the process. However, depending on the sophistication of the compiler you are using (in my case Visual C++), this is not always the case. 

Moreover if this situation occurs, there is a possible 16! explosion of template instantiations (if you are irresponsible, to say the least) that will inflict pain upon your compiler. Thus, our main resolve is to minimize the amount of code to be copied by the compiler if this happens. Moving the core functionality into a base class, and keeping the phantom type class as a thin wrapper around the base.

Although we would like to rely on the compiler properly "optimizing out" the access rights as a template parameter it is sometimes
the case that copies of each instantiation will creep their way into our binaries, despite their equivalent behaviour. Let us investigate how a compiler might deal with this situation. Armed with nothing but Visual Studio 2017, the following program was compiled on release mode with full program optimization enabled. 

Note that we use printf instead of std::cout to improve readability of the generated assembly that we shall see

```c++
int main()
{
	// Contains access rights for various windows objects
	using access_rights = distant::access_rights::process;

	constexpr auto access = access_rights::vm_read | access_rights::query_information;

	// Open processs 1 with pid 7372, and process 2 with pid 12664
	distant::process<access> proc1(7372);
	distant::process<access | access_rights::synchronize> proc2(12664);

	// Retrieve the file paths of their executables
	const auto fp1 = proc1.file_path();
	const auto fp2 = proc2.file_path();
	
	printf("File path for proc1 = %s\n", fp1.c_str());
	printf("File path for proc2 = %s\n", fp2.c_str());

	//std::cin.get();
	return 0;
}
```

This is a simple example that obtains the file path to the given process' executable file. We create two distant::process objects, both with differing access rights. The former is the process with process id (pid) 12664 with vm_read and query_information access rights, and the latter is the current process, with all access requested. Again, we omit any error checking for readability of the generated assembly we will be looking at. However, we will change the implementation of distant::process slightly so as to observe the affects of how the compiler inlines our code.

First, a few details about the implementation of distant::process. As we mentioned before, distant::process is a very thin phantom type wrapper around the core functionality of distant::process. The implementation is encapsulated in distant::windows::kernel::detail::process_base, in which every constructor is defined as either _declspec(noinline), or inline. 

We shall first choose to inline all constructors for process_base. Keeping this in mind, we might expect the compiled main to call both process<access>::{ctor}} and process<access | synchronize>::{ctor}, which would then both call process_base::{ctor}. Ideally, the compiler would recognize the identical functionality of the wrapper constructors, and remove the wrapper call altogether. However, this behaviour was not observed upon inspection of the (C++ converted) generated assembly:

The disassembler IDA Pro is used together with the Hex Rays plugin since it is easier to read than the assembly itself

![alt text](http://i.imgur.com/EeoY0dO.png)

Our first observation is that the compiler has created two identical copies of process::{ctor}, which is a consequence of encoding the access rights into its type. The process with access rights 1040 (vm_read | query_information) is constructed first, followed by the process with access rights 1049616 (vm_read | query_information | synchronize). We see that the disassembled C++ version did not pick up the initialized process ids of 7372 and 12664, which are certainly noticeable in the assembly of main (http://i.imgur.com/0w6i5Kb.png if you would like to see).

A nice optimization provided by the compiler is that both instantiations of process::file_path compiled into one copy of process::file, and any incompatible phantom types would be implicitly casted into the correct type in order to use that version of file_path. This means that the compiler recognized the identical functionality of each instantiation of process::file_path, which is what we might expect.

For completeness, we include see the list of compiled distant library functions, again proving that the compiler has indeed created two copies of the process constructor:

![alt text](http://i.imgur.com/kv16cVs.png)

We see that the compiler has inlined the call to process_base::{ctor}.

Next, we shall compile the implementation of process_base that uses the _declspec(noinline) keyword, to forbid the compiler from inlining the process_base constructors:

![alt text](http://i.imgur.com/KF4eLrh.png)

This time we observe that both processes are constructed directly through process_base, meaning the call to process::{ctor} has been inlined by the compiler. This is the ideal situation in order to reduce code bloat. The process_base constructor is first called with process id 7372 (0x1CCC), and then with process id 12664 (0x3178).

Again, the compiler provides us with this nice reinterpret_cast optimization.

We see that in the list of compiled distant library functions, no distant::process constructor is compiled, effectively combining both previous distant::process constructors (1024 and 1024614) into a single call to the base constructor:

![alt text](http://i.imgur.com/ywKDIkg.png)

The output for running both the inlined and noinlined versions is as follows:

(Chrome and Adobe Reader were the processes with pids 7372 and 12664 at the time of running the tests)

```
File path for proc1 = C:\Program Files (x86)\Google\Chrome\Application\chrome.exe
File path for proc2 = C:\Program Files (x86)\Common Files\Adobe\ARM\1.0\AdobeARM.exe
```
In conclusion, this might seem to be extremely pedantic, but even so it is interesting to investigate. As a library writer, it is important to understand how your library could possibly be used by compilers, and to provide the best possible compatibility. 

In this case, the compiler seems to inline the deepest nested function call first, then continue upwards to the highest level, possibly inlining function calls along the way. It is probably a good idea to let the compiler perform its optimizations despite our investigations. Moreover, this also shows that the compiler will be able to significantly reduce code bloat to the point of becoming a non-issue.
