extern crate core;

#[lang="stack_exhausted"]
extern fn stack_exhausted() {}

#[lang="eh_personality"]
extern fn eh_personality() {}

#[lang="panic_fmt"]
pub fn panic_fmt(_fmt: &core::fmt::Arguments, _file_line: &(&'static str, usize)) -> ! {
    loop {}
}

#[doc(hidden)]
#[no_mangle]
pub unsafe extern fn __aeabi_memset(dest: *mut u8, size: usize, value: u32) {
    use core::intrinsics::volatile_set_memory;
    volatile_set_memory(dest, value as u8, size);
}

#[doc(hidden)]
#[no_mangle]
pub unsafe extern fn __aeabi_memclr(dest: *mut u8, size: usize) {
    use core::intrinsics::volatile_set_memory;
    volatile_set_memory(dest, 0, size);
}

#[doc(hidden)]
#[no_mangle]
pub unsafe extern fn __aeabi_memclr4(dest: *mut u8, size: usize) {
    __aeabi_memclr(dest, size);
}

