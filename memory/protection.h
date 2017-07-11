#pragma once

namespace distant {
namespace memory  {

	// See https://msdn.microsoft.com/en-us/library/windows/desktop/aa366786(v=vs.85).aspx
	// for further information about memory protection
	enum class protection
	{
		execute = PAGE_EXECUTE,
		execute_r = PAGE_EXECUTE_READ,
		execute_rw = PAGE_EXECUTE_READWRITE,
		execute_write_copy = PAGE_EXECUTE_WRITECOPY,
		
		no_access = PAGE_NOACCESS,
		read_only = PAGE_READONLY,
		rw = PAGE_READWRITE,
		write_copy = PAGE_WRITECOPY,
		//page_targets_invalid = PAGE_TARGETS_INVALID,
		//page_targets_no_update = PAGE_TARGETS_NO_UPDATE,
		
		guard = PAGE_GUARD,
		no_cache = PAGE_NOCACHE,
		write_combine = PAGE_WRITECOMBINE,
	};

	DEFINE_ENUM_FLAG_OPERATORS(memory::protection);

} // end namespace memory
} // end namespace distant