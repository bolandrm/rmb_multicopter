#![feature(allocator)]
#![allocator]

#![no_std]

// Allocators are not allowed to depend on the standard library which in turn
// requires an allocator in order to avoid circular dependencies. This crate,
// however, can use all of libcore.

// Let's give a unique name to our custom allocator
#![crate_name = "embedded_allocator"]
#![crate_type = "rlib"]

// Listed below are the five allocation functions currently required by custom
// allocators. Their signatures and symbol names are not currently typechecked
// by the compiler, but this is a future extension and are required to match
// what is found below.
//
// Note that the standard `malloc` and `realloc` functions do not provide a way
// to communicate alignment so this implementation would need to be improved
// with respect to alignment in that aspect.

#[repr(u8)]
pub enum c_void {
    __variant1,
    __variant2,
}

extern {
    pub fn malloc(size: u32) -> *mut c_void;
    pub fn realloc(p: *mut c_void, size: u32) -> *mut c_void;
    pub fn free(p: *mut c_void);
}

#[no_mangle]
pub extern fn __rust_allocate(size: usize, _align: usize) -> *mut u8 {
	unsafe { malloc(size as u32) as *mut u8 }
}

#[no_mangle]
pub extern fn __rust_deallocate(ptr: *mut u8, _old_size: usize, _align: usize) {
	unsafe { free(ptr as *mut c_void); }
}

#[no_mangle]
pub extern fn __rust_reallocate(ptr: *mut u8, _old_size: usize, size: usize,
                                _align: usize) -> *mut u8 {
	unsafe { realloc(ptr as *mut c_void, size as u32) as *mut u8 }
}

#[no_mangle]
pub extern fn __rust_reallocate_inplace(_ptr: *mut u8, old_size: usize,
                                        _size: usize, _align: usize) -> usize {
    old_size
}

#[no_mangle]
pub extern fn __rust_usable_size(size: usize, _align: usize) -> usize {
    size
}
