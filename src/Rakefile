require "json"
require "rake/clean"

if File.exists?("project.json")
  CONFIG = JSON.parse(File.read("project.json"))
else
  CONFIG = {}
end

CONFIG["target"] ||= "main"
CONFIG["options"] ||= ["-DF_CPU=96000000", "-DUSB_SERIAL", "-DLAYOUT_US_ENGLISH", "-D__MK20DX256__", "-DARDUIO=104"]
CONFIG["tools_path"] ||= "/Applications/Arduino.app/Contents/Resources/Java/hardware/tools"
CONFIG["arduino_libs"] ||= []
CONFIG["libraries"] ||= "../libraries"
CONFIG["arduino_libs_path"] ||= "/Applications/Arduino.app/Contents/Resources/Java/libraries"
CONFIG["compiler_path"] ||= "/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/arm-none-eabi/bin"
CONFIG["core_path"] ||= "/Applications/Arduino.app/Contents/Resources/Java/hardware/teensy/cores/teensy3"
CONFIG["build_dir"] ||= "build"

# preprocessor flags
CONFIG["cppflags"] ||= ["-Wall", "-g", "-Os", "-mcpu=cortex-m4", "-mthumb", "-nostdlib", "-MMD", "-ffunction-sections", "-fdata-sections"]
# c++ flags
CONFIG["cxxflags"] ||= ["-std=gnu++0x", "-felide-constructors", "-fno-exceptions", "-fno-rtti", "-fpermissive"]
# c flags
CONFIG["cflags"] ||= []
# linker flags
CONFIG["ldflags"] ||= ["-Os", "-Wl,--gc-sections", "-mcpu=cortex-m4", "-mthumb", "-T#{CONFIG["core_path"]}/mk20dx256.ld"]
# additional libraries
CONFIG["libs"] ||= []#["-lm", "-larm_cortexM4l_math"]

CC      = "#{CONFIG["compiler_path"]}/arm-none-eabi-gcc"
CXX     = "#{CONFIG["compiler_path"]}/arm-none-eabi-g++"
OBJCOPY = "#{CONFIG["compiler_path"]}/arm-none-eabi-objcopy"
SIZE    = "#{CONFIG["compiler_path"]}/arm-none-eabi-size"

hex_file = "#{CONFIG["build_dir"]}/#{CONFIG["target"]}.hex"
elf_file = "#{CONFIG["build_dir"]}/#{CONFIG["target"]}.elf"

SRC = FileList['*.c', '*.cpp', "#{CONFIG["libraries"]}/**/*.c", "#{CONFIG["libraries"]}/**/*.cpp", "#{CONFIG["core_path"]}/**/*.c", "#{CONFIG["core_path"]}/**/*.cpp"]

# add the arduino libs
CONFIG["arduino_libs"].each do |lib|
  if File.exists?("#{CONFIG["arduino_libs_path"]}/#{lib}")
    SRC.include("#{CONFIG["arduino_libs_path"]}/#{lib}/*.c", "#{CONFIG["arduino_libs_path"]}/#{lib}/*.cpp")
  end
end

OBJ = SRC.collect { |fn| File.join(CONFIG["build_dir"], File.basename(fn).ext('o')) }

INCLUDES = [".", CONFIG["core_path"]]
SRC.each do |f|
  dir = File.dirname(f)
  INCLUDES << dir unless INCLUDES.include?(dir)
end
INCLUDES.map! { |dir| "-I#{dir}" }

puts OBJ
puts
puts SRC
puts
puts INCLUDES
puts

CLEAN.include(OBJ, hex_file, elf_file, CONFIG["build_dir"])

desc "create the hex file"
task :compile => [hex_file]

file hex_file => [elf_file] do |t|
  sh "#{SIZE} #{elf_file}"
  sh "#{OBJCOPY} -O ihex -R .eeprom #{elf_file} #{t.name}"
end

file elf_file => OBJ do |t|
  args = CONFIG["ldflags"] + CONFIG["libs"]
  sh "#{CC} #{args.join(" ")} -o #{t.name} #{t.prerequisites.join(" ")} -Lbuild"
end

directory CONFIG["build_dir"]

rule ".o" => lambda { |objfile| find_source(objfile) } do |t|
  Rake::Task[CONFIG["build_dir"]].invoke
  args = CONFIG["options"] + CONFIG["cppflags"] + INCLUDES
  if File.extname(t.source) == ".c"
    sh "#{CC} #{args.join(" ")} #{t.source} -c -o #{t.name}"
  else
    args += CONFIG["cxxflags"]
    sh "#{CXX} #{args.join(" ")} -x c++ #{t.source} -c -o #{t.name}"
  end
end

def find_source(objfile)
  base = File.basename(objfile, '.o')
  SRC.find { |s| File.basename(s, '.c') == base || File.basename(s, '.cpp') == base }
end
