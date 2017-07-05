# distant
A set of windows C++ process and remote memory tools 

```c++
int main()
{
	using namespace distant;
	
	// Open process with process id 312, and with the follwing access rights
	process example(312, process::access_rights::vm_read | 
						 process::access_rights::query_information);
	
	auto mem = example.get_vm();
	auto bytes = example.read<int>(0x400000);
	
	std::cout << "Exe base bytes: " << bytes << std::endl;
	
	process current = process::get_current();
	
	std::cout << "Current process Id = " << current.get_id() << std::endl;

	return 0;
}
```
