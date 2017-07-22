# distant
A set of windows C++ process and remote memory tools 

```c++
template <distant::access_rights T>
void display_info(const distant::process<T>& p)
{
	std::cout << "PID = " << p.get_id() << std::endl;
	std::cout << "Is Running  = " << p.is_running() << std::endl << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace distant;

	// Open process with process id 10576, with the vm_operation access rights
	// NOTE: This will produce a compiler error, since vm_operation is not a sufficient
	// process access right for calling p.is_running() above.
	process<access_rights::vm_operation> some_process(10576);
	
	// Open the current process with the synchronize access rights
	auto current = process<access_rights::synchronize>::get_current();

	std::cout << "some_process:" << std::endl;
	display_info(some_process);

	std::cout << "Current Process:" << std::endl;
	display_info(current);
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